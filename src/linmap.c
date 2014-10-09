#include <pthread.h>
#include <stdlib.h>

#include "linmap.h"
void linmap(size_t n, void(*method)(size_t)) {
    pthread_t* threads = malloc(n * sizeof(pthread_t));
    for (size_t i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, (void* (*) (void*)) method, (void*) i);
    }
    for (size_t i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
}
