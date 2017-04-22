#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER 2000

void *readMessage(void *);
char username[50];

int main(int argc , char *argv[]) {
    struct sockaddr_in server;
    char message[BUFFER];
    int clientSocket;
    pthread_t thread;

    /// Socket erstellen
    clientSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (clientSocket == -1) {
        perror("Socket konnte nicht erstellt werden");
    }


    /// Struct definieren (IP, Protokoll, Port)
    server.sin_addr.s_addr = INADDR_ANY; //-> Es wird der localhost verwendet; auch möglich: inet_addr(IP-Adresse)
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);


    /// Verbindung zum Server aufbauen
    if (connect(clientSocket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Verbindung zum Server fehlgeschlagen!");
        return 1;
    }

    printf("Username: ");
    scanf("%s", username);
    fflush(stdin);

    write(clientSocket, username, strlen(username));

    /// Thread für's lesen von Nachrichten erstellen
    pthread_create(&thread, NULL, readMessage, (void *) clientSocket);


    char myMessage[BUFFER];
    /// Chat
    while (1){
        scanf("%s", message);
        fflush(stdin);

        for (int j = 0; j < BUFFER; ++j) {
            if (message[j] == '\n'){
                message[j] = '\0';
            }
        }

        for (int i = 0; i < BUFFER; ++i) {
            myMessage[i] = 0;
        }

        strcat(myMessage, username);
        strcat(myMessage, ": ");
        strcat(myMessage, message);

        write(clientSocket, myMessage, strlen(myMessage));

        if (strcmp(message, "exit") == 0){
            pthread_join(thread, NULL);
            close(clientSocket);
            printf("Verbindung zum Server wurde beendet!");
            return 0;
        }
    }
}

void *readMessage(void *clientSocket){
    int socket = (int) clientSocket;
    char nachricht[BUFFER];

    while (1){
        read(socket, nachricht, sizeof(nachricht));

        if (strstr(nachricht, "exit")){
            close(socket);
            pthread_exit(NULL);
        }

        puts(nachricht);
    }
}