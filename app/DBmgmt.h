#ifndef DBmgmt.h
#define DBmgmt.h

    #include <mongoc/mongoc.h>
    #include <bson/bson.h>

    #define MONGODB_URI "mongodb://localhost:27017"
    #define DATABASE "my_database"
    #define COLLECTION "my_collection"

    mongoc_client_t *DBConnect();                               //Crea la connessione a MongoDB
    mongoc_database_t *get_database(mongoc_client_t *client);   //Crea la connessione col database
    mongoc_collection_t *get_collection();                      //Crea la connessione con la collezione
    bson_t *load_json(const char *json);                        //Converte il JSON in BSON per MongoDB
    void caricaJSON(const char *json);                          //Carica il BSON nella collezione
    void DBDisconnect();                                        //Chiude la connessione

    //Crea la connessione con MongoDB
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
        bson_t *document = bson_new_from_json(json, strlen(json));

        // Controlla gli errori
        if (!document) {
            fprintf(stderr, "Errore: impossibile caricare il JSON\n");
            return NULL;
        }

        return document;
    }

    //Carica il JSON nella collezione    
    void caricaJSON(const char *json){
        mongoc_client_t *client = DBConnect();
        mongoc_database_t *database = get_database(client);
        mongoc_collection_t *collection = get_collection();
        bson_t *device = load_json(json);
        mongoc_collection_insert_one(collection, device, NULL, NULL, &error);
        if(error){
            fprintf(stderr, "Errore: %s\n", error.message);
            mongoc_error_cleanup(&error);
            return EXIT_FAILURE;
        }
        DBDisconnect(client,database,collection,document);
    }

    //Chiude la connessione
    void DBDisconnect(mongoc_client_t *client,mongoc_collection_t *collection, mongoc_database_t *db,bson_t *doc){
        bson_free(doc);
        mongoc_collection_destroy(collection);
        mongoc_database_destroy(db);
        mongoc_client_destroy(client);
    }

#endif