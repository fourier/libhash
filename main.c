#include <stdio.h>
#include <math.h>
#include "libhash.h"

static void test1()
{
  int i;
  hash_table* hash = hash_table_alloc(6);

  for ( i = 0; i < 13; ++i )
    hash_table_insert(hash,i*2,0.5);
  
  hash_table_print(hash);

  hash_table_remove(hash,12);
  hash_table_remove(hash,20);
  hash_table_remove(hash,0);
  hash_table_remove(hash,1222);

  hash_table_print(hash);

  hash = hash_table_free(hash);
}

int main(int argc, char *argv[])
{
  test1();
  return 0;
}
