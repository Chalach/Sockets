#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
void readSingleMessage(int);

int main(int argc, char *argv[]){
    int serverSocket, new_socket, c;
    struct sockaddr_in server, client;
    char server_reply[2000];
    pthread_t pthread[5];

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
    while (1){
        c = sizeof(struct sockaddr_in);
        new_socket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&c);
        if (new_socket < 0) {
            perror("Anfrage verweigert!");
            return 1;
        }

        pthread_create(pthread, NULL, (void*(*)(void*)) &readSingleMessage, &socket);
        pthread_join((pthread_t) pthread, NULL);
    }

    /// Nachrichten vom Client lesen und bearbeiten
    while (1){
        read(new_socket, server_reply, 2000);
        puts(server_reply);

        if (strcmp(server_reply, "exit") == 0){
            break;
        } else if (strcmp(server_reply, "ein") == 0){
            puts("Raspi LED ON:");
        } else if (strcmp(server_reply, "aus") == 0){
            puts("Raspi LED OFF:");
        } else if (strcmp(server_reply, "blick") == 0){
            puts("Raspi LED BLINK:");
        }
    }

    /// Programm, sowie Sockets schließen
    puts("Verbindung wurde erfolgreich beendet!");
    close(serverSocket);
    close(new_socket);
    return 0;
}

/// Anwort vom Server erhalten
void readSingleMessage(int clientSocket){

    char server_reply[2000];
    puts("Get Message");
    getchar();
    while (1){
        read(clientSocket, server_reply, sizeof(server_reply));
//        if(recv(socket, server_reply , 2000 , 0) < 0) {
//            perror("Keine Antwort vom Server erhalten!");
//        }
        if (strcmp(server_reply, "exit") == 0){
            break;
        }
        puts(server_reply);
        puts("END FIRST LOOP");
        getchar();
    }
    close(clientSocket);
    //free(clientSocket);
    puts("Thread closed");
    pthread_exit(NULL);
}