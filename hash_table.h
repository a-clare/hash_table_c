#ifndef ATC_HASH_TABLE_H
#define ATC_HASH_TABLE_H

/**
 * @brief Struct for holding key value pairs
 * 
 */
typedef struct {
  char* key;
  char* value;
} ht_item;

/**
 * @brief Hash table
 * 
 * Stores an array of pointers to hash items (ht_items)
 * 
 * Contains the size of the hash table (size) and the number of elements 
 * currently in the hash table (count)
 * 
 */
typedef struct {
  int size;
  int count;
  ht_item** items;
} ht_hash_table;

ht_hash_table* ht_new();
void ht_del_hash_table(ht_hash_table* ht);

#endif