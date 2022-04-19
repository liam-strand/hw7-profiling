/*
 * main.c
 *
 * COMP 40 HW6: um
 *
 * By: Liam Strand (lstran01)
 *     Matt Ung    (mung01)
 *
 * On: April 2022
 *
 * The driver for the universal machine.
 * 
 * Usage:     ./um <um_file>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <assert.h>

#include "um_state.h"

/* prepare_args
 *    Purpose: Prepares and verifies the command line arguments
 * Parameters: The number of arguments and the arguments themselves
 *    Returns: An opened FILE pointer to the umfile, or NULL
 *    Effects: Prints to stderr if contract violation or file cannot be opened
 *       CREs: none
 *      Notes: none
 */
FILE *prepare_args(int argc, char *argv[]);

/* main
 *    Purpose: The Program Driver
 * Parameters: The number of CLAs and the CLAs themselves
 *    Returns: EXIT_SUCCESS if the file can be opened and the machine runs
 *             EXIT_FAILURE if the file cannot be opened
 *    Effects: none
 *       CREs: Wrong number of CLAs, file cannot be opened.
 *      Notes: none
 */
int main(int argc, char *argv[])
{
    FILE *input_file = prepare_args(argc, argv);
    
    if (input_file == NULL) {
        return EXIT_FAILURE;
    } 

    um_run(input_file, argv[1]);

    fclose(input_file);

    return EXIT_SUCCESS;
}

/* prepare_args
 * 1. Ensure that there are the correct number of arguments
 * 2. Open the file
 * 3. Check that the file opened
 */
FILE *prepare_args(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: um <umfile>\n");
        return NULL;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        fprintf(stderr, "%s could not be opened\n", argv[1]);
        return NULL;
    }

    return file;
}
