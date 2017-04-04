#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int createSocket();
void createConnection(int socket);

int main(){
    createConnection(createSocket());

    return 0;
}

int createSocket(){
    int sSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (sSocket == -1){
        perror("Socket wurde nicht erstellt!");
        return 1;
    }
    return sSocket;
}

void createConnection(int socket){
    if (connect(socket, (sockaddr *) & serv_addr, sizeof(sockaddr)) == -1) {
        perror ("connect()");
    }
}

