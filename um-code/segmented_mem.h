#ifndef SEGMENTED_MEM_INCLUDED
#define SEGMENTED_MEM_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "segment.h"

typedef struct Segmented_Mem_T {
    Seg_T  *segs;
    size_t  len;
    size_t  capacity;
} *Segmented_Mem_T;

Segmented_Mem_T Segments_New(unsigned hint)
{
    Segmented_Mem_T mem = malloc(sizeof(*mem));
    mem->segs = malloc(sizeof(Seg_T) * hint);
    mem->len = 1;
    mem->capacity = hint;

    if (mem == NULL || mem->segs == NULL) {
        fprintf(stderr, "Error: No more memory\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned i = 0; i < hint; i++) {
        mem->segs[i] = NULL;
    }

    return mem;
}

void Segments_Free(Segmented_Mem_T *mem_p)
{
    Segmented_Mem_T my_segs = *mem_p;
    for (size_t i = 1; i < my_segs->len; i++) {
        if (my_segs->segs[i] != NULL){
            Seg_free(&(my_segs->segs[i]));
        }
    }

    free(my_segs->segs);
    free(my_segs);
    *mem_p = NULL;
}
Seg_T Segments_Access(Segmented_Mem_T mem, uint32_t seg_id)
{
    return mem->segs[seg_id];
}

uint32_t Segments_Get(Segmented_Mem_T mem, uint32_t seg_id, uint32_t idx)
{
    return mem->segs[seg_id][idx + 1];
}
void Segments_Put(Segmented_Mem_T mem, uint32_t seg_id, uint32_t idx, uint32_t value)
{
    mem->segs[seg_id][idx + 1] = value;
}

uint32_t Segments_Size(Segmented_Mem_T mem, uint32_t seg_id)
{
    return Seg_len(mem->segs[seg_id]);
}

uint32_t Segments_Map(Segmented_Mem_T mem, uint32_t size, size_t index)
{
    if (index == 0) {
        if (mem->capacity == mem->len) {
            mem->capacity *= 2;

            Seg_T *new_segs = malloc(sizeof(*new_segs) * mem->capacity);

            for (size_t i = 1; i < mem->len; i++) {
                new_segs[i] = mem->segs[i];
            }

            free(mem->segs);
            mem->segs = new_segs;
            // mem->segs = realloc(mem->segs, sizeof(Seg_T) * mem->capacity);
        }

        mem->segs[mem->len] = Seg_new(size);
        (mem->len)++;

        return mem->len - 1;

    } else {

        Seg_T temp = Seg_new(size);
        mem->segs[index] = temp;
        return index;
    }
}

void Segments_Unmap(Segmented_Mem_T mem, uint32_t seg_id)
{
    free(mem->segs[seg_id]);
    mem->segs[seg_id] = NULL;
}


#endif /* SEGMENTED_MEM_INCLUDED */
