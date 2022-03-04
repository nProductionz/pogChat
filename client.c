
#include "utils.h"
#include "client.h"

int deadEnd = FALSE;               // il programma deve morire

int sockfd;                     // socked file desc.

pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;   // implementare la mutua esclusione tra thread

char sendBuffer[BUF_SIZE];
char receiveBuffer[BUF_SIZE + USERNAME_LEN + 2];



// funzione per inviare messaggi al server

void *handle_send() {
    while(1) {
        bzero(sendBuffer, BUF_SIZE);
        fgets(sendBuffer, BUF_SIZE, stdin);

        // Invia messaggi al server 
        if(send(sockfd, sendBuffer, strlen(sendBuffer), 0) == SYSERR) {
            perror("send");
            deadEnd = TRUE;
            pthread_mutex_destroy(&mutexsum);
            pthread_exit(NULL);
        }

        // Controllo per le situazioni di interruzione
        if(strcmp(sendBuffer, CLOSE) == 0 || strcmp(sendBuffer, EXIT) == 0) {
            deadEnd = TRUE;
            pthread_mutex_destroy(&mutexsum);
            pthread_exit(NULL);
        }

        pthread_mutex_unlock(&mutexsum);
    }
}


// funzione per leggere i messaggi girati dal server

void *handle_receive() {
    int nbytes;
    
    while(1) {
        bzero(receiveBuffer, BUF_SIZE);

        // scaricare i messaggi dal server finché non si verifica un errore
        if((nbytes = recv(sockfd, receiveBuffer, BUF_SIZE - 1, 0)) == SYSERR) {
            perror("Errore nel RECV - handlReceive");
            deadEnd = TRUE;
            pthread_mutex_destroy(&mutexsum);
            pthread_exit(NULL);
        }

        receiveBuffer[nbytes] = '\0';
        if(strcmp(ERROR, receiveBuffer) == 0) {
            printf("ERRORE: Username: %s già in uso, scegline un altro.\n", sendBuffer);
            deadEnd = TRUE;
            pthread_mutex_destroy(&mutexsum);
            pthread_exit(NULL);
        }
        else {
            printf("%s", receiveBuffer);
            pthread_mutex_unlock(&mutexsum);
        }
    }
}


int main(int argc, char **argv) {
    

    if(argc != 4) {
        fprintf(stderr, "Connessione a: %s [server] [port] [username]\n", argv[0]);
        exit(EXIT_FAILURE);
    } 

    int port;
    char username[USERNAME_LEN];

    port = atoi(argv[2]);
    strncpy(username, argv[3], USERNAME_LEN);

    printf("Server: %s\n", argv[1]);
    printf("Porta: %d\n", port);
    printf("Username: %s\n", username);

    struct hostent *server_host;

    if((server_host = gethostbyname(argv[1])) == NULL) {        
        fprintf(stderr, "Impossibile connettersi al server\n");
        exit(EXIT_FAILURE);
    }

    printf("Host: %s\n", server_host->h_name);
    printf("IP dell'host: %s\n", inet_ntoa((struct in_addr)*((struct in_addr *)server_host->h_addr)));

    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) {          // controllo errori
        close(sockfd);
        fprintf(stderr, "Errore socket\n");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;   
    server_addr.sin_port = htons(port);      
    server_addr.sin_addr = *((struct in_addr *) server_host->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);   

    // Connessione al server
    if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == SYSERR) {
        close(sockfd);
        fprintf(stderr, "Errore nella connessione al server\n");
        exit(EXIT_FAILURE);
    }

    // Invio messaggio di ingresso alla chatroom
    strcpy(sendBuffer, username);
    if(send(sockfd, sendBuffer, strlen(sendBuffer), 0) == SYSERR) {
        perror("Errore nel send iniziale");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Gestisco la comunicazione con due thread, uno per inviare ed uno per ricevere
    //Thread 1: handle_send(), permette di inviare i messaggi prendendoli in input
    //Thread 2: handle_receive(), scarica i messaggi che arrivano dal server, mostrandoli
    // Set up threads
    pthread_t threads[2];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Lancio i thread
    // Sperando che funzioni... (sta volta mi son ricordato di commentarlo)
    pthread_create(&threads[0], &attr, handle_send, NULL);
    pthread_create(&threads[1], &attr, handle_receive, NULL);

    // Si attende finché non si attiva la deadEnd flag.
    while(!deadEnd);

    close(sockfd);
    return EXIT_SUCCESS;

      
}
