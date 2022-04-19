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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <mem.h>
// #include "bitpack.h"

#include "prepare.h"

static const unsigned BYTE_SIZE = 8;

extern inline uint32_t Bitpack_newu(uint32_t word, unsigned width, unsigned lsb,
                      uint32_t value)
{
        // assert(width <= 64);
        unsigned hi = lsb + width; /* one beyond the most significant bit */
        // assert(hi <= 64);
        // if (!Bitpack_fitsu(value, width))
        //         RAISE(Bitpack_Overflow);
        return ((word >> hi) << hi) |((word << (32 - lsb)) >> (32 - lsb)) | value << lsb;
        //
        // return shl(shr(word, hi), hi)                 /* high part */
        //         | shr(shl(word, 32 - lsb), 32 - lsb)  /* low part  */
        //         | (value << lsb);                     /* new part  */
}

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
uint32_t read_one_instruction(FILE *input_file);

/* parse_file
 * Goes through input file and pushes instructions to add to zero segment
 * whose length was obtained through system stat interface
 */
extern uint32_t *parse_file(FILE *input_file, char *file_path)
{
    struct stat buf;
    stat(file_path, &buf);

    size_t inst_count = buf.st_size / 4;

    uint32_t *zero_seg = ALLOC(sizeof(uint32_t) * inst_count);

    for (size_t i = 0; i < inst_count; i++) {
        zero_seg[i] = read_one_instruction(input_file);
    }

    return zero_seg;
}

/* read_one_instruction
 * Reads one instruction (4 bytes) from the opened FILE pointer, bitpacks it
 * into a uint32, then returns it
 */
uint32_t read_one_instruction(FILE *input_file)
{
    uint32_t inst = 0;

    for (int i = 3; i >= 0; i--) {
        unsigned char byte = fgetc(input_file);
        inst = Bitpack_newu(inst, BYTE_SIZE, i * BYTE_SIZE, byte);
    }

    return inst;
}
