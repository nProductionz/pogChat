#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>


// Dichiarazione delle funzioni

void send_message(char *s, int uid);
void *handle_client(void *arg);
void print_client_addr(struct sockaddr_in addr);
void queue_remove(int uid);
void str_trim (char* arr, int length);
void str_overwrite_stdout();