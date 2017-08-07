#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "full-dict/util.h"

size_t ceil_div(size_t x, size_t y) {
    if (x == 0) {
        return 0;
    }
    return 1 + ((x - 1) / y);
}

char* file_to_buffer(char *filename) {
    FILE * pFile;
    size_t lSize;
    char * buffer;
    size_t result;

    pFile = fopen(filename , "rb");
    if (pFile == NULL) {
        fprintf(stderr, "File error\n");
        exit(1);
    }

    // obtain file size:
    fseek(pFile , 0 , SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc(sizeof(char) * lSize);
    if (buffer == NULL) {
        fprintf(stderr, "Memory error\n");
        exit(2);
    }

    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) {
        fprintf(stderr, "Reading error\n");
        exit(3);
    }

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose(pFile);
    return buffer;
}

MemoryMap mmap_file(char *filename) {
    struct stat sb;
    stat(filename, &sb);
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "File error\n");
        exit(1);
    }
    char *buffer;
    buffer = (char*) mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (buffer == MAP_FAILED) {
        fprintf(stderr, "Mapping error\n");
        exit(2);
    }
    madvise(buffer, sb.st_size, MADV_RANDOM);
    return (MemoryMap) {fd, sb.st_size, buffer};
}

void munmap_file(MemoryMap map) {
    munmap(map.buffer, map.length);
    close(map.file_descriptor);
}
