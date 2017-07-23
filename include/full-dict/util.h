#ifndef FULL_DICT_UTIL_H_GUARD
#define FULL_DICT_UTIL_H_GUARD

#include <stddef.h>

size_t ceil_div(size_t x, size_t y) {
    if (x == 0) {
        return 0;
    }
    return 1 + ((x - 1) / y);
}
#endif
