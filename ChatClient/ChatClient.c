#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 9090
#define BUFFER 2000

void *readMessage(void *);

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
    if (connect(clientSocket , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Verbindung zum Server fehlgeschlagen!");
        return 1;
    }


    /// Thread für's lesen von Nachrichten erstellen
    pthread_create(&thread, NULL, readMessage, (void *) clientSocket);


    /// Chat
    while (1){
        printf("Message: ");
        fgets(message, sizeof(message), stdin);

        /// Fehler bei der Ausgabe am Server beheben
        for (int i = 0; i < BUFFER; ++i) {
            if (message[i] == '\n'){
                message[i] = '\0';
                break;
            }
        }
        write(clientSocket, message, strlen(message));

        /// Message leeren
        for (int i = 0; i < BUFFER; ++i) {
            message[i] = 0;
        }
        if (strcmp(message, "exit") == 0){
            puts("Beenden!");
            break;
        }
    }

    /// Thread, sowie Sockets schließen

    // TODO: Client wird nicht richtig beendet
    pthread_join(thread, NULL);
    close(clientSocket);
    puts("Client erfolgreich beendet");
    return 0;
}

void *readMessage(void *clientSocket){
    int socket = (int) clientSocket;
    char nachricht[BUFFER];

    while (1){
        read(socket, nachricht, sizeof(nachricht));

        /// Fehler bei der Ausgabe am Server beheben
        for (int i = 0; i < BUFFER; ++i) {
            if (nachricht[i] == '\n'){
                nachricht[i] = '\0';
                break;
            }
        }

        if (strcmp(nachricht, "exit") == 0){
            break;
        }
        printf("\nServer: %s\n", nachricht);

        /// Message leeren
        for (int i = 0; i < BUFFER; ++i) {
            nachricht[i] = 0;
        }
    }

    close(socket);
    pthread_exit(NULL);
}