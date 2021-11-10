#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 20
#define BUFFER_SIZE 2048

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
    char buffer_output[BUFFER_SIZE];
    char username[36];
    int deadEnd_flag = 0;     // this is a reference

    client_counter++;
    client_t *client = (client_t *)arg;

    // Assegnazione Username
    if(recv(client->sockfd, username, 36, 0) <= 0 || strlen(username) < 2 || strlen(username) >= 36-1) {
       printf("Didn't enter the username.\nAborting...\n");
       deadEnd_flag = 1; 
    } else {
        strcpy(client->username, username);
        sprintf(buffer_output, "%s has joined the chatroom\n", client->username);
        printf("%s", buffer_output);    
        // deve fa morì il client 
        }

    bzero(buffer_output, BUFFER_SIZE); // Ripulisce la memoria usara per il buffer

    while(1) {
        if (deadEnd_flag) {
            break;
        }
        int receive = recv(client->sockfd, buffer_output, BUFFER_SIZE, 0);
        if(receive > 0) {
            if(strlen(buffer_output) > 0){
                // inviare il messaggio
                printf("%s -> %s\n", buffer_output, client->username);
            } else if (receive == 0 || strcmp(buffer_output, "exit") == 0) {}
        }
    }
}



int main(int argc, char **argv){

}