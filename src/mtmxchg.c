#include "consensus.h"
#include "mtmxchg.h"

#ifndef NULL
#define NULL (void*)0
#endif

#include <pthread.h>
#include <stdatomic.h>
#include <strings.h>

/**
 * Atomic memory-to-memory xchg is not possible in every architecture that I am aware of.
 * Therefore, these methods only provide an abstraction of memory-to-memory-xchg.
**/

#define NUM_SECTIONS 2

static long memory[NUM_SECTIONS][0xFFF];
static pthread_mutex_t locks[NUM_SECTIONS];

static long read(uint16_t section, uint16_t loc) {
    pthread_mutex_lock(&locks[section]);
    long ret = memory[section][loc];
    pthread_mutex_unlock(&locks[section]);
    return ret;
}

static void write(uint16_t section, uint16_t loc, long val) {
    pthread_mutex_lock(&locks[section]);
    memory[section][loc] = val;
    pthread_mutex_unlock(&locks[section]);
}

static void xchg(uint16_t section, uint16_t loc1,  uint16_t loc2) {
    pthread_mutex_lock(&locks[section]);
    memory[section][loc1] ^= memory[section][loc2];
    memory[section][loc2] ^= memory[section][loc1];
    memory[section][loc1] ^= memory[section][loc2];
    pthread_mutex_unlock(&locks[section]);
}

void mtmxchg_setup(void) {
    bzero(memory, sizeof(memory));
    for (uint16_t i = 0; i < NUM_SECTIONS; i++) {
        pthread_mutex_init(&locks[i], NULL);
    }
}

#define VALUE_SECTION 0
#define INDEX_SECTION 1

void* mtmxchg_consensus(void* arg) {
    struct consensus_input* input = arg;
    size_t id = input->thread_id + 1;
    write(VALUE_SECTION, id, (long) input->input);
    write(INDEX_SECTION, id, id);
    xchg(INDEX_SECTION, 0, id);
    uint16_t curr = id;
    while (1) {
        uint16_t next = read(INDEX_SECTION, curr);
        if (next == 0) {
            return (void*) read(VALUE_SECTION, curr);
        }
        curr = next;
    }
}

