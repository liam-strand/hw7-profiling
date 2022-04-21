#ifndef INSTRUCTIONS_INCLUDED
#define INSTRUCTIONS_INCLUDED

/*
 * instructions.h
 *
 * COMP 40 HW6: um
 *
 * By: Liam Strand (lstran01)
 *     Matt Ung    (mung01)
 *
 * On: April 2022
 *
 * The functions representing the 14 possible UM instructions. Each function
 * takes references (yes we know not optimized for speed hehe) to the relevant
 * locations in memory and any necessary additional information, and performs
 * the required computation.
 * 
 */

#include <stdlib.h>
#include <stdint.h>
#include "segmented_mem.h"

/* Load Program
 *    Purpose: Duplicates $m[$r[B]] (segment indicated by reg B) and 
 *             replaces $m[0] with duplicated segment. Shifts program 
 *             to point to the segment zero at instruction number reg C
 * Parameters: other_segs: sequence representing other segments
 *             Pointers too...
 *               - register B
 *               - register C
 *               - C-array zero program segment
 *               - program counter
 *    Returns: none
 *    Effects: Performs a jump and potentially changes the program segment 
 *             depending on value in register B since that segment replaces
 *             zero segment. Potentially does not duplicate if $r[B] is zero
 *       CREs: none
 *      Notes: none
 */
extern void I_load_p(uint32_t **prog_seg_p, Segmented_Mem_T oth_segs, uint32_t reg_b);

#endif /* INSTRUCTIONS_INCLUDED */
