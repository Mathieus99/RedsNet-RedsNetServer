#ifndef DBmgmt.h
#define DBmgmt.h
    #include <libpq-fe.h>
    const char* DBNAME = ;
    const char* USER = ;
    const char* PASSWORD = ;

    PGconn *conn;

    //Open DB connection with error check
    int DBConnect(){
        conn = PQconnectdb("host=localhost dbname=%s user=%s password=%s",DBNAME,USER,PASSWORD);
        if (PQStatus(conn) != CONNECTION_OK){
            fprintf(stderr,"Connessione al database fallita: %s\n",PQerrorMessage(conn));
            PQfinish(conn);
            return 1;
        }
        else return 0;
    }

    int insertQuery(deviceInfo *device){

        //TODO: Formattare gli array di input della insert

        PGresult *res = PQexecParams(conn, "INSERT INTO devices VALUES ($1,$2,$3,$4,$5,$6,$7,$8,$9,$10)",
                                    10,NULL,paramValues,paramLenghts,paramFormats,0);
        if (PQresultStatus(res) != PGRES_COMMAND_OK){
            fprintf(stderr,"Inserimento fallito: %s\n",PQresultErrorMessage(res));
            PQclear(res);
            PQfinish(conn);
            return 1;
        }
        else return 0
    }

    void closeConnection() { PQfinish(conn); return; }

#endif