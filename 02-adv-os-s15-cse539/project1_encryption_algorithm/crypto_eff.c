/*****************************************************************************/
/*                                                                           */
/* Author 1 : Mukund Manikarnike          			             			 */
/* ASU ID   : 1208597425						     					 	 */
/* e-mail   : mmanikar@asu.edu; mukunm@gmail.com			     			 */
/*                                                                           */
/* Author 2 : Lakshmi Srinivas                                               */
/* ASU ID   : 1208635554                                                     */
/* e-mail   : lsriniv2@asu.edu; laksh91@gmail.com		             		 */
/*                                                                           */
/* Course    : CSE 539                                                       */
/* Instructor: Partha Dasgupta                                               */
/* Semester  : Spring - 2015                                                 */
/* 								             								 */
/*****************************************************************************/

/*****************************************************************************/
/* File Includes                                                             */
/*****************************************************************************/

#include "file_if.h"

/*****************************************************************************/
/*                                                                           */
/* Description: This function is the entry point to the test program which   */
/*              takes in a plain-text, performs encryption on it with all    */
/*              possible 32-bit keys and checks the number of changes in the */
/*              bits in the cipher text produced with a change in 1-bit in   */
/*              the plain text.                                              */
/*																			 */
/* Input      : 1. Number of Arguments to the program                        */
/*              2. All command line arguments passed to the program   		 */
/* 																			 */
/* Output     :	Statistics of performance of the encryption algorithm        */
/*																			 */
/* Return     : None														 */
/*              															 */
/*****************************************************************************/

int main(int argc, char *argv[])
{
	UWORD32 u4_i					= 0;
	UWORD32 u4_j					= 0;
	UWORD32 u4_key         			= 0;
	UWORD32 u4_shift       			= 1;
    UWORD32 u4_temp        			= 0;
	UWORD32 u4_plain_text  			= 0;
    UWORD32 u4_cipher_text_previous	= 0;
    UWORD32 u4_mask			      	= 1;
    UWORD32 u4_change 	            = 0;
	UWORD32 u4_cipher_text_present  = 0;
    UWORD32 u4_bit_change           = 0;
	UWORD32 u4_sum_1                = 0;
	UWORD32 u4_sum_2                = 0;
	UWORD32 u4_avg                  = 0;
	UWORD32 u4_count                = 0;
    UWORD32 u4_max_bit_change  		= 0;
    UWORD32 u4_min_bit_change  		= 32;

	if(argc != 2)
	{
		printf("Please enter key string as an argument to run analysis\n");
		return 0;
	}

    if(validate_key_string(argv[1]) == FALSE)
    {
		printf("Oops!! Your input text was invalid \n");
		printf("Please enter 8 characters and ensure that \n");
		printf("the characters are within 0-9, A-F and a-f \n");
		return 0;
	}

	/* Convert plain text into an integer */
	u4_plain_text = (UWORD32) strtol(argv[1], NULL, 16);

	/* Loops until the largest key is obtained. For each key the following   */
	/* algorithm is carried out.                                             */
	/*																		 */
	while(1)
	{
		u4_key     = u4_temp;
		u4_shift   = 1;

		/* Loop for 31 times and for each loop, an encryption is performed */
		/* using the key obtained. The cipher text of the current loop and */
		/* the cipher text of the previous loop are compared to see the    */
		/* number of bytes that differ which is then stored as a stat.     */
		for(u4_i = 1; u4_i <= 31 ; u4_i++)
		{
			u4_bit_change  = 0;
			u4_mask 	       = 0x01;

			/* Performs encryption for the current key */
			u4_cipher_text_present = encrypt(u4_plain_text, u4_key);

			/* Calculates the change in the Cipher text between the current */
			/* and the previous cipher text.                                */
			u4_change = u4_cipher_text_present ^ u4_cipher_text_previous;

			/* Calculates the number of bits that have changed between       */
			/* encryptions 													 */
			for(u4_j = 0; u4_j < 32; u4_j++)
			{
				if (u4_change & u4_mask)
					u4_bit_change++;

				u4_mask = u4_mask << 1;
			}

			/* Computes Max, Min and Average statistics of bit change */
			if(u4_bit_change != 0xFFFFFFFF)
			{
				if (u4_bit_change > u4_max_bit_change)
					u4_max_bit_change = u4_bit_change;

				if (u4_bit_change < u4_min_bit_change)
					u4_min_bit_change = u4_bit_change;

				if(u4_temp <= 0xFFFF)
					u4_sum_1 += u4_bit_change;
				else
					u4_sum_2 += u4_bit_change;

				 u4_count++;
			}

			/* Computes the new key to be used for the next round */
			if (u4_temp & u4_shift)
				u4_key = u4_temp ^ u4_shift;
			else
				u4_key = u4_temp | u4_shift;

			u4_shift = u4_shift << 1;
		}

		u4_cipher_text_previous = u4_cipher_text_present;
		u4_temp = u4_temp + 1;

		if(u4_temp == 0x0000FFFF)
			break;
	}

	u4_avg = (u4_sum_1 >> 31) + (u4_sum_2 >> 31);

	printf("MIN_BIT_CHANGE ==== %d\n", u4_min_bit_change);
	printf("MAX_BIT_CHANGE ==== %d\n", u4_max_bit_change);
	printf("SUM-1          ==== %d\n", u4_sum_1);
	printf("SUM-2          ==== %d\n", u4_sum_2);
	printf("Count          ==== %d\n", u4_count);
	printf("AVG_BIT_CHANGE ==== %d\n", u4_avg);

	return 0;
}
