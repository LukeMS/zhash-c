#ifndef ZHASH_H
#define ZHASH_H

#include <stdbool.h>
#include <stdint.h>

// hash table
// keys are strings
// values are void *pointers

#define COUNT_OF(arr) (sizeof(arr) / sizeof(*arr))
#define zfree free

#define DEFAULT_ZHASH_SIZE_INDEX 0



/************************************************************************//**
 * struct ZHashEntry, ZHashEntry: structure for hash tables entries.
 ***************************************************************************/
struct ZHashEntry {
    char *key;
    void *val;
    struct ZHashEntry *bucket_next;
    struct ZHashEntry *linked_next;
    struct ZHashEntry *linked_prev;
} ZHashEntry;


/************************************************************************//**
 * struct ZHashTable, ZHashTable: structure representing the hash table.
 ***************************************************************************/
struct ZHashTable {
    size_t size_index;  // index into the hash_sizes array in hash.c
    size_t entry_count;
    struct ZHashEntry **entries;
    struct ZHashEntry *head;  // used for linked-list-like iteration
    struct ZHashEntry *tail;  // used for linked-list-like iteration
};



/****************************************************************************
 *
 * PUBLIC INTERFACE
 *
 ***************************************************************************/


/*************************************************************//**
 * zcreate_hash_table:  Creates a hash table.
 *
 * Complexity: O(m), where m is the number of buckets in the hash table.
 *     By default the table starts with index 0 = 53 buckets.
 *
 *
 * \return struct ZHashTable instance.
 ****************************************************************/
struct ZHashTable *zcreate_hash_table(void);


/*************************************************************//**
 * zcreate_hash_table_with_size: Creates a hash table.
 *
 * Complexity: O(m), where m is the number of buckets in the hash table's
 *      `size_index` parameter.
 *
 * \param size_index Index corresponding to one of the bucket sizes at
 *     the `zhash_sizes` array.
 *
 * \return struct ZHashTable instance.
 ****************************************************************/
struct ZHashTable *zcreate_hash_table_with_size(size_t size_index);


/*************************************************************//**
 * zfree_hash_table:
 *
 * \param *destroy provides a way to free dynamically allocated data when
 *     zfree_hash_table is called. For example, if the hash table contains
 *     data dynamically allocated using malloc, destroy should be set to free
 *     to free the data as the hash table is destroyed. For structured data
 *     containing several dynamically allocated members, destroy should be
 *     set to a user-defined function that calls free for each dynamically
 *     allocated member as well as for the structure itself. For a hash table
 *     containing data that should not be freed, destroy should be set to
 *     NULL.
****************************************************************/
void zfree_hash_table(struct ZHashTable *hash_table, void zhash_dtor(void*));


/*************************************************************//**
 * zhash_set: Inserts an element into the `hash table`.
 *
 * The new element contains a pointer to data, so the memory referenced by
 * data should remain valid as long as the element remains in the hash table.
 * It is the responsibility of the caller to manage the storage associated
 * with data.
 * Complexity: O(1)
 *
 * \param *hash_table pointer to a `ZHashTable` struct
 * \param *data pointer to the data that will be referenced by the new element
 ****************************************************************/
void zhash_set(struct ZHashTable *hash_table, char *key, void *val);


/*************************************************************//**
 * zhash_get: Returns the data of the element matching `key`.
 *
 * Complexity: O(1)
 *
 * \param *hash_table pointer to a `ZHashTable` struct
 * \param *key string matching the key of the element to be searched
 *
 * \return void pointer to the data previously stored at the element.
 ****************************************************************/
void *zhash_get(struct ZHashTable *hash_table, char *key);


/*************************************************************//**
 * zhash_delete: Removes the element matching `key` from `hash_table`.
 *
 * It is the responsibility of the caller to manage the storage associated
 * with the data.
 * Complexity: O(1)
 *
 * \param *hash_table pointer to a `ZHashTable` struct
 * \param *key string matching the key of the element to be removed
 *
 * \return void pointer to the data previously stored in the removed element.
 ****************************************************************/
void *zhash_delete(struct ZHashTable *hash_table, char *key);

/*************************************************************//**
 * zhash_exists: Check if an element matching `key` exists at `hash_table`.
 *
 * Complexity: O(1)
 *
 * \param *hash_table pointer to a `ZHashTable` struct
 * \param *key string matching the key of the element to be searched
 *
 * \return true if a matching element is found, false otherwise.
 ****************************************************************/
bool zhash_exists(struct ZHashTable *hash_table, char *key);


#endif