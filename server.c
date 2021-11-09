#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 20

// Struttura del client

typedef struct {
    struct sockaddr_in address;
    int sockfd;
    int uid;
    char username[36];
} client_t;

client_t *clients[MAX_CLIENTS];


static _Atomic unsigned int client_counter = 0;
void *handle_client(void *arg) {
    char buff_output[BUFFER_SZ];
    char username[36];
    int deadEnd_flag = 0;     // this is a reference

    client_counter++;
    client_t *client = (client_t *)arg;

    // Assegnazione Username
    if(recv(client->sockfd, name, 36, 0) <= 0 || strlen(username) < 2 || strlen(username) >= 36-1) {
       printf("Didn't enter the username.\nAborting...\n");
       deadEnd_flag = 1; 
    } else {
        strcpy(client->username, username);
        sprintf(buff_out, "%s has joined the chatroom\n", client->username);
        printf("%s", buff_out);    
        //
        }
}



int main(int argc, char **argv){

}