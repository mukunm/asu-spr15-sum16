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
/* This file contains all the user defined type definitions.                 */
/*																			 */
/*****************************************************************************/

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/
#define BYTE_LENGTH 8

/* Macro to combine bytes into words */
#define MAKE_WORD16(lsb, msb) ((((UWORD16)(msb) << 8) & 0xFF00) | (lsb))
#define MAKE_WORD32(lsw, msw) ((((UWORD32)(msw) << 16) & 0xFFFF0000) | (lsw))

#define INLINE static __inline

/*****************************************************************************/
/* Enums                                                                     */
/*****************************************************************************/
typedef enum {
	FALSE = 0,
	TRUE  = 1
} BOOL_T;

/*****************************************************************************/
/* Typedefs                                                                  */
/*****************************************************************************/
typedef char  WORD8;
typedef short WORD16;
typedef int   WORD32;

typedef unsigned char  UWORD8;
typedef unsigned short UWORD16;
typedef unsigned int   UWORD32;

/*****************************************************************************/
/* Variables                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/