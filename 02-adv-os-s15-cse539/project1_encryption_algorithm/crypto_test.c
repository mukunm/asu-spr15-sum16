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
/*                                                                           */
/* Description: This function is the entry point to the test program which   */
/*              takes in a 32-bit plain text, a 32-bit key and produces a    */
/*              cipher text that's 32-bit wide.                              */
/*                                                                           */
/* Input      : 1. Number of Arguments to the program                        */
/*              2. All command line arguments passed to the program   		 */
/* 																			 */
/* Output     :	Encryption/Decryption based on what the user desires         */
/*																			 */
/* Return     : None														 */
/*              															 */
/*****************************************************************************/

int main()
{
	UWORD32 u4_key          = 0;
	UWORD32 u4_plain_text   = 0;
	UWORD32 u4_cipher_text  = 0;
	UWORD32 u4_decrypt_text = 0;

	printf("Enter plain text in hex: ");
	scanf("%x", &u4_plain_text);
	printf("\n");

	printf("Enter key in hex: ");
	scanf("%x", &u4_key);
	printf("\n");

	printf("========PT, K===========\n\n");
	printf("plain_text = 0x%x \n\r", u4_plain_text);
	printf("key = 0x%x\n\n\r", u4_key);
	printf("\n\n");

	u4_cipher_text = encrypt(u4_plain_text, u4_key);

	printf("===========CT===========\n\n");
	printf("cipher_text = 0x%x \n\n\r", u4_cipher_text);
	printf("\n\n");

	u4_decrypt_text = decrypt(u4_cipher_text, u4_key);

	printf("=========Decrypted=======\n\n");
	printf("decrypt_text = 0x%x \n\n\r", u4_decrypt_text);
	printf("\n\n");

	return 0;
}
