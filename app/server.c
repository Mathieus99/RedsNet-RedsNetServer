#include "mythreads.h"

//int deviceno = 0;

int main(int argc,char *argv[]){

    int server_socket, client_socket, addr_size;
    SA_IN server_address, client_address;
    
    //Initialize server socket
    initializeServer(server_socket, server_address);    

    //Allocate threads for thread pool
    for(int i=0;i<THREAD_POOL_SIZE;i++){
        pthread_create(&thread_pool[i], NULL, thread_function());
    }

    //Open DB connection
    int try = 0;
    int error = DBConnect();
    while(error != 0 && try<10){
        printf(("Errore di connessione al database\n"));
        error = DBConnect();
        try++;
    }

    while (1){
        printf("Waiting for connections...\n");
        // accept incoming connection
        addr_size = sizeof(SA_IN);
        check((client_socket = accept(server_socket, (SA*)&client_address,(socklen_t*)&addr_size)), "accept failed!");
        printf("Connected\n");

        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;
        //Make sure that enqueue and dequeue are safe
        pthread_mutex_lock(&mutex);
        thread_enqueue(pclient);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}