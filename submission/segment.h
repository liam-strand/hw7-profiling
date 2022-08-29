#ifndef SEGMENT_INCLUDED
#define SEGMENT_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef uint32_t *Seg_T;


static inline Seg_T Seg_new(uint32_t size)
{
    Seg_T seg = malloc(sizeof(*seg) * (size + 1));
    if (seg == NULL) {
        fprintf(stderr, "Error: Ran out of memory\n");
        exit(EXIT_FAILURE);
    }

    seg[0] = size;

    for (uint32_t i = 0; i < size; i++) {
        seg[i + 1] = 0;
    }

    return seg;
}

static inline void Seg_free(Seg_T *seg_p)
{
    free(*seg_p);
    *seg_p = NULL;
}

static inline uint32_t Seg_get(Seg_T seg, uint32_t idx)
{
    return seg[idx + 1];
}

static inline void Seg_set(Seg_T seg, uint32_t idx, uint32_t value)
{
    seg[idx + 1] = value;
}

static inline uint32_t Seg_len(Seg_T seg)
{
    return seg[0];
}


#endif /* SEGMENT_INCLUDED */
