[![Build Status](https://travis-ci.org/fourier/libhash.svg?branch=master)](https://travis-ci.org/fourier/libhash)

libhash
=======
Copyright (C) 2011 Alexey Veretennikov (alexey dot veretennikov at gmail dot com)

Description
-----------
Libhash is a Library providing access to easy-to-use hash table implementation.
It is not fast enough, since it is a toy project(not a production one).
Currently it provides the following set of the hashing functions:
 * division hash
 * multiplication hash
 * hash function for string(K&P The Art of Programming)
 * open address hashing with linear and quadratic probes
TODO:
 * Carefully test open address hashing
 * Implement open double hashing in open address hashin
 * Implement several other hash functions
 * Make a generic implementation for keys as strings
 * Implement other hashing schemas, from wiki(http://en.wikipedia.org/wiki/Hash_table):
   * Coalesced hashing
   * Robin Hood hashing
   * Cuckoo hashing
   * Hopscotch hashing
