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

/* Condtional Move
 *    Purpose: Does a move from source register to destination given the check
 *             register does not equal 0
 * Parameters: Pointers too...
 *               - source register ($r[B])
 *               - destination register ($r[A])
 *               - register to check ($r[C] against 0)
 *    Returns: none
 *    Effects: Shifts values between registers given a condition
 *       CREs: none
 *      Notes: none
 */
extern void I_c_mov(uint32_t *source, uint32_t *dest, uint32_t *check);

/* Segment Load
 *    Purpose: Does a move from source segment section to destination register
 * Parameters: Pointers too...
 *               - source (pointer to segment section $m[$r[B]$r[C]])
 *               - destination register ($r[A])
 *    Returns: none
 *    Effects: Shifts value from segment section to register
 *       CREs: none
 *      Notes: none
 */
extern void I_seg_load(uint32_t *source, uint32_t *dest);

/* Segment Store
 *    Purpose: Does a move from source register to destination segment section
 * Parameters: Pointers too...
 *               - source ($r[C])
 *               - destination (pointer to segment section $m[$r[A]$r[B]])
 *    Returns: none
 *    Effects: Shifts value from register to segment section
 *       CREs: none
 *      Notes: none
 */
extern void I_seg_store(uint32_t *source, uint32_t *dest);

/* Addition
 *    Purpose: Adds the values between two registers and puts in destination
 *             register
 * Parameters: Pointers too...
 *               - register B
 *               - register C 
 *               - destination register
 *    Returns: none
 *    Effects: Adds register b to c and pastes to destination register
 *       CREs: none
 *      Notes: none
 */
extern void I_add(uint32_t *reg_b, uint32_t *reg_c, uint32_t *dest);

/* Multiplication
 *    Purpose: Multiplies the values between two registers and puts in 
 *             destination register
 * Parameters: Pointers too...
 *               - register B
 *               - register C 
 *               - destination register
 *    Returns: none
 *    Effects: Multiplies register b to c and pastes to destination register
 *       CREs: none
 *      Notes: none
 */
extern void I_mult(uint32_t *reg_b, uint32_t *reg_c, uint32_t *dest);

/* Division
 *    Purpose: Divides the values between two registers and puts in destination
 *             register
 * Parameters: Pointers too...
 *               - register B
 *               - register C 
 *               - destination register
 *    Returns: none
 *    Effects: Divides register b to c and pastes to destination register
 *       CREs: none
 *      Notes: none
 */
extern void I_div(uint32_t *reg_b, uint32_t *reg_c, uint32_t *dest);

/* Bitwise NAND
 *    Purpose: Does the bitwise nand operation between register C and register
 *             B and puts it into register A
 * Parameters: Pointers too...
 *               - register B
 *               - register C 
 *               - destination register
 *    Returns: none
 *    Effects: Nands register b to c and pastes to destination register
 *       CREs: none
 *      Notes: none
 */
extern void I_nand(uint32_t *reg_b, uint32_t *reg_c, uint32_t *dest);

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
extern void I_map(Seq_T     other_segs, Seq_T    available_indices, 
                  uint32_t *dest,       uint32_t num_words);

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
extern void I_unmap(Seq_T other_segs, Seq_T     available_indices, 
                                      uint32_t *source);

/* Output
 *    Purpose: Prints to standard out the value in given register
 * Parameters: Pointers too...
 *               - source register
 *    Returns: none
 *    Effects: Prints register to stdout given valid char
 *       CREs: If the register holds a value that is out of range (0, 255)
 *      Notes: none
 */
extern void I_out(uint32_t *reg);

/* Input
 *    Purpose: Takes in from standard in and pushes value into given register
 * Parameters: Pointers too...
 *               - destination register
 *    Returns: none
 *    Effects: Changes a register to hold value from stdin
 *       CREs: none
 *      Notes: none
 */
extern void I_in(uint32_t *reg);

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
extern void I_load_p(uint32_t **prog_seg_p, Seq_T   oth_segs, uint32_t *reg_b, 
                     uint32_t  *reg_c,      size_t *p_counter);

/* Load Value
 *    Purpose: Takes in a value and pushes this to a register
 * Parameters: value: 32 bit int to push to destination register
 *             Pointers too...
 *               - destination register
 *    Returns: none
 *    Effects: Pushes a given value to register
 *       CREs: none
 *      Notes: none
 */
extern void I_load_v(uint32_t value, uint32_t *dest_reg);

#endif /* INSTRUCTIONS_INCLUDED */
