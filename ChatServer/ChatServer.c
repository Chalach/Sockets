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
#define MAX_CLIENTS 16

void *readSingleMessage(void *);
void writeMessages(int socket, char message[]);

typedef struct {
    int clientSockets[MAX_CLIENTS];
    char* usernames[MAX_CLIENTS];
}Client;

Client clientConfig;
int clientCount = 0;

// TODO: Username mit Client schicken

int main(int argc, char *argv[]){
    struct sockaddr_in server, client;
    int serverSocket, c;
    pthread_t threads[MAX_CLIENTS];


    /// Array mit Sockets leeren
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clientConfig.clientSockets[i] = NULL;
    }


    /// Socket erstellen
    serverSocket = socket(AF_INET, SOCK_STREAM , 0);
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
    for (int i = 0; i < MAX_CLIENTS; i++) {
        c = sizeof(struct sockaddr_in);
        clientConfig.clientSockets[i] = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&c);

        if (clientConfig.clientSockets[i] < 0) {
            perror("Anfrage verweigert!");
            break;
        }

        char username[50];
        read(clientConfig.clientSockets[i], username, sizeof(username));

        char* usr = username;
        clientConfig.usernames[i] = usr;

        clientCount++;
//        puts(clientCount);
        pthread_create(&threads[i], NULL, readSingleMessage, (void *) clientConfig.clientSockets[i]);
        printf("%s hat sich angemeldet.\n", username);
    }


    /// Threads, sowie Sockets schließen
    for (int j = 0; j < MAX_CLIENTS; ++j){
        write(clientConfig.clientSockets[j], "close", 5);
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

    int myClient = clientCount;

    while (1){
        read(socket, message, sizeof(message));
        if (strcmp(message, "exit") == 0){
            puts("Verbindung beenden...");
            write(socket, "exit", 4);
            close(socket);
            printf("Verbindung zu %s wurde getrennt!\n", clientConfig.usernames[myClient]);
            pthread_exit(NULL);
        }

        printf("%s: ", clientConfig.usernames[myClient]);
        puts(message);
        writeMessages(socket, message);
    }
}

///Nachricht an alle Clients schicken
void writeMessages(int socket, char message[]){

    for (int i = 0; i < clientConfig.clientSockets[i] != NULL; ++i) {
        if (clientConfig.clientSockets[i] != socket){ /// -> Nachricht soll nicht mehr an den Eingehenden geschickt werden
            write(clientConfig.clientSockets[i] , message, sizeof(message));
            printf("Nachricht an %s geschrieben!\n", clientConfig.usernames[i]);
        }
    }

    for (int j = 0; j < BUFFER; ++j) {
        message[j] = 0;
    }
}