#include "consensus.h"
#include "xchg.h"

#ifndef NULL
#define NULL (void*)0
#endif

#include <stdatomic.h>

static void* volatile _xchg_arr[2];
static volatile uint16_t _xchg_i;

void xchg_setup(void) {
    _xchg_arr[0] = NULL;
    _xchg_arr[1] = NULL;
    _xchg_i = 0;
}

void* xchg_consensus(void* arg) {
    struct consensus_input* input = arg;
    uint16_t thread_id = input->thread_id;
    _xchg_arr[thread_id] = input->input;
    uint16_t fetch = atomic_exchange(&_xchg_i, 1);
    return _xchg_arr[thread_id ^ fetch];
}
