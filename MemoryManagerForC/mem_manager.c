#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "mem_manager.h"

// Global variables
header_t* head = NULL, *tail = NULL;
pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

header_t* get_free_block(size_t size) {
    header_t* curr = head;
    while(curr) {
        if(curr->s.is_free && curr->s.size >= size) {
            return curr;
        }
        curr = curr->s.next;
    }

    return NULL;
}

void* malloc(size_t size) {
    size_t total_size;
    void* block = NULL;
    header_t* header;

    if(!size) {
        return NULL;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = get_free_block(size);
    if(header) {
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }

    total_size = sizeof(header_t) + size;
    block = sbrk(total_size);
    if(block == (void*) - 1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = block;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;

    if(!head) {
        head = header;
    }

    if(tail) {
        tail->s.next = header;
    }

    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);

    return (void*)(header + 1);
}

void free(void* block) {
    header_t* header = NULL, *temp = NULL;
    void* program_break = NULL;

    if(!block) {
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = (header_t*)block - 1;
    program_break = sbrk(0);

    if((char*)block + header->s.size == program_break) {
        if(head == tail) {
            head = tail = NULL;
        }
        else {
            temp = head;
            while(temp) {
                if(temp->s.next == tail) {
                    temp->s.next = NULL;
                    tail = temp;
                }
                temp = temp->s.next;
            }
        }
        
        sbrk(0 - (sizeof(header_t) + header->s.size));
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }

    header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}

void* calloc(size_t num, size_t nSize) {
    size_t size;
    void* block = NULL;

    if(!num || !nSize) {
        return NULL;
    }

    size = num * nSize;
    if(nSize != (size/num)) {
        return NULL;
    }

    block = malloc(size);
    if(!block) {
        return NULL;
    }

    memset(block, 0, size);
    
    return block;
}

void* realloc(void* block, size_t size) {
    header_t* header = NULL;
    void* ret = NULL;

    if(!block || !size) {
        return malloc(size);
    }

    header = (header_t*)block - 1;
    if(header->s.size >= size) {
        return block;
    }

    ret = malloc(size);
    if(ret) {
        memcpy(ret, block, header->s.size);
        free(block);
    }

    return ret;
}

void print_mem_list() {
    header_t* current = head;
    printf("head = %p, tail = %p\n", (void*)head, (void*)tail);

    while(current) {
        printf("addr = %p, size = %zu, is_free = %u, next = %p\n", (void*)current, current->s.size, current->s.is_free, (void*)current->s.next);
        current = current->s.next;
    }
}

int main() {
    void *p1 = malloc(100);
    if(!p1) {
        printf("malloc failed\n");
        return -1;
    }

    void *p2 = malloc(200);
    if(!p2) {
        printf("malloc failed\n");
        return -1;
    }
    printf("p1 = %p, p2 = %p\n", p1, p2);
    printf("Memory list after malloc\n");
    print_mem_list();

    if(p1) {
        free(p1);
    }
    p1 = NULL;
    printf("Memory list after p1 freed\n");
    print_mem_list();

    void *p3 = calloc(10, sizeof(int));
    if(!p3) {
        printf("calloc failed\n");
        return -1;
    }
    printf("p3 = %p\n", p3);
    printf("Memory list after calloc\n");
    print_mem_list();

    if(p2) {
        free(p2);
    }
    p2 = NULL;
    printf("Memory list after p2 freed\n");
    print_mem_list();
    
    if(p3) {
        free(p3);
    }
    p3 = NULL;
    printf("Memory list after p3 freed\n");
    print_mem_list();
    

    return 0;
}

/*
Sample Output:
p1 = 0x102350018, p2 = 0x102350094
Memory list after malloc
head = 0x102350000, tail = 0x10235007c
addr = 0x102350000, size = 100, is_free = 0, next = 0x10235007c
addr = 0x10235007c, size = 200, is_free = 0, next = 0x0
Memory list after p1 freed
head = 0x102350000, tail = 0x10235007c
addr = 0x102350000, size = 100, is_free = 1, next = 0x10235007c
addr = 0x10235007c, size = 200, is_free = 0, next = 0x0
p3 = 0x102350018
Memory list after calloc
head = 0x102350000, tail = 0x10235007c
addr = 0x102350000, size = 100, is_free = 0, next = 0x10235007c
addr = 0x10235007c, size = 200, is_free = 0, next = 0x0
Memory list after p2 freed
head = 0x102350000, tail = 0x102350000
addr = 0x102350000, size = 100, is_free = 0, next = 0x0
Memory list after p3 freed
head = 0x102350000, tail = 0x102350000
addr = 0x102350000, size = 100, is_free = 1, next = 0x0
*/