# HashTable Implementation

This project implements a simple HashTable data structure in C. The HashTable uses separate chaining to handle collisions. The implementation includes functions for creating a HashTable, inserting items, searching for items, deleting items, and freeing memory.

## Table of Contents

- [Introduction](#introduction)
- [Code Explanation](#code-explanation)
- [Execution](#execution)

## Introduction

A HashTable is a data structure that allows for efficient lookup, insertion, and deletion of key-value pairs. In this implementation, we use a simple hash function to map keys to indices in an array. If two keys hash to the same index (a collision), we handle it using separate chaining, where each index in the array holds a linked list of key-value pairs.

## Code Explanation

### 1. Hash Table Creation and Initialization:
The _create_table_ function allocates memory for the hash table and initializes its properties such as size, count, and the array of items.
It initializes each bucket in the hash table to NULL pointers to linked lists using the _create_overflow_buckets_ function. This function allocates memory for an array of linked lists to handle collisions.
### 2. Hash Function:
The _hash_function_ computes the hash value for a given string key. It iterates over each character in the key and sums up their ASCII values, then takes the modulo operation with a predefined capacity to ensure the hash value falls within the range of the hash table.
### 3. Item Management:
The _create_item_ function creates a new key-value pair (item) and allocates memory for the key and value strings. It copies the provided key and value into the allocated memory. <br/>
The _free_item_ function deallocates memory for a given item, including its key and value strings.
### 4. Collision Handling:
When a collision occurs during insertion (ht_insert), the _handle_collision_ function is called to resolve it. It either inserts the new item into an existing overflow linked list or creates a new one if it doesn't exist. <br/>
Collision handling ensures that multiple items with different keys but the same hash value can be stored and retrieved correctly.
### 5. Insertion, Search, and Deletion:
_ht_insert_ inserts a new item into the hash table. It computes the hash value for the key, checks for collisions, and inserts the item accordingly. <br/>
_ht_search_ searches for an item in the hash table based on the given key. It computes the hash value and checks both the main table and overflow linked lists for the item.<br/>
_ht_delete_ deletes an item from the hash table based on the given key. It handles various cases, including removing the item from the main table or from an overflow linked list. <br/>
### 6. Printing:
The _print_table_ function prints the contents of the hash table, including index, key, and value, providing insight into the current state of the table.
_print_search_ is a utility function to display the value associated with a specific key in the hash table.

## Execution

**Compile the source files:** <br>
gcc -c hashtableMain.c -o hashtableMain.o <br>
gcc -c Hashtable.c -o Hashtable.o <br>

**Link all the object files into a single file(hashtable)**
gcc hashtableMain.o Hashtable.o -o hashtable <br>

**Run the executable**
./hashtable
