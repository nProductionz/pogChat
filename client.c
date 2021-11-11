#include "client.h"

int sockfd = 0;
char username[36];

void catch_ctrl_c_exit(int sig) {
    deadEnd_flag = 1;
}

int main(int argc, char **argv) {
    if(argc != 2){
        printf("Porta in uso: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }   

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);w

    signal(SIGINT, catch_ctrl_c_exit); // Per terminare il processo con CTRL + C

    printf("Inserire l'username: ");
    fgets(username, 36, stdin);
    //
    // verifica che l'username rispetti i parametri
    if(strlen(username) > 36 || strlen(username) < 2) {
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
    send(sockfd, username, 36, 0)
    // sperando che tutto funzioni...
    printf("+++ BENVENUTO NELLA POGCHAT +++\n");
    // Creare un thread per inviare e gestire i messaggi
    //

}
