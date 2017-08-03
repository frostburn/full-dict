#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "full-dict/indicator.h"

int some_indicator(keys_t key) {
    return ((((key ^ 12398) + 234) * 329879) % 7) == 5;
}

int main() {
    size_t size = 2048;
    char *buffer = malloc(size * sizeof(char));
    buffer[50] = 111;
    FILE *stream = fmemopen(buffer, size, "w+");
    assert(stream);
    keys_t key_space_size = 200;
    IndicatorDict *dict = indicator_dict_new(some_indicator, key_space_size, 16);
    indicator_dict_write(dict, stream);
    fclose(stream);
    IndicatorDict *other = malloc(sizeof(IndicatorDict));
    char *rest = indicator_dict_associate(other, buffer);

    assert(rest < buffer + size);
    assert(dict->min_key == other->min_key);
    assert(dict->max_key == other->max_key);
    assert(dict->num_keys == other->num_keys);

    indicator_dict_delete(dict);
    return 0;
}
