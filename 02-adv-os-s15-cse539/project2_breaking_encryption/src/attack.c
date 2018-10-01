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

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "md5.h"

/*****************************************************************************/
/* Macro Definitions                                                         */
/*****************************************************************************/

#define TXT 1
#define PNG 2
#define PDF 3

/*****************************************************************************/
/* Global Variable Declarations                                              */
/*****************************************************************************/

unsigned int key;
int buf, n, infile, outfile;
MD5_CTX mdContext;

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* This function checks if the decrypted file was successfully decrypted or  */
/* not. The check is performed by checking if the given file contains        */
/* characters that do not fall in the ASCII text file range.                 */
/*****************************************************************************/

int check_decryption_succ(FILE *file, int size)
{
	int yes = 0;
	int i = 0;

	/* If the file is a text file, it checks if there are any characters */
	/* in the text file which are beyond the acceptable range of ASCII   */
	/* characters 0x7F 													 */
	for(i = 0; i < size; i++)
	{
		unsigned char c = 0;
		fread(&c, 1, 1, file);

		if(c > 0x7F)
			break;
	}

	if(i == size)
		yes = 1;

	return yes;
}

/*****************************************************************************/
/* This function was provided as part of Course material. It writes size     */
/* number of bytes one at a time to the provided output file.                */
/*****************************************************************************/

void lastbytes(int outfile, int size, int buf)
{
	int i = 0;
	char *last;

	last = (char*) &buf;

	for(i = 0; i < size; i++)
	{
		write(outfile, &last[i], 1);
	}
}

/*****************************************************************************/
/* This function was provided as part of Course material. Small changes like */
/* the addition of loop input parameter was made during integration to       */
/* support writing to different file names based on the loop in which        */
/* decryption was being performed.                                           */
/*                                                                           */
/* The decryption algorithm uses MD5 on the key provided to obtain a rolling */
/* key which gets XORed in some fashion with the plain text to achieve       */
/* confusion which is the desired property of any decryption algorithm.      */
/* More details are present in the report.                                   */
/*****************************************************************************/

void decrypt(int key, int loop)
{
	struct stat st;
	int size,fsize;
	int *temp, result;
	int rollingkey;
	char opfile[32];
	rollingkey = key;

	infile = open("output", O_RDONLY);

	if(infile<0)
	{
		printf("input open error\n"); exit(0);
	}

	buf = 0;
	read(infile, &buf, 4);

	/* get plaintext size */
	size = buf;

	/* ciphertext has xtra 4 bytes (size) and padding */
	/* get ciphertext size */
	stat("output", &st);

	fsize = st.st_size;

	if((fsize < 8)|| (size > fsize)|| (size < (fsize-8)))
	{
		printf("file size sanity check failed\n");
		exit(0);
	}

	snprintf(opfile, sizeof(char) * 32, "output-dec-%i", loop);

	outfile = open (opfile, O_RDWR|O_CREAT|O_TRUNC, 0700);

	if(outfile < 0)
	{
		printf("output open error\n");
		exit(0);
	}

	while ((n = read(infile, &buf, 4))> 0)
	{
		/* doing the reverse of encrypt */
		buf = buf ^ rollingkey;
		MD5Init(&mdContext);
		MD5Update(&mdContext, &rollingkey, 4);
		MD5Final(&mdContext);
		temp = (int *)&mdContext.digest[12];

		/* result is 32 bits of MD5 of key */
		result = *temp;

		/* new key */
		rollingkey = rollingkey ^ result;

		if (size >= 4)
			write(outfile, &buf, 4);
		else
			lastbytes(outfile, size, buf);

		/* repeat, keep track of output size in size. */
		buf  = 0;
		size = size - 4;
	}

	close(infile);
	close(outfile);
}

/*****************************************************************************/
/* This function performs a known plain-text attack on PNG files. Details of */
/* the attack are mentioned in the report.                                   */
/*****************************************************************************/

void brute_force_png()
{
	FILE *file = NULL;
	unsigned int value = 0;
	unsigned int key = 0;

	file = fopen("output", "r");

	if(file == NULL)
	{
		printf("File cannot be opened\n");
		return;
	}

	fread(&value, 4, 1, file);
	fread(&value, 4, 1, file);

	fclose(file);

	key = value ^ 0x474E5089;

	printf("key = 0x%x\n", key);

	decrypt(key, 0);
}

/*****************************************************************************/
/* This function performs a known plain-text attack on PDF files. Details of */
/* the attack are mentioned in the report.                                   */
/*****************************************************************************/

void brute_force_pdf()
{
	FILE *file = NULL;
	unsigned int value = 0;
	unsigned int key = 0;

	file = fopen("output", "r");

	if(file == NULL)
	{
		printf("File cannot be opened\n");
		return;
	}

	fread(&value, 4, 1, file);
	fread(&value, 4, 1, file);

	fclose(file);

	key = value ^ 0x46445025;

	printf("key = 0x%x\n", key);

	decrypt(key, 0);
}

/*****************************************************************************/
/* Since Text files do not have a known format, a chosen plain-text attack   */
/* cannot be performed as it is done for PDF and PNG files in the above      */
/* functions. This function performs a brute force attack by looping through */
/* each possible key and checking if decryption is successful.               */
/*****************************************************************************/

void brute_force_txt()
{
	FILE *file = NULL;
	unsigned int value = 0;
	unsigned int pt = 0;
	unsigned int key = 0;
	unsigned int file_size = 0;
	int i = 0;

	file = fopen("output", "r");

	if(file == NULL)
	{
		printf("File cannot be opened\n");
		return;
	}

	fread(&value, 4, 1, file);
	file_size = value;

	fread(&value, 4, 1, file);

	printf("value = 0x%x\n\r", value);

	fclose(file);

	key = 0xFFFFFFFF;

	while(1)
	{
		unsigned char byte1 = 0;
		unsigned char byte2 = 0;
		unsigned char byte3 = 0;
		unsigned char byte4 = 0;

		if(((key & 0x000080) == 0) || ((key & 0x00008000) != 0) ||
		   ((key & 0x800000) == 0) || ((key & 0x80000000) == 0))
		{
		   key--;

			if(key == 0)
				break;

		   continue;
		}

		pt = key ^ value;

		byte1 =  pt & 0x000000FF;
		byte2 = (pt & 0x0000FF00) >> 8;
		byte3 = (pt & 0x00FF0000) >> 16;
		byte4 = (pt & 0xFF000000) >> 24;

		if((byte1 < 0x7F) &&
		   (byte2 < 0x7F) &&
		   (byte3 < 0x7F) &&
		   (byte4 < 0x7F))
		{
			int yes = 0;

			printf("Decryption attempt successful, key = 0x%x\n\r", key);
			decrypt(key, 0);

			FILE *file = fopen("output-dec-0", "r");

			if(file == NULL)
			{
				printf("Failed to open output file while brute-forcing. Proceeding to next\n");
				key++;
				continue;
			}

			yes = check_decryption_succ(file, file_size);

			fclose(file);

			if(yes == 1)
			{
				printf("Brute Force attack successful, Key = 0x%x\n", key);
				break;
			}

			i++;
		}

		key--;

		if(key == 0)
			break;
	}

}

/*****************************************************************************/
/* This is the entry point of the program through which brute force attack   */
/* on all files are brute forced.                                            */
/*****************************************************************************/

int main(int argc, char *argv[])
{
#ifdef BRUTE_FORCE_PNG
	brute_force_png();
#endif /* BRUTE_FORCE_PNG */

#ifdef BRUTE_FORCE_PDF
	brute_force_pdf();
#endif /* BRUTE_FORCE_PDF */

#ifdef BRUTE_FORCE_TXT
	brute_force_txt();
#endif /* #ifdef BRUTE_FORCE_TXT */
	return 0;
}
