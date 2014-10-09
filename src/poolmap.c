#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logmap.h"
#include "poolmap.h"

static long max;

// Currently no concurrent uses of poolmap are possible
static size_t n;
static void (*func)(size_t);
static void pool(size_t i) {
    while (i < n) {
        func(i);
        i += max;
    }
}

void poolmap(size_t N, void(*method)(size_t)) {
    max = sysconf(_SC_NPROCESSORS_ONLN);
    if (max == -1) {
        perror("_SC_NPROCESSORS_ONLN");
        exit(errno);
    }
    n = N;
    func = method;
    logmap(max < N ? max : N, pool);
}
