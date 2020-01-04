#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

/**
 * @brief Create a new hash table item
 * 
 * Function is static since it will only be called internally to the hash table
 * 
 * @param k key for item
 * @param v value for item
 * @return ht_item* pointer to new hash table item
 */
static ht_item* ht_new_item(const char* k, const char* v) {

  // Allocate memory for a a new hashtable item
  ht_item* i = malloc(sizeof(ht_item));
  // Duplicate and store the key and value. strdup() uses malloc so will need
  // to make sure to free the memory later
  i->key = strdup(k);
  i->value = strdup(v);
  return i;
}

/**
 * @brief Create a new hash table
 * 
 * @return ht_hash_table* 
 */
ht_hash_table* ht_new() {

  // Allocate memory for a new hash table
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));

  // Fix the size for now
  ht->size = 53;
  // The hash table is empty
  ht->count = 0;
  // Create memory for all the hash table items, and initialize to NULL (calloc)
  // If an attempt to access an item, and its NULL it means the item has not 
  // been initialized
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
}

/**
 * @brief Delete a hash table item, frees the memory used by the key, value,
 *  and the hash item
 * 
 * @param i 
 */
static void ht_del_item(ht_item* i) {
  free(i->key);
  free(i->value);
  free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    // Remember if the memory is NULL it means items[i] has never been used
    if (item != NULL) {
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}