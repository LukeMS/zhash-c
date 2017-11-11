#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "zhash.h"



static size_t zgenerate_hash(struct ZHashTable *hash, char *key);
static void zhash_rehash(struct ZHashTable *hash_table, size_t size_index);
static struct ZHashEntry *zcreate_entry(char *key, void *val);
static void zfree_entry(struct ZHashEntry *entry, bool recursive);
static size_t next_size_index(size_t size_index);
static size_t previous_size_index(size_t size_index);
static void *zmalloc(size_t size);
static void *zcalloc(size_t num, size_t size);


/************************************************************************//**
 * array zhash_sizes: possible sizes for hash table (must be prime numbers).
 ***************************************************************************/
const size_t zhash_sizes[] = {
  53,         // 0
  101,        // 1
  211,        // 2
  503,        // 3
  1553,       // 4
  3407,       // 5
  6803,       // 6
  12503,      // 7
  25013,      // 8
  50261,      // 9
  104729,     // 10
  250007,     // 11
  500009,     // 12
  1000003,    // 13
  2000029,    // 14
  4000037,    // 15
  10000019,   // 16
  25000009,   // 17
  50000047,   // 18
  104395301,  // 19
  217645177,  // 20
  512927357,  // 21
  1000000007  // 22
};


/*****************************************************************
 *
 * zcreate_hash_table
 *
 *****************************************************************/
struct ZHashTable *zcreate_hash_table(void)
{
    return zcreate_hash_table_with_size(0);
}


/*****************************************************************
 *
 * zcreate_hash_table_with_size
 *
 *****************************************************************/
struct ZHashTable *zcreate_hash_table_with_size(size_t size_index)
{
    struct ZHashTable *hash_table;

    hash_table = zmalloc(sizeof(struct ZHashTable));

    hash_table->size_index = size_index;
    hash_table->entry_count = 0;
    hash_table->head = NULL;
    hash_table->tail = NULL;
    hash_table->entries = zcalloc(zhash_sizes[size_index], sizeof(void *));

    return hash_table;
}


/*****************************************************************
 *
 * zfree_hash_table
 *
 *****************************************************************/
void zfree_hash_table(struct ZHashTable *hash_table,
        void (*destroy)(void*data)) {
    struct ZHashEntry *entry = hash_table->head;
    while((entry = hash_table->head)) {
        void *data = zhash_delete(hash_table, entry->key);
        if (destroy)
            destroy(data);
    }

    zfree(hash_table->entries);
    zfree(hash_table);
}


/*****************************************************************
 *
 * zhash_set
 *
 *****************************************************************/
void zhash_set(struct ZHashTable *hash_table, char *key, void *val) {
    size_t size, hash;
    struct ZHashEntry *entry;

    hash = zgenerate_hash(hash_table, key);
    entry = hash_table->entries[hash];

    while (entry) {
        if (strcmp(key, entry->key) == 0) {
            entry->val = val;
            return;
        }
        entry = entry->bucket_next;
    }

    entry = zcreate_entry(key, val);

    entry->bucket_next = hash_table->entries[hash];
    hash_table->entries[hash] = entry;


    /*****************************************************************
    * Handle insertion at an empty hash_table.
    *****************************************************************/
    if (hash_table->head == NULL) {
        hash_table->tail = entry;
        entry->linked_next = NULL;
        entry->linked_prev = NULL;
    /*****************************************************************
    * Handle insertion at an non-empty hash_table.
    *****************************************************************/
    } else {
        entry->linked_next = hash_table->head->linked_next;
        entry->linked_next->linked_prev = entry;
    }
    /*****************************************************************
    * Every new entry is placed at the head of the hash_table.
    *****************************************************************/
    hash_table->head = entry;
    hash_table->entry_count++;


    size = zhash_sizes[hash_table->size_index];

    if (hash_table->entry_count > size / 2) {
        zhash_rehash(hash_table, next_size_index(hash_table->size_index));
    }
}


/*****************************************************************
 *
 * zhash_get
 *
 *****************************************************************/
void *zhash_get(struct ZHashTable *hash_table, char *key)
{
    size_t hash;
    struct ZHashEntry *entry;

    hash = zgenerate_hash(hash_table, key);
    entry = hash_table->entries[hash];

    while (entry && strcmp(key, entry->key) != 0) entry = entry->bucket_next;

    return entry ? entry->val : NULL;
}


/*****************************************************************
 *
 * zhash_delete
 *
 *****************************************************************/
void *zhash_delete(struct ZHashTable *hash_table, char *key) {
    size_t size, hash;
    struct ZHashEntry *entry;
    void *val;

    hash = zgenerate_hash(hash_table, key);
    entry = hash_table->entries[hash];

    if (entry && strcmp(key, entry->key) == 0) {
        hash_table->entries[hash] = entry->bucket_next;
    } else {
        while (entry) {
            if (entry->bucket_next && strcmp(key, entry->bucket_next->key) == 0) {
                struct ZHashEntry *deleted_entry;

                deleted_entry = entry->bucket_next;
                entry->bucket_next = entry->bucket_next->bucket_next;
                entry = deleted_entry;
                break;
            }
            entry = entry->bucket_next;
        }
    }

    if (!entry) return NULL;

    val = entry->val;

    // MOD
    if (entry == hash_table->head) {
            /*****************************************************************
            * Handle removal from the head of the hash_table.
            *****************************************************************/
            hash_table->head = entry->linked_next;

            if (hash_table->head == NULL) {
                    hash_table->tail = NULL;

            } else {
                    entry->linked_next->linked_prev = NULL;
            }

    } else {
            /*****************************************************************
            * Handle removal from somewhere other than the head.
            *****************************************************************/
            entry->linked_prev->linked_next = entry->linked_next;

            if (entry->linked_next == NULL) {
                    hash_table->tail = entry->linked_prev;

            } else {
                    entry->linked_next->linked_prev = entry->linked_prev;
            }
    }
    // END OF MOD

    zfree_entry(entry, false);
    hash_table->entry_count--;


    size = zhash_sizes[hash_table->size_index];

    if (hash_table->entry_count < size / 8) {
        zhash_rehash(hash_table, previous_size_index(hash_table->size_index));
    }

    return val;
}


/*****************************************************************
 *
 * zhash_exists
 *
 *****************************************************************/
bool zhash_exists(struct ZHashTable *hash_table, char *key)
{
    size_t hash;
    struct ZHashEntry *entry;

    hash = zgenerate_hash(hash_table, key);
    entry = hash_table->entries[hash];

    while (entry && strcmp(key, entry->key) != 0) entry = entry->bucket_next;

    return entry ? true : false;
}


/*****************************************************************
 *
 * zcreate_entry
 *
 *****************************************************************/
struct ZHashEntry *zcreate_entry(char *key, void *val)
{
    struct ZHashEntry *entry;
    char *key_cpy;

    key_cpy = zmalloc((strlen(key) + 1) * sizeof(char));
    entry = zmalloc(sizeof(struct ZHashEntry));

    strcpy(key_cpy, key);
    entry->key = key_cpy;
    entry->val = val;

    return entry;
}


/*****************************************************************
 *
 * zfree_entry
 *
 *****************************************************************/
void zfree_entry(struct ZHashEntry *entry, bool recursive)
{
    if (recursive && entry->bucket_next) zfree_entry(entry->bucket_next, recursive);

    zfree(entry->key);
    zfree(entry);
}


/*****************************************************************
 *
 * zgenerate_hash
 *
 *****************************************************************/
size_t zgenerate_hash(struct ZHashTable *hash_table, char *key)
{
    size_t size, hash;
    char ch;

    size = zhash_sizes[hash_table->size_index];
    hash = 0;

    while ((ch = *key++)) hash = (17 * hash + ch) % size;

    return hash;
}


/*****************************************************************
 *
 * zhash_rehash
 *
 *****************************************************************/
void zhash_rehash(struct ZHashTable *hash_table, size_t size_index)
{
    size_t hash, size, ii;
    struct ZHashEntry **entries;

    if (size_index == hash_table->size_index) return;

    size = zhash_sizes[hash_table->size_index];
    entries = hash_table->entries;

    hash_table->size_index = size_index;
    hash_table->entries = zcalloc(zhash_sizes[size_index], sizeof(void *));

    for (ii = 0; ii < size; ii++) {
        struct ZHashEntry *entry;

        entry = entries[ii];
        while (entry) {
            struct ZHashEntry *next_entry;

            hash = zgenerate_hash(hash_table, entry->key);
            next_entry = entry->bucket_next;
            entry->bucket_next = hash_table->entries[hash];
            hash_table->entries[hash] = entry;

            entry = next_entry;
        }
    }

    zfree(entries);
}


/*****************************************************************
 *
 * next_size_index
 *
 *****************************************************************/
static size_t next_size_index(size_t size_index)
{
    if (size_index == COUNT_OF(zhash_sizes)) return size_index;

    return size_index + 1;
}


/*****************************************************************
 *
 * previous_size_index
 *
 *****************************************************************/
static size_t previous_size_index(size_t size_index)
{
    if (size_index == 0) return size_index;

    return size_index - 1;
}


/*****************************************************************
 *
 * zmalloc
 *
 *****************************************************************/
static void *zmalloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);

    if (!ptr) exit(EXIT_FAILURE);

    return ptr;
}


/*****************************************************************
 *
 * zcalloc
 *
 *****************************************************************/
static void *zcalloc(size_t num, size_t size)
{
    void *ptr;

    ptr = calloc(num, size);

    if (!ptr) exit(EXIT_FAILURE);

    return ptr;
}
