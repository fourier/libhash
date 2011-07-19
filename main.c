#include <stdio.h>
#include <math.h>
#include "libhash.h"

static void test1()
{
  int i;
  hash_table* hash = ht_alloc_with_type(6,ELINEARPROBING);

  for ( i = 0; i < 13; ++i )
    ht_insert(hash,i*2,0.5);
  
  ht_print(hash);

  ht_remove(hash,12);
  ht_print(hash);
  ht_remove(hash,20);
  ht_print(hash);
  ht_remove(hash,0);
  ht_print(hash);
  ht_remove(hash,1222);
  ht_print(hash);

  hash = ht_free(hash);
}

int main(int argc, char *argv[])
{
  test1();
  return 0;
}
