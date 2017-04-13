#include <stdio.h>
#include <pthread.h>

void *print_char (void *ch) {
    int i;
    for (i=0; i<10; i++)
        printf ("%c", *(char*)ch);
    puts("");
    pthread_exit(NULL);
}

int main () {
    char ch1='*';
    pthread_t threads[5];

    /// Länge von Array bestimmen
    int lenght = (int) sizeof(threads) / sizeof(threads[0]);
    printf("%i\n", lenght);

    for (int i = 0; i < lenght; ++i) {
        pthread_create(&threads[i], NULL, print_char, &ch1);
    }

    for (int j = 0; j < lenght; ++j) {
        pthread_join(threads[j], NULL);
    }

    puts("END");
    return 0;
}