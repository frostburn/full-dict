#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "full-dict/full.h"

int compare(const void *a, const void *b) {
    short int x = *(short int*)a;
    short int y = *(short int*)b;
    return x - y;
}

int main() {
    short int value;
    FullDict *dict = full_dict_new(sizeof(short int), compare);
    value = 1;
    full_dict_append(dict, &value);
    value = 2;
    full_dict_append(dict, &value);
    full_dict_append(dict, &value);
    assert(full_dict_contains(dict, &value));
    value = 5;
    full_dict_append(dict, &value);
    full_dict_append(dict, &value);
    full_dict_append(dict, &value);
    full_dict_append(dict, &value);
    full_dict_append(dict, &value);
    full_dict_append(dict, &value);
    full_dict_append(dict, &value);
    full_dict_append(dict, &value);
    value = 3;
    full_dict_append(dict, &value);
    assert(full_dict_contains(dict, &value));

    full_dict_finalize(dict);
    assert(dict->num_keys == 4);

    value = 1;
    assert(full_dict_index(dict, &value) == 0);
    value = 5;
    assert(full_dict_index(dict, &value) == 3);
    value = 3;
    assert(full_dict_index(dict, &value) == 2);

    assert(*((short int*)full_dict_key(dict, 0)) == 1);

    full_dict_delete(dict);
}
