typedef struct {
  char* key;
  char* value;
} ht_item_t;

typedef struct {
  int size;
  int item_count;
  item_t** items;
} ht_table_t;
