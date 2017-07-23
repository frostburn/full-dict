#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "full-dict/util.h"
#include "full-dict/indicator.h"

#define HINT_MAX (255)

void indicator_dict_init(IndicatorDict *dict, indicator_fun indicator, keys_t key_space_size, size_t memory_constant) {
    dict->indicator = indicator;
    dict->num_keys = 0;
    dict->min_key = ~0ULL;
    dict->max_key = 0ULL;
    for (keys_t key = 0; key < key_space_size; ++key) {
        if (indicator(key)) {
            dict->min_key = key;
            break;
        }
    }
    dict->memory_constant = memory_constant;
    dict->num_checkpoints = ceil_div(key_space_size, memory_constant);
    dict->checkpoints = (checkpoint_t*) malloc(dict->num_checkpoints * sizeof(checkpoint_t));
    dict->hints = (hint_t*) malloc(dict->num_checkpoints * sizeof(hint_t));
    int first = 1;
    hint_t hint = 0;
    for (keys_t key = 0; key < key_space_size; ++key) {
        if (key % memory_constant == 0) {
            dict->checkpoints[key / memory_constant] = dict->num_keys;
            hint = 0;
            first = 1;
        }
        if (indicator(key)) {
            dict->num_keys++;
            dict->max_key = key;
            first = 0;
            dict->hints[key / memory_constant] = hint;
        }
        else if (first && hint < HINT_MAX){
            hint++;
        }
    }
    dict->num_checkpoints = ceil_div(dict->max_key + 1, memory_constant);
    dict->checkpoints = (checkpoint_t*) realloc(dict->checkpoints, dict->num_checkpoints * sizeof(checkpoint_t));
    dict->hints = (hint_t*) realloc(dict->hints, dict->num_checkpoints * sizeof(hint_t));
}

size_t indicator_dict_index(IndicatorDict *dict, keys_t key) {
    size_t index = key / dict->memory_constant;
    keys_t start_key = dict->memory_constant * index + dict->hints[index];
    index = dict->checkpoints[index];
    for (keys_t test_key = start_key; test_key < key; ++test_key) {
        if (dict->indicator(test_key)) {
            index++;
        }
    }
    return index;
}

keys_t indicator_dict_next(IndicatorDict *dict, keys_t last) {
    if (last > dict->max_key) {
        return ~0;
    }
    while(1) {
        if (dict->indicator(++last)) {
            return last;
        }
    }
}

void indicator_dict_write(IndicatorDict *dict, FILE *stream) {
    fwrite((void*) dict, sizeof(IndicatorDict), 1, stream);
    fwrite((void*) dict->checkpoints, sizeof(checkpoint_t), dict->num_checkpoints, stream);
    fwrite((void*) dict->hints, sizeof(hint_t), dict->num_checkpoints, stream);
}

char* indicator_dict_associate(IndicatorDict *dict, char *buffer) {
    *dict = *((IndicatorDict*) buffer);
    buffer += sizeof(IndicatorDict);
    dict->checkpoints = (checkpoint_t*) buffer;
    buffer += dict->num_checkpoints * sizeof(checkpoint_t);
    dict->hints = (hint_t*) buffer;
    buffer += dict->num_checkpoints * sizeof(hint_t);
    return buffer;
}
