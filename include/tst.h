#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "consensus.h"

int main() {
    pthread_t threads[CONSENSUS_NUMBER];
    // TODO logmap this instead
    struct consensus_input* args[CONSENSUS_NUMBER];
    void* results[CONSENSUS_NUMBER];
    for (size_t i = 0; i < CONSENSUS_NUMBER; i++) {
        args[i] = malloc(sizeof(struct consensus_input));
        args[i]->thread_id = i;
        args[i]->input = malloc(4);
        pthread_create(threads + i, NULL, CONSENSUS_METHOD, args[i]);
    }
    for (size_t i = 1; i < CONSENSUS_NUMBER; i++) {
        pthread_join(threads[i], results + i);
    }
    for (size_t i = 0; i < CONSENSUS_NUMBER; i++) {
        assert(results[i] == results[i-1]);
    }
    return 0;
}
