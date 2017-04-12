#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Port 8080

int main(int argc , char *argv[]) {
    int clientSocket;
    struct sockaddr_in server;
    char message[100];

    /// Socket erstellen
    clientSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (clientSocket == -1) {
        perror("Socket konnte nicht erstellt werden");
    }

    /// Struct definieren (IP, Protokoll, Port)
    server.sin_addr.s_addr = INADDR_ANY; //-> Es wird der localhost verwendet; auch möglich: inet_addr(IP-Adresse)
    server.sin_family = AF_INET;
    server.sin_port = htons(Port);

    ///Verbindung zum Server aufbauen
    if (connect(clientSocket , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Verbindung zum Server fehlgeschlagen!");
        return 1;
    }
    puts("Verbindung hergestellt...");

    /// Nachrichten an den Server schicken; evtl. auch lesen (mehrfach)
    while (1){
        //read(clientSocket, server_reply, sizeof(server_reply));
        puts("Message:");
        fgets(message, sizeof(message), stdin);

        /// Fehler bei der Ausgabe am Server beheben
        for (int i = 0; i < 2000; ++i) {
            if (message[i] == '\n'){
                message[i] = '\0';
                break;
            }
        }
        //read(clientSocket, server_reply, 2000);
        write(clientSocket, message, strlen(message));
        if (strcmp(message, "exit") == 0){
            break;
        }
    }

    /// Programm, sowie Sockets schließen
    puts("Client beendet");
    close(clientSocket);
    return 0;
}

/// Anwort vom Server erhalten
void readSingleMessage(int clientSocket, char server_reply[2000]){
    if(recv(clientSocket, server_reply , 2000 , 0) < 0) {
        perror("Keine Antwort vom Server erhalten!");
    }
}
///Nachricht an den Server schicken
int writeSingleMessage(int clientSocket, char* message){
    if(send(clientSocket , message , strlen(message) , 0) < 0) {
        perror("Nachricht konnt nicht versendet werden!");
        return 1;
    }
    return 0;
}