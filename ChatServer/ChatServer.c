#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_THREADS 5

void *readSingleMessage(void *);

int main(int argc, char *argv[]){
    int serverSocket, new_socket, c;
    struct sockaddr_in server, client;
    pthread_t threads[MAX_THREADS];

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

    ///Auf dem erstellten Port lauschen & Anzahl der der Clients in der Warteschlange
    listen(serverSocket, 2);

    /// Verbindung zulassen und antworten
    puts("Warten auf Verbindungen...");
    for (int i = 0; i < MAX_THREADS; ++i) {
        c = sizeof(struct sockaddr_in);
        new_socket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&c);
        if (new_socket < 0) {
            perror("Anfrage verweigert!");
            break;
        }
        pthread_create(&threads[i], NULL, readSingleMessage, (void *) new_socket);
    }

    /// Threads, sowie Sockets schließen
    for (int j = 0; j < MAX_THREADS; ++j){
        pthread_join(threads[j], NULL);
    }

    close(serverSocket);

    puts("Verbindung wurde erfolgreich beendet!");

    return 0;
}

/// Anwort vom Server erhalten
void *readSingleMessage(void *clientSocket){
    int socket = (int) clientSocket;
    char antwort[2000];

    puts("Thread started");
    while (1){
        printf("Client: ");
        read(socket, antwort, sizeof(antwort));
        printf("%s\n", antwort); //TODO Ausgabe fixen
        if (strncmp(antwort, "exit", 4) == 0){
            break;
        }
    }

    close(socket);
    puts("Thread closed");

    pthread_exit(NULL);
}