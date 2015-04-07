/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 Copyright (C) 2011 Alexey Veretennikov (alexey dot veretennikov at gmail.com)
 
 This file is part of libhash.

 libhash is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 libhash is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with libhash.  If not, see <http://www.gnu.org/licenses/>.
*/

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

/* Constant marking deleted entry in the table with open adressing */
hash_bucket* DELETED = (hash_bucket*)(-1);

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

static
long hash_function_open_address(hash_table* table,long key,long i)
{
  long result = 0;
  /*
   * Constants for quadratic probing.
   * From http://en.wikipedia.org/wiki/Quadratic_probing could be
   * c1 = c2 = 1/2, c1 = c2 = 1, and c1 = 0,c2 = 1
   */
  double c1 = 0.5,c2 = 0.5;
  switch(table->type)
  {
  case ELINEARPROBING:
    result = (table->hash(table,key) + i) % table->size;
    break;
  case EQUADRATICPROBING:
    result = (table->hash(table,key) + (int)(c1*i) + (int)(c2*i*i))
      % table->size;
    break;
  case EDOUBLEHASHING:
    break;
  case ECHAINEDHASH:
  default:
    assert(0);
    break;
  }
  return result;
}

/*
 * hash-table related functions 
 */
hash_table* ht_alloc_with_type_and_func(long hash_size,
                                        CollisionResolutionType type,
                                        HashFunctionType func)
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
  table->type = type;
  switch (func)
  {
  case EMULTIPLICATIVE:
    table->hash = hash_function_mul;
    break;
  case EDIVISION:
    table->hash = hash_function_div;
    break;
  default:
    assert(0);
    break;
  }
  return table;
}

hash_table* ht_alloc_with_func(long hash_size,HashFunctionType func)
{
  return ht_alloc_with_type_and_func(hash_size,ECHAINEDHASH,func);
}

hash_table* ht_alloc_with_type(long hash_size,CollisionResolutionType type)
{
  return ht_alloc_with_type_and_func(hash_size,type,EDIVISION);
}

hash_table* ht_alloc(long hash_size)
{
  return ht_alloc_with_type_and_func(hash_size,ECHAINEDHASH,EDIVISION);
}

hash_table* ht_free(hash_table* table)
{
  long i;
  hash_bucket* bucket, *next;
  assert(table);
  for ( i = 0; i < table->size; ++ i)
  {
    bucket = table->table[i];
    while(bucket && bucket != DELETED)
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

static
long ht_find_open_addressing(hash_table* table, long key)
{
  hash_bucket* result = (hash_bucket*)0;
  long i = 0, j = 0;
  assert(table->type != ECHAINEDHASH);
  do
  {
    j = hash_function_open_address(table,key,i);
    result = table->table[j];
    if ( result == DELETED)
    {
      result = (hash_bucket*)0;
      break;
    }
    if ( result && result->key == key)
      break;
    i++;
  } while ( table->table[j] && i < table->size);
  return result ? j : -1;
}

hash_bucket* ht_find(hash_table* table, long key)
{
  hash_bucket* result = (hash_bucket*)0;
  long i = 0, j = 0;
  if ( table->type == ECHAINEDHASH)
  {
    i = table->hash(table,key);
    result = table->table[i];
    while ( result && result->key != key)
      result = result->next;
  }
  else                          /* open address hash */
  {
    j = ht_find_open_addressing(table,key);
    if ( j != -1 )
      result = table->table[j];
  }
  return result;
}


void ht_insert(hash_table* table, long key,double value)
{
  long i = 0, j = 0;
  hash_bucket* bucket = (hash_bucket*)0;
  printf("attempt to insert: %ld\n",key);
  if (table->type == ECHAINEDHASH)
  {
    i = table->hash(table,key);
    bucket = table->table[i];
    table->table[i] = hash_bucket_alloc(key,value,bucket);
    table->occupied_size++;
  }
  else                          /* open adressing hash */
  {
    while ( i < table->size )
    {
      j = hash_function_open_address(table,key,i);
      if ( !table->table[j] || table->table[j] == DELETED)
      {
        table->table[j] = hash_bucket_alloc(key,value,bucket);
        table->occupied_size++;
        return;
      }
      i++;
    }
    fprintf(stderr,"Hash table overflow! ");
    fprintf(stderr,"size = %ld, occupied = %ld\n",
            table->size,table->occupied_size);
  }
}

void ht_remove(hash_table* table, long key)
{
  hash_bucket* bucket = (hash_bucket*)0;
  hash_bucket* prev = (hash_bucket*)0;
  long position;
  printf("attempt to remove: %ld\n",key);
  if (table->type == ECHAINEDHASH)
  {
    position = table->hash(table,key);
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
      table->occupied_size--;
    }
  }
  else                          /* open addressing hash */
  {
    position = ht_find_open_addressing(table,key);
    if ( position != -1 )
    {
      hash_bucket_free(table->table[position]);
      table->table[position] = DELETED;
      table->occupied_size--;
    }
  }
}

void ht_print(hash_table* table)
{
  long i;
  hash_bucket* bucket;
  long no_collisions = 0;
  
  for ( i = 0; i < table->size; ++ i)
  {
    bucket = table->table[i];
    if (bucket)
    {
      printf("%ld: ", i);
      if (bucket != DELETED)
      {
        while (bucket)
        {
          printf("%ld ", bucket->key);
          bucket = bucket->next;
          if (bucket) no_collisions++;
              
        }
      }
      else
        printf("DELETED");
      printf("\n");
    }
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
