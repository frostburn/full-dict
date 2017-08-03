#include "full-dict/types.h"

int compare_keys(const void *a, const void *b) {
    const keys_t x = *((keys_t*)a);
    const keys_t y = *((keys_t*)b);
    if (x < y) {
        return -1;
    }
    if (x > y) {
        return 1;
    }
    return 0;
}
