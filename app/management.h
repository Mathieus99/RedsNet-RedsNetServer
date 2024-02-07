/*
Library for socket and connections management
*/
#ifndef MANAGEMENT_H_INCLUDED
#define MANAGEMENT_H_INCLUDED
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>


    #define PORT 8801           //Server connection port
    #define BUFFERSIZE 8192     //Buffer size for packets
    #define SOCKETERROR (-1)    //Socket error code
    #define SERVER_BACKLOG 20   //Max incoming connections in pending

    typedef struct sockaddr_in SA_IN;
    typedef struct sockadrr SA;

    int check(int, const char *);       //check for problems from functions
    void *handle_connection(void *);    //do operations with connections
    char *downloadString(int socket);             //Download function for string
    int creaFile(char *string);        //Create file from string

    int check(int exp, const char *msg){
        if(exp == SOCKETERROR){
            perror(msg);
        }
        return exp;
    }

    // Function that provide thread work
    void *handle_connection(void *p_client_socket){
        printf("Connection handled\n");
        int client_socket = *((int *)p_client_socket);
        free(p_client_socket);                              //empty the socket for other connections
        printf("Downloading device info\n");
        char *device = downloadString(client_socket);       //receiving JSON
        printf("Device info downloaded\n");
        printf("device in handle:\n%s\n",device);
        if(creaFile(device) == -1)
            printf("Errore nel salvare il file\n");
        //caricaJSON(json);                                   //Carica il JSON nel DB        
    }

    char *downloadString(int socket){
        // Dichiarazione delle variabili
        char *buffer = malloc(BUFFERSIZE);
        size_t bytes_received = 0;
        size_t total_bytes_received = 0;
        size_t message_size = 0;

        // Ricezione della dimensione del messaggio
        recv(socket, &message_size, sizeof(size_t), 0);

        printf("Message size: %d\n",message_size);
        sleep(5);
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

    int creaFile(char *string){
        FILE *device;
        static int i = 0;                               //Numero del file, quindi del device
        int len = 0;
        char *s1 = "Device ", *s2 = ".txt", *filename;

        //Preaggiornamento numero file
        i++;

        //Allocazione filename
        len = strlen(s1) + strlen(s2) + sizeof(i);
        filename = malloc(len * sizeof(char));

        //Costruzione nome file dinamico
        strcpy(filename, s1);
        sprintf(filename + strlen(s1), "%d", i);
        strcat(filename, s2);

        //Apertura file
        device = fopen(filename,"w");
        if (device == NULL)
            return -1;
        
        //Scrittura e salvataggio
        fprintf(device,"%s", string);
        fclose(device);
        printf("Device salvato\n");
        return 0;
    }

#endif