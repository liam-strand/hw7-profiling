#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "segmented_mem.h"

struct Segmented_Mem_T {
    uint32_t **segs;
    size_t     len;
    size_t     capacity;
};

extern Segmented_Mem_T Segments_New(unsigned hint)
{
    Segmented_Mem_T mem = malloc(sizeof(*mem));
    mem->segs = malloc(sizeof(uint32_t*) * hint);
    mem->len = 1;
    mem->capacity = hint;

    if (mem == NULL || mem->segs == NULL) {
        fprintf(stderr, "Error: No more memory\n");
        exit(EXIT_FAILURE);
    }

    mem->segs[0] = NULL;

    return mem;
}

extern void Segments_Free(Segmented_Mem_T *mem_p)
{
    Segmented_Mem_T my_segs = *mem_p;
    *mem_p = NULL;
    size_t max = my_segs->capacity;

    for (size_t i = 0; i < max; i++) {
        free(my_segs->segs[i]);
    }

    free(my_segs);
}

extern uint32_t *Segments_Access(Segmented_Mem_T mem, uint32_t seg_id)
{
    return mem->segs[seg_id];
}

extern uint32_t Segments_Get(Segmented_Mem_T mem, uint32_t seg_id, uint32_t idx)
{
    return mem->segs[seg_id][idx + 1];
}
extern void Segments_Put(Segmented_Mem_T mem, uint32_t seg_id, uint32_t idx, uint32_t value)
{
    mem->segs[seg_id][idx + 1] = value;
}

extern uint32_t Segments_Size(Segmented_Mem_T mem, uint32_t seg_id)
{
    return mem->segs[seg_id][0];
}

extern uint32_t Segments_Map(Segmented_Mem_T mem, uint32_t size, size_t index)
{
    if (index == 0) {
        if (mem->capacity == mem->len) {
            mem->capacity *= 2;
            mem->segs = realloc(mem->segs, mem->capacity);
        }

        uint32_t *temp_ptr = malloc(sizeof(uint32_t) * (size + 1));
        
        if (temp_ptr == NULL) {
            fprintf(stderr, "Error: No more memory\n");
            exit(EXIT_FAILURE);
        }

        temp_ptr[0] = size;
        mem->segs[mem->len] = temp_ptr;
        (mem->len)++;

        return mem->len - 1;

    } else {
        uint32_t *temp_ptr = malloc(sizeof(uint32_t) * (size + 1));
        
        if (temp_ptr == NULL) {
            fprintf(stderr, "Error: No more memory\n");
            exit(EXIT_FAILURE);
        }

        temp_ptr[0] = size;
        mem->segs[index] = temp_ptr;

        return index;
    }
}

extern void Segments_Unmap(Segmented_Mem_T mem, uint32_t seg_id)
{
    free(mem->segs[seg_id]);
}
