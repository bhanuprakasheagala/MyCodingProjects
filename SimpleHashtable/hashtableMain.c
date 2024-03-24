#include "Hashtable.h"
#include "constants.h"
#include <stdio.h>

int main() {
    HashTable* ht = create_table(CAPACITY);
    ht_insert(ht, "1", "First address");
    ht_insert(ht, "2", "Second address");
    ht_insert(ht, "Hel", "Third address");
    ht_insert(ht, "lo", "Fourth address");

    print_search(ht, "1");
    print_search(ht, "2");
    print_search(ht, "4");

    print_table(ht);
    free_table(ht);

    return 0;
}