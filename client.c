#include "client.h"

// Variabili globali.
int sockfd = 0;
char username[32];
volatile sig_atomic_t deadEnd_flag = 0;

void catch_ctrl_c_exit(int sig) {
    deadEnd_flag = 1;
}

// trim per ripulire la stringa nel caso di andate a capo
void str_trim (char* arr, int length){
    int i;
    for( i = 0; i < length; i++) {
        if(arr[i] == '\n'){
            arr[i] = '\0';
            break;
        }
    }
}

// per printare il nome prima del messaggio
void str_overwrite_stdout() {
    printf("%s", "> ");
    fflush(stdout);
}

void send_msg_handler() {
    char msg[LENGTH] = {};
    char buffer[LENGTH + 34] = {};
    
    while(1) {
        str_overwrite_stdout();
        fgets(msg, LENGTH, stdin);
        str_trim(msg, LENGTH);

        if (strcmp(msg, "exit") == 0) {
            break;  // uscire dalla chat
        }   else    {
            sprintf(buffer, "%s: %s\n", username, msg);
            send(sockfd, buffer, strlen(buffer), 0);
        }
        bzero(msg, LENGTH);
        bzero(buffer, LENGTH + 32);
    }
    catch_ctrl_c_exit(2);
}

void recv_msg_handler(){
    char message[LENGTH] = {};
    while(1) {
        int receive = recv(sockfd, message, LENGTH, 0);
        if(receive > 0) {
            printf("%s", message);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;   // se non riceve, chiudere il ciclo di ricezione
        } else {
            // -1
        }
        memset(message, 0, sizeof(message));    // azzera il messaggio
    }
}

int main(int argc, char **argv) {
    if(argc != 2){
        printf("Porta in uso: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }   

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);

    signal(SIGINT, catch_ctrl_c_exit); // Per terminare il processo con CTRL + C

    printf("Inserire l'username: ");
    fgets(username, 32, stdin);
    str_trim(username, strlen(username));
    // verifica che l'username rispetti i parametri
    if(strlen(username) > 32 || strlen(username) < 2) {
        printf("Non fare scherzi e rispetta le regole col nome.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;

    // Impostazioni della socket per la connessione
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // Connessione col server
    int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(err == -1) {
        printf("ERROR: connect\n");
        return EXIT_FAILURE;
    }    

    // Inviare l'username al server
    send(sockfd, username, 32, 0);
    // sperando che tutto funzioni...
    printf("+++ BENVENUTO NELLA POGCHAT +++\n");
    // Creare un thread per inviare e gestire i messaggi
    pthread_t send_msg_thread;
    if(pthread_create(&send_msg_thread, NULL, (void *)send_msg_handler, NULL) != 0){
        // implementare la funzione per ricevere messaggi
        printf("ERROR: creazione del thread\n");
        return EXIT_FAILURE;
    }
    pthread_t recv_msg_thread;
    if(pthread_create(&recv_msg_thread, NULL, (void *)recv_msg_handler, NULL) != 0) {
        printf("ERROR: creazione pthread del recv >\n");
        return EXIT_FAILURE;
    }
    while(1) {
        if(deadEnd_flag) {   // quando si attiva la deadEnd muore il main
            printf("\nBella, se beccamo.\n");
            break;
        }
    }
    close(sockfd);
    return EXIT_SUCCESS;
}
