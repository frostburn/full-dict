#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "full-dict/full.h"

FullDict* full_dict_new(size_t key_size, int(*compare)(const void *, const void *)) {
    FullDict *dict = malloc(sizeof(FullDict));
    dict->keys = NULL;
    dict->key_size = key_size;
    dict->compare = compare;
    dict->num_keys = dict->num_sorted = dict->size = 0;
    dict->valid = 0;
    return dict;
}

void full_dict_delete(FullDict *dict) {
    free(dict->keys);
    free(dict);
}

int full_dict_contains(FullDict *dict, void *key) {
    if (bsearch(key, dict->keys, dict->num_sorted, dict->key_size, dict->compare) != NULL) {
        return 1;
    }
    for (size_t i = dict->num_sorted; i < dict->num_keys; ++i) {
        if (dict->compare(dict->keys + i * dict->key_size, key) == 0) {
            return 1;
        }
    }
    return 0;
}

void full_dict_append(FullDict *dict, void *key) {
    if (!dict->size){
        dict->size = 8;
        dict->num_sorted = 1;
        dict->keys = malloc(dict->size * dict->key_size);
        dict->valid = 1;
    }
    else if (dict->num_keys >= dict->size) {
        dict->num_sorted = dict->size;
        qsort(dict->keys, dict->num_sorted, dict->key_size, dict->compare);
        dict->size = dict->size * 3;
        dict->size >>= 1;
        dict->keys = realloc(dict->keys, dict->size * dict->key_size);
        dict->valid = 0;
    }
    memcpy(dict->keys + dict->num_keys * dict->key_size, key, dict->key_size);
    ++dict->num_keys;
}

void full_dict_add(FullDict *dict, void *key) {
    if (full_dict_contains(dict, key)) {
        return;
    }
    full_dict_append(dict, key);
}

void full_dict_finalize(FullDict *dict) {
    if (!dict->num_keys) {
        return;
    }
    dict->num_sorted = dict->num_keys;
    qsort(dict->keys, dict->num_sorted, dict->key_size, dict->compare);
    void *last = dict->keys;
    for (size_t i = 1; i < dict->num_keys; ++i) {
        void *current = dict->keys + i * dict->key_size;
        if (dict->compare(last, current) != 0) {
            last += dict->key_size;
            if (last != current) {
                memcpy(last, current, dict->key_size);
            }
        }
    }
    dict->size = dict->num_sorted = dict->num_keys = 1 + ((last - dict->keys) / dict->key_size);
    dict->keys = realloc(dict->keys, dict->size * dict->key_size);
    dict->valid = 1;
}

size_t full_dict_index(FullDict *dict, void *key) {
    assert(dict->valid);
    void *result = bsearch(key, dict->keys, dict->num_sorted, dict->key_size, dict->compare);
    return (result - dict->keys) / dict->key_size;
}

void* full_dict_key(FullDict *dict, size_t index) {
    assert(dict->valid);
    return dict->keys + index * dict->key_size;
}
