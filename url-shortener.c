#include <string.h>
#include <stdio.h>

#include <mongoc/mongoc.h>


void b62_converter(char* , char [] );
bool insert_DB( mongoc_client_t*, char*, char*);
bool query_DB(mongoc_client_t*, char*);

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
	char hashed_url[27];
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

void  b62_converter( char* url, char m_url[] ){

	unsigned int url_length = strlen(url); 
	unsigned int base = 62;

	char compressor[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	long url_num = 0;

	for(unsigned int i = 0; i < url_length; ++i){
		url_num += (int) url[i];
	}

	int counter = 0;
	while( url_num > 0){
		m_url[counter] = compressor[ url_num % base ]; 
		
		url_num /= base;
		counter++;
	}
	m_url[counter] = '\0';

}

bool query_DB(mongoc_client_t *client, char* key){

	// Variables:
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;
	
	mongoc_collection_t *collection = mongoc_client_get_collection( client, "URL_lib","coll_name");
	
	query = bson_new();
	bson_append_regex(query, key,-1, ".", NULL);
	cursor = mongoc_collection_find_with_opts( collection, query, NULL, NULL);

	while( mongoc_cursor_next( cursor, &doc) ){
		str = bson_as_json(doc, NULL);
		printf("%s\n", str);
		bson_free(str);
	}

	// Clean libmongoc:
	mongoc_collection_destroy(collection);
	mongoc_cursor_destroy(cursor);

	bson_destroy(query);
	
	return true;
}


bool insert_DB(mongoc_client_t *client, char* key, char* value){
	
	// Variables:	
	bson_t *command, reply, *insert;
	bson_error_t error;
	
	// Get Database & Collection:
	mongoc_database_t *database = mongoc_client_get_database( client, "URL_lib");
	mongoc_collection_t *collection = mongoc_client_get_collection( client, "URL_lib","coll_name");

	command = BCON_NEW("ping", BCON_INT32(1));

	bool retval = mongoc_client_command_simple(client, "admin", command,NULL,&reply,
			&error);

	if(!retval){
		fprintf( stderr, "%s\n",error.message);
		return false;
	}

	char *str = bson_as_json(&reply, NULL);
	printf("%s\n", str);

	insert = BCON_NEW(key , BCON_UTF8(value) );// Key: value

	if(!mongoc_collection_insert(collection,MONGOC_INSERT_NONE,insert,NULL, &error)){
		fprintf( stderr, "%s\n",error.message);
	}

	// Clean libmongc:
	mongoc_collection_destroy (collection);
	mongoc_database_destroy (database);

	bson_destroy (insert);
	bson_destroy (&reply);
	bson_destroy (command);
	bson_free  (str);

	return true;

}

