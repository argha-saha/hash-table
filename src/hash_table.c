#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

// FNV-1a constants
const uint32_t FNV_OFFSET_BASIS = 2166136261U;
const uint32_t FNV_PRIME = 16777619U;

// Allocate memory for a hash table item
static ht_item_t* ht_new_item(const char* key, const char* value) {
  if (key == NULL || value == NULL) {
    return NULL;
  }

  ht_item_t* item = malloc(sizeof(ht_item_t));

  if (item == NULL) {
    fprintf(stderr, "Hash table item memory allocation failed.\n");
    return NULL;
  }

  item->key = strdup(key);
  item->value = strdup(value);

  return item;
}

// Initialize a new hash table
ht_table_t* ht_new_table(size_t ht_size) {
  if (!ht_size) {
    ht_size = 64;
  }

  ht_table_t* table = malloc(sizeof(ht_table_t));

  if (table == NULL) {
    fprintf(stderr, "Hash table memory allocation failed.\n");
    return NULL;
  }

  table->size = ht_size;
  table->item_count = 0;
  table->items = calloc(table->size, sizeof(ht_item_t*));
  
  if (table->items == NULL) {
    fprintf(stderr, "Item pointer memory allocation failed.\n");
    free(table);
    return NULL;
  }

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
  if (table == NULL) {
    return;
  }

  for (size_t i = 0; i < table->size; ++i) {
    ht_item_t* item = table->items[i];

    if (item != NULL) {
      ht_delete_item(item);
    }
  }

  free(table->items);
  free(table);
}

// FNV-1a hash function
static uint32_t ht_fnv1a_hashing(const char* s) {
  uint32_t hash = FNV_OFFSET_BASIS;

  for (size_t i = 0; s[i] != '\0'; ++i) {
    hash ^= (uint32_t) s[i];
    hash *= FNV_PRIME;
  }

  return hash;
}

// Insert a key-value pair into the hash table
void ht_insert(ht_table_t* table, const char* key, const char* value) {
  if (table == NULL || key == NULL || value == NULL) {
    return;
  }

  uint32_t hash_value = ht_fnv1a_hashing(key);
  size_t index = hash_value % table->size;
  size_t test_index;
  ht_item_t* current_item;

  // Linear probing
  for (size_t i = 0; i < table->size; ++i) {
    test_index = (index + i) % table->size;
    current_item = table->items[test_index];

    if (current_item == NULL) {
      // Empty slot
      table->items[test_index] = ht_new_item(key, value);
      table->item_count++;
      return;
    } else if (strcmp(current_item->key, key) == 0) {
      // Key match -> update value
      free(current_item->value);
      current_item->value = strdup(value);
      return;
    }
  }

  fprintf(stderr, "Hash table is full.\n");
}

char* ht_search(ht_table_t* table, const char* key) {
  if (table == NULL || key == NULL) {
    return NULL;
  }

  uint32_t hash_value = ht_fnv1a_hashing(key);
  size_t index = hash_value % table->size;
  size_t test_index;
  ht_item_t* current_item;

  // Linear probing
  for (size_t i = 0; i < table->size; ++i) {
    test_index = (index + i) % table->size;
    current_item = table->items[test_index];

    if (current_item == NULL) {
      return NULL;
    } else if (strcmp(current_item->key, key) == 0) {
      // Key match -> return value
      return current_item->value;
    }
  }

  return NULL;
}

void ht_delete_table_item(ht_table_t* table, const char* key) {
  if (table == NULL || key == NULL) {
    return;
  }

  uint32_t hash_value = ht_fnv1a_hashing(key);
  size_t index = hash_value % table->size;
  size_t test_index;
  ht_item_t* current_item;

  // Linear probing
  for (size_t i = 0; i < table->size; ++i) {
    test_index = (index + i) % table->size;
    current_item = table->items[test_index];

    if (current_item == NULL) {
      return;
    } else if (strcmp(current_item->key, key) == 0) {
      // Key match -> delete item
      ht_delete_item(current_item);
      table->items[test_index] = NULL;
      table->item_count--;
      return;
    }
  }
}

int main() {
  // Test: ht_new_item()
  char* test_key = "key1";
  char* test_val = "value1";
  ht_item_t* item = ht_new_item(test_key, test_val);

  if (item == NULL) {
    printf("ht_new_item() FAIL: Nemory allocation failed.\n");
  } else if (strcmp(item->key, test_key) || strcmp(item->value, test_val)) {
    printf("ht_new_item() FAIL: Failed to set key and value.\n");
  } else {
    printf("ht_new_item() PASS: Created a valid item. Key: '%s', Value: '%s'\n", item->key, item->value);
  }

  // Test: ht_delete_item()
  ht_delete_item(item);
  printf("ht_delete_item(): Must check leaks/valgrind to verify correctness.\n");

  // Test: ht_new_table()
  size_t table_size = 64;
  ht_table_t* table = ht_new_table(table_size);

  if (table == NULL) {
    printf("ht_new_table() FAIL: Memory allocation failed.\n");
  } else if (table->size != table_size || table->item_count != 0 || table->items == NULL) {
    printf("ht_new_table() FAIL: Failed to initialize table.\n");
  } else {
    printf("ht_new_table() PASS: Created a valid table.\n");
  }

  // Test: ht_fnv1a_hashing()
  uint32_t hash = ht_fnv1a_hashing("hashtable");
  printf("ht_fnv1a_hashing(): Hash value for 'hashtable' is %u.\n", hash);

  // Test: ht_insert()
  if (table) {
    const char* insert_key = "insertkeytest";
    const char* insert_val = "insertvaluetest";
    ht_insert(table, insert_key, insert_val);

    size_t expected_index = ht_fnv1a_hashing(insert_key) % table->size;
    ht_item_t* inserted_item = table->items[expected_index];

    if (inserted_item == NULL) {
      printf("ht_insert() FAIL: Failed to insert item.\n");
    } else if (strcmp(inserted_item->key, insert_key) || strcmp(inserted_item->value, insert_val)) {
      printf("ht_insert() FAIL: Failed to set key and value.\n");
    } else {
      printf("ht_insert() PASS: Inserted a valid item. Key: '%s', Value: '%s'\n", inserted_item->key, inserted_item->value);
      printf("table->item_count: %zu\n", table->item_count);
    }
  }

  // Test: ht_search()
  if (table) {
    const char* search_key = "insertkeytest";
    const char* search_val = ht_search(table, search_key);

    if (search_val == NULL) {
      printf("ht_search() FAIL: Failed to find key.\n");
    } else if (strcmp(search_val, "insertvaluetest")) {
      printf("ht_search() FAIL: Failed to return correct value.\n");
    } else {
      printf("ht_search() PASS: Found the correct value. Key: '%s', Value: '%s'\n", search_key, search_val);
    }
  }

  // Test: ht_delete_table_item()
  if (table) {
    size_t current_item_count = table->item_count;
    
    ht_delete_table_item(table, "insertkeytest");

    size_t expected_index = ht_fnv1a_hashing("insertkeytest") % table->size;
    ht_item_t* deleted_item = table->items[expected_index];


    if (deleted_item != NULL) {
      printf("ht_delete_table_item() FAIL: Failed to delete item.\n");
    } else {
      printf("ht_delete_table_item() PASS: Deleted item successfully.\n");
    }

    if (table->item_count != current_item_count - 1) {
      printf("ht_delete_table_item() FAIL: Failed to update item count.\n");
    } else {
      printf("ht_delete_table_item() PASS: Updated item count successfully.\n");
    }
  }

  // Test: ht_delete_table()
  ht_delete_table(table);
  printf("ht_delete_table(): Must check leaks/valgrind to verify correctness.\n");

  return EXIT_SUCCESS;
}
