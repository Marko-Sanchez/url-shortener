#include <string.h>

#include "functions.h"

int main(int argc, char* argv[]){
	
	// Variables:
	mongoc_client_t *client;
	char* url;

	// Get URl:
	if( argc > 0){
		url = (char*) argv[--argc];
	}else{
		return EXIT_FAILURE;
	}

	// INIT: libmongoc's internals.
	mongoc_init();

	// Create new client instance:

	// Hash URL:
	char hashed_url[27] = "hello";
	if(false){
	b62_converter(url,hashed_url);
	printf("URL: %s\nHash: %s\n",url,hashed_url);
	}

	// Insertion:
	if(false)
	if( !insert_DB(client, hashed_url, url) ){
		fprintf(stderr, "Failed Insertion\n");
	}
	
	// Query:
	if(true)
	if( ! query_DB(client,hashed_url) ){
		fprintf(stderr, "Query Failed\n");
	}

	// Clean up libmongoc.
	mongoc_client_destroy (client);
	mongoc_cleanup ();

	return EXIT_SUCCESS;
	
}


