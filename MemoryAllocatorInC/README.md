## Custom Memory Manager for C

You can delve deeper into the concepts and code through my three-part series on my Hashnode blog. Each part is a concise 4-5 minute read:

- **PART 1: Exploring the Memory Layout and the `sbrk()` System Call**  
  Understand the different sections of memory and how the `sbrk()` system call is used to manage heap memory.  
  [Read more here](https://bhanuprakasheagala.hashnode.dev/heap-memory-allocator-in-c-programming)

- **PART 2: Behind the Scenes of the `malloc()` Function**  
  Gain insights into the background and implementation details of the `malloc()` function in C.  
  [Read more here](https://bhanuprakasheagala.hashnode.dev/simple-memory-allocator-in-c-programming-1)

- **PART 3: Implementing `free()`, `calloc()`, and `realloc()` Functions**  
  Learn how to implement these essential memory management functions and understand their internal workings.  
  [Read more here](https://bhanuprakasheagala.hashnode.dev/heap-memory-allocator-in-c-programming-1)

### Description

This code implements a custom memory manager in C, providing functionality similar to the standard `malloc`, `free`, `calloc`, and `realloc` functions, but with a more hands-on control over memory allocation and deallocation. The implementation is designed to work directly with the system's heap using the `sbrk` system call to manage the program break, which determines the end of the process's data segment. 

#### Key Features and Components

1. **Memory Block Management**: 
   - Memory is managed in blocks, each of which has a header (`header_t`) containing metadata about the block, such as its size, a flag indicating if it is free, and a pointer to the next block in a singly linked list.
   - The memory blocks are aligned to 16 bytes for efficiency and to meet common platform alignment requirements.

2. **Dynamic Memory Allocation Functions**:
   - **`malloc(size_t size)`**: Allocates a block of memory of at least `size` bytes. If a suitable free block is found, it reuses it; otherwise, it requests more memory from the OS using `sbrk`.
   - **`free(void *block)`**: Frees a previously allocated block, making it available for future allocations. If the block is at the end of the heap, it releases the memory back to the OS, effectively shrinking the heap.
   - **`calloc(size_t num, size_t size)`**: Allocates memory for an array of `num` elements, each of `size` bytes, and initializes all bytes to zero. This function checks for overflow in the multiplication of `num` and `size`.
   - **`realloc(void *block, size_t size)`**: Resizes a previously allocated block to the new size. If the current block is too small, a new block is allocated, the contents are copied over, and the old block is freed.

3. **Thread Safety**:
   - To ensure thread safety, especially when modifying the global linked list of memory blocks or adjusting the program break, all memory management functions are protected using a `pthread_mutex_t` lock. This prevents data races and ensures that memory operations are atomic.

4. **Memory Allocation Strategy**:
   - The memory manager uses a **first-fit strategy** to find a free block that is large enough to satisfy the allocation request. If no suitable block is found, it expands the heap using `sbrk`.

5. **Debugging and Monitoring**:
   - **`print_mem_list()`**: A utility function that prints the current state of the memory blocks. It provides a snapshot of the linked list of memory blocks, showing each block's address, size, free status, and the address of the next block. This is useful for debugging and understanding how memory is being managed and utilized.

#### Potential Use Cases

- **Learning and Educational Purposes**: This code is an excellent tool for learning how dynamic memory allocation works at a low level in C, offering insight into the inner workings of memory management without relying on the standard library.
- **Custom Memory Management Needs**: In environments where control over memory allocation is crucial—such as embedded systems, real-time applications, or systems with limited resources—this custom memory manager can be tailored to specific needs, offering more predictability and control than the standard library.
- **Performance Optimization**: Developers can modify this implementation to optimize memory allocation patterns specific to their application, potentially improving performance by reducing fragmentation or by implementing custom allocation strategies.

#### Limitations and Considerations

- **Not a Drop-in Replacement**: This custom memory manager is not a direct replacement for the standard C library functions (`malloc`, `free`, etc.) and should be used only in environments where complete control over memory allocation is desired.
- **Thread Safety Limitations**: While the current implementation uses mutex locks to ensure thread safety, the underlying `sbrk` function is not thread-safe. In a multi-threaded environment, unexpected results could occur if `sbrk` is called concurrently by multiple threads.
- **Fragmentation**: Like any memory manager, this implementation can suffer from fragmentation, especially if the allocation and deallocation patterns are erratic. Developers should be aware of this and use strategies to mitigate fragmentation if needed.
