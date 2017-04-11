#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char *argv[]){
    int serverSocket, new_socket, c;
    struct sockaddr_in server, client;
    //char* message;
    char server_reply[2000];

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
    listen(serverSocket, 1);

    /// Verbindung zulassen und antworten
    puts("Warten auf Verbindungen...");
    c = sizeof(struct sockaddr_in);
    new_socket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&c);
    if (new_socket < 0) {
        perror("Anfrage verweigert!");
        return 1;
    }

    /// Evtl. Antwortnachricht
    //message = "Anfrage von Client erhalten...";

    /// Nachrichten vom Client lesen und bearbeiten
    while (1){
        if (strncmp(server_reply, "exit", 4) == 0){
            break;
        }
        //write(new_socket, message, strlen(message));
        read(new_socket, server_reply, 2000);
        puts(server_reply);
    }

    puts("Verbindung wurde erfolgreich beendet!");
    return 0;
}

