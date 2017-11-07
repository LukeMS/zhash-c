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
    zhash_set(hash_table, "ka", (void *) "va");
    zhash_set(hash_table, "kb", (void *) "vb");
    zhash_set(hash_table, "kc", (void *) "vc");
    zhash_set(hash_table, "kd", (void *) "vd");
    zhash_set(hash_table, "ke", (void *) "ve");

    for (struct ZHashEntry* p = hash_table->head; p; p = p->lnknext)
    {
        printf("%s: %s; ", p->key, (char *) p->val);
    }
    printf("\n");

    if (zhash_exists(hash_table, "kb")) {
        zhash_delete(hash_table, "kb");
    }
    if (zhash_exists(hash_table, "kd")) {
        zhash_delete(hash_table, "kd");
    }

    for (struct ZHashEntry* p = hash_table->head; p; p = p->lnknext)
    {
        printf("%s: %s; ", p->key, (char *) p->val);
    }
    printf("\n");

    if (zhash_exists(hash_table, "ka")) {
        zhash_delete(hash_table, "ka");
    }
    if (zhash_exists(hash_table, "ke")) {
        zhash_delete(hash_table, "ke");
    }
    for (struct ZHashEntry* p = hash_table->head; p; p = p->lnknext)
    {
        printf("%s: %s; ", p->key, (char *) p->val);
    }
    printf("\n");

    if (zhash_exists(hash_table, "kc")) {
        zhash_delete(hash_table, "kc");
    }
    for (struct ZHashEntry* p = hash_table->head; p; p = p->lnknext)
    {
        printf("%s: %s; ", p->key, (char *) p->val);
    }
    printf("\n");

    zfree_hash_table(hash_table);

    return 0;
}
