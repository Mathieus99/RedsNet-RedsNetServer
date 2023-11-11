#ifndef DBMGMT_H_INCLUDED
#define DBMGMT_H_INCLUDED

    #include <mongoc/mongoc.h>
    #include <bson/bson.h>

    #define MONGODB_URI "mongodb://localhost:27017"
    #define DATABASE "local"
    #define COLLECTION "Devices"

    mongoc_client_t *DBConnect();                                       //Crea la connessione a MongoDB
    mongoc_database_t *get_database(mongoc_client_t *client);           //Crea la connessione col database
    mongoc_collection_t *get_collection();                              //Crea la connessione con la collezione
    bson_t *load_json(const char *json);                                //Converte il JSON in BSON per MongoDB
    void caricaJSON(const char *json,mongoc_collection_t *collection);  //Carica il BSON nella collezione
    //Chiude la connessione
    void DBDisconnect(mongoc_client_t *client,mongoc_collection_t *collection, mongoc_database_t *db);  

    bson_error_t *error;

    //Crea la connessione con MongoDB
    mongoc_client_t * DBConnect(){
        // Crea un client MongoDB
        mongoc_client_t *client = mongoc_client_new(MONGODB_URI);

        // Controlla gli errori
        if (!client){
            fprintf(stderr, "Errore: impossibile connettersi a MongoDB\n");
            return NULL;
        }

        return client;
    }
    
    //Crea la connessione con il DB
    mongoc_database_t *get_database(mongoc_client_t *client){
        // Crea una connessione al database
        mongoc_database_t *database = mongoc_client_get_database(client, DATABASE);

        // Controlla gli errori
        if (!database) {
            fprintf(stderr, "Errore: impossibile ottenere il database '%s'\n", database);
            return NULL;
        }

        return database;
    }

    //Crea una connessione con la collezione
    mongoc_collection_t *get_collection(){
        // Crea una collezione
        mongoc_collection_t *collection = mongoc_database_get_collection(DATABASE, COLLECTION);

        // Controlla gli errori
        if (!collection) {
            fprintf(stderr, "Errore: impossibile ottenere la collezione '%s'\n", collection);
            return NULL;
        }

        return collection;
    }

    //Converte il JSON in BSON per MongoDB
    bson_t *load_json(const char *json){
        // Carica un JSON
        bson_t *document = bson_new_from_json(json, strlen(json), &error);

        // Controlla gli errori
        if (!document) {
            fprintf(stderr, "Errore: impossibile caricare il JSON\n");
            mongoc_error_cleanup(&error);
            return NULL;
        }

        return document;
    }

    //Carica il JSON nella collezione    
    void caricaJSON(const char *json, mongoc_collection_t *collection){
        bson_t *device = load_json(json);
        mongoc_collection_insert_one(collection, device, NULL, NULL, &error);
        if(error){
            fprintf(stderr, "Errore: %s\n", error->message);
            mongoc_error_cleanup(&error);
            bson_free(device);
            return EXIT_FAILURE;
        }
        bson_free(device);
    }

    //Chiude la connessione
    void DBDisconnect(mongoc_client_t *client,mongoc_collection_t *collection, mongoc_database_t *db){
        mongoc_collection_destroy(collection);
        mongoc_database_destroy(db);
        mongoc_client_destroy(client);
    }

#endif