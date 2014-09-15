#include "consensus.h"
#include "cmpxchg.h"

#ifndef NULL
#define NULL (void*)0
#endif

#include <stdatomic.h>

void* _cmpxchg_result;

void cmpxchg_setup(void) {
    _cmpxchg_result = NULL;
}

void* cmpxchg_consensus(void* arg) {
    struct consensus_input* input = arg;
    void* expected = NULL;
    atomic_compare_exchange_strong(&_cmpxchg_result, &expected, input->input);
    return _cmpxchg_result;
}
