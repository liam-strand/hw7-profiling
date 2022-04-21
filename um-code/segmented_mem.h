#ifndef SEGMENTED_MEM_INCLUDED
#define SEGMENTED_MEM_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "segment.h"

typedef struct Segmented_Mem_T *Segmented_Mem_T;

extern Segmented_Mem_T Segments_New(unsigned hint);
extern void            Segments_Free(Segmented_Mem_T *segs_p);

extern Seg_T    Segments_Access(Segmented_Mem_T mem, uint32_t seg_id);
extern uint32_t Segments_Get(Segmented_Mem_T mem, uint32_t seg_id, uint32_t idx);
extern void     Segments_Put(Segmented_Mem_T mem, uint32_t seg_id, uint32_t idx, uint32_t value);
extern uint32_t Segments_Size(Segmented_Mem_T mem, uint32_t seg_id);


extern uint32_t Segments_Map(Segmented_Mem_T mem, uint32_t size, size_t index);
extern void     Segments_Unmap(Segmented_Mem_T mem, uint32_t seg_id);

#endif /* SEGMENTED_MEM_INCLUDED */
