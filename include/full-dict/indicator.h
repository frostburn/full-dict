#ifndef FULL_DICT_INDICATOR_H_GUARD
#define FULL_DICT_INDICATOR_H_GUARD

#include <stddef.h>
#include <stdio.h>

#include "full-dict/types.h"

typedef unsigned int checkpoint_t;
typedef unsigned char hint_t;
typedef int (*indicator_fun)(keys_t key);

/*
    Low memory indicator function indexer

    * Members *
    indicator: Function that returns non-zero for keys that should be included in the index
    num_keys: Number of keys in the index
    min_key: Lowest key in the index
    max_key: Highest key in the index
    memory_constant: Trades between speed of indexing and memory
    checkpoints: Know indices
    hints: Twiddles for known indices
*/
typedef struct IndicatorDict
{
    indicator_fun indicator;
    size_t num_keys;
    keys_t min_key;
    keys_t max_key;
    size_t memory_constant;
    size_t num_checkpoints;
    checkpoint_t *checkpoints;
    hint_t *hints;
} IndicatorDict;

/*
    Returns a pointer to a new initialized IndicatorDict

    indicator: Function that returns non-zero for keys that should be included in the index
    key_space_size: Size of the key space of the indicator function
    memory_constant: How light the dictionary should be on memory.
*/
IndicatorDict* indicator_dict_new(indicator_fun indicator, keys_t key_space_size, size_t memory_constant);

/*
    Properly frees the memory used by an IndicatorDict
*/
void indicator_dict_delete(IndicatorDict *dict);

/*
    Returns the index of the given key

    dict: Intialized pointer to an IndicatorDict
    key: The key of interest
*/
size_t indicator_dict_index(IndicatorDict *dict, keys_t key);

/*
    Iterate to the next key in the set

    dict: Intialized pointer to an IndicatorDict
    last: The last key in the indicator set

    returns ULLONG_MAX if there is no next key
*/
keys_t indicator_dict_next(IndicatorDict *dict, keys_t last);

/*
    Write indicator dictionary to a stream

    dict: Intialized pointer to an IndicatorDict
    stream: Stream opened for writing
*/
void indicator_dict_write(IndicatorDict *dict, FILE *stream);

/*
    Associate indicator dict with a buffer

    dict: Uninitialized pointer to an IndicatorDict
    buffer: Buffer containing a previously written dictionary

    returns: The remaining buffer after association

*/
char* indicator_dict_associate(IndicatorDict *dict, char *buffer);

#endif /* !FULL_DICT_INDICATOR_H_GUARD */
