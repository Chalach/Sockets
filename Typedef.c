#include <stdio.h>

typedef struct {
    int socket[16];
    char* username[16];
}Client;

int main(int argc , char *argv[]) {
    Client client;

    client.username[0] = "Peter";
    client.username[1] = "Walter";

    client.socket[0] = 1;
    client.socket[1] = 0;

    for (int i = 0; i < 2; ++i) {
        printf("Username: %s\n", client.username[i]);
    }

    for (int j = 0; j < 2; ++j) {
        printf("Username: %i\n", client.socket[j]);
    }

    char name[20];
    printf("Name: ");
    scanf("%s", name);
    printf(name);

    return 0;
}
