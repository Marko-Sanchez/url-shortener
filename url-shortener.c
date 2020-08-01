#include <string.h>
#include <stdio.h>

#include <mongoc/mongoc.h>

//char* b62_converter(char* url);
bool insert_DB( mongoc_client_t* ,mongoc_collection_t*, mongoc_database_t* );
bool query_DB(mongoc_client_t*);

int main(){
	
	// Variables:
	mongoc_client_t *client;
	mongoc_database_t *database;
	mongoc_collection_t *collection;

	// INIT: libmongoc's internals.
	mongoc_init();

	// Create new client instance:

	// Insertion:
	if(false)
	if( !insert_DB(client, collection, database) ){
		fprintf(stderr, "Failed Insertion\n");
	}

	// Query:
	if( ! query_DB(client) ){
		fprintf(stderr, "Query Failed\n");
	}

	// Clean up libmongoc.
	mongoc_collection_destroy (collection);
	mongoc_database_destroy (database);
	mongoc_client_destroy (client);
	mongoc_cleanup ();

	return EXIT_SUCCESS;
	
}

bool query_DB(mongoc_client_t *client){

	// Variables:
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;
	
	collection = mongoc_client_get_collection( client, "URL_lib","coll_name");
	
	query = bson_new();
	//query = BCON_NEW("1adfgs",BCON_TYPE_REGEX("."));
	bson_append_regex(query, "1adfgs",-1, ".", NULL);
	//BSON_APPEND_UTF8(query, "1adfgs", BCON_TYPE_REGEX(".") );
	cursor = mongoc_collection_find_with_opts( collection, query, NULL, NULL);

	while( mongoc_cursor_next( cursor, &doc) ){
		str = bson_as_json(doc, NULL);
		printf("%s\n", str);
		bson_free(str);
	}

	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	
	return true;
}


bool insert_DB(mongoc_client_t *client,mongoc_collection_t *collection, mongoc_database_t *database){
	
	// Variables:	
	bson_t *command, reply, *insert;
	bson_error_t error;
	
	// Get Database & Ccollection
	database = mongoc_client_get_database( client, "URL_lib");
	collection = mongoc_client_get_collection( client, "URL_lib","coll_name");

	command = BCON_NEW("ping", BCON_INT32(1));

	bool retval = mongoc_client_command_simple(client, "admin", command,NULL,&reply,
			&error);

	if(!retval){
		fprintf( stderr, "%s\n",error.message);
		return false;
	}

	char *str = bson_as_json(&reply, NULL);
	printf("%s\n", str);

	insert = BCON_NEW("1adfgs",BCON_UTF8("www.youtube.com/"));// Key: value

	if(!mongoc_collection_insert(collection,MONGOC_INSERT_NONE,insert,NULL, &error)){
		fprintf( stderr, "%s\n",error.message);
	}

	bson_destroy (insert);
	bson_destroy (&reply);
	bson_destroy (command);
	bson_free  (str);

	return true;

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
