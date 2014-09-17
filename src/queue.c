#include "consensus.h"
#include "queue.h"

#include <stdatomic.h>
#include <stdint.h>

static void* _q_arr[2];

#define _q_size 5
static uint16_t _q_q[_q_size];
static uint16_t _q_t = 0;
static uint16_t _q_h = 0;

// being wait-free, undefined behavior occurs if the queue is full for enq
static void enq(uint16_t data) {
    // TODO wait-free this
    _q_q[_q_h++ % _q_size] = data;
}

// being wait-free, undefined behavior occurs if the queue is empty for deq
static uint16_t deq(void) {
    uint16_t i = atomic_fetch_add(&_q_t, 1);
    return _q_q[i % _q_size];
}

void queue_setup(void) {
    enq(0);
    enq(1);
}

void* queue_consensus(void* arg) {
    struct consensus_input* input = arg;
    uint16_t id = input->thread_id;
    _q_arr[id] = input->input;
    uint16_t i = deq();
    return _q_arr[id ^ i];
}
