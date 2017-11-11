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
    struct ZHashEntry* entry;

    // set key to val (if there is already a value, overwrite it)
    // void zhash_set(struct ZHashTable *hash_table, char *key, void *val);
    zhash_set(hash_table, "ka", (void *) "va");
    zhash_set(hash_table, "kb", (void *) "vb");
    zhash_set(hash_table, "kc", (void *) "vc");
    zhash_set(hash_table, "kd", (void *) "vd");
    zhash_set(hash_table, "ke", (void *) "ve");


    /*
    loop1 ka: va
    loop1 kb: vb
    loop1 kc: vc
    loop1 kd: vd
    loop1 ke: ve
    */
    entry = hash_table->head;
    if (hash_table->head != NULL) {
        for (; entry; entry = entry->linked_next) {
            printf("loop1 %s: %s\n", entry->key, (char *) entry->val);
        }
    }


    if (zhash_exists(hash_table, "kb")) {
        printf("removed value %s\n", (char *) zhash_delete(hash_table, "kb"));
    }
    if (zhash_exists(hash_table, "kd")) {
        printf("removed value %s\n", (char *) zhash_delete(hash_table, "kd"));
    }


    /*
    loop2 ka: va
    loop2 kc: vc
    loop2 ke: ve
    */
    entry = hash_table->head;
    if (hash_table->head != NULL) {
        for (; entry; entry = entry->linked_next) {
            printf("loop2 %s: %s\n", entry->key, (char *) entry->val);
        }
    }


    if (zhash_exists(hash_table, "ka")) {
        printf("removed value %s\n", (char *) zhash_delete(hash_table, "ka"));
    }
    if (zhash_exists(hash_table, "ke")) {
        printf("removed value %s\n", (char *) zhash_delete(hash_table, "ke"));
    }


    /*
    loop3 kc: vc
    */
    entry = hash_table->head;
    if (hash_table->head != NULL) {
        for (; entry; entry = entry->linked_next) {
            printf("loop3 %s: %s\n", entry->key, (char *) entry->val);
        }
    }

    if (zhash_exists(hash_table, "kc")) {
        printf("removed value %s\n", (char *) zhash_delete(hash_table, "kc"));
    }


    /*
    no output
    */
    entry = hash_table->head;
    if (hash_table->head != NULL) {
        for (; entry; entry = entry->linked_next) {
            printf("loop4 %s: %s\n", entry->key, (char *) entry->val);
        }
    } else {
        printf("loop4: no entries\n");
    }

    zfree_hash_table(hash_table, NULL);

    return 0;
}
