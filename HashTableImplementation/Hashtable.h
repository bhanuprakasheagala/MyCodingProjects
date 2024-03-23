#ifndef HASHTABLE_H
#define HASHTABLE_H

// Define HashTable datastructure
typedef struct Ht_item {
    char *key;
    char *value;
}Ht_item;

// Implement the overflow bucket list
typedef struct LinkedList {
    Ht_item *item;
    struct LinkedList *next;
}LinkedList;

// Now, the HashTable has an array of pointers that point to Ht_item, so it is a double pointer
typedef struct HashTable {
    Ht_item **items;
    LinkedList **overflow_buckets;
    int size;
    int count;
}HashTable;

// Function declarations
HashTable* create_table(int size);
void free_table(HashTable *table);
void ht_insert(HashTable *table, char *key, char *value);
char* ht_search(HashTable *table, char *key);
void ht_delete(HashTable *table, char *key);
void print_search(HashTable *table, char *key);
void print_table(HashTable *table);

#endif
