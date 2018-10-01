/*****************************************************************************/
/*                                                                           */
/* Author: Mukund Manikarnike												 */
/* ASU ID: 1208597425														 */
/* e-mail: mmanikar@asu.edu; mukunm@gmail.com								 */
/* Course: CSE 536                                                           */
/* Instructor: Dr. Sandeep Gupta                                             */
/* 																			 */
/* Algorithms, implementation of algorithms in the form of source code and   */
/* ideas that they represent are the intellectual property of the author     */
/* mentioned above. Any replication/re-use partially or completely shall be  */
/* carried out only with the permission of the author and the instructor     */
/* mentioned above and any such replications/re-uses shall carry this notice.*/
/* Redistributions without the author/instructor's permission shall not be   */
/* allowed.                                     							 */
/*                                                                           */
/* Any violation of the same shall be referred to the author or the          */
/* instructor as a possible violation of ASU Policies on Academic Integrity. */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*																			 */
/* Description:                                                              */
/*                                                                           */
/* This file contains the functionality for journal storage manager 		 */
/*																			 */
/*****************************************************************************/

/*****************************************************************************/
/* File Includes                                                             */
/*****************************************************************************/
#include "cell_storage_system.h"

/*****************************************************************************/
/* Structs                                                                   */
/*****************************************************************************/
/* Structure used for read operation input */
typedef struct {
	UWORD8 *pu1_file_name;
} read_input_t;

/* Structure used for write operation input */
typedef struct {
	UWORD8 *pu1_file_name;
	UWORD8 *pu1_file_data;
	UWORD16 u2_file_size;
} write_input_t;

/* Structure used for alloc operation input */
typedef struct {
	UWORD8 *pu1_file_name;
} alloc_input_t;

/* Structure used for de-alloc operation input */
typedef struct {
	UWORD8 *pu1_file_name;
} dealloc_input_t;

/* Structure used for abort operation input */
typedef struct {
	UWORD8 u1_op_num;
} abort_input_t;

/* Structure used for commit operation input */
typedef struct {
	UWORD8 u1_op_num;
} commit_input_t;


