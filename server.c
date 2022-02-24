#include "server.h"


int main(int argc, char **argv){
    if(argc != 2){
        printf("ERRORE - argomenti\nUscita in corso...\n\n");
        return EXIT_FAILURE;
    }

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);
    int option = 1;
    int listenfd = 0, connfd = 0;

	printf("Porta in uso %d\n\n",port);

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
    
        // Impostazioni del Client
        client_t *client = (client_t *)malloc(sizeof(client_t));    // aggiunta indrizzo e id
        client->address = client_addr;
        client->sockfd = connfd;
        client->uid = uid++;
    
        queue_add(client);
        pthread_create(&tid, NULL, &handle_client, (void*)client);

        sleep(1);
    }

    return EXIT_SUCCESS;
}
