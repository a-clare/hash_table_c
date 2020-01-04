#include "hash_table.h"
#include <stdio.h>

int main() {
  ht_hash_table* ht = ht_new();

  ht_insert(ht, "cat", "dog");

  char* value = ht_search(ht, "cat");
  printf("The current value for 'cat' in the hash table is %s\n", 
    ht_search(ht, "cat"));

  printf("Updating 'cat' to 'hamster'");
  ht_insert(ht, "cat", "hamster");
  printf("The current value for 'cat' in the hash table is %s\n", 
    ht_search(ht, "cat"));

  ht_del_hash_table(ht);
  return 0;
}