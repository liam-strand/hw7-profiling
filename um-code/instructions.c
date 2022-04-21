/*
 * instructions.c
 *
 * COMP 40 HW6: um
 *
 * By: Liam Strand (lstran01)
 *     Matt Ung    (mung01)
 *
 * On: April 2022
 *
 * The implementation for the 13 (excluding halt) UM instructions. Changes 
 * values in registers and in segmented memory using pointers, in accordance
 * with the instructions' instructions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <seq.h>
#include <mem.h>
#include <uarray.h>

#include "instructions.h"
#include "segment.h"

/* Map
 * Creates a new segment and utilizes recycling older (unmapped) segments
 * to maintain neccessary memory use and pushes index to desired register
 */
extern void I_map(Seq_T other_segs, int available_index, uint32_t *dest, uint32_t num_words)
{
    Seg_T mapped_arr = Seg_new(num_words);
    if (available_index != -1) {
        // int *recycled_index = (int *) Seq_remlo(available_indices);
        Seq_put(other_segs, available_index, mapped_arr);
        *dest = available_index;
        // FREE(recycled_index);
    }
    else {
        int new_index = Seq_length(other_segs);
        Seq_addhi(other_segs, mapped_arr);
        *dest = new_index;
    }
}

/* Unmap
 * Removes desired segment and pushes index to available_indices for reuse
 * to maintain neccessary memory use
 */
 extern void I_unmap(Seq_T     other_segs, 
                     uint32_t *source)
 {
     // int *free_index = ALLOC(sizeof(*free_index));
     // *free_index = *(int *) source;
     Seg_T to_free = (Seg_T)Seq_put(other_segs, *source, NULL);
     Seg_free(&(to_free));
     //Seq_addhi(available_indices, free_index);

 }


/* Load program
 * Duplicates a desired segment and replaces the program segment, and 
 * shifts the program segment according to instruction
 */
extern void I_load_p(uint32_t **prog_seg_p, Seq_T oth_segs, uint32_t reg_b)
{
    
    free(*prog_seg_p);
    Seg_T to_copy = (Seg_T) Seq_get(oth_segs, reg_b);
    int len = Seg_len(to_copy);
    uint32_t *new_prog = malloc(sizeof(*new_prog) * len);

    if (new_prog == NULL) {
        fprintf(stderr, "Error: Ran out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < len; i++) {
        new_prog[i] = Seg_get(to_copy, i);
    }
    *prog_seg_p = new_prog;
}
