# pogChat
PogChat: A multithread chatroom for the O.S. Exam 2020/21

Il progetto prevede l'utilizzo di un server e un client per il funzionamento di una Chatroom multithread.
Il server riceve in ingresso le richieste di connessione dei client, e funziona da tramite per permettere ad ogni client di ricevere i messaggi inviati dagli altri.

Al momento dell'avvio, si lancia il server indicando su che porta accetta le connessioni. (l'ip è impostato di default su 127.0.0.1 per evitare danni)
In seguito si avviano i client, specificando la stessa porta su cui comunica il server.

All'avvio del client, viene richiesto di inserire uno username, che verrà visualizzato all'interno della chat.
Dopodiché, il sistema di chat viene avviato ed è possibile iniziare a messaggiare.
Nel momento in cui un utente digiterà "exit" o la combinazione Ctrl+C, il client "morirà" e il server lo rimuoverà dalla coda dei client attivi, interrompendo il relativo thread.
