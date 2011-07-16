#ifndef _LIBHASH_H_
#define _LIBHASH_H_

typedef struct hash_bucket hash_bucket;
struct hash_bucket
{
  int key;
  double value;
  hash_bucket* next;
};

typedef struct hash_table_tag
{
  int size;
  int occupied_size;
  hash_bucket** table;
} hash_table;

hash_table* hash_table_alloc(int hash_size);
hash_table* hash_table_free(hash_table* table);


hash_bucket* hash_table_find(hash_table* table, int key);
void hash_table_insert(hash_table* table, int key,double value);

void hash_table_print(hash_table* table);

hash_bucket* hash_bucket_alloc(int key, double value, hash_bucket* next);
hash_bucket* hash_bucket_free(hash_bucket* bucket);


#endif /* _LIBHASH_H_ */
