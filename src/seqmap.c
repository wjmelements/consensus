#include <stdlib.h>
#include "seqmap.h"

void seqmap(size_t n, void (*method)(size_t)) {
    for (size_t i = 0; i < n; i++) {
        method(i);
    }
}
