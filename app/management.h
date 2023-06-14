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
    #include "DBmgmt.h"

    #define PORT 8800
    #define BUFFERSIZE 8192           
    #define SOCKETERROR (-1)
    #define SERVER_BACKLOG 20

    typedef struct sockaddr_in SA_IN;
    typedef struct sockadrr SA;
    typedef struct{
        char[15] date;
        char[150] deviceModel;
        char[150] deviceManufacturer;
        long totalRAM;
        long freeRAM;
        long usedRAM;
        long totalStorage;
        long usedStorage;
        long availableStorage;
        float batteryPercentage;    
    }deviceInfo;

    int check(int, const char *);       //check for problems from functions
    void *handle_connection(void *);    //do operations with connections
    void initializeServer(int, SA_IN);  //initialize server socket

    int check(int exp, const char *msg){
        if(exp == SOCKETERROR){
            perror(msg);
            exit(EXIT_FAILURE);
        }
        return exp;
    }

    void initializeServer(int server_socket, SA_IN server_address){
        // Creating socket
        check((server_socket = socket(AF_INET,SOCK_STREAM,0)), "socket failed");

        //Initialize address struct
        server_address.sin_family = AF_INET;           //setting address version IPv4
        server_address.sin_addr.s_addr = INADDR_ANY;   //set IP address
        server_address.sin_port = htons(PORT);         //set address port 8081

        // Attaching socket to PORT
        check(bind(server_socket,(SA*)&server_address,sizeof(server_address)),"Bind Failed!");
        
        // Put socket in listen for incoming connections
        check(listen(server_socket, SERVER_BACKLOG));
    }

    void *handle_connection(void *p_client_socket){
        int client_socket = *((int *)p_client_socket);
        free(p_client_socket);  //empty the socket for other connections
        char buffer[BUFFERSIZE];
        bzero(&buffer, sizeof(buffer));
        deviceInfo device;
        
        //receiving device info
        

        //Save info on Database
    }
#endif