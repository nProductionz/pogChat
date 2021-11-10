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
    int port = atoi(argv[1]);

    signal(SIGINT, catch_ctrl_c_exit); // Per terminare il processo con CTRL + C

    printf("Inserire l'username: ");
    fgets(username, 36, stdin);
    //
    if(strlen(name) > 36 || strlen(username) < 2) {
        printf("Non fare scherzi e rispetta le regole col nome.\n");
        return EXIT_FAILURE;
    }

    

}
