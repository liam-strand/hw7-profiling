#include "segment.h"
#include "segmented_mem.h"
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

int main() {
    Segmented_Mem_T seg_mem = Segments_New(5);

    for (unsigned i = 1; i < 50; i++) {
        Segments_Map(seg_mem, 5000000, 0);
        for (unsigned j = 0; j < 5000000; j++) {
            Segments_Put(seg_mem, i, j, j + 100);
        }
    }

    for (unsigned i = 1; i < 50; i++) {
        for (unsigned j = 0; j < 5000000; j++) {
            assert(j + 100 == Segments_Get(seg_mem, i, j));
        }
    }

    Segments_Free(&seg_mem);

    return EXIT_SUCCESS;
}

/* int main() {
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
} */
