#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "full-dict/full.h"
#include "full-dict/types.h"

int main() {
    keys_t value;
    FullDict *dict = full_dict_new(sizeof(keys_t), compare_keys);
    value = 1;
    full_dict_append(dict, &value);
    value = 2;
    full_dict_append(dict, &value);
    value = 5;
    full_dict_append(dict, &value);
    full_dict_finalize(dict);

    FullDict *other = full_dict_new(sizeof(keys_t), compare_keys);
    value = 3;
    full_dict_append(other, &value);
    value = 5;
    full_dict_append(other, &value);
    value = 6;
    full_dict_append(other, &value);
    full_dict_finalize(other);

    FullDict *merger = full_dict_merge(dict, other);

    assert(merger->num_keys == 5);

    value = 6;
    assert(full_dict_index(merger, &value) == 4);

    full_dict_delete(dict);
    full_dict_delete(other);
    full_dict_delete(merger);
}
