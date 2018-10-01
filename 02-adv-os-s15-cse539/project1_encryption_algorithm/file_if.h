/*****************************************************************************/
/*                                                                           */
/* Author 1 : Mukund Manikarnike											 */
/* ASU ID   : 1208597425													 */
/* e-mail   : mmanikar@asu.edu; mukunm@gmail.com						     */
/*                                                                           */
/* Author 2 : Lakshmi Srinivas                                               */
/* ASU ID   : 1208635554                                                     */
/* e-mail   : lsriniv2@asu.edu; laksh91@gmail.com							 */
/*                                                                           */
/* Course    : CSE 539                                                       */
/* Instructor: Partha Dasgupta                                               */
/* Semester  : Spring - 2015                                                 */
/* 																			 */
/*****************************************************************************/

/*****************************************************************************/
/* File Includes                                                             */
/*****************************************************************************/
#include "crypto.h"

/*****************************************************************************/
/* Structure definitions                                                     */
/*****************************************************************************/

/* Structure to store data and metadata of a file read. */
typedef struct {
	UWORD8  *pu1_file_data;   /* Pointer to the input data */
	UWORD32 u4_file_sz_bytes; /* File Size in Bytes      */
} s_file_data_t;

/*****************************************************************************/
/* Extern function declarations                                              */
/*****************************************************************************/

extern s_file_data_t read_from_file(UWORD8 *pu1_file_name, BOOL_T e_ecrypt);

extern void encrypt_file_data(s_file_data_t s_ip_file_data, UWORD32 u4_key);

extern BOOL_T decrypt_file_data(s_file_data_t s_ip_file_data, UWORD32 u4_key);

extern void write_to_file(UWORD8 *pu1_file_name, s_file_data_t s_op_file_data,
				   		  BOOL_T e_ecrypt);

/*****************************************************************************/
/* Inline Functions                                                          */
/*****************************************************************************/

/* This function validates the key string passed to it and checks if only */
/* characters from 0-9, A-F and a-f are only given as inputs.             */

INLINE BOOL_T validate_key_string(UWORD8 *pu1_key)
{
	BOOL_T e_validate = TRUE;
	UWORD8 u1_i = 0;
	UWORD8 u1_str_len = strlen(pu1_key);

	for(u1_i = 0; u1_i < u1_str_len; u1_i++)
	{
		if(!((pu1_key[u1_i] >= 48 && pu1_key[u1_i] <= 57) ||
		     (pu1_key[u1_i] >= 65 && pu1_key[u1_i] <= 70) ||
		     (pu1_key[u1_i] >= 97 && pu1_key[u1_i] <= 102)))
		{
		   e_validate = FALSE;
		   break;
	    }
	}

	return e_validate;
}