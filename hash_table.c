#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "hash_table.h"

static const int HT_PRIME_1 = 151;
static const int HT_PRIME_2 = 163;

static ht_item HT_DELETED_ITEM = {NULL, NULL};

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

  /* Allocate memory for a a new hashtable item */
  ht_item* i = malloc(sizeof(ht_item));
  /* strdup (tutorial recommendation) was throwing errors, so instead create 
    the memory for the string and then copy the string in separate steps */
  i->key = malloc(strlen(k) + 1);
  strcpy(i->key, k);
  i->value = malloc(strlen(v) + 1);
  strcpy(i->value, v);
  return i;
}

/**
 * @brief Create a new hash table
 * 
 * @return ht_hash_table* 
 */
ht_hash_table* ht_new() {

  /* Allocate memory for a new hash table */
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));

  /* Fix the size for now */
  ht->size = 53;
  /* The hash table is empty */
  ht->count = 0;
  /* Create memory for all the hash table items, and initialize to NULL (calloc)
  If an attempt to access an item, and its NULL it means the item has not 
  been initialized */
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
    /* Remember if the memory is NULL it means items[i] has never been used */
    if (item != NULL) {
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

/**
 * @brief Hash function. Takes a string and hashes to an integer between
 * 0 and m
 * 
 * Made the function non static to test/try it in main.c. 
 * In the tutorial online its static but shouldnt make a big difference
 * 
 * @param s string to hash (key)
 * @param a prime number larger than the size of the alphabet
 * @param m max range of hash integer
 * @return int 
 */
int ht_generic_hash(const char* s, const int a, const int m) {

  long hash = 0;
  const int len_s = strlen(s);
  for (int i = 0; i < len_s; i++) {
    hash += (long)pow(a, (len_s - (i + 1))) * s[i];
    /* Make sure the hash value does not exceed m by taking the remainder */
    hash = hash % m;
  }
  return (int)hash;
}

static int ht_hash(const char* s, const int num_buckets, const int attempt) {

  const int hash_a = ht_generic_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_generic_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

/**
 * @brief Insert a new key value pair into a hash table
 * 
 * Iterate through indexes until an empty bucket is found
 * 
 * @param ht 
 * @param key 
 * @param value 
 */
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {

  /* Create the new hash table item */
  printf("Creating new item with key '%s' and value '%s'\n", key, value);
  ht_item* item = ht_new_item(key, value);
  printf("Created new item with key '%s' and value '%s'\n", item->key, item->value);
  /* Get the hash for the new item, this is attempt 0 */
  int index = ht_hash(item->key, ht->size, 0);
  printf("Hashed index is %i\n", index);
  /* Need to check if there is already an item at the calculated hash index */
  ht_item* cur_item = ht->items[index];
  /* Start iterating attempts until a non used index is found */
  int i = 1;
  /* Remember we used calloc to initialze the hash, non used items will be 
   initialized to NULL */
  while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
    /* If we try to update the value for a given key and the key already exists
      in the hash table, delete the old item and update the index with the 
      new item */
    if (strcmp(cur_item->key, key) == 0) {
      ht_del_item(cur_item);
      ht->items[index] = item;
      return;
    }
    index = ht_hash(item->key, ht->size, i);
    cur_item = ht->items[index];
    i++;
  }

  ht->items[index] = item;
  ht->count++;
}

/**
 * @brief Search the hash table for a given key
 * 
 * @param ht 
 * @param key 
 * @return char* 
 */
char* ht_search(ht_hash_table* ht, const char* key) {

  /* Calculate the hash */
  int index = ht_hash(key, ht->size, 0);
  /* Get the item, will be NULL if it does not exist */
  ht_item* item = ht->items[index];
  int i = 1;
  while (item != NULL && item != &HT_DELETED_ITEM) {
    /* Found the matching key. We may have had collisions from the hash, so
      a valid index (not null) does not neccessarily mean we have found the 
      key, need to compare the strings/keys */
    if (strcmp(item->key, key) == 0) {
      return item->value;
    }
    index = ht_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  } 
  return NULL;
}

/**
 * @brief Deletes an item from the hash
 * 
 * A deleted item may be part of the collision chain (attempt 0, 1, 2, ...)
 * and deleteing it will break that chain (for example deteling attempt 1). 
 * To get around this we mark the item as deleted by replacing the pointer
 * 
 * @param ht 
 * @param key 
 */
void ht_delete(ht_hash_table* ht, const char* key) {

  /* Calculate the hash */
  int index = ht_hash(key, ht->size, 0);
  /* Get the item, will be NULL if it does not exist */
  ht_item* item = ht->items[index];
  int i = 1;
  while (item != NULL) {

    if (item != &HT_DELETED_ITEM) {
      /* Found a valid key, remember could be part of a collision so a valid
        index does not neccessarily correspond to the correct key */
      if (strcmp(item->key, key) == 0) {
        /* Delete (Free) the item */
        ht_del_item(item);
        /* Replace the item with a null pointer */
        ht->items[index] = &HT_DELETED_ITEM;
      }
    }
    index = ht_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  /* Decrement the counter of number of items in the hash table */
  ht->count--;
}