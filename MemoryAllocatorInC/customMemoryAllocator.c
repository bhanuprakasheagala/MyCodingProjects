#include <unistd.h>
#include <string.h>
#include <pthread.h>

typedef char ALIGN[16];

union header {
    struct {
        size_t size;
        unsigned is_free;
        union header *next;
    } s;
    ALIGN stub; // The header must be aligned to 16 bytes
};

typedef union header header_t;

header_t *head = NULL, *tail = NULL;
pthread_mutex_t global_malloc_lock;

header_t *get_free_block(size_t size) {
    header_t *curr = head;
    while(curr) {
        /* Check if there is a free block that can accomodate requested size */
        if(curr->s.is_free && curr->s.size >= size) {
            return curr;
        }
        curr = curr->s.next;
    }
    return NULL;
}

void free(void *block) {
    header_t *head, *temp;
    void *program_break; // program break is the end of the process' data segment

    if(!block){
        return;
    }
    pthread_mutex_lock(&global_malloc_lock);
    header = (header_t*)block - 1;
    program_break = sbrk(0); // sbrk(0) gives the current program break address

    /*
    Check if the block to be freed is the last one in the linked list.
    If it is, then we can shrink the size of the heap and release memory to OS.
    Else, We keep the block but mark it as free.
    */
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

        /*
        sbrk() with negative argument decrements the program break. So memory is released by the program
        to Operating System.
        */
        sbrk(0 - header->s.size - sizeof(header_t));

        // Unlock the mutex lock
        /* This lock does not really assure thread safety as sbrk() itself is not really thread safe.
        Suppose there occurs a foregin sbrk(N) (from external sources) after we find the program break
        but before we decrement it, then we end up relasing the memory obtained by the foregin sbrk()
        */
        pthread_mutex_unlock(&global_malloc_lock);
        
        return;
    }
    header->s.is_free = true;
    pthread_mutex_unlock(&global_malloc_lock);
}

void *malloc(size_t size) {
    size_t totalSize;
    void* block;
    header_t *header;
    if(!size) {
        return NULL;
    }
    pthread_mutex_lock(&global_malloc_lock);
    header = get_free_block(size);
    if(header) {
        // Found a free block to accomodate requested memory
        header->s.free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        // Return the pointer from the beginning of the actual block of memory excluding header
        return (void*)(header+1);
    }
    // Need to get memory to fit in the requested block and header from OS
    totalSize = sizeof(header_t) + size;
    block = sbrk(totalSize);
    if(block == (void*)-1) { // Unable to allocate the memory
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }
    header = block;
    header->s.size = size;
    header->s.is_free = 0;
    header->next = NULL;
    if(!head) {
        head = header;
    }
    if(tail) {
        tail->s.next = header;
    }
    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    
    return (void*)(header+1);
}

