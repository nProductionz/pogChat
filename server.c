#include "server.h"

// Struttura del client

typedef struct {
    struct sockaddr_in address;
    int sockfd;
    int uid;
    char username[36];
} client_t;

client_t *clients[MAX_CLIENTS];

// funzione per stampare l'indirizzo di un client
void print_client_addr(struct sockaddr_in addr){
    printf("%d.%d.%d.%d",
    addr.sin_addr.s_addr & 0xff,
    (addr.sin_addr.s_addr & 0xff00) >> 8,
    (addr.sin_addr.s_addr & 0xff0000) >> 16,
    (addr.sin_addr.s_addr & 0xff000000) >> 24);
}

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Funzione che permette di inviare i messaggi ai vari client

void send_message(char *s, int uid) {
    pthread_mutex_lock(&clients_mutex); // creare i thread nel main

    for(int  i=0; i<MAX_CLIENTS; ++i){
        if(clients[i]){
            if(clients[i]->uid != uid) {
                if(write(clients{i}->sockfd, s, strlen(s)) < 0) {
                    perror("ERROR: write to descriptor failed");
                    break;
                }
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

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
                }
            }
        else if (receive == 0 || strcmp(buffer_output, "exit") == 0) {
            sprintf(buffer_output, "%s ha abbandonato la chatroom.\n", client->username);
            printf("%s", buffer_output);
            // mandare il messaggio di morte al client
            deadEnd_flag = 1;
        } else {
            printf("ERROR: -1\n");
            deadEnd_flag = 1;
        }
        bzero(buffer_output, BUFFER_SIZE);
    }

    // cancellare il client e liberare il thread
    close(client->sockfd);

    // il client va rimosso dalla coda, implementare la funzione

    free(client);
    client_counter--;
    pthread_detach(pthread_self());

    return NULL;
}




int main(int argc, char **argv){
    if(argc != 2){
        printf("Porta in uso: %s", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);
    int option = 1;
    int listenfd = 0, connfd = 0;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    pthread_t tid;

    // Setting socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // Ignore Pipe Signal
    signal(SIGPIPE, SIG_IGN);
    if(setsockopt(listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option, sizeof(option)) < 0) {
        perror("ERROR: setsockport failed");
        return EXIT_FAILURE;
    }

    // Bind
    if(bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("ERROR: Socket listening failed");
        return EXIT_FAILURE;
    }

    // Listen
    if(listen(listenfd,10) < 0) {
        perror("ERROR: Socket listening failed");
        return EXIT_FAILURE;
    }

    printf(" +++ BENVENUTO NELLA CHATROOM +++\nsperiamo che sta cosa funzioni...\n");
    while(1) {
        socklen_t clientlen = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &clientlen);

        // controllo se son stati accettati il massimo numero di client
        if((client_counter + 1) == MAX_CLIENTS){
            printf("Raggiunto il massimo numero di client. Rifiutato: ");
            print_client_addr(client_addr);
            printf(":%d\n", client_addr.sin_port);
            close(connfd);
            continue;
        }
    
        
    
    }


}