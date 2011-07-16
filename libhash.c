#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libhash.h"

hash_table* hash_table_alloc(int hash_size)
{
  hash_table* table = calloc(1,sizeof(hash_table));
  assert(hash_size > 0);
  table->size = hash_size;
  table->table = calloc(table->size,sizeof(hash_table*));
  return table;
}

hash_table* hash_table_free(hash_table* table)
{
  int i;
  hash_bucket* bucket, *next;
  assert(table);
  for ( i = 0; i < table->size; ++ i)
  {
    bucket = table->table[i];
    while(bucket)
    {
      next = bucket->next;
      hash_bucket_free(bucket);
      bucket = next;
    }
  }
  free(table->table);
  free(table);
  return (hash_table*)0;
}

static int hash_function(hash_table* table, int key)
{
  return key % table->size;
}

hash_bucket* hash_table_find(hash_table* table, int key)
{
  hash_bucket* result;
  int position = hash_function(table,key);
  result = table->table[position];
  while ( result && result->next && result->key != key)
    result = result->next;
  return result;
}

void hash_table_insert(hash_table* table, int key,double value)
{
  int position = hash_function(table,key);
  hash_bucket* bucket = table->table[position];
  table->table[position] = hash_bucket_alloc(key,value,bucket);
  table->occupied_size++;
}

void hash_table_print(hash_table* table)
{
  int i;
  hash_bucket* bucket;
  int no_collisions = 0;
  
  for ( i = 0; i < table->size; ++ i)
    if (table->table[i])
    {
      printf("%d: ", i);
      bucket = table->table[i];
      while (bucket)
      {
        printf("%d ", bucket->key);
        bucket = bucket->next;
        if (bucket) no_collisions++;
      }
      printf("\n");
    }
  printf("alpha = %f\n",table->occupied_size/(double)table->size);
}


hash_bucket* hash_bucket_alloc(int key, double value, hash_bucket* next)
{
  hash_bucket* bucket = calloc(1,sizeof(hash_bucket));
  bucket->key = key;
  bucket->value = value;
  bucket->next = next;
  return bucket;
}

hash_bucket* hash_bucket_free(hash_bucket* bucket)
{
  free(bucket);

  return (hash_bucket*)0;
}
