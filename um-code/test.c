#include "segment.h"
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
/*
int main() {
    Seg_T new_seg = Seg_new(50000000);

    for (unsigned i = 0; i < 50000000; i++) {
        Seg_set(new_seg, i, i);
    }

    // assert(Seg_len(new_seg) == 50000000);

    for (unsigned i = 0; i < 50000000; i++) {
        assert(Seg_get(new_seg, i) == i);
    }

    Seg_free(&new_seg);

    return EXIT_SUCCESS;
}
*/
int main() {
    uint32_t *new_seg = malloc(sizeof(uint32_t) * 50000000);

    for (unsigned i = 0; i < 50000000; i++) {
        new_seg[i] = i;
    }

    // assert(Seg_len(new_seg) == 50000000);

    for (unsigned i = 0; i < 50000000; i++) {
        assert(new_seg[i] == i);
    }

    free(new_seg);

    return EXIT_SUCCESS;
}
