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

## `sbrk()`: A Detailed Overview

`sbrk()` is a system call in Unix-like operating systems that is used to manage the program's data segment, also known as the heap. The heap is a portion of a process's memory that is used for dynamic memory allocation, which is where `malloc`, `calloc`, `realloc`, and `free` operate. 

#### What is `sbrk()`?

The `sbrk()` system call is used to increment or decrement the program's data segment (heap). It essentially adjusts the "program break," which is the end of the process's data segment. The data segment is initially set up by the operating system when a program is loaded into memory, but it can be expanded or contracted using `sbrk()`.

- **Syntax**:
  ```c
  void *sbrk(intptr_t increment);
  ```

  - **`increment`**: The amount by which the program break should be increased or decreased. A positive value increases the size of the heap, while a negative value decreases it.
  - **Return Value**: `sbrk()` returns the previous program break address before the increment is applied. If there is an error (e.g., the increment would exceed the process's allowed data segment size), it returns `(void *)-1`.

#### How Does `sbrk()` Work?

When a process needs more heap memory, `sbrk()` can be called to move the program break forward by a certain amount. This effectively increases the size of the heap, allowing more memory to be allocated. Conversely, if a process wants to release some of its heap memory back to the system, it can call `sbrk()` with a negative increment to reduce the size of the heap.

The actual memory allocation typically happens in conjunction with the `malloc` family of functions. When `malloc` requests more memory than is currently available in the free list, it might call `sbrk()` to request more memory from the OS.

#### Examples of Using `sbrk()`

1. **Expanding the Heap**:
   ```c
   #include <unistd.h>
   #include <stdio.h>

   int main() {
       // Current end of the data segment
       void *initial_break = sbrk(0);
       printf("Initial program break: %p\n", initial_break);

       // Expand the heap by 1024 bytes
       if (sbrk(1024) == (void *)-1) {
           perror("sbrk");
           return 1;
       }

       void *new_break = sbrk(0);
       printf("New program break: %p\n", new_break);

       return 0;
   }
   ```

   **Output**:
   ```
   Initial program break: 0x55d1e0
   New program break: 0x55d220
   ```

   In this example, the program break is increased by 1024 bytes, effectively expanding the heap.

2. **Reducing the Heap**:
   ```c
   #include <unistd.h>
   #include <stdio.h>

   int main() {
       // Expand heap by 1024 bytes
       void *initial_break = sbrk(1024);
       printf("Initial break after expanding: %p\n", initial_break);

       // Reduce heap by 512 bytes
       if (sbrk(-512) == (void *)-1) {
           perror("sbrk");
           return 1;
       }

       void *reduced_break = sbrk(0);
       printf("Break after reducing: %p\n", reduced_break);

       return 0;
   }
   ```

   **Output**:
   ```
   Initial break after expanding: 0x55d1e0
   Break after reducing: 0x55d1a0
   ```

   This example shows how to reduce the heap size by moving the program break backwards.

#### Use Cases of `sbrk()`

1. **Custom Memory Allocators**:  
   - `sbrk()` was historically used in custom memory management routines where the programmer wanted finer control over the allocation and deallocation of memory directly at the heap level.
   - Low-level systems programming often involved using `sbrk()` to implement memory pools, custom allocation schemes, and to manage fragmented memory effectively.

2. **Dynamic Memory Management Libraries**:
   - Early implementations of dynamic memory management functions like `malloc`, `calloc`, and `realloc` internally used `sbrk()` to request additional memory from the OS when the free list was exhausted.

3. **Legacy Software Systems**:
   - Older software systems and legacy codebases often relied on `sbrk()` for heap management due to its simplicity and direct control over the heap. Applications written in C in the 1980s and 1990s, as well as some UNIX utilities, might have direct calls to `sbrk()`.

#### Is `sbrk()` Still Used in Modern Codebases?

**Deprecation**:
- `sbrk()` is now considered a deprecated system call. Modern operating systems, including macOS and Linux, discourage its use in favor of more robust and safer memory allocation mechanisms like `mmap()` and `munmap()`.
- `sbrk()` does not provide any guarantees about memory alignment, is not thread-safe, and does not work well with modern virtual memory systems where the heap and other memory regions (like shared libraries or memory-mapped files) need to be managed more flexibly.

**Modern Alternatives**:
- **`mmap()`/`munmap()`**: These system calls provide a more flexible way to manage memory. They allow for allocation and deallocation of memory at a specific address, making them better suited for modern applications that require precise control over memory layout.
- **`malloc` and `free`**: The standard C library’s dynamic memory allocation functions are now backed by more sophisticated memory management techniques, often implemented using `mmap()` or using a combination of `mmap()` and `brk()` (a more flexible version of `sbrk()`).

**Legacy Codebases**:
- Some legacy systems, embedded systems, or older applications still rely on `sbrk()` due to their simplicity and the environment in which they run (e.g., an OS with limited memory management capabilities). However, this is increasingly rare as most systems transition to modern practices.
