#include "server.h"
#include "utils.h"

// struttura per i client in ingresso (simil LinkedList), usato anche per printare l'username
typedef struct client {
    struct sockaddr_in address;         
    char username[USERNAME_LEN]; 
    struct client *next;
} client;

int sockfd;

client clientList;
char requestBuffer[BUF_SIZE];
char responseBuffer[BUF_SIZE + USERNAME_LEN];

char sender_name[USERNAME_LEN];

// QUANDO DOVRESTI FIXARE IL PROGETTO MA SCOPRI CHE PUOI METTERE I COLORI
// E ALLORA CHE FAI??? NON LI AGGIUNGI???
char *colors[NCOLORS] = { RED, GREEN, YELLOW, BLUE, WHITE};

void userColor(int n) {
    strcat(responseBuffer, colors[(n % NCOLORS)]);
}

// funzione per verificare se due client sono uguali
// utilizzata principalmemte per capire se è il mittente del messaggio
// o se il client è già nella lista
int clientCompare(struct sockaddr_in client1, struct sockaddr_in client2) {
    if(strncmp((char *) &client1.sin_addr.s_addr, (char *) &client2.sin_addr.s_addr, sizeof(unsigned long)) == 0) {
        if(strncmp((char *) &client1.sin_port, (char *) &client2.sin_port, sizeof(unsigned short)) == 0) {
            if(strncmp((char *) &client1.sin_family, (char *) &client2.sin_family,sizeof(unsigned short)) == 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// funzione per inviare messaggi a tutti gli utenti connessi tranne il mittente 
void handle_send_messages(struct sockaddr_in sender, int global) {
    client *cli = clientList.next;      //client list iterator

    while(cli != NULL) {
        // if per controllare che non venga inviato al mittente
        if(clientCompare(sender, cli->address) == FALSE ) {
            printf("Invio del messaggio a %s\n", cli->username);
            if((sendto(sockfd, responseBuffer, strlen(responseBuffer), 0,(struct sockaddr *) &cli->address, sizeof(struct sockaddr))) == SYSERR) {
                perror("Errore nel send - handle");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
        }

        cli = cli->next;
    }
}

// controllare che il client non sia già connesso

int isConnected(struct sockaddr_in newClient) {
    client *element = &clientList;

    // ciclo per controllare che il client non sia già presente
    while(element != NULL) {
        if(clientCompare(element->address, newClient)) {
            strncpy(sender_name, element->username, USERNAME_LEN);
            printf("Client connesso.\n");
            return TRUE;
        }
        element = element->next;
    }
    printf("Il client non è connesso .\n");
    return FALSE;
}


// funzione per connettere i client alla LL man mano che vengono inserite

int connectClient(struct sockaddr_in newClient, char *username) {
    printf("Connessione client: %s\n", username);
    client *element = &clientList;

    // solito ciclo sugli elementi della LL
    while(element != NULL) {
        if(strcmp(element->username, username) == 0) {
            printf("Impossibile connettere il client, esiste già il nome\n");
            strcpy(responseBuffer, "");
            strcat(responseBuffer, ERROR);
            if((sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *) &newClient,sizeof(struct sockaddr))) == SYSERR) {
                perror("Errore nel send - connect client");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            return SYSERR;
        }
        element = element->next;
    }

    element = &clientList;
    while(element->next != NULL) {
        element = element->next;
    }
    element->next = malloc(sizeof(client));
    element = element->next;

    element->address = newClient;
    strncpy(element->username, username, USERNAME_LEN);
    element->next = NULL;
    printf("Finalmente si è connesso.\n");
    return 0;
}

// Disconnetti i client dalla LL
int disconnectClient(struct sockaddr_in oldClient) {
    printf("Disconnessione client\n");
    client *temp;
    client *element = &clientList;

    while(element->next != NULL) {
        if(clientCompare(oldClient, element->next->address)) {
            temp = element->next->next;
            free(element->next);
            element->next = temp;
            printf("Finalmente se n'è andato.\n");
            return 0;
        }
        element = element->next;
    }

    printf("Errore nell'espulsione, non si può avere pace\n");
    return SYSERR;
}

// print dei client connessi per debug e stile
void printClientList() {
    client *cli = clientList.next;
    int count = 1;

    while(cli != NULL) {
        printf("Client %d\n", count);
        printf("%s\n", cli->username);
        printf("Ip: %s\n", inet_ntoa(cli->address.sin_addr));
        printf("Porta: %d\n\n", ntohs(cli->address.sin_port));
        cli = cli->next;
        count++;
    }
}

void sendClientList(struct sockaddr_in sender) {
    client *cli = clientList.next;

    while(cli != NULL) {
        if(clientCompare(sender, cli->address) == FALSE) {
            strcpy(responseBuffer, "");
            userColor(cli->address.sin_port);
            strcat(responseBuffer, cli->username);
            strcat(responseBuffer, RESET "\n");
            if((sendto
                (sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *) &sender,
                 sizeof(struct sockaddr))) == SYSERR) {

                perror("Errore nella send");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
        }
        cli = cli->next;
    }
}

// Main del server
int main(int argc, char *argv[]) {

    int server_port, nbytes;
    int address_size = sizeof(struct sockaddr_in);
    struct sockaddr_in server_addr;
    struct sockaddr_in sender_addr;     // addr per gestire l'invio dei messaggi

    bzero(requestBuffer, BUF_SIZE);
    bzero(responseBuffer, BUF_SIZE + USERNAME_LEN);

    if(argc != 2) {
        fprintf(stderr, "In uso la porta: %s \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_port = atoi(argv[1]);

    //SOCK_DGRAM son datagram che utilizzano l'udp
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);    // ora posso usare recv() e send()
    // controllo errori in socket() 
    if(sockfd == SYSERR) {      
        close(sockfd);
        fprintf(stderr, "Errore in socket().\n");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;   
    server_addr.sin_port = htons(server_port);  
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // connette a tutte le interfacce del computer
    

    memset(&(server_addr.sin_zero), '\0', 8);

    if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == SYSERR) {
        close(sockfd);
        fprintf(stderr, "Errore nel bind della socket\n");
        exit(EXIT_FAILURE);
    }

    // ciclo principale del server
    while(1) {                  
        //azzeramento del buffer
        bzero(responseBuffer, BUF_SIZE + USERNAME_LEN); 

        // LISTEN per messaggi in arrivo
        if((nbytes =
            recvfrom(sockfd, requestBuffer, BUF_SIZE - 1, 0, (struct sockaddr *) &sender_addr, (unsigned int *) &address_size)) == SYSERR) {
            perror("Errore nella recv - ciclo while (main)");
            close(sockfd);
            exit(EXIT_FAILURE);
        }


        // assicurarsi che il messaggio sia terminato
        requestBuffer[nbytes] = '\0';  

        printf("-- Ricevuto paccketto %d bytes\n~\n%s\n~\n\n", nbytes, requestBuffer);

        // si controlla se chi ha inviato il messaggio si è connesso
        if(isConnected(sender_addr)) {
            // gli si assegna un colore (anche l'occhio vuole la sua parte)
            userColor(sender_addr.sin_port);
            strcat(responseBuffer, sender_name);
            // comando per l'uscita
            if(strcmp(CLOSE, requestBuffer) == 0) {
                if(disconnectClient(sender_addr) == 0) {
                    // invio del messaggio che il client si è disconnesso
                    strcat(responseBuffer, RED " ci ha lasciati, RIP." RESET "\n");
                    handle_send_messages(sender_addr, TRUE);       
                }
                printClientList();
            }
            else if(strcmp(EXIT, requestBuffer) == 0) {
                strcat(responseBuffer, RED " Arresto del server. " RESET "\n");
                handle_send_messages(sender_addr, TRUE);
                printf("CIAONE.\n");
                close(sockfd);
                exit(0);
            }
            
            else {
                strcat(responseBuffer, RESET);
                // Aggiungere l'username prima del nome
                strcat(responseBuffer, USERNAMExMESSAGE);       
                strcat(responseBuffer, requestBuffer);

                printf("Messaggio:\n[%s]\n", responseBuffer);
                // invio del messaggio a tutti i client che non sono il mittente
                handle_send_messages(sender_addr, FALSE);
            }
        }
        else {
            
            if(connectClient(sender_addr, requestBuffer) == 0) {
                userColor(sender_addr.sin_port);
                strcat(responseBuffer, requestBuffer);
                strcat(responseBuffer, GREEN " connesso" RESET "\n");
                handle_send_messages(sender_addr, TRUE);
                sendClientList(sender_addr);
            }
            printClientList();
        }

    }

    close(sockfd);
    return EXIT_SUCCESS;
}
