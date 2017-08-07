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

FullDict* full_dict_merge(FullDict *a, FullDict *b) {
    assert(a->valid);
    assert(b->valid);
    assert(a->key_size == b->key_size);
    FullDict *dict = full_dict_new(a->key_size, a->compare);
    void *last_a = a->keys;
    void *last_b = b->keys;
    while (last_a < a->keys + a->key_size * a->num_keys && last_b < b->keys + b->key_size * b->num_keys) {
        int cmp = dict->compare(last_a, last_b);
        if (cmp < 0) {
            full_dict_append(dict, last_a);
            last_a += dict->key_size;
        } else if (cmp > 0) {
            full_dict_append(dict, last_b);
            last_b += dict->key_size;
        } else {
            full_dict_append(dict, last_a);
            last_a += dict->key_size;
            last_b += dict->key_size;
        }
    }
    while (last_a < a->keys + a->key_size * a->num_keys) {
        full_dict_append(dict, last_a);
        last_a += dict->key_size;
    }
    while (last_b < b->keys + b->key_size * b->num_keys) {
        full_dict_append(dict, last_b);
        last_b += dict->key_size;
    }
    full_dict_finalize(dict);
    return dict;
}

void full_dict_write(FullDict *dict, FILE *stream) {
    fwrite((void*) dict, sizeof(FullDict), 1, stream);
    fwrite((void*) dict->keys, dict->key_size, dict->num_keys, stream);
}

FullDict* full_dict_read(FILE *stream) {
    FullDict *dict = malloc(sizeof(FullDict));
    fread((void*) dict, sizeof(FullDict), 1, stream);
    dict->keys = malloc(dict->key_size * dict->num_keys);
    fread((void*) dict->keys, dict->key_size, dict->num_keys, stream);
    return dict;
}

char* full_dict_associate(FullDict *dict, int(*compare)(const void *, const void *), char *buffer) {
    *dict = *((FullDict*) buffer);
    buffer += sizeof(FullDict);
    dict->compare = compare;
    dict->keys = (void*) buffer;
    buffer += dict->num_keys * dict->key_size;
    return buffer;
}
