#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Port 8888

int main(int argc, char *argv[]){
    int serverSocket, new_socket, c;
    struct sockaddr_in server, client;
    char *message;

    //Socket erstellen
    serverSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (serverSocket == -1) {
        perror("Socket konnte nicht erstellt werden");
    }

    //Konfiguration
    server.sin_addr.s_addr = INADDR_ANY; //-> Es wird der localhost verwendet; auch möglich: inet_addr(IP-Adresse)
    server.sin_family = AF_INET;
    server.sin_port = htons(Port);

    //Socket einbinden
    if(bind(serverSocket,(struct sockaddr*)&server , sizeof(server)) < 0) {
        perror("Socket einbinden fehlgeschlagen!");
        return 1;
    }

    //Auf dem erstellten Port lauschen
    listen(serverSocket, 3);

    //Verbindung zulassen und antworten
    puts("Warten auf Verbindungen...");
    c = sizeof(struct sockaddr_in);
    new_socket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&c);
    if (new_socket < 0) {
        perror("Anfrage verweigert!");
        return 1;
    }

    //Verbindungsbestätigung schicken
    message = "Anfrage von Client erhalten...";
    write(new_socket, message, strlen(message));

    puts("Verbindung wurde akzeptiert!");
    return 0;
}

