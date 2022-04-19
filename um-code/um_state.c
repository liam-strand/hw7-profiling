/*
 * um_state.c
 *
 * COMP 40 HW6: um
 *
 * By: Liam Strand (lstran01)
 *     Matt Ung    (mung01)
 *
 * On: April 2022
 *
 * The implementation of the universal machine. Manages segmented memory and
 * instruction execution. Memory is cleaned up afterwards.
 * 
 */


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <mem.h>
#include <seq.h>
#include <assert.h>
#include <bitpack.h>
#include <uarray.h>

#include "um_state.h"
#include "prepare.h"
#include "instructions.h"


/* execute_instructions
 *    Purpose: Contains the main program loop. Reads through the program
 *             segment and executes instructions in order.
 * Parameters: Pointers to...
 *               - The program counter
 *               - The program segment
 *               - The registers
 *             Hanson Sequences containing...
 *               - The non-program segments
 *               - The recyclable indices
 *    Returns: none
 *    Effects: Modifies all arguments to reflect the state of the program as
 *             it is executed
 *       CREs: none
 *      Notes: none
 */
void execute_instructions(size_t   *program_counter,
                          uint32_t **prog_seg,
                          uint32_t *regs,
                          Seq_T     other_segs,
                          Seq_T     available_indices);

/* clean_up
 *    Purpose: Frees memory associated with heap-allocated data structures
 * Parameters: Pointers to...
 *               - The array holding the program (zero) segment
 *               - The Seq holding the other segments
 *               - The Seq holding recycled segment IDs
 *    Returns: none
 *    Effects: Recycles all memory associated with the data structures above.
 *             Additionally sets their values to NULL to prevent unwanted
 *             access of uninitialized memory.
 *       CREs: Any of the parameters, or their dereferences are NULL
 *      Notes: none
 */
void clean_up(uint32_t **prog_seg_p, Seq_T *other_segs_p, Seq_T *recycled_p);

/* unwrap_instruction
 *    Purpose: Extracts the opcode and register indices from the instruction
 * Parameters: The instruction to be unwrapped
 *             Addresses of places to store the following...
 *               - The opcode
 *               - The indices of three registers
 *    Returns: none
 *    Effects: Stores an opcode and three register indices in the locations
 *             referenced by the last four parameters.
 *       CREs: none
 *      Notes: Though several instructions do not require the use of all three
 *             registers, this function handles most of the cases.
 */
void unwrap_instruction(uint32_t  inst, uint32_t *op_p, uint32_t *ra_p, 
                        uint32_t *rb_p, uint32_t *rc_p);

/* prepare_lv
 *    Purpose: Unpacks a load_value instruction
 * Parameters: An instructio to unpack, pointers to...
 *               - a place to store the destination register id 
 *               - a place to store the value to load
 *    Returns: none
 *    Effects: Populates reg_id and value
 *       CREs: none
 *      Notes: none
 */
void prepare_lv(uint32_t inst, uint32_t *reg_id, uint32_t *value);

/* seg_source
 *    Purpose: Accesses a location in segmented memory given a segment ID and
 *             an index in that segment.
 * Parameters: A pointer to the program segment
 *             A Hanson Sequence of all other segments
 *             The segment number and desired index
 *    Returns: A pointer to the desired location in segmented memory
 *    Effects: none
 *       CREs: none
 *      Notes: none
 */
uint32_t *seg_source(uint32_t *prog_seg, Seq_T    other_segs,
                     uint32_t  seg_num,  uint32_t seg_index);

/* deep_free_uarray
 *    Purpose: Frees the uarrays referenced by a Hanson Sequence
 * Parameters: A Hanson Sequence
 *    Returns: none
 *    Effects: Frees everything pointed to by the parameter
 *       CREs: The parameter is NULL
 *      Notes: none
 */
void deep_free_uarray(Seq_T seq);

/* deep_free_int
 *    Purpose: Frees the ints referenced by a Hanson Sequence
 * Parameters: A Hanson Sequence
 *    Returns: none
 *    Effects: Frees everything pointed to by the parameter
 *       CREs: The parameter is NULL
 *      Notes: none
 */
void deep_free_int(Seq_T seq);

/* um_run
 * 1. Parse the input file and initialize the program segment
 * 2. Initialize the program counter and registers to hold 0
 * 3. Initialize the Hanson sequence holding the other segments to hold
 *    a single NULL pointer.
 * 4. Initialize The Hanson sequence holding the recycled 
 */
extern void um_run(FILE *input_file, char *file_path)
{
    uint32_t *prog_seg = parse_file(input_file, file_path);

    size_t prog_counter = 0;

    uint32_t r[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    Seq_T other_segs = Seq_new(5);
    Seq_addlo(other_segs, NULL);

    Seq_T recycled_ids = Seq_new(5);

    execute_instructions(&prog_counter, &prog_seg, r, other_segs, 
                                                      recycled_ids);

    clean_up(&prog_seg, &other_segs, &recycled_ids);
}

/* execute_instructions
 * Until reaching a halt instruction...
 * 1. Read the instruction in program segment at the program counter
 * 2. Un-bitpack the instruction
 * 4. Increment the program counter
 * 3. Execute the command associated with the instruction
 */
void execute_instructions(size_t   *program_counter,
                          uint32_t **prog_seg,
                          uint32_t *regs,
                          Seq_T     other_segs,
                          Seq_T     available_indices)
{
    bool shouldContinue = true;

    while (shouldContinue) {

        uint32_t inst = (*prog_seg)[*program_counter];

        uint32_t op, ra, rb, rc, value;
        unwrap_instruction(inst, &op, &ra, &rb, &rc);

        (*program_counter)++;

        switch(op) {
            
            /* Conditional Move */
            case 0:
                I_c_mov(&regs[rb], &regs[ra], &regs[rc]);
                break;
            
            /* Segmented Load */
            case 1:
                I_seg_load(seg_source(*prog_seg, other_segs, 
                                      regs[rb], regs[rc]), &regs[ra]); 
                break;
            
            /* Segmented Store */
            case 2:
                I_seg_store(&regs[rc], 
                            seg_source(*prog_seg, other_segs, regs[ra], 
                                                              regs[rb])); 
                break;
            
            /* Add */
            case 3:
                I_add(&regs[rb], &regs[rc], &regs[ra]);
                break;
            
            /* Multiply */
            case 4:
                I_mult(&regs[rb], &regs[rc], &regs[ra]);
                break;

            /* Integer Divide */
            case 5:
                I_div(&regs[rb], &regs[rc], &regs[ra]);
                break;

            /* NAND */
            case 6:
                I_nand(&regs[rb], &regs[rc], &regs[ra]);
                break; 

            /* Break */
            case 7:
                shouldContinue = false;
                break; 

            /* Map Segment */
            case 8:
                I_map(other_segs, available_indices, &regs[rb], regs[rc]);
                break;

            /* Unmap Segment */
            case 9:
                I_unmap(other_segs, available_indices, &regs[rc]);
                break;

            /* Output */
            case 10:
                I_out(&regs[rc]);
                break;

            /* Input */
            case 11:
                I_in(&regs[rc]);
                break;

            /* Load Program */
            case 12:
                I_load_p(prog_seg, other_segs, &regs[rb], 
                        &regs[rc], program_counter);
                break;

            /* Load Value */
            case 13:
                prepare_lv(inst, &ra, &value);
                I_load_v(value, &regs[ra]);
                break;
            default:
                shouldContinue = false;
        }
    }
}   

/* unwrap_instruction
 * Un-bitpack the opcode and the three registers and store in referenced
 * locations.
 */
void unwrap_instruction(uint32_t inst, uint32_t *op_p, uint32_t *ra_p, 
                             uint32_t *rb_p, uint32_t *rc_p)
{
    *op_p = Bitpack_getu(inst, 4, 28);
    *ra_p = Bitpack_getu(inst, 3,  6);
    *rb_p = Bitpack_getu(inst, 3,  3);
    *rc_p = Bitpack_getu(inst, 3,  0);
}

/* prepare_lv
 * The special un-bitpack for the load value instruction. Un-bitpacks the 
 * destination register and the loading value, stores in referenced variables.
 */
void prepare_lv(uint32_t inst, uint32_t *reg_id, uint32_t *value)
{
    *reg_id = Bitpack_getu(inst, 3, 25);
    *value  = Bitpack_getu(inst, 25, 0);
}

/* seg_source
 *  1. Determines if the desired memory is in the program segment or in another
 *     segment.
 * 2a. If the desired memory is in the program segment, return the address of
 *     the desired word.
 * 2b. If the desired memory is in another segment, get the segment, access the
 *     desired word in that segment, and return the address of that word.
 */
uint32_t *seg_source(uint32_t *prog_seg, Seq_T    other_segs,
                     uint32_t  seg_num,  uint32_t seg_index)
{
    if (seg_num == 0) {
        return &prog_seg[seg_index];
    } else {
        UArray_T segment = (UArray_T)Seq_get(other_segs, seg_num);
        return (uint32_t *)UArray_at(segment, seg_index);
    }
}

/* clean_up
 * 1. Free the program segment
 * 2. Free other allocated segments
 * 3. Free the integers representing reusable segment identifiers
 * 4. Free the Hanson sequences
 */
void clean_up(uint32_t **prog_seg_p, Seq_T *other_segs_p, Seq_T *recycled_p)
{
    assert(prog_seg_p   != NULL && *prog_seg_p   != NULL);
    assert(other_segs_p != NULL && *other_segs_p != NULL);
    assert(recycled_p   != NULL && *recycled_p   != NULL);
    
    FREE(*prog_seg_p);

    deep_free_uarray(*other_segs_p);
    deep_free_int(*recycled_p);

    Seq_free(other_segs_p);
    Seq_free(recycled_p);

    *prog_seg_p   = NULL;
    *other_segs_p = NULL;
    *recycled_p   = NULL;
}

/* deep_free_uarray
 * Iterate through a Hanson sequence and free the UArrays referenced by the
 * sequence's elements. If an element is NULL, don't attempt to free!
 */
void deep_free_uarray(Seq_T seq)
{
    assert(seq != NULL);

    unsigned len = Seq_length(seq);

    for (unsigned i = 1; i < len; i++) {
        UArray_T ua = (UArray_T)Seq_get(seq, i);
        if (ua != NULL) {
            UArray_free(&ua);
        }
    }
}

/* deep_free_int
 * Iterate through a Hanson sequence and free the ints referenced by the
 * sequence's elements. If an element is NULL, don't attempt to free!
 */
void deep_free_int(Seq_T seq)
{
    assert(seq != NULL);
    
    unsigned len = Seq_length(seq);
    for (unsigned i = 0; i < len; i++) {
        int *n = (int *)Seq_get(seq, i);
        if (n != NULL) {
            FREE(n);
        }
    }
}
