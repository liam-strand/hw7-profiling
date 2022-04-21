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

#include "instructions.h"
#include "segmented_mem.h"
#include "segment.h"

/* Load program
 * Duplicates a desired segment and replaces the program segment, and 
 * shifts the program segment according to instruction
 */
extern void I_load_p(uint32_t **prog_seg_p, Segmented_Mem_T oth_segs, uint32_t reg_b)
{
    free(*prog_seg_p);
    Seg_T to_copy = Segments_Access(oth_segs, reg_b);
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
