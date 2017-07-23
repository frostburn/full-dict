#include <assert.h>
#include <stdlib.h>

#include "full-dict/indicator.h"

int some_indicator(keys_t key) {
    return ((((key ^ 12398) + 234) * 329879) % 7) == 5;
}

int main() {
    IndicatorDict *dict = malloc(sizeof(IndicatorDict));
    keys_t key_space_size = 99;
    indicator_dict_init(dict, some_indicator, key_space_size, 10);
    assert(dict->num_keys == 14);
    assert(dict->min_key == 3);
    assert(dict->max_key == 91);

    keys_t MEMBERS[14] = {3, 8, 21, 26, 36, 45, 49, 54, 63, 64, 73, 78, 82, 91};
    size_t index = 0;
    keys_t key = dict->min_key;
    while (key <= dict->max_key) {
        assert(key == MEMBERS[index++]);
        key = indicator_dict_next(dict, key);
    }
    return 0;
}
