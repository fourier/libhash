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

int main()
{
  test1();
  return 0;
}
