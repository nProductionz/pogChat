#define BUF_SIZE 1024
#define USERNAME_LEN 64

// Server action definitions
// Comandi utilizzati per il DEBUG

#define CLOSE "close\n"
#define EXIT "exit\n"
#define ERROR "error\n"

// Per non dover concatenare ogni volta le stringhe
#define USERNAMExMESSAGE ": "

// Definire i booleani per comodità
#define TRUE 1
#define FALSE 0

// Gestione errori 
#define SYSERR -1


// I COLORI
#define NCOLORS 5
#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define WHITE  "\x1B[37m"
#define RESET "\x1B[0m"
