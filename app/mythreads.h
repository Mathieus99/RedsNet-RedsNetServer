/*
Library for threads and thread pool's management
*/
#ifndef MYTHREADS_H_INCLUDED
#define MYTHREADS_H_INCLUDED
    #include "DBmgmt.h"
    #include "management.h"
    #include <pthread.h>

    #define THREAD_POOL_SIZE 10

    //Create thread pool array, mutex and condition variable
    pthread_t thread_pool[THREAD_POOL_SIZE];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

    typedef struct node{
        int *client_socket;
        struct node *next;
    } node_t;

    node_t *head = NULL;
    node_t *tail = NULL;

    void thread_enqueue(int *);
    int* thread_dequeue();
    void *thread_function(void *arg);

    //enqueue incoming connections for threads
    void thread_enqueue(int *client_socket){
        node_t *newnode = malloc(sizeof(node_t));

        newnode->client_socket = client_socket;
        newnode->next = NULL;
        if (tail == NULL){
            head = newnode;
        }
        else {
            head->next = newnode;
        }
        tail = newnode;
    }

    int* thread_dequeue(){
        if (head == NULL){
            return NULL;
        }
        else {
            int *result = head ->client_socket;
            node_t *temp = head;
            head = head ->next;
            if (head == NULL) { tail = NULL; }
            free(temp);
            return result;
        }
    }

    //Manage dequeue of threads from thread pool and launch the function on the client
    void *thread_function(void *arg){
        while(1){
            int *pclient;
            pthread_mutex_lock(&mutex);
            if((pclient = thread_dequeue()) == NULL){
                pthread_cond_wait(&condition_var, &mutex);
                //try again
                pclient = thread_dequeue();
            }
            pthread_mutex_unlock(&mutex);
            if (pclient != NULL){
                handle_connection(pclient);
            }
        }
    }
#endif