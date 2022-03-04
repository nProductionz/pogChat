# pogChat
PogChat: A multithread chatroom for the O.S. Exam 2020/21

Il progetto permette di comunicare tramite l'utilizzo di client UDP.

I client si connettono ad un server comune, che fa solo da manager dei messaggi: prende un messaggio da un mittente e lo invia a tutti gli altri client connessi.

Questi da prima son stati inseriti in una struttura connessa (LinkedList artigianale) che permette di avere un elenco di tutti coloro che son connessi al server.


I client a loro volta invece utilizzano due thread ciascuno:

1. Permette di controllare se sono presenti nuovi messaggi da parte del server (che gira i messaggi degli altri utenti), li scarica e mostra a schermo.
2. Permette di inviare messaggi al server, che li invierà agli altri utenti connessi.

Sono stati implementati alcuni comandi utilizzati per il debug del programma, disponibili all'interno del file "utils.h", insieme ad altri elementi utili per entrambi i programmi.