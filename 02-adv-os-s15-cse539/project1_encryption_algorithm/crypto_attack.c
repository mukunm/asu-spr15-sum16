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

#include "file_if.h"

/*****************************************************************************/
/* Function Defintions                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Description: This function is the entry point to the whole program. It    */
/*              takes the file name to be decrypted by breaking.             */
/*																			 */
/* Input      : 1. Number of Arguments to the program                        */
/*              2. All command line arguments passed to the program   		 */
/* 																			 */
/* Output     :	Decryption through a brute force attack.                     */
/*																			 */
/* Return     : None														 */
/*              															 */
/*****************************************************************************/

int main(int argc, char *argv[])
{
	BOOL_T  e_attack_status      = FALSE;
	BOOL_T  e_decrypt_status     = FALSE;
	UWORD8  u1_file_name[100]    = {0};
	UWORD32 u4_i                 = 0;
	UWORD32 u4_j                 = 0;
	UWORD32 u4_key               = 0;
	UWORD32 u4_sample_ctext      = 0;
	UWORD32 u4_sample_icv        = 0;
	s_file_data_t s_ip_file_data = {0};

	/* If 2 arguments are not passed as input, do not proceed further. */
	if(argc != 2)
	{
		printf("=====================================================\n");
		printf("This application will perform a brute-force attack on\n");
		printf("the cryptographic algorith, display the key used for \n");
		printf("encryption and decrypt the file provided.            \n");
		printf("=====================================================\n");
		printf("\n\n");
		printf("=====================================================\n");
		printf("Usage:\n\r");
		printf("%s <Input-File-Name>\n\r", argv[0]);
		printf("Give the Encrypted file name as an input to the app  \n");
		printf("=====================================================\n");
		printf("\n\n");

		return 0;
	}

	/* Copies the file name to a temporary variable */
	strcpy(u1_file_name, argv[1]);

	/* Read from the file */
	s_ip_file_data = read_from_file(u1_file_name, FALSE);

	/* If the read fails, print an error message and exit */
	if(s_ip_file_data.pu1_file_data == NULL)
	{
		printf("Error: Memory allocation during the read failed. Exiting\n\r");
		return 0;
	}

	/* Extract a sample cipher text from the file data */
	u4_sample_ctext =
			MAKE_WORD32(MAKE_WORD16(s_ip_file_data.pu1_file_data[1],
									s_ip_file_data.pu1_file_data[2]),
						MAKE_WORD16(s_ip_file_data.pu1_file_data[3],
									s_ip_file_data.pu1_file_data[4]));

	/* Extract a sample ICV from the file data */
	u4_sample_icv =
			MAKE_WORD32(MAKE_WORD16(s_ip_file_data.pu1_file_data[5],
									s_ip_file_data.pu1_file_data[6]),
						MAKE_WORD16(s_ip_file_data.pu1_file_data[7],
									s_ip_file_data.pu1_file_data[8]));

	/* Perform a brute force attack by looping through until all keys */
	/* have been tried.                                               */
	while(u4_key < 0xFFFFFFFF)
	{
		UWORD32 u4_attempted_ptext = 0;
		UWORD32 u4_icv_check 	   = 0;

		BOOL_T e_decip = decipher(u4_sample_ctext, u4_key,
								  u4_sample_icv, &u4_attempted_ptext);

		if(e_decip == TRUE)
		{
			u4_i++;
			printf("========================\n\r");
			printf("Brute Force attack found a key\n\r");
			printf("Possible key No. %d is 0x%x\n\r", u4_i, u4_key);
			printf("Attempting Decryption Now..\n");

			e_attack_status = TRUE;

			sprintf(u1_file_name, "%s", argv[1]);

			printf("Decryption - Attempt %d \n\r", u4_i);

			/* Decrypt File Data */
			e_decrypt_status = decrypt_file_data(s_ip_file_data, u4_key);

			if(e_decrypt_status == FALSE)
			{
				printf("Decryption failed\n\r");
				printf("========================\n\r");
				printf("\n\n");
			}
			else
			{

				printf("Decryption Successful\n\r");
				printf("========================\n\r");
				printf("\n\n");

				/* create a temporary file name to store the decrypted output. */
				sprintf(u1_file_name, "%s.%d", u1_file_name, u4_i);
				strcat(u1_file_name, ".dcrypt");

				/* Write the decrypted data to the output file. */
				/* Pass FALSE to indicate decryption.           */
				write_to_file(u1_file_name, s_ip_file_data, FALSE);

				break;
			}

		}

		u4_key++;
	}

	/* Check if attack was successful */
	if(e_attack_status == FALSE)
		printf("Brute Force attack failed\n\r");

	/* Free the memory allocated. */
	free(s_ip_file_data.pu1_file_data);

	return 0;
}
