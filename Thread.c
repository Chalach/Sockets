#include <stdio.h>
#include <pthread.h>

#define MAX_CLIENTS 5

void *print_char (void *ch) {
    for (int i = 0; i < 10; i++){
        printf ("%c", *(char*)ch);
    }
    puts("");
    pthread_exit(NULL);
}

int main () {
    char ch1='*';
    pthread_t threads[MAX_CLIENTS];

    /// Länge von Array bestimmen (Nur einmal im Code möglich)
    int lenght = (int) sizeof(threads) / sizeof(threads[0]);
    printf("%i\n", lenght);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (pthread_create(&threads[i], NULL, print_char, &ch1) != 0){
            puts("Thread konnte nicht gestartet werden!");
        }
    }

    for (int j = 0; j < MAX_CLIENTS; ++j) {
        pthread_join(threads[j], NULL);
    }

    puts("END");
    return 0;
}