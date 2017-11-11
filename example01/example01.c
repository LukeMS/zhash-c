#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include "zhash.h"


int main(void) {
    // create hash table
    // struct ZHashTable *zcreate_hash_table(void);
    struct ZHashTable *hash_table;
    hash_table = zcreate_hash_table();

    // set key to val (if there is already a value, overwrite it)
    // void zhash_set(struct ZHashTable *hash_table, char *key, void *val);
    zhash_set(hash_table, "hello", (void *) "world");

    assert(hash_table->head == hash_table->tail);
    printf("0x%" PRIXPTR " ", (uintptr_t)hash_table->head);
    printf("0x%" PRIXPTR "\n", (uintptr_t)hash_table->tail);

    zhash_set(hash_table, "goodbye", (void *) "world");
    assert(hash_table->head != hash_table->tail);
    printf("0x%" PRIXPTR " ", (uintptr_t)hash_table->head);
    printf("0x%" PRIXPTR "\n", (uintptr_t)hash_table->tail);

    for (struct ZHashEntry* p = hash_table->head; p; p = p->linked_next)
    {
        printf("%s: %s\n", p->key, (char *) p->val);
    }




    // return 1 if there is a value stored at the key and 0 otherwise
    // bool zhash_exists(struct ZHashTable *hash_table, char *key);
    if (zhash_exists(hash_table, "goodbye")) {
        // delete the entry stored at key and return the value
        // void *zhash_delete(struct ZHashTable *hash_table, char *key);
        zhash_delete(hash_table, "goodbye");
    }

    if (zhash_exists(hash_table, "hello")) {
        // get the value stored at key (if no value, then return NULL)
        // void *zhash_get(struct ZHashTable *hash_table, char *key);
        printf("hello %s\n", (char *) zhash_get(hash_table, "hello"));
    }

    if (zhash_exists(hash_table, "goodbye")) {
        printf("goodbye %s\n", (char *) zhash_get(hash_table, "goodbye"));
    }

    // free hash table (note that this only frees the table and the entry structs, not the values)
    // void zfree_hash_table(struct ZHashTable *hash_table);
    zfree_hash_table(hash_table);

    hash_table = NULL;

    return 0;
}
