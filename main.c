#include "libhash.h"


int main(int argc, char *argv[])
{
  int i;
  hash_table* hash = hash_table_alloc(5);

  for ( i = 0; i < 13; ++i )
    hash_table_insert(hash,i*2,0.5);
  
  hash_table_print(hash);
  hash = hash_table_free(hash);
  return 0;
}
