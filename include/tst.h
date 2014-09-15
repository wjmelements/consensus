#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "consensus.h"
#include "logmap.h"

#define TRIALS 10
struct consensus_input* args[CONSENSUS_NUMBER];
void* results[CONSENSUS_NUMBER];
void logmap_method(size_t i) {
    args[i] = malloc(sizeof(struct consensus_input));
    args[i]->thread_id = i;
    args[i]->input = malloc(4);
    results[i] = CONSENSUS_METHOD(args[i]);
}
int main() {
    for (size_t i = 0; i < TRIALS; i++) {
        CONSENSUS_SETUP;
        logmap(CONSENSUS_NUMBER, logmap_method);
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
    }
    return 0;
}
