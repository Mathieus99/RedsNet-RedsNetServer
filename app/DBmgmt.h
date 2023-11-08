#ifndef DBmgmt.h
#define DBmgmt.h

    #include <mongoc/mongoc.h>
    #include <bson/bson.h>

    #define MONGODB_URI "mongodb://localhost:27017"
    #define DATABASE "my_database"
    #define COLLECTION "my_collection"

    mongoc_client_t *DBConnect(const char *uri);        //Connection to DB
    mongoc_database_t *get_database(mongoc_client_t *client, const char *database);
    mongoc_collection_t *get_collection(mongoc_database_t *database, const char *collection);
    bson_t *load_json(const char *json);

    //TODO: DB Connection
    void DBConnect(){

    }
    //TODO: Caricamento JSON    

#endif