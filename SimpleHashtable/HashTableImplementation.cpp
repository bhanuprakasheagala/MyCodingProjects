#include <iostream>
#include <stdlib.h>
#include <string.h>

// Defining HashTable Data structure
typedef struct Ht_item {
    char* key;
    char* value;
}Ht_item;

// Implementing the overflow bucket list
typedef struct LinkedList{
    Ht_item* item;
    struct LinkedList* next;
} LinkedList;

// Now, the HashTable has an array of pointers that point to Ht_item, so it is a double-pointer
typedef struct HashTable {
    // Contains an array of pointers to items
    Ht_item** items;
    LinkedList** overflow_buckets;
    int size;
    int count;
} HashTable;

// Create the overflow buckets; An array of linked lists
LinkedList** create_overflow_buckets(HashTable* table) {
    LinkedList** buckets = (LinkedList**)calloc(table->size, sizeof(LinkedList*));

    for(int i=0; i<table->size; i++) {
        buckets[i] = NULL;
    }

    return buckets;
}

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

// Free all the overflow bucket lists
void free_overflow_buckets(HashTable* table) {
    LinkedList** buckets = table->overflow_buckets;
    for(int i=0; i<table->size; ++i) {
        free_linkedlist(buckets[i]);
    }
    free(buckets);
}
// Choosing a hash function
#define CAPACITY 500 // Size of the HashTable
unsigned long hash_function(char *str) {
    unsigned long i = 0;

    for(int j=0; str[j]; j++) {
        i += str[j];
    }

    return i%CAPACITY;
}


// Allocates memory for a LinkedList pointer
LinkedList* allocate_list() {
    
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    return list;
}

// Inserts the item onto the LinkedList
LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item) {
    
    if(!list) {
        LinkedList* head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;

        return list;
    }
    else if(list->next == NULL) {
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
// Removes the head of the LinkedList, returns the item of the popped element.
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


// Create items by allocating memory for a key and value, and return a pointer to the item
Ht_item* create_item(char* key, char* value) {
    Ht_item* item = (Ht_item*)malloc(sizeof(Ht_item));
    item->key = (char*)malloc(strlen(key)+1);
    item->value = (char*)malloc(strlen(value)+1);
    strcpy(item->key, key);
    strcpy(item->value, value);

    return item;
}

// Create the table by allocating memory and setting size, count, and items
HashTable* create_table(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**)calloc(table->size, sizeof(Ht_item*));

    for(int i=0; i<table->size; ++i) {
        table->items[i] = NULL;
    }
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

// Write functions that free up a table item and the whole table
void free_item(Ht_item* item) {
    // Frees an item
    free(item->key);
    free(item->value);
    free(item);
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

void handle_collision(HashTable *table, unsigned long index, Ht_item *item) {
    LinkedList *head = table->overflow_buckets[index];

    if(head == NULL) {
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;

        return;
    }
    else {
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
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

/* Handling Collisions using Separate Chaining
which aims to create independent chains for all items that have the same index.

Whenever there is a collision, additional items that collide on the same index are added to an
overflow bucket list. Thus, we will not have to delete any existing records on the hash table.

Due to linked lists having O(n) time complexity for insertion, searching, and deletion, in case of
a collision, you will have a worst-case access time of O(n) as well.
The advantage of this method is that it is a good choice if your hash table has a low capacity.
*/




// Inserting into the HashTable
void ht_insert(HashTable* table, char* key, char* value) {
    // Creates the item
    Ht_item* item = create_item(key, value);

    // Computes the index
    int index = hash_function(key);

    Ht_item* current_item = table->items[index];

    if(current_item == NULL) {
        // Key does not exist
        if(table->count == table->size) {
            // HashTable is full
            printf("Insert Error: HashTable is full\n");
            free_item(item);
            return;
        }
        // Insert directly
        table->items[index] = item;
        table->count++;
    }
    else {
            // Scenario1: Key Value pair already exists. Update the item value to the new one.
            if(strcmp(current_item->key, key) == 0) {
                strcpy(table->items[index]->value, value);
                return;
            }
            else {
                // Scenario2: Handle the collision.
                handle_collision(table, index, item);
                return;
            }
        }
}

// Searching for an item in the HashTable
char* ht_search(HashTable* table, char* key) {
    // Searchs for the key in the HashTable. Returns NULL if it doesn't exist.
    int index = hash_function(key);
    Ht_item* item = table->items[index];

    LinkedList* head = table->overflow_buckets[index];

    // Provide only non-null values
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

// Delete an item from the table
void ht_delete(HashTable *table, char *key) {
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    if(item == NULL) {
        std::cout << "Empty!!\n";
        return;
    }
    else {
        // Collision chain does not exist : Remove the item and set table index to NULL
        if(head == NULL && strcmp(item->key, key) == 0) {
            table->items[index] = NULL;
            free_item(item);
            table->count--;

            return;
        }
        else if(head != NULL) {
            // Collision chain exists: Remove the this item and set the head of the list as the new item
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

int main()
{
    HashTable* ht = create_table(CAPACITY);
    ht_insert(ht, (char*)"1", (char*)"First address");
    ht_insert(ht, (char*)"2", (char*)"Second address");
    ht_insert(ht, (char*)"Hel", (char*)"Third address");
    ht_insert(ht, (char*)"lo", (char*)"Fourth address");

    print_search(ht, (char*)"1");
    print_search(ht, (char*)"2");
    print_search(ht, (char*)"4");

    print_table(ht);
    free_table(ht);

    return 0;
}