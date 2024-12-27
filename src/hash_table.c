#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

// Allocate memory for a hash table item
static ht_item_t* ht_new_item(const char* key, const char* value) {
  if (key == NULL || value == NULL) {
    return NULL;
  }

  ht_item_t* item = malloc(sizeof(ht_item_t));

  if (item == NULL) {
    fprintf("Hash table item memory allocation failed.\n");
    return NULL;
  }

  item->key = strdup(key);
  item->value = strdup(value);

  return item;
}

// Initialize a new hash table
ht_table_t* ht_new_table() {
  ht_table_t* table = malloc(sizeof(ht_table_t));

  if (table == NULL) {
    fprintf(stderr, "Hash table memory allocation failed.\n");
    return NULL;
  }

  table->size = 63;
  table->item_count = 0;
  table->items = malloc(sizeof(ht_item_t*) * table->size);
  
  if (table->items == NULL) {
    fprintf("Item pointer memory allocation failed.\n");
    free(table);
    return NULL;
  }

  memset(table->items, 0, sizeof(ht_item_t*) * table->size);

  return table;
}

// Free the memory allocated for an item
static void ht_delete_item(ht_item_t* item) {
  if (item == NULL) {
    return;
  }

  free(item->key);
  free(item->value);
  free(item);
}

// Free the memory allocated for a table
void ht_delete_table(ht_table_t* table) {
  for (int i = 0; i < table->size; ++i) {
    ht_item_t* item = table->items[i];

    if (item != NULL) {
      ht_delete_item(item);
    }
  }

  free(table->items);
  free(table);
}
