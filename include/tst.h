#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "consensus.h"

#define str(X) #X
#define stringify(x) str(x)

#ifndef map
#define map linmap
#endif
#define map_header map.h
#include stringify(map_header)


#define TRIALS (0xFFF / CONSENSUS_NUMBER)
struct consensus_input* args[CONSENSUS_NUMBER];
void* results[CONSENSUS_NUMBER];
void map_method(size_t i) {
    args[i] = malloc(sizeof(struct consensus_input));
    args[i]->thread_id = i;
    args[i]->input = malloc(0);
    results[i] = CONSENSUS_METHOD(args[i]);
}
int main() {
    for (size_t i = 0; i < TRIALS; i++) {
        CONSENSUS_SETUP;
        map(CONSENSUS_NUMBER, map_method);
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
        for (size_t j = 0 ; j < CONSENSUS_NUMBER; j++) {
            free(args[j]->input);
            free(args[j]);
        }
    }
    return 0;
}
