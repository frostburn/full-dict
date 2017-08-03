#ifndef FULL_DICT_FULL_H_GUARD
#define FULL_DICT_FULL_H_GUARD

#include <stddef.h>

/*
    Low memory arbitrary key indexer

    * Members *
    keys: Keys in the index. May contain duplicates during building
    key_size: sizeof(key type)
    compare: Comparison function for the keys
    num_keys: Number of keys in the index. May overshoot during building
    num_sorted: Internal variable telling how much of keys array is sorted
    size: Internal variable telling how much memory the array takes
    valid: Non-zero if the indexing is possible
*/
typedef struct FullDict {
    void *keys;
    size_t key_size;
    int(*compare)(const void *, const void *);
    size_t num_keys;
    size_t num_sorted;
    size_t size;
    int valid;
} FullDict;

FullDict* full_dict_new(size_t key_size, int(*compare)(const void *, const void *));
void full_dict_delete(FullDict *dict);
int full_dict_contains(FullDict *dict, void *key);
void full_dict_append(FullDict *dict, void *key);
void full_dict_add(FullDict *dict, void *key);
void full_dict_finalize(FullDict *dict);
size_t full_dict_index(FullDict *dict, void *key);
void* full_dict_key(FullDict *dict, size_t index);

#endif /* !FULL_DICT_FULL_H_GUARD */
