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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

/*****************************************************************************/
/* Macro Definitions                                                         */
/*****************************************************************************/

#define INLINE static __inline

#define BIT31 (1 << 31)

#define WORD8   8
#define WORD32 32

/* Macros to perform circular shift */
#define WORD8_LCSH(value, shift_value) ((value << shift_value) | (value >> (WORD8 - shift_value)))
#define WORD8_RCSH(value, shift_value) ((value >> shift_value) | (value << (WORD8 - shift_value)))

/* Macro to combine bytes into words */
#define MAKE_WORD16(lsb, msb) ((((UWORD16)(msb) << 8) & 0xFF00) | (lsb))
#define MAKE_WORD32(lsw, msw) ((((UWORD32)(msw) << 16) & 0xFFFF0000) | (lsw))

/* This is a random 32-bit number chosen to perform Integrity Check */
#define CRC32_POLYNOMIAL 0x42FCE586

/*****************************************************************************/
/* Typedefs                                                                  */
/*****************************************************************************/

typedef unsigned char  UWORD8;
typedef unsigned short UWORD16;
typedef unsigned int   UWORD32;

/*****************************************************************************/
/* Enums                                                                     */
/*****************************************************************************/

typedef enum {
	FALSE = 0,
	TRUE  = 1
} BOOL_T;

/*****************************************************************************/
/* Structure Definitions                                                     */
/*****************************************************************************/

typedef struct {
	UWORD32 u4_ctext; /* Cipher Text */
	UWORD32 u4_icv;   /* Integrity Check Value */
} s_cipher_t;

/*****************************************************************************/
/* Extern function declarations                                              */
/*****************************************************************************/

extern UWORD32 encrypt_one_round(UWORD32 u4_ptext, UWORD32 u4_key);
extern UWORD32 decrypt_one_round(UWORD32 u4_ctext, UWORD32 u4_key);

/*****************************************************************************/
/* Inline Functions                                                          */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* This is the entry function to the encryption algorithm which in turn uses */
/* encrypt_one_round to perform each round of encryption on the plain text.  */
/* Details on working of the algorithm are mentioned in comments inside the  */
/* function. Details of how each round of encryption is performed is mention */
/* -ed in the encrypt_one_round founction.                                   */
/*                                                                           */
/*****************************************************************************/

INLINE UWORD32 encrypt(UWORD32 u4_ptext, UWORD32 u4_key)
{
	/*************************************************************************/
	/*                                                                       */
	/* The algorithm sees the input key as 2 16 bit words.                   */
	/* The focus on the algorithm is to obtain 2 sub-keys and use each of    */
	/* in alternative rounds of encryption.                                  */
	/*                                                                       */
	/* The sub-keys are obtained as follows.                                 */
	/* Key 1 - Same as input key.                                            */
	/* Key 2 - by swapping the most significant 16-bit word with the least   */
	/*         significant 16-bit word                                       */
	/*                                                                       */
	/* The algorithm runs for 7 such rounds with Key 1 and Key 2 used in     */
	/* alternation.                                                          */
	/*                                                                       */
	/*************************************************************************/

	UWORD32 u4_ctext    = 0;

	UWORD32 u4_sub_key1 =   u4_key;
	UWORD32 u4_sub_key2 = ((u4_key & 0x0000FFFF) << 16) |
						  ((u4_key & 0xFFFF0000) >> 16);

	u4_ctext = encrypt_one_round(u4_ptext, u4_sub_key1);
	u4_ctext = encrypt_one_round(u4_ctext, u4_sub_key2);
	u4_ctext = encrypt_one_round(u4_ctext, u4_sub_key1);
	u4_ctext = encrypt_one_round(u4_ctext, u4_sub_key2);
	u4_ctext = encrypt_one_round(u4_ctext, u4_sub_key1);
	u4_ctext = encrypt_one_round(u4_ctext, u4_sub_key2);
	u4_ctext = encrypt_one_round(u4_ctext, u4_sub_key1);

	return u4_ctext;
}

/*****************************************************************************/
/*                                                                           */
/* The decryption algorithm works exactly like the encrypt, except that this */
/* function calls the decrypt_one_round function instead of the encrypt_one  */
/* _round function.                                                          */
/*                                                                           */
/*****************************************************************************/

INLINE UWORD32 decrypt(UWORD32 u4_ctext, UWORD32 u4_key)
{
	UWORD32 u4_ptext    = 0;
	UWORD32 u4_sub_key1 =   u4_key;
	UWORD32 u4_sub_key2 = ((u4_key & 0x0000FFFF) << 16) |
						  ((u4_key & 0xFFFF0000) >> 16);

	u4_ptext = decrypt_one_round(u4_ctext, u4_sub_key1);
	u4_ptext = decrypt_one_round(u4_ptext, u4_sub_key2);
	u4_ptext = decrypt_one_round(u4_ptext, u4_sub_key1);
	u4_ptext = decrypt_one_round(u4_ptext, u4_sub_key2);
	u4_ptext = decrypt_one_round(u4_ptext, u4_sub_key1);
	u4_ptext = decrypt_one_round(u4_ptext, u4_sub_key2);
	u4_ptext = decrypt_one_round(u4_ptext, u4_sub_key1);

	return u4_ptext;
}

/*****************************************************************************/
/*                                                                           */
/* This function computes a 32 bit CRC by performing a polynomial division.  *
/* This function has been written using the folllowing link as a reference   */
/* http://www.zlib.net/crc_v3.txt                                            */
/*                                                                           */
/*****************************************************************************/

INLINE UWORD32 compute_crc32(UWORD32 u4_ptext)
{
	UWORD8  u1_i           = 0;
	UWORD32 u4_crc         = 0;
	UWORD32 u4_crc_pnomial = CRC32_POLYNOMIAL;

	u4_crc = u4_ptext;

	for(u1_i = 0; u1_i < WORD32; u1_i++)
	{
		if(u4_crc & BIT31)
			u4_crc ^= u4_crc_pnomial;

		u4_crc <<= 1;
	}

	return u4_crc;
}


/*****************************************************************************/
/*                                                                           */
/* This function computes the cipher text on the plain text using the encrypt*/
/* function and computes the integrity check using CRC 32 and returns the    */
/* Cipher computed in the form of a structure.                               */
/*                                                                           */
/*****************************************************************************/

INLINE s_cipher_t compute_cipher(UWORD32 u4_ptext, UWORD32 u4_key)
{
	s_cipher_t s_cipher = {0};

	s_cipher.u4_ctext = encrypt(u4_ptext, u4_key);
	s_cipher.u4_icv = compute_crc32(u4_ptext);

	return s_cipher;
}

/*****************************************************************************/
/*                                                                           */
/* This function takes as input, the cipher text, the key and the integrity  */
/* check value that came along with the cipher text. It decrypts the cipher  */
/* text, computes the integrity check on the plain text obtained through     */
/* decryption and compares the obtained integrity check value with the one   */
/* that came along with the cipher text. If they match, it means that the    */
/* decryption was successful. If not, the decryption wasn't successful, the  */
/* status of which is returned by the function.                              */
/*                                                                           */
/*****************************************************************************/

INLINE BOOL_T decipher(UWORD32 u4_ctext, UWORD32 u4_key,
					   UWORD32 u4_icv, UWORD32 *pu4_ptext)
{
	BOOL_T e_decipher_status = FALSE;
	UWORD32 u4_check_icv     = 0;

	*pu4_ptext   = decrypt(u4_ctext, u4_key);
	u4_check_icv = compute_crc32(*pu4_ptext);

	if(u4_check_icv == u4_icv)
		e_decipher_status = TRUE;

	return e_decipher_status;
}
