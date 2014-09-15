#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "consensus.h"

int main() {
    CONSENSUS_SETUP;
    pthread_t threads[CONSENSUS_NUMBER];
    struct consensus_input* args[CONSENSUS_NUMBER];
    void* results[CONSENSUS_NUMBER];
    // TODO logmap this instead
    for (size_t i = 0; i < CONSENSUS_NUMBER; i++) {
        args[i] = malloc(sizeof(struct consensus_input));
        args[i]->thread_id = i;
        args[i]->input = malloc(4);
        pthread_create(&threads[i], NULL, CONSENSUS_METHOD, args[i]);
    }
    for (size_t i = 0; i < CONSENSUS_NUMBER; i++) {
        pthread_join(threads[i], &results[i]);
    }
    for (size_t i = 1; i < CONSENSUS_NUMBER; i++) {
        assert(results[i] == results[i-1]);
    }
    int found = 0;
    for (size_t j = 0; j < CONSENSUS_NUMBER; j++) {
        void* input = args[j]->input;
        if (input == results[0]) {
            found = 1;
            break;
        }
    }
    assert(found);

    return 0;
}
