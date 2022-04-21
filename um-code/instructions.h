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
#include <seq.h>


/* Map
 *    Purpose: Creates a UArray and maps it either in an unmapped (recycled) 
 *             index or a new segment in sequence for non-zero segments
 *             and pushes the new index to destination register
 * Parameters: other_segs: sequence representing other segments
 *             available_indices: sequence representing unmapped segments of
 *               other_segs to map to first for recycling
 *             Number of words for new mapped segments
 *             Pointers too...
 *               - destination register
 *    Returns: none
 *    Effects: Adds a segment for access in other_segs
 *       CREs: none
 *      Notes: none
 */
extern void I_map(Seq_T other_segs, int  available_index, uint32_t *dest, uint32_t num_words);

/* Unmap
 *    Purpose: Accesses the other segs array and removes a segment indicated by
 *             given register and adds this segment to available_indices to
 *             recycle later
 * Parameters: other_segs: sequence representing other segments
 *             available_indices: sequence representing unmapped segments of
 *               other_segs to map to first for recycling
 *             Pointers too...
 *               - source register
 *    Returns: none
 *    Effects: Removes a segment to make it inaccessible
 *       CREs: none
 *      Notes: none
 */
extern void I_unmap(Seq_T other_segs, uint32_t *source);

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
extern void I_load_p(uint32_t **prog_seg_p, Seq_T   oth_segs, uint32_t reg_b);

#endif /* INSTRUCTIONS_INCLUDED */
