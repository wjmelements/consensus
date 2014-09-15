#include <pthread.h>
#include <stdlib.h>

struct logmap_arg {
    size_t i; // 1-indexed
    size_t n;
    void (*method)(size_t);
};
void* logmap_thread(void* varg) {
    struct logmap_arg* arg = varg;
    const size_t i = arg->i;
    const size_t n = arg->n;
    pthread_t threads[2];
    for (size_t j = 0; j < 2; j++) {
        size_t child_i = (i << 1) + j;
        if (child_i <= n) {
            struct logmap_arg* child_arg = malloc(sizeof(struct logmap_arg));
            child_arg->i = child_i;
            child_arg->n = n;
            child_arg->method = arg->method;
            pthread_create(&threads[j], NULL, logmap_thread, child_arg);
        }
    }
    arg->method(i-1);
    for (size_t j = 0; j < 2; j++) {
        size_t child_i = (i << 1) + j;
        if (child_i <= n) {
            pthread_join(threads[j], NULL);
        }
    }
    free(arg);
    return NULL;
}
void logmap(size_t n, void (*method)(size_t)) {
    if (n) {
        struct logmap_arg* arg = malloc(sizeof(struct logmap_arg));
        arg->i = 1;
        arg->n = n;
        arg->method = method;
        logmap_thread(arg);
    }
}
