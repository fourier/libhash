#ifndef _LIBHASH_H_
#define _LIBHASH_H_

typedef struct hash_bucket hash_bucket;
struct hash_bucket
{
  long key;
  double value;
  hash_bucket* next;
};

typedef struct hash_table hash_table;
typedef long (*hash_function_t) (hash_table* table,long key);

struct hash_table
{
  long size;
  long occupied_size;
  hash_bucket** table;
  hash_function_t hash;
};

hash_table* hash_table_alloc(long hash_size);
hash_table* hash_table_free(hash_table* table);


hash_bucket* hash_table_find(hash_table* table, long key);
void hash_table_insert(hash_table* table, long key,double value);
void hash_table_remove(hash_table* table, long key);
void hash_table_print(hash_table* table);

hash_bucket* hash_bucket_alloc(long key, double value, hash_bucket* next);
hash_bucket* hash_bucket_free(hash_bucket* bucket);


#endif /* _LIBHASH_H_ */
