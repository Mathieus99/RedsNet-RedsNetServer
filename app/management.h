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


    #define PORT 8800
    #define BUFFERSIZE 8192           
    #define SOCKETERROR (-1)
    #define SERVER_BACKLOG 20

    typedef struct sockaddr_in SA_IN;
    typedef struct sockadrr SA;

    int check(int, const char *);       //check for problems from functions
    void *handle_connection(void *);    //do operations with connections

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
        free(p_client_socket);  //empty the socket for other connections
        char buffer[BUFFERSIZE];
        bzero(&buffer, sizeof(buffer));
        
        //receiving device info
        

        //Save info on Database
    }
#endif