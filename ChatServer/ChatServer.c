#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 9090
#define BUFFER 2000
#define MAX_THREADS 5

void *readSingleMessage(void *);
void writeMessages(int socket, char message[]);

int clientSockets[MAX_THREADS];

int main(int argc, char *argv[]){
    struct sockaddr_in server, client;
    int serverSocket, newClientSocket, c;
    pthread_t threads[MAX_THREADS];


    /// Array mit Sockets leeren
    for (int i = 0; i < MAX_THREADS; ++i) {
        clientSockets[i] = NULL;
    }


    /// Socket erstellen
    serverSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (serverSocket == -1) {
        perror("Socket konnte nicht erstellt werden");
    }


    /// Struct definieren (IP, Protokoll, Port)
    server.sin_addr.s_addr = INADDR_ANY; //-> Es wird der localhost verwendet; auch möglich: inet_addr(IP-Adresse)
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);


    /// Socket einbinden
    if(bind(serverSocket,(struct sockaddr*)&server , sizeof(server)) < 0) {
        perror("Socket einbinden fehlgeschlagen!");
        return 1;
    }

    /// Auf erstellten Port lauschen & Anzahl der der Clients in der Warteschlange
    listen(serverSocket, 3);


    /// Verbindung zulassen und antworten

    puts("Warten auf Verbindungen...");
    for (int i = 0; i < MAX_THREADS; ++i) {
        c = sizeof(struct sockaddr_in);

        newClientSocket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&c);
        clientSockets[i] = newClientSocket;

        if (newClientSocket < 0) {
            perror("Anfrage verweigert!");
            break;
        }

        pthread_create(&threads[i], NULL, readSingleMessage, (void *) newClientSocket);
    }


    /// Threads, sowie Sockets schließen
    for (int j = 0; j < MAX_THREADS; ++j){
        pthread_join(threads[j], NULL);
    }

    close(serverSocket);
    puts("Server erfolgreich beendet!");

    return 0;
}

/// Anwort von Client erhalten
void *readSingleMessage(void *clientSocket){
    int socket = (int) clientSocket;
    char message[BUFFER];

    while (1){
        printf("Client: ");
        read(socket, message, sizeof(message));

        // puts(message); // -> Nachricht von Clients ausgeben
        writeMessages(socket, message);

        if (strcmp(message, "exit") == 0){
            break;
        } else {
            /// Message leeren
            for (int i = 0; i < BUFFER; ++i) {
                message[i] = 0;
            }
        }
    }

    close(socket);
    pthread_exit(NULL);
}

///Nachricht an alle Clients schicken
void writeMessages(int socket, char message[]){
    int count = 0;
    while (clientSockets[count] != NULL) {

        /// Fehler bei der Ausgabe beheben
        for (int i = 0; i < BUFFER; ++i) {
            if (message[i] == '\n'){
                message[i] = '\0';
                break;
            }
        }

        // printf("Nachricht an Client[%i] geschrieben!\n", count);

        /// Nachricht die vom eingehenden Port kommen werden nicht doppelt versendet
        if (clientSockets[count] != socket){
            write(clientSockets[count] , message , strlen(message));
        }
        count++;
    }
}