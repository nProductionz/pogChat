#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 20
#define BUFFER_SIZE 2048

void send_message(char *s, int uid);
void *handle_client(void *arg);