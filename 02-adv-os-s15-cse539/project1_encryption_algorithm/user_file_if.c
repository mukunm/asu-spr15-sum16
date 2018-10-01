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
/*                                                                           */
/* Description: This function is the entry point to the whole program. It    */
/*              the file name to be encrypted/decrypted to be provided as    */
/*              a command line argument. On obtaining the input file, it     */
/*              does the following											 */
/*				1. Read from input file										 */
/*				2. Perform encrypt/decrypt 									 */
/*				3. Write the output to another file							 */
/*																			 */
/* Input      : 1. Number of Arguments to the program                        */
/*              2. All command line arguments passed to the program   		 */
/* 																			 */
/* Output     :	Encryption/Decryption based on what the user desires         */
/*																			 */
/* Return     : None														 */
/*              															 */
/*****************************************************************************/

int main(int argc, char *argv[])
{
	UWORD8  u1_file_name[100]    = {0};
	UWORD8  u1_key_string[9]     = {0};
	UWORD32 u4_input_value       = 2;
	UWORD32 u4_key               = 0;
	s_file_data_t s_ip_file_data = {0};

	/* If 2 arguments are not passed as input, do not proceed further. */
	if(argc != 2)
	{
		printf("=====================================================\n");
		printf("This application will encrypt or decrypt the input   \n");
		printf("file provided with the key provided as input.        \n");
		printf("=====================================================\n");
		printf("\n\n");
		printf("=====================================================\n");
		printf("Usage:\n\r");
		printf("%s <Input-File-Name>\n\r", argv[0]);
		printf("Give the file name as an input to the app            \n");
		printf("=====================================================\n");
		printf("\n\n");
		return 0;
	}

	/* Copies the file name to a temporary variable */
	strcpy(u1_file_name, argv[1]);

	/* Reads the the operation desired by the user and also the key to perform */
	/* the encryption or decryption.                                           */
	printf("Input: Enter 0 to Encrypt, 1 to Decrypt: ");
	scanf("%d", &u4_input_value);
	printf("\n\r");

	printf("Enter the key in hex: ");
	scanf("%s", u1_key_string);
	printf("\n\r");

	if(validate_key_string(u1_key_string) == FALSE)
	{
		printf("Oops!! Your input key was invalid \n");
		printf("Please enter 8 characters and ensure that \n");
		printf("the characters are within 0-9, A-F and a-f \n");
		return 0;
	}

	u4_key = (UWORD32)strtol(u1_key_string, NULL, 16);
	printf("Key is 0x%x\n\r", u4_key);

	/* Encrypt File */
	if(u4_input_value == 0)
	{
		/* Call the read function to open the file. */
		/* Pass TRUE to indicate encryption.        */
		s_ip_file_data = read_from_file(u1_file_name, TRUE);

		if(s_ip_file_data.pu1_file_data == NULL)
		{
			printf("Error: Couldn't allocate memory while reading the file\n");
			return 0;
		}

		/* Encrypt File Data */
		encrypt_file_data(s_ip_file_data, u4_key);

		/* Create a temporary file name to store the encrypted output. */
		strcat(u1_file_name, ".ecrypt");

		/* Write the encrypted data to the output file. */
		/* Pass TRUE to indicate encryption.            */
		write_to_file(u1_file_name, s_ip_file_data, TRUE);
	}
	/* Decrypt File */
	else if(u4_input_value == 1)
	{
		BOOL_T e_decrypt_status = FALSE;

		/* Call the read function to open the file. */
		/* Pass FALSE to indicate decryption.        */
		s_ip_file_data = read_from_file(u1_file_name, FALSE);

		if(s_ip_file_data.pu1_file_data == NULL)
		{
			printf("Error: Couldn't allocate memory while reading the file\n");
			return 0;
		}

		/* Decrypt File Data */
		e_decrypt_status = decrypt_file_data(s_ip_file_data, u4_key);

		if(e_decrypt_status == FALSE)
		{
			printf("Decryption failed\n\r");

			/* Free the memory allocated to store file data */
			free(s_ip_file_data.pu1_file_data);

			return 0;
		}

		/* create a temporary file name to store the decrypted output. */
		strcat(u1_file_name, ".dcrypt");

		/* Write the decrypted data to the output file. */
		/* Pass FALSE to indicate decryption.           */
		write_to_file(u1_file_name, s_ip_file_data, FALSE);
	}
	/* Invalid Input */
	else
	{
		printf("Invalid Input \n\r");
	}

	/* Free the memory allocated to store file data */
	free(s_ip_file_data.pu1_file_data);

	return 0;
}