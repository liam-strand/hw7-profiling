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

#include "um_state.h"
#include "prepare.h"
#include "segment.h"
#include "segmented_mem.h"


void execute_instructions(uint32_t       **prog_seg,
                          uint32_t       *regs,
                          Segmented_Mem_T other_segs);

void clean_up(uint32_t **prog_seg_p, Segmented_Mem_T *other_segs_p);

static inline int expand(uint32_t **recycled, int capacity)
{
    *recycled = realloc(*recycled, (sizeof(uint32_t) * capacity * 2));
    return capacity * 2;
}

static inline uint32_t Bitpack_getu(uint32_t word, unsigned width, unsigned lsb)
{
    return (word << (32 - (lsb + width))) >> (32 - width);
}


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

    uint32_t r[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    Segmented_Mem_T other_segs = Segments_New(5);

    execute_instructions(&prog_seg, r, other_segs);

    clean_up(&prog_seg, &other_segs);
}

/* execute_instructions
 * Until reaching a halt instruction...
 * 1. Read the instruction in program segment at the program counter
 * 2. Un-bitpack the instruction
 * 4. Increment the program counter
 * 3. Execute the command associated with the instruction
 */
void execute_instructions(uint32_t       **prog_seg,
                          uint32_t       *regs,
                          Segmented_Mem_T other_segs)
{
    bool shouldContinue = true;
    
    int capacity = 1000;
    int num_recycled = 0;
    
    size_t program_counter = 0;

    uint32_t *recycled = malloc(sizeof(uint32_t) * 1000);
    if (recycled == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    while (shouldContinue) {
        // fprintf(stderr, "%lu\n", count++);
        
        uint32_t inst = (*prog_seg)[program_counter];
        
        
        
        
        uint32_t op, ra, rb, rc, value;
        //unwrap_instruction(inst, &op, &ra, &rb, &rc);
        op = Bitpack_getu(inst, 4, 28);

        program_counter++;

        switch(op) {

            /* Conditional Move */
            case 0:
                ra = Bitpack_getu(inst, 3,  6);
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);
                switch (regs[rc]) {
                    case 0:
                        break;
                    default:
                        regs[ra] = regs[rb];
                }
                break;

            /* Segmented Load */
            case 1:
                ra = Bitpack_getu(inst, 3,  6);
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);

                switch (regs[rb]) {
                    case 0:
                        regs[ra] = (*prog_seg)[regs[rc]];
                        break;
                    default:
                        regs[ra] = Segments_Get(other_segs, regs[rb], regs[rc]);
                }
                break;

            /* Segmented Store */
            case 2:
                ra = Bitpack_getu(inst, 3,  6);
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);

                switch (regs[ra]) {
                    case 0:
                        (*prog_seg)[regs[rb]] = regs[rc];
                        break;
                    default:
                        Segments_Put(other_segs, regs[ra], regs[rb], regs[rc]);
                }
                break;

            /* Add */
            case 3:
                ra = Bitpack_getu(inst, 3,  6);
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);
                regs[ra] = regs[rb] + regs[rc];
                break;

            /* Multiply */
            case 4:
                ra = Bitpack_getu(inst, 3,  6);
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);
                regs[ra] = regs[rb] * regs[rc];
                break;

            /* Integer Divide */
            case 5:
                ra = Bitpack_getu(inst, 3,  6);
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);
                regs[ra] = regs[rb] / regs[rc];
                break;

            /* NAND */
            case 6:
                ra = Bitpack_getu(inst, 3,  6);
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);
                regs[ra] = ~(regs[rb] & regs[rc]);
                break;

            /* Break */
            case 7:
                shouldContinue = false;
                break;

            /* Map Segment */
            case 8:
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);
                switch (num_recycled) {
                    case 0:
                        regs[rb] = Segments_Map(other_segs, regs[rc], 0);
                        break;
                    default:
                        num_recycled -= 1;
                        regs[rb] = Segments_Map(other_segs, regs[rc], recycled[num_recycled]);
                }
                break;

            /* Unmap Segment */
            case 9:
                rc = Bitpack_getu(inst, 3,  0);
                
                if (num_recycled == capacity) {
                    capacity = expand(&recycled, capacity);
                }
                
                recycled[num_recycled++] = regs[rc];
                Segments_Unmap(other_segs, regs[rc]);
                break;

            /* Output */
            case 10:
                rc = Bitpack_getu(inst, 3,  0);
                if (regs[rc] >= 256) {
                    fprintf(stderr, "Error, output out of range\n");
                    exit(EXIT_FAILURE);
                }
                fputc(regs[rc], stdout);
                break;

            /* Input */
            case 11:
                rc = Bitpack_getu(inst, 3,  0);
                regs[rc] = (uint32_t)fgetc(stdin);
                break;

            /* Load Program */
            case 12:
                rb = Bitpack_getu(inst, 3,  3);
                rc = Bitpack_getu(inst, 3,  0);

                switch (regs[rb]) {
                    case 0:
                        break;
                    default:
                        free(*prog_seg);
                        Seg_T to_copy = Segments_Access(other_segs, regs[rb]);
                        int len = Seg_len(to_copy);
                        uint32_t *new_prog = malloc(sizeof(*new_prog) * len);

                        if (new_prog == NULL) {
                            fprintf(stderr, "Error: Ran out of memory\n");
                            exit(EXIT_FAILURE);
                        }

                        for (int i = 0; i < len; i++) {
                            new_prog[i] = Seg_get(to_copy, i);
                        }
                        *prog_seg = new_prog;
                }

                program_counter = regs[rc];
                break;

            /* Load Value */
            case 13:
                ra    = Bitpack_getu(inst, 3, 25);
                value = Bitpack_getu(inst, 25, 0);

                regs[ra] = value;
                break;
            default:
                shouldContinue = false;
        }
    }
    free(recycled);
}

/* clean_up
 * 1. Free the program segment
 * 2. Free other allocated segments
 * 3. Free the integers representing reusable segment identifiers
 * 4. Free the Hanson sequences
 */
void clean_up(uint32_t **prog_seg_p, Segmented_Mem_T *other_segs_p)
{
    bool good_ptrs = (prog_seg_p   != NULL && *prog_seg_p   != NULL &&
                      other_segs_p != NULL && *other_segs_p != NULL);

    if (!good_ptrs) {
        fprintf(stderr, "Error: clean_up passed nullptr\n");
        exit(EXIT_FAILURE);
    }

    free(*prog_seg_p);

    Segments_Free(other_segs_p);

    *prog_seg_p   = NULL;
    *other_segs_p = NULL;
}
