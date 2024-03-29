/*
 * prepare.c
 *
 * COMP 40 HW6: um
 *
 * By: Liam Strand (lstran01)
 *     Matt Ung    (mung01)
 *
 * On: April 2022
 *
 * Implements a function to parse a .um file and initilize the program segment
 * of the universal machine.
 */


#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "prepare.h"

typedef union Word {
    uint32_t integer;
    unsigned char str[4]; 
} Word;

/* read_one_instruction
 *    Purpose: Reads chars from input file and pushes 32-bit encoded as output
 * Parameters: Pointers too...
 *               - File for reading in
 *    Returns: 32-bit int instruction
 *    Effects: Parse together 4 characters using bitpacking to create one
 *             instruction
 *       CREs: If heap allocation of zero segment fails, throws CRE
 *      Notes: Utilizes bitpack interface to create code words from characters
 *
 */
static inline uint32_t read_one_instruction(FILE *input_file)
{
    Word inst;

    for (int i = 3; i >= 0; i--) {
        unsigned char byte = fgetc(input_file);
        inst.str[i] = byte;
    }

    return inst.integer;
}

/* parse_file
 * Goes through input file and pushes instructions to add to zero segment
 * whose length was obtained through system stat interface
 */
extern uint32_t *parse_file(FILE *input_file, char *file_path)
{
    struct stat buf;
    stat(file_path, &buf);

    size_t inst_count = buf.st_size / 4;

    uint32_t *zero_seg = malloc(sizeof(*zero_seg) * inst_count);
    if (zero_seg == NULL) {
        fprintf(stderr, "Error: ran out of memory\n");
        exit(EXIT_FAILURE);
    }


    for (size_t i = 0; i < inst_count; i++) {
        zero_seg[i] = read_one_instruction(input_file);
    }

    return zero_seg;
}

/* read_one_instruction
 * Reads one instruction (4 bytes) from the opened FILE pointer, bitpacks it
 * into a uint32, then returns it
 */

