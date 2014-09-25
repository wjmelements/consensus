#include "consensus.h"
#include "queue.h"

#include <stdatomic.h>
#include <stdint.h>

#ifndef NULL
#define NULL ((void*) 0)
#endif

// for our particular algorithm, the queue only need have size one
static void* queues[2];

// restriction: queue size is one
static void enq(uint16_t q_id, void* data) {
    queues[q_id] = data;
}

// being wait-free, return NULL when empty
static void* deq(uint16_t q_id) {
    return atomic_exchange(&queues[q_id], NULL);
}

void queue_setup(void) {
    // empty both queues
    deq(0);
    deq(1);
}

void* queue_consensus(void* arg) {
    struct consensus_input* input = arg;
    uint16_t id = input->thread_id;
    uint16_t their_id = id ^ 1;
    void* mine = input->input;
    enq(id, mine);
    void* theirs = deq(their_id);
    if (theirs) {
        if (deq(id)) {
            return theirs;
        } else {
            return theirs < mine ? theirs : mine;
        }
    } else {
        return mine;
    }
}
