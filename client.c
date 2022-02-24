#include "client.h"

// Variabili globali.
int sockfd = 0;
char username[32];
volatile sig_atomic_t deadEnd_flag = 0;

void catch_ctrl_c_exit(int sig) {
    deadEnd_flag = 1;
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

    
    socklen_t addr_size;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, '\0', sizeof(server_addr));

      
}