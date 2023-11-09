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
        // Crea un client MongoDB
        mongoc_client_t *client = mongoc_client_new(MONGODB_URI);

        // Controlla gli errori
        if (!client){
            fprintf(stderr, "Errore: impossibile connettersi a MongoDB\n");
            return NULL;
        }

        return client;
    }
    
    mongoc_database_t *get_database(mongoc_client_t *client, const char *database){
        // Crea una connessione al database
        mongoc_database_t *database = mongoc_client_get_database(client, DATABASE);

        // Controlla gli errori
        if (!database) {
            fprintf(stderr, "Errore: impossibile ottenere il database '%s'\n", database);
            return NULL;
        }

        return database;
    }

    mongoc_collection_t *get_collection(mongoc_database_t *database, const char *collection){
        // Crea una collezione
        mongoc_collection_t *collection = mongoc_database_get_collection(DATABASE, COLLECTION);

        // Controlla gli errori
        if (!collection) {
            fprintf(stderr, "Errore: impossibile ottenere la collezione '%s'\n", collection);
            return NULL;
        }

        return collection;
    }

    bson_t *load_json(const char *json){
        // Carica un JSON
        bson_t *document = bson_new_from_json(json, strlen(json));

        // Controlla gli errori
        if (!document) {
            fprintf(stderr, "Errore: impossibile caricare il JSON\n");
            return NULL;
        }

        return document;
    }

    //TODO: Caricamento JSON    
    void caricaJSON(){

    }

    void DBDisconnect(){

    }

#endif