#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Port 8888

int main(int argc , char *argv[]) {
    int clientSocket;
    struct sockaddr_in server;
    char *message , server_reply[2000];

    //Socket erstellen
    clientSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (clientSocket == -1) {
        perror("Socket konnte nicht erstellt werden");
    }

    server.sin_addr.s_addr = INADDR_ANY; //-> Es wird der localhost verwendet; auch möglich: inet_addr(IP-Adresse)
    server.sin_family = AF_INET;
    server.sin_port = htons(Port);

    //Verbindung zum Server aufbauen
    if (connect(clientSocket , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Verbindung zum Server fehlgeschlagen!");
        return 1;
    }

    puts("Verbindung hergestellt...");

    //Nachricht an den Server schicken
    message = "Hello World";
    if(send(clientSocket , message , strlen(message) , 0) < 0) {
        perror("Nachricht konnt nicht versendet werden!");
        return 1;
    }
    puts("Nachricht versendet...");

    //Anwort vom Server erhalten
    if(recv(clientSocket, server_reply , 2000 , 0) < 0) {
        perror("Keine Antwort vom Server erhalten!");
    }
    read(clientSocket, server_reply, 2000);
    puts(server_reply);
    puts("Anwort erhalten...");

    return 0;
}