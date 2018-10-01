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

void reset_key_bit_num(UWORD32 *pu4_key, UWORD8 u1_bit_num)
{
	*pu4_key = *pu4_key ^ (1 << u1_bit_num);
}

UWORD8 find_lsb_set(UWORD32 u4_key)
{
	UWORD8 u1_i = 0;

	/* AND one bit at a time with the key provided from the least significant*/
	/* bit through to the most significant, exit on finding a bit that is set*/
	for(u1_i = 0; u1_i < 32; u1_i++)
	{
		UWORD32 u4_temp = (u4_key & (1 << u1_i)) >> u1_i;

		if(u4_temp == 1)
			return u1_i;
	}

	u1_i = 32;
	return u1_i;
}

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
	UWORD32 u4_plain_text = 0;
	UWORD32 u4_curr_ctext = 0;
	UWORD32 u4_prev_ctext = 0;
	UWORD32 u4_key_cnt    = 0;
	UWORD32 u4_key 		  = 0;
	UWORD32 u4_sum_1      = 0;
	UWORD32 u4_sum_2      = 0;
	UWORD32 u1_min_change = 32;
	UWORD32 u1_max_change = 0;
	UWORD32 u4_count      = 0;

	/* If the number of arguments passed doesn't match the requirement */
	/* display a help message and return.                              */
	if(argc != 2)
	{
		printf("=======================================================\n");
		printf("Please pass the plain text for analysis as an argument \n");
		printf("as shown below.\n");
		printf("%s 1f23456d\n", argv[0]);
		printf("=======================================================\n");
		return 0;
	}

	/* Check if the plain text input matches the required plain text */
	/* characteristics 												 */
    if(validate_key_string(argv[1]) == FALSE)
    {
		printf("Oops!! Your input text was invalid \n");
		printf("Please enter 8 characters and ensure that \n");
		printf("the characters are within 0-9, A-F and a-f \n");
		return 0;
	}

	/* Convert plain text into an integer */
	u4_plain_text = (UWORD32) strtol(argv[1], NULL, 16);

	/* Loop until all keys are exhausted */
	while(1)
	{
		/* Perform an encryption */
		u4_curr_ctext = encrypt(u4_plain_text, u4_key);

		/* Perform a comparison of previous and current cipher texts provided*/
		/* one encryption has already been done previously.                  */
		if(u4_key_cnt != 0)
		{
			UWORD32 u4_change      = u4_curr_ctext ^ u4_prev_ctext;
			UWORD32 u1_bit_changes = 0;
			UWORD32 u1_i 	       = 0;

			/* Compute number of bits set to obtain number of bits changed */
			for(u1_i = 0; u1_i < 32; u1_i++)
			{
				if((u4_change & (1 << u1_i)) != 0)
					u1_bit_changes++;
			}

			/* Calculate the Sum total of bit changes */
			if(u4_key_cnt < 0xFFFFFFF)
				u4_sum_1 += u1_bit_changes;
			else
				u4_sum_2 += u1_bit_changes;

			/* Compute minimum and maximum bit changes */
			u1_min_change = (u1_bit_changes < u1_min_change) ?
							 u1_bit_changes : u1_min_change;
			u1_max_change = (u1_bit_changes > u1_max_change) ?
							 u1_bit_changes : u1_max_change;

			u4_count++;
		}

		/* Set the previous cipher text to be used for the next loop */
		u4_prev_ctext = u4_curr_ctext;

		/* On reaching the last key, break */
		if(u4_key_cnt == 0xFFFFFFFF)
			break;

		/* Increment the key count */
		u4_key_cnt++;

		/* Derive the next key */
		if((u4_key_cnt % 4) == 0)
		{
			/* Find the least significant bit that is set in the key count */
			UWORD8 u1_bit_num = find_lsb_set(u4_key_cnt);

			/* Return if an appropriate bit number couldn't be found */
			if(u1_bit_num == 32)
			{
				printf("ERROR: Didn't find a bit set in the key count.\n");
				return 0;
			}

			/* Reset the bit number obtained in the key */
			reset_key_bit_num(&u4_key, u1_bit_num);
		}
		else
		{
			/* Reset BIT 1 if even and 0 if odd */
			if((u4_key_cnt % 2) == 0)
				reset_key_bit_num(&u4_key, 1);
			else
				reset_key_bit_num(&u4_key, 0);
		}
	}

	printf("Min Change      = 0x%x\n", u1_min_change);
	printf("Max Change      = 0x%x\n", u1_max_change);
	printf("Sum - 1 Changes = 0x%x\n", u4_sum_1);
	printf("Sum - 2 Changes = 0x%x\n", u4_sum_2);
	printf("Key Count       = 0x%x\n", u4_count);

	return 0;
}
