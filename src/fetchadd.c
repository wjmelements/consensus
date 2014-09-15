#include "consensus.h"
#include "fetchadd.h"

#ifndef NULL
#define NULL (void*)0
#endif

#include <stdatomic.h>

void* _fa_arr[2];
uint16_t _fa_i;

void fetch_add_setup(void) {
    _fa_arr[0] = NULL;
    _fa_arr[1] = NULL;
    _fa_i = 0;
}

void* fetch_add_consensus(void* arg) {
    struct consensus_input* input = arg;
    uint16_t thread_id = input->thread_id;
    _fa_arr[thread_id] = input->input;
    uint16_t fetch = atomic_fetch_add(&_fa_i, 1);
    return _fa_arr[thread_id ^ fetch];
}
