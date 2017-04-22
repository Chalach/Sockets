#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER 2000
#define MAX_CLIENTS 16

void *readSingleMessage(void *);
void writeMessages(int socket, char message[]);

int clientSockets[MAX_CLIENTS];

int main(int argc, char *argv[]){
    struct sockaddr_in server, client;
    int serverSocket, c;
    pthread_t threads[MAX_CLIENTS];


    /// Array mit Sockets leeren
    for (int i = 0; i < MAX_CLIENTS; ++i) {
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
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        c = sizeof(struct sockaddr_in);

        clientSockets[i] = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&c);

        if (clientSockets[i] < 0) {
            perror("Anfrage verweigert!");
            break;
        }

        char username[50];
        read(clientSockets[i], username, sizeof(username));
        printf("%s hat sich zum Server verbunden!\n", username);

        pthread_create(&threads[i], NULL, readSingleMessage, (void *) clientSockets[i]);

    }


    /// Threads, sowie Sockets schließen
    for (int j = 0; j < MAX_CLIENTS; ++j){
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
        read(socket, message, sizeof(message));

        if (strstr(message, "exit")){
            write(socket, "exit", 4);
            close(socket);

            char* username;
            const char s[2] = ":";
            username = strtok(message,s);
            printf("Verbindung zu %s wurde beendet!\n", username);
            pthread_exit(NULL);
        }
//        puts(message); // -> Nachricht von Clients ausgeben

        writeMessages(socket, message);
    }
}

///Nachricht an alle Clients schicken
void writeMessages(int socket, char message[]){
    for (int i = 0; i < clientSockets[i] != NULL; ++i) {
        /// Nachricht die vom eingehenden Port kommen werden nicht doppelt versendet
        if (clientSockets[i] != socket){
            write(clientSockets[i], message, strlen(message));
        }
    }
}