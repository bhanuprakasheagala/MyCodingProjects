#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stddef.h>

// Align to 16 bytes
typedef char ALIGN[16];

union header {
    struct {
        size_t size;
        unsigned is_free;
        union header* next;
    }s;
    ALIGN stub;  // The header must be aligned to 16 bytes
};

typedef union header header_t;

void* malloc(size_t size);
void free(void* block);
void* calloc(size_t num, size_t size);
void* realloc(void* block, size_t size);
void print_mem_list();

#endif  // MEM_MANAGER_H