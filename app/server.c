#include "mythreads.h"

int main(int argc,char *argv[]){

    int server_socket, client_socket, addr_size;
    SA_IN server_address, client_address;
    
    //Initialize server socket ----------------------------------------------------------
    printf("Initializing server\n");
    check((server_socket = socket(AF_INET,SOCK_STREAM,0)), "socket failed");    // Creating socket

    //Initialize address struct
    server_address.sin_family = AF_INET;           //setting address version IPv4
    server_address.sin_addr.s_addr = INADDR_ANY;   //set IP address
    server_address.sin_port = htons(PORT);         //set address port 8081

    // Attaching socket to PORT
    check(bind(server_socket,(SA*)&server_address,sizeof(server_address)),"Bind Failed!");
    check(listen(server_socket, SERVER_BACKLOG),"Listen Error!");      // Put socket in listen for incoming connections
    printf("Socket opened\n");
    // ----------------------------------------------------------------------------------  

    //Allocate threads for thread pool
    for(int i=0;i<THREAD_POOL_SIZE;i++){
        pthread_create(&thread_pool[i], NULL, thread_function, NULL);
    }
    printf("Thread pool created\n");
    
    //TODO: Open DB Connection

    // Start listening incoming connections
    printf("Waiting for connections...\n");
    while (1){
        addr_size = sizeof(SA_IN);
        //accept incoming connections
        check((client_socket = accept(server_socket, (SA*)&client_address,(socklen_t*)&addr_size)), "accept failed!");
        printf("Connected\n");
        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;
        //Make sure that enqueue and dequeue are safe
        pthread_mutex_lock(&mutex);             
        thread_enqueue(pclient);                //Assign thread to client
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}
