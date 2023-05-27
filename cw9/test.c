#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 10

typedef struct {
    int *array;
    int index;
} ThreadArg;

void *threadFunction(void *arg) {
    ThreadArg *threadArg = (ThreadArg *)arg;
    int index = threadArg->index;
    int element = threadArg->array[index];
    printf("Wątek o indeksie %d odczytał element: %d\n", index, element);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadArg threadArgs[NUM_THREADS];
    int array[ARRAY_SIZE];

    // Inicjalizacja tablicy
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }

    // Tworzenie wątków
    for (int i = 0; i < NUM_THREADS; i++) {
        threadArgs[i].array = array;
        threadArgs[i].index = i;
        pthread_create(&threads[i], NULL, threadFunction, (void *)&threadArgs[i]);
    }

    // Oczekiwanie na zakończenie wątków
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
