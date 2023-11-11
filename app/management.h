/*
Library for socket and connections management
*/
#ifndef management.h
#define management.h
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>


    #define PORT 8800           //Server connection port
    #define BUFFERSIZE 8192     //Buffer size for packets
    #define SOCKETERROR (-1)    //Socket error code
    #define SERVER_BACKLOG 20   //Max incoming connections in pending

    typedef struct sockaddr_in SA_IN;
    typedef struct sockadrr SA;

    int check(int, const char *);       //check for problems from functions
    void *handle_connection(void *);    //do operations with connections
    char *downloadString();             //Download function for string
    FILE *downloadFile();               //Download function for file

    int check(int exp, const char *msg){
        if(exp == SOCKETERROR){
            perror(msg);
            exit(EXIT_FAILURE);
        }
        return exp;
    }

    // Function that provide thread work
    void *handle_connection(void *p_client_socket){
        int client_socket = *((int *)p_client_socket);
        free(p_client_socket);                              //empty the socket for other connections
        const char *json = downloadString(client_socket);   //receiving JSON        
        caricaJSON(json);                                   //Carica il JSON nel DB        
    }

    char *downloadString(int socket){
        // Dichiarazione delle variabili
        char *buffer = malloc(BUFFERSIZE);
        size_t bytes_received = 0;
        size_t total_bytes_received = 0;
        size_t message_size = 0;

        // Ricezione della dimensione del messaggio
        recv(socket, &message_size, sizeof(size_t), 0);

        // Ciclo di ricezione dei pacchetti
        while (total_bytes_received < message_size) {
            // Ricezione del pacchetto
            bytes_received = recv(socket, buffer + total_bytes_received, BUFFERSIZE - total_bytes_received, 0);

            // Controlla gli errori
            if (bytes_received == -1) {
                free(buffer);
                return NULL;
            }

            // Aggiorna la dimensione dei dati ricevuti
            total_bytes_received += bytes_received;
        }

        // Rimuovi il carattere di terminazione della stringa
        buffer[total_bytes_received - 1] = '\0';

        return buffer;
    }

#endif