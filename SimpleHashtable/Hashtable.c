#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Hashtable.h" // Include the header file where function declarations are defined
#include "constants.h"

// Function to create the overflow buckets (linked lists)
LinkedList** create_overflow_buckets(HashTable* table) {
    LinkedList** buckets = (LinkedList**)calloc(table->size, sizeof(LinkedList*));

    for(int i = 0; i < table->size; i++) {
        buckets[i] = NULL;
    }

    return buckets;
}

// Function to free a linked list
void free_linkedlist(LinkedList* list) {
    LinkedList* temp = list;

    while(list) {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}

// Function to free all overflow bucket lists
void free_overflow_buckets(HashTable* table) {
    LinkedList** buckets = table->overflow_buckets;
    for(int i = 0; i < table->size; ++i) {
        free_linkedlist(buckets[i]);
    }
    free(buckets);
}

// Hash function
unsigned long hash_function(char *str) {
    unsigned long i = 0;

    for(int j = 0; str[j]; j++) {
        i += str[j];
    }

    return i % CAPACITY;
}

// Function to allocate memory for a linked list
LinkedList* allocate_list() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    return list;
}

// Function to insert an item into a linked list
LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item) {
    if(!list) {
        LinkedList* head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    } else if(list->next == NULL) {
        LinkedList* node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }
    LinkedList* temp = list;
    while(temp->next->next) {
        temp = temp->next;
    }
    LinkedList* node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    return list;
}

// Function to remove the head of a linked list and return the item of the popped element
Ht_item* linkedlist_remove(LinkedList* list) {
    if(!list || !list->next) {
        return NULL;
    }
    LinkedList* node = list->next;
    LinkedList* temp = list;
    temp->next = NULL;
    list = node;
    Ht_item* it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item->key);
    free(temp->item->value);
    free(temp->item);
    free(temp);
    return it;
}

// Function to create an item
Ht_item* create_item(char* key, char* value) {
    Ht_item* item = (Ht_item*)malloc(sizeof(Ht_item));
    item->key = (char*)malloc(strlen(key) + 1);
    item->value = (char*)malloc(strlen(value) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

// Function to create the table
HashTable* create_table(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**)calloc(table->size, sizeof(Ht_item*));
    for(int i = 0; i < table->size; ++i) {
        table->items[i] = NULL;
    }
    table->overflow_buckets = create_overflow_buckets(table);
    return table;
}

// Function to free an item
void free_item(Ht_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}

// Function to handle collisions
void handle_collision(HashTable *table, unsigned long index, Ht_item *item) {
    LinkedList *head = table->overflow_buckets[index];
    if(head == NULL) {
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    } else {
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
}

// Function to insert into the hash table
void ht_insert(HashTable* table, char* key, char* value) {
    Ht_item* item = create_item(key, value);
    int index = hash_function(key);
    Ht_item* current_item = table->items[index];
    if(current_item == NULL) {
        if(table->count == table->size) {
            printf("Insert Error: HashTable is full\n");
            free_item(item);
            return;
        }
        table->items[index] = item;
        table->count++;
    } else {
        if(strcmp(current_item->key, key) == 0) {
            strcpy(table->items[index]->value, value);
            return;
        } else {
            handle_collision(table, index, item);
            return;
        }
    }
}

// Function to search for an item in the hash table
char* ht_search(HashTable* table, char* key) {
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];
    if(item != NULL) {
        if(strcmp(item->key, key) == 0) {
            return item->value;
        }
        if(head == NULL)
            return NULL;
        item = head->item;
        head = head->next;
    }
    return NULL;
}

// Function to delete an item from the hash table
void ht_delete(HashTable *table, char *key) {
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];
    if(item == NULL) {
        printf("Empty!!\n");
        return;
    } else {
        if(head == NULL && strcmp(item->key, key) == 0) {
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        } else if(head != NULL) {
            if(strcmp(item->key, key) == 0) {
                free_item(item);
                LinkedList *node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }
            LinkedList *curr = head;
            LinkedList *prev = NULL;
            while(curr) {
                if(strcmp(curr->item->key, key) == 0) {
                    if(prev == NULL) {
                        // First element of the chain : Remove the chain
                        free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else {
                        // This is somewhere in the chain
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;

                        return;
                    }
                }
                curr = curr->next;
                prev = curr;
            }
        }
    }
}
void free_table(HashTable* table) {
    // Frees the table
    for(int i=0; i<table->size; ++i) {
        Ht_item* item = table->items[i];
        if(item != NULL) {
            free_item(item);
        }
    }
    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

// Print table
void print_table(HashTable* table) {
    printf("\nHashTable\n-----------------------\n");
    for(int i=0; i<table->size; ++i) {
        if(table->items[i]) {
            printf("Index:%d, Key:%s, Value:%s\n", i, table->items[i]->key, table->items[i]->value);
        }
    }

    printf("----------------------\n\n");
}

// Display the item that matches the search key
void print_search(HashTable* table, char* key) {
    char* val;
    if((val = ht_search(table, key)) == NULL) {
        printf("Key: %s does not exist\n", key);
        return;
    }
    else {
        printf("Key: %s, Value: %s\n", key, val);
    }
}
