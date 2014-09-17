#include "consensus.h"
#include "xchg.h"

#ifndef NULL
#define NULL (void*)0
#endif

#include <stdatomic.h>

void* volatile _fa_arr[2];
volatile uint16_t _fa_i;

void xchg_setup(void) {
    _fa_arr[0] = NULL;
    _fa_arr[1] = NULL;
    _fa_i = 0;
}

void* xchg_consensus(void* arg) {
    struct consensus_input* input = arg;
    uint16_t thread_id = input->thread_id;
    _fa_arr[thread_id] = input->input;
    uint16_t fetch = atomic_exchange(&_fa_i, 1);
    return _fa_arr[thread_id ^ fetch];
}
