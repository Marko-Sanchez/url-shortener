#include <string.h>
#include <stdio.h>

#include <mongoc/mongoc.h>

#define CUSTOM_URL www.barko.9/

//char* b62_converter(char* url);

int main(int argc,char argv[]){
	
	// Variables:
	mongoc_client_t *client;
	mongoc_database_t *database;
	mongoc_collection_t *collection;

	bson_t *command, reply, *insert;
	bson_error_t error;

	// INIT: libmongoc's internals.
	mongoc_init();

	// Create new client instance:

	database = mongoc_client_get_database( client, "URL_lib");
	collection = mongoc_client_get_collection( client, "URL_lib","coll_name");

	command = BCON_NEW("ping", BCON_INT32(1));

	bool retval = mongoc_client_command_simple(client, "admin", command,NULL,&reply,
			&error);

	if(!retval){
		fprintf( stderr, "%s\n",error.message);
		return EXIT_FAILURE;
	}

	char *str = bson_as_json(&reply, NULL);
	printf("%s\n", str);

	insert = BCON_NEW("hello",BCON_UTF8("world"));// Key: value

	if(!mongoc_collection_insert(collection,MONGOC_INSERT_NONE,insert,NULL, &error)){
		fprintf( stderr, "%s\n",error.message);
	}

	bson_destroy (insert);
	bson_destroy (&reply);
	bson_destroy (command);
	bson_free  (str);

	// Clean up libmongoc.
	mongoc_collection_destroy (collection);
	mongoc_database_destroy (database);
	mongoc_client_destroy (client);
	mongoc_cleanup ();

	return EXIT_SUCCESS;
	
}
/*
char*  b62_converter( char* url ){

	char compressor[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* compressed_str;
	long url_num;
	int url_length = url.length(); 
	int base = 62;

	for(unsigned int i = 0; i < url_length; ++i){
		url_num += (int) url[i];
	}

	while( url_num > 0){
		compressed_str = compressor[url_num % base ] + compressed_str;
		url_num /= base;
	}

	return  compressed_str;
}
*/
