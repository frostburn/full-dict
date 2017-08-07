#ifndef FULL_DICT_UTIL_H_GUARD
#define FULL_DICT_UTIL_H_GUARD

#include <stddef.h>

typedef struct MemoryMap {
    int file_descriptor;
    size_t length;
    char *buffer;
} MemoryMap;

size_t ceil_div(size_t x, size_t y);

char* file_to_buffer(char *filename);

#endif
