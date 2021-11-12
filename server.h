#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>


// Variabili globali

#define MAX_CLIENTS 20
#define BUFFER_SIZE 2048

// Dichiarazione delle funzioni

void send_message(char *s, int uid);
void *handle_client(void *arg);
void print_client_addr(struct sockaddr_in addr);
void queue_remove(int uid);
void str_trim (char* arr, int length);
void str_overwrite_stdout();