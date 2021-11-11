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

// variabili globali

#define LENGTH 2086

// funzioni
void catch_ctrl_c_exit(int sig);
void send_msg_handler();
void recv_msg_handler();
void str_trim (char* arr, int legth);
void str_overwrite_stdout();