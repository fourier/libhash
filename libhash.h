#ifndef _LIBHASH_H_
#define _LIBHASH_H_

typedef enum
{
  EDIVISION,
  EMULTIPLICATIVE
} HashFunctionType;

typedef enum
{
  ECHAINEDHASH,                 /* determines if the collision
                                 * resolution scheme is Chained Hash
                                 */
  /* All the following are for Open Adressing collision resolution scheme */
  ELINEARPROBING,
  EQUADRATICPROBING,
  EDOUBLEHASHING
} CollisionResolutionType;

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
  CollisionResolutionType type;
};

/*
 * Creates a hash table structure. By default the function type
 * is EDIVISION, collision resolution scheme = ECHAINEDHASH
 */
hash_table* ht_alloc(long hash_size);
/*
 * Creates a hash table with specified function type (it has type ECHAINEDHASH)
 */
hash_table* ht_alloc_with_func(long hash_size, HashFunctionType func);
hash_table* ht_alloc_with_type(long hash_size, CollisionResolutionType type);
hash_table* ht_alloc_with_type_and_func(long hash_size,
                                        CollisionResolutionType type,
                                        HashFunctionType func);

hash_table* ht_free(hash_table* table);

hash_bucket* ht_find(hash_table* table, long key);
void ht_insert(hash_table* table, long key,double value);
void ht_remove(hash_table* table, long key);
void ht_print(hash_table* table);

hash_bucket* hash_bucket_alloc(long key, double value, hash_bucket* next);
hash_bucket* hash_bucket_free(hash_bucket* bucket);


#endif /* _LIBHASH_H_ */
