#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#include "libhash.h"
#include "primes.h"

/* Constant for hash function from K&P*/
const int MULTIPLIER = 31;      /* other can be 37 */

/* Constant for multiplication hash function */
const double A = 0.6180339887498949;


/*
 * Hash functions
 */

/* Simple divide-based hash function */
static long hash_function_div(hash_table* table, long key)
{
  return key % table->size;
}

/*
 * Hash function for strings, from K&P "The Practice of Programming"
 * chapter 2.9
 */

static
long hash_function_practice_of_programming(hash_table* table,const char* str)
{
  long h = 0;
  unsigned char* p;
  for ( p = (unsigned char*)str; *p != '\0'; ++p )
    h = MULTIPLIER * h + *p;
  return h % table->size;
}

static
long hash_function_mul(hash_table* table,long key)
{
  long h = 0;
  double r;
  r = key*A;
  r = r - floor(r);
  h = floor(table->size*r);
  return h;
}

/*
 * Other hash-table related functions 
 */

hash_table* hash_table_alloc(long hash_size)
{
  int i;
  hash_table* table = calloc(1,sizeof(hash_table));
  assert(hash_size > 0);
  /* find first prime number > hash_size */
  for ( i = 0; i < PRIMES_COUNT; ++ i)
    if (PRIMES[i] >= hash_size)
    {
      table->size = PRIMES[i];
      break;
    }
  if (!table->size)
    table->size = hash_size;

  table->table = calloc(table->size,sizeof(hash_table*));
  table->hash = hash_function_mul;
  return table;
}

hash_table* hash_table_free(hash_table* table)
{
  long i;
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


hash_bucket* hash_table_find(hash_table* table, long key)
{
  hash_bucket* result = (hash_bucket*)0;
  long position = table->hash(table,key);
  result = table->table[position];
  while ( result && result->key != key)
    result = result->next;
  return result;
}

void hash_table_insert(hash_table* table, long key,double value)
{
  long position = table->hash(table,key);
  hash_bucket* bucket = table->table[position];
  table->table[position] = hash_bucket_alloc(key,value,bucket);
  table->occupied_size++;
}

void hash_table_remove(hash_table* table, long key)
{
  hash_bucket* bucket = (hash_bucket*)0;
  hash_bucket* prev = (hash_bucket*)0;
  long position = table->hash(table,key);
  bucket = table->table[position];
  while ( bucket && bucket->key != key)
  {
    prev = bucket;
    bucket = bucket->next;
  }
  if (bucket)                   /* found bucket to delete */
  {
    if ( prev)
      prev->next = bucket->next;
    else
    {
      prev = table->table[position]->next;
      table->table[position] = prev;
    }
    hash_bucket_free(bucket);
  }
}

void hash_table_print(hash_table* table)
{
  long i;
  hash_bucket* bucket;
  long no_collisions = 0;
  
  for ( i = 0; i < table->size; ++ i)
    if (table->table[i])
    {
      printf("%ld: ", i);
      bucket = table->table[i];
      while (bucket)
      {
        printf("%ld ", bucket->key);
        bucket = bucket->next;
        if (bucket) no_collisions++;
              
      }
      printf("\n");
    }
  printf("hash table size = %ld, occupied = %ld\n",
         table->size,table->occupied_size);
  printf("alpha = %f\n",table->occupied_size/(double)table->size);
  printf("number of collisions = %ld, collisions to occupied ratio = %f\n",
         no_collisions,no_collisions/(double)table->occupied_size);
}


hash_bucket* hash_bucket_alloc(long key, double value, hash_bucket* next)
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
