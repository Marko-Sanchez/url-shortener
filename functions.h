#ifndef functions
#define functions

#include <stdio.h>

#include <mongoc/mongoc.h>

void b62_converter(char* , char [] );
bool insert_DB( mongoc_client_t*, char*, char*);
bool query_DB(mongoc_client_t*, char*);

#endif

