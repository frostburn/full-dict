#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "full-dict/full.h"
#include "full-dict/types.h"

int main() {
    size_t size = 2048;
    char *buffer = malloc(size * sizeof(char));
    FILE *stream = fmemopen(buffer, size, "w");
    assert(stream);
    FullDict *dict = full_dict_new(sizeof(keys_t), compare_keys);
    keys_t key = 11;
    full_dict_append(dict, &key);
    full_dict_finalize(dict);
    full_dict_write(dict, stream);
    fclose(stream);

    stream = fmemopen(buffer, size, "r");
    FullDict *another = full_dict_read(stream);
    fclose(stream);

    FullDict *other = malloc(sizeof(FullDict));
    char *rest = full_dict_associate(other, compare_keys, buffer);

    assert(rest < buffer + size);
    assert(full_dict_index(other, &key) == 0);
    assert(full_dict_index(another, &key) == 0);

    key = 1;
    full_dict_append(another, &key);
    full_dict_finalize(another);
    key = 11;
    assert(full_dict_index(another, &key) == 1);

    full_dict_delete(dict);
    full_dict_delete(another);
    free(other);
    free(buffer);

    return 0;
}
