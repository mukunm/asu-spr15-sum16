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
/* Function Definitions                                                      */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Description: This function performs appropriate sanity checks upon        */
/*				opening the file, calculates the size of the input file, 	 */
/*				allocates appropriate amount of memmory for the file data 	 */
/*				as well a padding.											 */
/*																			 */
/*				The padding is required to make the file data 32-bit word 	 */
/*				aligned since the encryption and decryption algorithms take	 */
/*				a 32 bit word only as input. The padded memory is filled	 */
/*				with zeroes.												 */
/*																			 */
/*				The function then reads each byte of the file and stores it	 */
/*				in the memory allocated and returns the structure containing */
/*				the data and metadata about the file.                        */
/*                                                                           */
/* Input      : 1. The input file name                                       */
/* 																			 */
/* Output     :	File Data in a structure               	                     */
/*																			 */
/* Return     : Structure of type s_file_data_t containing the data present  */
/*              in the file and the file size in bytes.                      */
/*																			 */
/*****************************************************************************/

s_file_data_t read_from_file(UWORD8 *pu1_file_name, BOOL_T e_ecrypt)
{
	UWORD32       u4_padding          = 0;
	UWORD32       u4_num_alloc_bytes  = 0;
	UWORD32       u4_ip_file_sz_bytes = 0;
	FILE 	      *pf_ip_file   	  = fopen(pu1_file_name, "r");
	s_file_data_t s_ip_file_data      = {0};

	/*************************************************************************/
	/*                                                                       */
	/* The final output structure which will contain the file data will be   */
	/* of the following format                                               */
	/*                                                                       */
	/* +-----------+----------------------------------------+--------------+ */
	/* | Padding   |               File Data                |  Padding     | */
	/* | Num Bytes |                                        |              | */
	/* +-----------+----------------------------------------+--------------+ */
	/* |           |                                        | x * 2bytes to| */
	/* |  1 byte   |        Size Number of bytes *  2       | make f-struct| */
	/* |(Value = x)|                                        | word aligned | */
	/* +-----------+----------------------------------------+--------------+ */
	/*             \______________________________________________________/  */
	/*                                        |								 */
    /*  								   f-struct 						 */
	/*                                                                       */
	/* The size being multiplied by 2 is because of the following reason     */
	/* f-struct which represents the file structure will internally be       */
	/* organized as shown below. Due to the Integrity Check Values that need */
	/* to be added, the size allocated will double.                          */
	/*                                                                       */
	/* +-----------+--------+-----------+--------+-----+-----------+--------+*/
	/* | File Data |   ICV  | File Data |  ICV   |     | File Data |   ICV  |*/
	/* |   Word 1  | Word 1 |   Word 2  | Word 2 | ... |   Word N  | Word N |*/
	/* +-----------+--------+-----------+--------+-----+-----------+--------+*/
	/* |  4 bytes  | 4 bytes|  4 bytes  | 4 bytes| ... |  4 bytes  | 4 bytes|*/
	/* +-----------+--------+-----------+--------+-----+-----------+--------+*/
	/*                                                                       */
	/*************************************************************************/

	/* If the input file pointer is null, file opening failed, return without*/
	/* any further processing.                                               */
	if(pf_ip_file == NULL)
	{
		printf("Input file couldn't be opened.\n\r");
		return s_ip_file_data;
	}

	/* Seek till the end of the file */
	fseek(pf_ip_file, 0, SEEK_END);

	/* Read the file size in number of bytes on reaching the end of the file */
	u4_ip_file_sz_bytes = ftell(pf_ip_file);

	/* Padding is required only during encryption because any read for the   */
	/* purpose of decryption doesn't need paddig to be done by the algorith  */
	/* because the algorithm assumes that a decryption would be requested if */
	/* an encryption has been done previously using the same algorithm. In   */
	/* such a case, there is no necessity for padding to be done during      */
	/* decryption because the already executed encryption would have rendered*/
	/* the input to decryption word-aligned.                                 */
	if(e_ecrypt == TRUE)
	{
		/* Calculate the padding required. Padding is the remainder obtained
		/* on division by 4. */
		u4_padding = 4 - u4_ip_file_sz_bytes +
					 4 * (u4_ip_file_sz_bytes >> 2);
	 }

	/* Allocate memory for 													 */
	/* File Size(Bytes) + Padding - To ensure they are 32-bit word aligned   */
	u4_num_alloc_bytes  = u4_ip_file_sz_bytes;

	/* Padding required only for encryption. */
	if(e_ecrypt == TRUE)
	{
		u4_num_alloc_bytes += u4_padding;

		/* Allocate double the number calculated to accomadate storage of */
		/* integrity checks.                                              */
		u4_num_alloc_bytes *= 2;

		/* Allocate one additional byte to store the number of bytes that */
		/* will be padded.                                                */
		u4_num_alloc_bytes += 1;
	}

	/* Populate the input file data structure with the pointer address */
	/* and file size.                                                  */
	s_ip_file_data.pu1_file_data = (UWORD8 *)malloc(u4_num_alloc_bytes);
	s_ip_file_data.u4_file_sz_bytes = u4_num_alloc_bytes;

	/* Reset the entire memory allocated to all zeroes */
	memset(s_ip_file_data.pu1_file_data, 0, u4_num_alloc_bytes);

	/* Return to the start of the file to read data from the beginning of the*/
	/* file.                                                                 */
	fseek(pf_ip_file, 0, SEEK_SET);

	/* Read the file data 1 byte at a time. */
	if(e_ecrypt == TRUE)
	{
		UWORD32 u4_i = 0;
		UWORD32 u4_file_bytes_read = 0;

		/* Store the padding number of bytes at the start */
		*(s_ip_file_data.pu1_file_data) = (u4_padding & 0x000000FF);

		/* The read is performed with 2 counters u4_i and u4_file_bytes read */
		/* because bytes in the file are read contiguously whereas the write */
		/* to the memory allocated has to be done to alternate 32-bit words. */

		/* The read is reduced by padding - 1 because the file size is */
		/* less than the memory that was allocated. The allocation was */
		/* carried out to fit the padding, padding metadata and the    */
		/* integrity check values.                                     */

		while(u4_file_bytes_read <
			  (((s_ip_file_data.u4_file_sz_bytes - 1) >> 1) - u4_padding))
		{
			/* The write to alternate words is achieved by doing the followig*/
			/* The counter u4_i + 4 is checked to see if it is divisible by 8*/
			/* If it is divisible by 8, u4_i is incremented by 4 and the     */
			/* current read loop is skipped.                                 */
			UWORD32 u4_rem_mod8 = (u4_i + 4) - (8 * ((u4_i + 4) >> 3));

			/* Skip 4 bytes as mentioned above */
			if((u4_i != 0) && (u4_rem_mod8 == 0))
				u4_i += 4;

			fread(s_ip_file_data.pu1_file_data + u4_i + 1, 1, 1, pf_ip_file);

			u4_file_bytes_read++;
			u4_i++;
		}

	}
	else
	{
		/* In the decryption case, the file is expected to have data in the */
		/* format that is mentioned in the comment at the start of this fu- */
		/* -nction. So, all the data is read from the file into the buffer  */
		fread(s_ip_file_data.pu1_file_data, 1,
			  s_ip_file_data.u4_file_sz_bytes, pf_ip_file);
	}

	/* Close the file */
	fclose(pf_ip_file);

	return s_ip_file_data;
}

/*****************************************************************************/
/*                                                                           */
/* Description: This function reads the file data byte by byte and performs  */
/*				an encryption 32-bit word by word by using the encryption    */
/*              algorithm.                                                   */
/*																			 */
/* Input      : 1. Input File Data Structure (s_file_data_t)                 */
/*              2. Key (32-bit word)										 */
/* 																			 */
/* Output     :	Encrypted Data in a structure (s_file_data_t)                */
/*																			 */
/* Return     : None														 */
/*              															 */
/*****************************************************************************/

void encrypt_file_data(s_file_data_t s_ip_file_data, UWORD32 u4_key)
{
	UWORD32 u4_i = 1;

	/* Loop until reaching the maximum file size. Don't encrypt the first    */
	/* byte because it contains the length of the padded bytes at the end of */
	/* the file.                                                             */

	/* Read alternate words because the file structure will have space for   */
	/* ICVs alternating between every word of plain-text in the file.        */
	for(u4_i = 1; u4_i < s_ip_file_data.u4_file_sz_bytes; u4_i += 8)
	{
		/* Combine each set of 4 bytes in the input to make a 32-bit word. */
		UWORD32 u4_temp_ptext =

			MAKE_WORD32(MAKE_WORD16(s_ip_file_data.pu1_file_data[u4_i],
									s_ip_file_data.pu1_file_data[u4_i + 1]),
						MAKE_WORD16(s_ip_file_data.pu1_file_data[u4_i + 2],
									s_ip_file_data.pu1_file_data[u4_i + 3]));

		/* Encrypt each 32-bit word to obtain a 32-bit word of cipher text. */
		s_cipher_t s_cipher = compute_cipher(u4_temp_ptext, u4_key);

		/* Write the encrypted cipher text into the file data structure */
		s_ip_file_data.pu1_file_data[u4_i]  =
										s_cipher.u4_ctext & 0x000000FF;

		s_ip_file_data.pu1_file_data[u4_i + 1] =
										(s_cipher.u4_ctext & 0x0000FF00) >> 8;

		s_ip_file_data.pu1_file_data[u4_i + 2] =
										(s_cipher.u4_ctext & 0x00FF0000) >> 16;

		s_ip_file_data.pu1_file_data[u4_i + 3] =
										(s_cipher.u4_ctext & 0xFF000000) >> 24;

		/* Write the Integrity check value into the file data structure */
		s_ip_file_data.pu1_file_data[u4_i + 4]  =
										s_cipher.u4_icv & 0x000000FF;

		s_ip_file_data.pu1_file_data[u4_i + 5] =
										(s_cipher.u4_icv & 0x0000FF00) >> 8;

		s_ip_file_data.pu1_file_data[u4_i + 6] =
										(s_cipher.u4_icv & 0x00FF0000) >> 16;

		s_ip_file_data.pu1_file_data[u4_i + 7] =
										(s_cipher.u4_icv & 0xFF000000) >> 24;
	}
}

/*****************************************************************************/
/*                                                                           */
/* Description: This function reads the file data byte by byte and performs  */
/*				a decryption 32-bit word by word by using the decryption     */
/*              algorithm.                                                   */
/*																			 */
/* Input      : 1. Input File Data Structure (s_file_data_t)                 */
/*              2. Key (32-bit word)										 */
/* 																			 */
/* Output     :	Decrypted Data in a structure (s_file_data_t)                */
/*																			 */
/* Return     : None														 */
/*              															 */
/*****************************************************************************/

BOOL_T decrypt_file_data(s_file_data_t s_ip_file_data, UWORD32 u4_key)
{
	UWORD8  *pu1_temp_ptxt = NULL;
	UWORD32 u4_i 		   = 1;
	BOOL_T  e_decip_status = TRUE;

	/* Allocate a temporary pointer to store the plain text */
	pu1_temp_ptxt = (UWORD8 *) malloc(s_ip_file_data.u4_file_sz_bytes - 1);

	/* If allocation failed, return */
	if(pu1_temp_ptxt == NULL)
	{
		e_decip_status = FALSE;
		return e_decip_status;
	}

	/* Reset temporary pointer */
	memset(pu1_temp_ptxt, 0, (s_ip_file_data.u4_file_sz_bytes - 1));

	/* Loop until reaching the maximum file size. Don't decrypt the first    */
	/* byte because it contains the number of bytes padded at the end.       */

	/* Read alternate words because the file structure will have space for   */
	/* ICVs alternating between every word of plain-text in the file.        */
	for(u4_i = 1; u4_i < s_ip_file_data.u4_file_sz_bytes; u4_i += 8)
	{
		UWORD32 u4_ptext = 0;

		UWORD32 u4_ctext =

			MAKE_WORD32(MAKE_WORD16(s_ip_file_data.pu1_file_data[u4_i],
									s_ip_file_data.pu1_file_data[u4_i + 1]),
						MAKE_WORD16(s_ip_file_data.pu1_file_data[u4_i + 2],
									s_ip_file_data.pu1_file_data[u4_i + 3]));

		UWORD32 u4_icv =

			MAKE_WORD32(MAKE_WORD16(s_ip_file_data.pu1_file_data[u4_i + 4],
									s_ip_file_data.pu1_file_data[u4_i + 5]),
						MAKE_WORD16(s_ip_file_data.pu1_file_data[u4_i + 6],
									s_ip_file_data.pu1_file_data[u4_i + 7]));

		/* Decrypt each 32-bit word to obtain a 32-bit word of plain text. */
		e_decip_status = decipher(u4_ctext, u4_key, u4_icv, &u4_ptext);

		if(e_decip_status == FALSE)
		{
			/* Free the temporary pointer */
			free(pu1_temp_ptxt);
			return e_decip_status;
		}

		/* Write the decrypted cipher text into the temporary pointer */
		pu1_temp_ptxt[u4_i - 1] = u4_ptext & 0x000000FF;
		pu1_temp_ptxt[u4_i]     = (u4_ptext & 0x0000FF00) >> 8;
		pu1_temp_ptxt[u4_i + 1] = (u4_ptext & 0x00FF0000) >> 16;
		pu1_temp_ptxt[u4_i + 2] = (u4_ptext & 0xFF000000) >> 24;
	}

	/* Copy the decrypted data from the temporary pointer to the file */
	/* structure.                                                     */
	memcpy((s_ip_file_data.pu1_file_data + 1), pu1_temp_ptxt,
		   (s_ip_file_data.u4_file_sz_bytes - 1));

	/* Free the temporary pointer */
	free(pu1_temp_ptxt);
	return e_decip_status;

}

/*****************************************************************************/
/*                                                                           */
/* Description: This function writes byte by byte from the output file data  */
/*              structure into the output file that is created/opened using  */
/*              the output file name that is given as input to this function */
/*																			 */
/* Input      : 1. Output File name                                          */
/*              2. Output File Data Structure (s_file_data_t)	     		 */
/* 																			 */
/* Output     :	File data in the output file                                 */
/*																			 */
/* Return     : None														 */
/*              															 */
/*****************************************************************************/

void write_to_file(UWORD8 *pu1_file_name, s_file_data_t s_op_file_data,
				   BOOL_T e_ecrypt)
{
	/* Open file in Write mode. */
	FILE *pf_op_file = fopen(pu1_file_name, "w");

	/*************************************************************************/
	/*                                                                       */
	/* The input file data structure which will contain the file data will   */
	/* be of the following format                                            */
	/*                                                                       */
	/* +-----------+----------------------------------------+--------------+ */
	/* | Padding   |               File Data                |  Padding     | */
	/* | Num Bytes |                                        |              | */
	/* +-----------+----------------------------------------+--------------+ */
	/* |           |                                        | x * 2bytes to| */
	/* |  1 byte   |        Size Number of bytes * 2        | make f-struct| */
	/* |(Value = x)|                                        | word aligned | */
	/* +-----------+----------------------------------------+--------------+ */
	/*             \______________________________________________________/  */
	/*                                        |								 */
    /*  								   f-struct 						 */
	/*                                                                       */
	/* The size being multiplied by 2 is because of the following reason     */
	/* f-struct which represents the file structure will internally be       */
	/* organized as shown below. Due to the Integrity Check Values that need */
	/* to be added, the size allocated will double.                          */
	/*                                                                       */
	/* +-----------+--------+-----------+--------+-----+-----------+--------+*/
	/* | File Data |   ICV  | File Data |  ICV   |     | File Data |   ICV  |*/
	/* |   Word 1  | Word 1 |   Word 2  | Word 2 | ... |   Word N  | Word N |*/
	/* +-----------+--------+-----------+--------+-----+-----------+--------+*/
	/* |  4 bytes  | 4 bytes|  4 bytes  | 4 bytes| ... |  4 bytes  | 4 bytes|*/
	/* +-----------+--------+-----------+--------+-----+-----------+--------+*/
	/*                                                                       */
	/*************************************************************************/

	/* Write 1 byte at a time until max file size is reached. */
	if(e_ecrypt == TRUE)
	{
		/* Write the entire encrypted data stored in the buffer to the file. */
		fwrite(s_op_file_data.pu1_file_data, 1,
			   s_op_file_data.u4_file_sz_bytes, pf_op_file);
	}
	else
	{
		/* In the decryption case, the 1st byte in the buffer and the last x */
		/* bytes which are padding will have to be discarded while writing   */
		/* the output to the file.                                           */
		UWORD8 u1_padding = *(s_op_file_data.pu1_file_data);

		UWORD32 u4_i = 0;
		UWORD32 u4_bytes_written = 0;

		/* The write is performed with 2 counters u4_i and u4_bytes_written  */
		/* because bytes in the file are written contiguously whereas the    */
		/* read from the memory allocated has to be done from alternate      */
		/* 32-bit words.                                                     */
		while(u4_bytes_written <
			  (((s_op_file_data.u4_file_sz_bytes - 1) >> 1) - u1_padding))
		{
			/* The read from alternate bytes is achieved by doing the        */
			/* following                                                     */
			/* The counter u4_i + 4 is checked to see if it is divisible by 8*/
			/* If it is divisible by 8, u4_i is incremented by 4             */
			UWORD32 u4_rem_mod8 = (u4_i + 4) - (8 * ((u4_i + 4) >> 3));

			/* Skip 4 bytes as mentioned above */
			if((u4_i != 0) && (u4_rem_mod8 == 0))
				u4_i += 4;

			fwrite(s_op_file_data.pu1_file_data + u4_i + 1, 1, 1, pf_op_file);

			u4_bytes_written++;
			u4_i++;
		}

	}

	fclose(pf_op_file);
}

