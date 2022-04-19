#ifndef UM_STATE_INCLUDED
#define UM_STATE_INCLUDED

/*
 * um_state.h
 *
 * COMP 40 HW6: um
 *
 * By: Liam Strand (lstran01)
 *     Matt Ung    (mung01)
 *
 * On: April 2022
 *
 * This module contains the logic for the universal machine's state and 
 * execution. 
 * 
 * The only exported function, um_run(), initilizes, runs, and cleans up after
 * the machine.
 * 
 */

#include <stdio.h>

/* um_run
 *    Purpose: Initilizes, runs, and cleans up the emulator
 * Parameters: An opened FILE pointer and the path to that file
 *    Returns: none
 *    Effects: none
 *       CREs: none
 *      Notes: file_path is only used to make the stat() system call to 
 *             get the size of the file without reading through it.
 */
extern void um_run(FILE *input_file, char *file_path);

#endif /* UM_STATE_INCLUDED */
