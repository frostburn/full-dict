#include <assert.h>
#include <stdlib.h>

#include "full-dict/types.h"

int main() {
    keys_t keys[5] = {2134, 543534, 32313, 345, 23987879};
    qsort(keys, 5, sizeof(keys_t), compare_keys);
    assert(keys[0] == 345);
    assert(keys[4] == 23987879);
}
