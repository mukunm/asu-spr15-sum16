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
/* Function Definitions                                                      */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Description: This function carries out one round of encryption on the     */
/*				input plain text provided with the key. The algorithm used to*/
/*              perform the encryption is as described in the comments in the*/
/*              function.  													 */
/*                                                                           */
/* Input      : 1. Plain Text (32 bit)                                       */
/*              2. Key        (32 bit)                                       */
/* 																			 */
/* Output     :	Cipher Text at the end of one round (32 bit)               	 */
/*																			 */
/* Return     : None														 */
/*																			 */
/*****************************************************************************/

UWORD32 encrypt_one_round(UWORD32 u4_ptext, UWORD32 u4_key)
{
	/******************************************************************/
	/* The encryption algorithm works as follows.                     */
	/*                                                                */
	/* The key (32 bits) is partitioned into 2 16 bit words the usage */
	/* of which is described below.                                   */
	/*                                                                */
	/*    +-------------------------+------------------------------+  */
	/*    |         Word 1          |          Word 0              |  */
	/*    +-------------------------+------------------------------+  */
	/*    | Used in 2nd phase       |    Used in 1st Phase         |  */
	/*    +-------------------------+------------------------------+  */
	/*                                                                */
	/******************************************************************/

	UWORD16 u2_key_w0 = u4_key & 0x0000FFFF;
	UWORD16 u2_key_w1 = (u4_key & 0xFFFF0000) >> 16;

	/******************************************************************/
	/* Phase 1                                                        */
	/*                                                                */
	/* A circular shift is performed based on Word 0 in the Key as per*/
	/* the description given below.                                   */
	/*                                                                */
	/* Each of the 4 nibbles of Word 0 of the key is used to Right or */
	/* left circular shift the corresponding byte of the plain text   */
	/* as per the description given below.                            */
	/*                                                                */
	/* +------------+-----------+-------------+-------------+         */
	/* |   BIT3     |    BIT2   |     BIT1    |    BIT0     |         */
	/* +------------+-----------+-------------+-------------+         */
	/* | 0 - LShift |                                       |         */
	/* +------------+           Shift Value                 +         */
	/* | 1 - RShift |                                       |         */
	/* +------------+---------------------------------------+         */
	/*                                                                */
	/******************************************************************/

	UWORD8 u1_p1_key0 =  u2_key_w0 & 0x000F;
	UWORD8 u1_p1_key1 = (u2_key_w0 & 0x00F0) >> 4;
	UWORD8 u1_p1_key2 = (u2_key_w0 & 0x0F00) >> 8;
	UWORD8 u1_p1_key3 = (u2_key_w0 & 0xF000) >> 12;

	UWORD8 u1_p1_ptext_byte0 =  u4_ptext & 0x000000FF;
	UWORD8 u1_p1_ptext_byte1 = (u4_ptext & 0x0000FF00) >> 8;
	UWORD8 u1_p1_ptext_byte2 = (u4_ptext & 0x00FF0000) >> 16;
	UWORD8 u1_p1_ptext_byte3 = (u4_ptext & 0xFF000000) >> 24;

	UWORD8 u1_p1_ctext_byte0 = ((u1_p1_key0 & 0x08) == 0) ?
					   WORD8_LCSH(u1_p1_ptext_byte0, (u1_p1_key0 & 0x07)) :
					   WORD8_RCSH(u1_p1_ptext_byte0, (u1_p1_key0 & 0x07));

	UWORD8 u1_p1_ctext_byte1 = ((u1_p1_key1 & 0x08) == 0) ?
					   WORD8_LCSH(u1_p1_ptext_byte1, (u1_p1_key1 & 0x07)) :
					   WORD8_RCSH(u1_p1_ptext_byte1, (u1_p1_key1 & 0x07));

	UWORD8 u1_p1_ctext_byte2 = ((u1_p1_key2 & 0x08) == 0) ?
					   WORD8_LCSH(u1_p1_ptext_byte2, (u1_p1_key2 & 0x07)) :
					   WORD8_RCSH(u1_p1_ptext_byte2, (u1_p1_key2 & 0x07));

	UWORD8 u1_p1_ctext_byte3 = ((u1_p1_key3 & 0x08) == 0) ?
					   WORD8_LCSH(u1_p1_ptext_byte3, (u1_p1_key3 & 0x07)) :
					   WORD8_RCSH(u1_p1_ptext_byte3, (u1_p1_key3 & 0x07));

	UWORD16 u2_p1_ctext_w0 = u1_p1_ctext_byte0 |
							(u1_p1_ctext_byte1 << 8);

	UWORD16 u2_p1_ctext_w1 = u1_p1_ctext_byte2 |
							(u1_p1_ctext_byte3 << 8);

	/********************************************************************/
	/* Phase 2                                                          */
	/*                                                                  */
	/* The circular shifted 16 bit words obtained from the plain-text   */
	/* are XORed with the Most significant 16-bit word of the key       */
	/* in the following manner to obtain 2 16-bit words of the cipher   */
	/* text which are then combined to obtain the final cipher text.    */
	/*                                                                  */
	/* 1. Word1 of the key is XORed as it is with Word0 of the output   */
	/*    from phase 1.                                                 */
	/*                                                                  */
	/* 2. MSB and LSB of Word1 of the key are interchanged and XORed    */
	/*    Word1 of the output from phase 1.                             */
	/*                                                                  */
	/********************************************************************/

	UWORD16 u2_p2_ctext_w0 = u2_p1_ctext_w0 ^ u2_key_w1;

	UWORD16 u2_p2_ctext_w1 = u2_p1_ctext_w1 ^
					      (((u2_key_w1 & 0xFF00) >> 8) |
					       ((u2_key_w1 & 0x00FF) << 8));

	UWORD32 u4_p2_ctext = (u2_p2_ctext_w1 << 16) | (u2_p2_ctext_w0);

	return u4_p2_ctext;

}

/*****************************************************************************/
/*                                                                           */
/* Description: This function does 1 round of decryption on the input cipher */
/*              text provided with the key. The algorithm used to perform the*/
/*              decryption is as described in the comments in the function.  */
/*                                                                           */
/* Input      : 1. Cipher Text (32 bit)                                      */
/*              2. Key        (32 bit)                                       */
/* 																			 */
/* Output     :	Plain Text at the end of one round (32 bit)                	 */
/*																			 */
/* Return     : None														 */
/*																			 */
/*****************************************************************************/

UWORD32 decrypt_one_round(UWORD32 u4_ctext, UWORD32 u4_key)
{
	/*************************************************************************/
	/* Decryption is the exact reverse of encryption which means that Phase 2*/
	/* is carried out first and then Phase 1.                                */
	/*************************************************************************/

	/******************************************************************/
	/* The decryption algorithm works as follows.                     */
	/*                                                                */
	/* The key (32 bits) is partitioned into 2 16 bit words the usage */
	/* of which is described below.                                   */
	/*                                                                */
	/*    +-------------------------+------------------------------+  */
	/*    |         Word 1          |          Word 0              |  */
	/*    +-------------------------+------------------------------+  */
	/*    | Used in 1st phase       |    Used in 1st Phase         |  */
	/*    +-------------------------+------------------------------+  */
	/*                                                                */
	/******************************************************************/

	UWORD16 u2_ctext_w0 =  u4_ctext & 0x0000FFFF;
	UWORD16 u2_ctext_w1 = (u4_ctext & 0xFFFF0000) >> 16;

	UWORD16 u2_key_w0 =  u4_key & 0x0000FFFF;
	UWORD16 u2_key_w1 = (u4_key & 0xFFFF0000) >> 16;

	/*********************************************************************/
	/* Phase 1 of the decryption process which is same as Phase 2 of the */
	/* encryption process. Refer to Phase 2 of encryption for more info. */
	/*********************************************************************/

	UWORD16 u2_p1_ctext_w0 = u2_ctext_w0 ^ u2_key_w1;
	UWORD16 u2_p1_ctext_w1 = u2_ctext_w1 ^
						  (((u2_key_w1 & 0xFF00) >> 8) |
						   ((u2_key_w1 & 0x00FF) << 8));

	UWORD8 u1_p1_ctext_byte0 =  u2_p1_ctext_w0 & 0x00FF;
	UWORD8 u1_p1_ctext_byte1 = (u2_p1_ctext_w0 & 0xFF00) >> 8;
	UWORD8 u1_p1_ctext_byte2 =  u2_p1_ctext_w1 & 0x00FF;
	UWORD8 u1_p1_ctext_byte3 = (u2_p1_ctext_w1 & 0xFF00) >> 8;

	/*********************************************************************/
	/* Phase 2 of the decryption process which is same as Phase 1 of the */
	/* encryption process. Refer to Phase 1 of encryption for more info. */
	/*********************************************************************/

	UWORD8 u1_p2_key0 =  u2_key_w0 & 0x000F;
	UWORD8 u1_p2_key1 = (u2_key_w0 & 0x00F0) >> 4;
	UWORD8 u1_p2_key2 = (u2_key_w0 & 0x0F00) >> 8;
	UWORD8 u1_p2_key3 = (u2_key_w0 & 0xF000) >> 12;

	UWORD8 u1_p2_ctext_byte0 = ((u1_p2_key0 & 0x08) == 0) ?
					   WORD8_RCSH(u1_p1_ctext_byte0, (u1_p2_key0 & 0x07)) :
					   WORD8_LCSH(u1_p1_ctext_byte0, (u1_p2_key0 & 0x07));

	UWORD8 u1_p2_ctext_byte1 = ((u1_p2_key1 & 0x08) == 0) ?
					   WORD8_RCSH(u1_p1_ctext_byte1, (u1_p2_key1 & 0x07)) :
					   WORD8_LCSH(u1_p1_ctext_byte1, (u1_p2_key1 & 0x07));

	UWORD8 u1_p2_ctext_byte2 = ((u1_p2_key2 & 0x08) == 0) ?
					   WORD8_RCSH(u1_p1_ctext_byte2, (u1_p2_key2 & 0x07)) :
					   WORD8_LCSH(u1_p1_ctext_byte2, (u1_p2_key2 & 0x07));

	UWORD8 u1_p2_ctext_byte3 = ((u1_p2_key3 & 0x08) == 0) ?
					   WORD8_RCSH(u1_p1_ctext_byte3, (u1_p2_key3 & 0x07)) :
					   WORD8_LCSH(u1_p1_ctext_byte3, (u1_p2_key3 & 0x07));

	UWORD32 u4_ptext = (u1_p2_ctext_byte3 << 24) |
					   (u1_p2_ctext_byte2 << 16) |
					   (u1_p2_ctext_byte1 << 8)  |
					    u1_p2_ctext_byte0;

	return u4_ptext;
}

