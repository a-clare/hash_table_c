#include "hash_table.h"
#include <stdio.h>

int main() {
  ht_hash_table* ht = ht_new();
  ht_del_hash_table(ht);

  printf("The hash of hash('cat', 151, 53) is %d\n", 
    ht_hash("cat", 151, 53));

  printf("The hash of hash('cat', 163, 53) is %d\n", 
    ht_hash("cat", 163, 53));
    
  return 0;
}