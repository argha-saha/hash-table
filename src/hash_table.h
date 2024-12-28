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

#endif // HASH_TABLE_H
