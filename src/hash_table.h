#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

typedef struct {
  char* key;
  char* value;
} ht_item_t;

typedef struct {
  size_t size;
  size_t item_count;
  ht_item_t** items;
} ht_table_t;

ht_table_t* ht_new_table(size_t ht_size);
void ht_delete_table(ht_table_t* table);
void ht_insert(ht_table_t* table, const char* key, const char* value);
char* ht_search(ht_table_t* table, const char* key);
void ht_delete_table_item(ht_table_t* table, const char* key);

#endif // HASH_TABLE_H
