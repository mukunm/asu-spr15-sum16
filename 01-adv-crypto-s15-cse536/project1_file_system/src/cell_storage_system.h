/*****************************************************************************/
/*                                                                           */
/* Author: Mukund Manikarnike												 */
/* ASU ID: 1208597425														 */
/* e-mail: mmanikar@asu.edu; mukunm@gmail.com								 */
/* Course: CSE 536                                                           */
/* Instructor: Dr. Sandeep Gupta                                             */
/* 																			 */
/* Algorithms, implementation of algorithms in the form of source code and   */
/* ideas that they represent are the intellectual property of the author     */
/* mentioned above. Any replication/re-use partially or completely shall be  */
/* carried out only with the permission of the author and the instructor     */
/* mentioned above and any such replications/re-uses shall carry this notice.*/
/* Redistributions without the author/instructor's permission shall not be   */
/* allowed.                                     							 */
/*                                                                           */
/* Any violation of the same shall be referred to the author or the          */
/* instructor as a possible violation of ASU Policies on Academic Integrity. */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*																			 */
/* Description:                                                              */
/*                                                                           */
/* This file contains function prototypes, macro definitions, enumerations   */
/* and strucuture definitions required for the emulated cell storage system  */
/*																			 */
/*****************************************************************************/

/*****************************************************************************/
/* File Includes			                                                 */
/*****************************************************************************/

#include "types.h"
#include "string.h"

/*****************************************************************************/
/* Macro Declarations                                                        */
/*****************************************************************************/
/* Macros for different types of operations */
#define CS_OPERTN_INIT  0xFF
#define CS_OPERTN_READ  0x00
#define CS_OPERTN_WRITE 0x01
#define CS_OPERTN_ALLOC 0x02
#define CS_OPERTN_DALOC 0x03

/*************************************************************************/
/*                                                                       */
/* Shared memory structure looks as shown below                          */
/* +------------+-----------+-----------+-----------+-----+------------+ */
/* | Operation  | Operation | Operation | File  - 1 |     | File   - 16| */
/* |   Type     |   Number  |  Bitmap   | Struct    | ... | Struct     | */
/* +------------+-----------+-----------+-----------+------------------+ */
/* |   1 Byte   |   1 Byte  |  2 Bytes  |  542 bytes|     |  542 bytes | */
/* |            |           |           |           | ... |            | */
/* +------------+-----------+-----------+-----------+------------------+ */
/*                                      \__________/                     */
/*                                            |							 */
/*	File Struct takes	+-----------+-----------+---------------+  		 */
/*	the form as shown   | File Size | File Name |   File Data   |  		 */
/*	here.   			|		    |           |               |  		 */
/*						+-----------+-----------+---------------+  		 */
/*						|  2 Bytes  |  28 bytes | CS_SHRD_MEM_  |  		 */
/*						| 	        |           | MAX_FILE_SIZE |  		 */
/*						+-----------+-----------+---------------+  		 */
/*																		 */
/*************************************************************************/

/* Macros for shared memory field lengths */
#define CS_SHRD_MEM_OPRTN_TYPE_FIELD_LEN   1
#define CS_SHRD_MEM_OPRTN_NUM_FILED_LEN    1
#define CS_SHRD_MEM_OPRTN_BITMAP_FIELD_LEN 2
#define CS_SHRD_MEM_MAX_OPRTNS_SUPPORTED  (CS_SHRD_MEM_OPRTN_BITMAP_FIELD_LEN * \
										   BYTE_LENGTH)

#define CS_SHRD_MEM_FILE_SIZE_FIELD_LEN    2
#define CS_SHRD_MEM_FILE_NAME_FIELD_LEN    28
#define CS_SHRD_MEM_MAX_FILE_SIZE		   512

#define CS_SHRD_MEM_FILE_STRUCT_SIZE      (CS_SHRD_MEM_FILE_SIZE_FIELD_LEN + \
										   CS_SHRD_MEM_FILE_NAME_FIELD_LEN + \
										   CS_SHRD_MEM_MAX_FILE_SIZE)

#define CS_SHRD_MEM_MAX_SIZE              (CS_SHRD_MEM_OPRTN_TYPE_FIELD_LEN + \
										   CS_SHRD_MEM_OPRTN_NUM_FILED_LEN  + \
										   CS_SHRD_MEM_OPRTN_BITMAP_FIELD_LEN + \
										 ((CS_SHRD_MEM_FILE_SIZE_FIELD_LEN + \
										   CS_SHRD_MEM_FILE_NAME_FIELD_LEN + \
										   CS_SHRD_MEM_MAX_FILE_SIZE) * \
										   CS_SHRD_MEM_MAX_OPRTNS_SUPPORTED))

/* Macros for shared memory field offsets */
#define CS_SHRD_MEM_OPRTN_TYPE_OFFSET     0
#define CS_SHRD_MEM_OPRTN_NUM_OFFSET     (CS_SHRD_MEM_OPRTN_TYPE_FIELD_LEN + \
										  CS_SHRD_MEM_OPRTN_TYPE_OFFSET)
#define CS_SHRD_MEM_OPRT_BMAP_OFFSET     (CS_SHRD_MEM_OPRTN_NUM_FILED_LEN + \
										  CS_SHRD_MEM_OPRTN_NUM_OFFSET)
#define CS_SHRD_MEM_FILE_STRUCT_OFFSET   (CS_SHRD_MEM_OPRTN_TYPE_FIELD_LEN + \
										  CS_SHRD_MEM_OPRTN_NUM_FILED_LEN + \
										  CS_SHRD_MEM_OPRTN_BITMAP_FIELD_LEN)

#define CS_SHRD_MEM_FILE_STRUCT_SIZE_OFFSET 0
#define CS_SHRD_MEM_FILE_STRUCT_NAME_OFFSET (CS_SHRD_MEM_FILE_STRUCT_SIZE_OFFSET + \
											 CS_SHRD_MEM_FILE_SIZE_FIELD_LEN)
#define CS_SHRD_MEM_FILE_STRUCT_DATA_OFFSET (CS_SHRD_MEM_FILE_STRUCT_NAME_OFFSET + \
											 CS_SHRD_MEM_FILE_NAME_FIELD_LEN)

/*************************************************************************/
/*                                                                       */
/* File structure memory looks as shown below                            */
/* +-------------------------+-----------+-------------+--------------+  */
/* |       File Inode        | File  - 1 |    ...      | File   - 512 |  */
/* |                         | Struct    |             | Struct       |  */
/* +-------------------------+-----------+-------------+--------------+  */
/* |  32 * CS_FSTRUCT_MAX_   |			 |	           |              |  */
/* |  FILES_SUPPORTED  Bytes |  2 Bytes  |    ...      |   2 bytes    |  */
/* +-------------------------+-----------+-------------+--------------+  */
/*  \_______________________/                           \____________/   */
/*             |							File Structure	   |		 */
/*			   |						    +-----------+---------------+*/
/*			   |						    | File Size |   File Data   |*/
/*			   |						    +-----------+---------------+*/
/*			   |						    |  2 Bytes  | CS_FSTRUCT_   |*/
/*			   |						    | 	        | MAX_FILE_SIZE |*/
/*			   |						    +-----------+---------------+*/
/*			   |	Inode Structure     								 */
/*	 +-------------+--------------+-----+-------------+--------------+   */
/*	 | File Name 1 |  File Name 2 | ... | File Name N | File Use Bmap|	 */
/*	 +-------------+--------------+-----+-------------+--------------+   */
/*	 |  28 Bytes   |   28 bytes   | ... |  28 Bytes   |   N/8 bytes  |   */
/*	 +-------------+--------------+-----+-------------+--------------+	 */
/*                                                                       */
/*************************************************************************/

/* Macros for file system structure lengths */
#define CS_FSTRUCT_FILE_NAME_FIELD_LENGTH   28
#define CS_FSTRUCT_FILE_USE_BIT_LENGTH      1
#define CS_FSTRUCT_INODE_ENTRY_LENGTH  	   (CS_FSTRUCT_FILE_NAME_FIELD_LENGTH + \
										    CS_FSTRUCT_FILE_USE_BIT_LENGTH)

#define CS_FSTRUCT_FILE_SIZE_FIELD_LENGTH   2
#define CS_FSTRUCT_MAX_FIlE_SIZE       	    512
#define CS_FSTRUCT_MAX_FILES_SUPPORTED 	    1024

#define CS_FSTRUCT_INODE_SIZE              (CS_FSTRUCT_INODE_ENTRY_LENGTH *\
											CS_FSTRUCT_MAX_FILES_SUPPORTED)
#define CS_FSTRUCT_ENTRY_MAX_SIZE          (CS_FSTRUCT_FILE_SIZE_FIELD_LENGTH + \
											CS_FSTRUCT_MAX_FIlE_SIZE)
#define CS_FSTRUCT_MAX_SIZE                (CS_FSTRUCT_INODE_ENTRY_LENGTH * \
											CS_FSTRUCT_MAX_FILES_SUPPORTED) + \
										  ((CS_FSTRUCT_FILE_SIZE_FIELD_LENGTH + \
										    CS_FSTRUCT_MAX_FIlE_SIZE) * \
										    CS_FSTRUCT_MAX_FILES_SUPPORTED)

/* Macros for file system structure offsets */
#define CS_FSTRUCT_INODE_OFFSET             0
#define CS_FSTRUCT_INODE_FNAME_OFFSET		0
#define CS_FSTRUCT_INODE_FUSE_BMAP_OFFSET   (CS_FSTRUCT_FILE_NAME_FIELD_LENGTH * \
											 CS_FSTRUCT_MAX_FILES_SUPPORTED)
#define CS_FSTRUCT_FILE_STRUCTS_OFFSET       ((CS_FSTRUCT_INODE_ENTRY_LENGTH * \
								 		       CS_FSTRUCT_MAX_FILES_SUPPORTED) + \
								 		       CS_FSTRUCT_INODE_OFFSET)

#define CS_FSTRUCT_FILE_STRUCTS_FSIZE_OFFSET   0
#define CS_FSTRUCT_FILE_STRUCTS_FDATA_OFFSET  (CS_FSTRUCT_FILE_STRUCTS_FSIZE_OFFSET + \
									           CS_FSTRUCT_FILE_SIZE_FIELD_LENGTH)

/* Semaphore related macros */
#define STATUS_OPRTN_SEM_NUM 				16
#define INIT_OPRTN_SEM_NUM   				1
#define STATUS_OPERTN_SEM_INIT_VALUE        0
#define INIT_OPERTN_SEM_INIT_VALUE          0

/* Other Macros */
#define CS_FSTRUCT_INVALID_FOPRTN_NUM       0xFF
#define CS_FSTRUCT_INVALID_FIDX             0xFFFF
#define CS_FSTRUCT_INVALID_FSIZE            0xFFFF

/* Logging Macros */
#define CS_LOG_START_IDX 0
#define CS_LOG_END_IDX   1
#define CS_LOG_SIZE      2

/*****************************************************************************/
/* Variable Declarations                                                     */
/*****************************************************************************/
extern UWORD8 *g_pu1_shrd_mem;
extern UWORD8 *g_pu1_fstruct_mem;

/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/
extern void *cs_read(void *pv_oprtn_num);
extern void *cs_write(void *pv_oprtn_num);
extern void *cs_allocate(void *pv_oprtn_num);
extern void *cs_deallocate(void *pv_oprtn_num);

/*****************************************************************************/
/* Inline Functions                                                          */
/*****************************************************************************/

/******************** Functions to Access Shared Memory **********************/

/* This function returns the operation type in shared memory */
INLINE UWORD8 get_shrd_mem_oprtn_type(void)
{
	return *(g_pu1_shrd_mem + CS_SHRD_MEM_OPRTN_TYPE_OFFSET);
}

/* This function sets the operation type in shared memory */
INLINE void set_shrd_mem_oprtn_type(UWORD8 u1_oprt_type)
{
	*(g_pu1_shrd_mem + CS_SHRD_MEM_OPRTN_TYPE_OFFSET) = u1_oprt_type;
}

/* This function gets the operation number in shared memory */
INLINE UWORD8 get_shrd_mem_oprtn_num(void)
{
	return *(g_pu1_shrd_mem + CS_SHRD_MEM_OPRTN_NUM_OFFSET);
}

/* This function sets the operation number in shared memory */
INLINE void set_shrd_mem_oprtn_num(UWORD8 u1_oprtn_num)
{
	*(g_pu1_shrd_mem + CS_SHRD_MEM_OPRTN_NUM_OFFSET) = u1_oprtn_num;
}

/* This function gets the operation bitmap in shared memory */
INLINE UWORD16 get_shrd_mem_oprtn_bmap(void)
{
	/* Extract Operation Bitmap */
	UWORD16 u2_ret_val = MAKE_WORD16(*(g_pu1_shrd_mem +
									   CS_SHRD_MEM_OPRT_BMAP_OFFSET),
									 *(g_pu1_shrd_mem +
									   CS_SHRD_MEM_OPRT_BMAP_OFFSET + 1));

	return u2_ret_val;
}

/* Finds the first bit in the bitamp that is set to 0 */
INLINE UWORD8 find_free_oprtn_num()
{
	UWORD16 u2_bmap = get_shrd_mem_oprtn_bmap();
	UWORD8  u1_i = CS_FSTRUCT_INVALID_FOPRTN_NUM;

	for(u1_i = 0; u1_i < 16; u1_i++)
	{
		if((u2_bmap & (1 << u1_i)) == 0)
			return u1_i;
	}

	return u1_i;
}

/* This function sets the operation bitmap in shared memory */
INLINE void set_shrd_mem_oprtn_bmap(UWORD16 u2_bmap)
{
	UWORD8 u1_bmap_byte_0 =  u2_bmap & 0x00FF;
	UWORD8 u1_bmap_byte_1 = (u2_bmap & 0xFF00) >> 8;

	*(g_pu1_shrd_mem + CS_SHRD_MEM_OPRT_BMAP_OFFSET)     = u1_bmap_byte_0;
	*(g_pu1_shrd_mem + CS_SHRD_MEM_OPRT_BMAP_OFFSET + 1) = u1_bmap_byte_1;
}

/* This function sets the bit corresponding to the bit provided */
INLINE void set_shrd_mem_oprtn_bmap_bit(UWORD8 u1_bit)
{
	UWORD16 u2_bmap = get_shrd_mem_oprtn_bmap();

	/* Set the bit desired */
	u2_bmap |= (1 << u1_bit);

	/* Set the new bitmap in shared memory */
	set_shrd_mem_oprtn_bmap(u2_bmap);
}

/* This function resets the bit corresponding to the bit provided */
INLINE void reset_shrd_mem_oprtn_bmap_bit(UWORD8 u1_bit)
{
	UWORD16 u2_bmap = get_shrd_mem_oprtn_bmap();

	/* Reset the bit desired */
	u2_bmap &= ~(1 << u1_bit);

	/* Set the new bitmap in shared memory */
	set_shrd_mem_oprtn_bmap(u2_bmap);
}

/* This function gets the file structure in shared memory */
INLINE UWORD8 *get_shrd_mem_fstruct(UWORD8 u1_oprtn_num)
{
	/* Extract the file structure from the file structure offset */
	UWORD8 *pu1_fstruct = g_pu1_shrd_mem + CS_SHRD_MEM_FILE_STRUCT_OFFSET +
						 (u1_oprtn_num * CS_SHRD_MEM_FILE_STRUCT_SIZE);

	return pu1_fstruct;
}

/* This function sets the file structure in shared memory */
INLINE void set_shrd_mem_fstruct(UWORD8 u1_oprtn_num, UWORD8 *pu1_fstruct)
{
	memcpy((g_pu1_shrd_mem + CS_SHRD_MEM_FILE_STRUCT_OFFSET +
			(u1_oprtn_num * CS_SHRD_MEM_FILE_STRUCT_SIZE)),
			pu1_fstruct, CS_SHRD_MEM_FILE_STRUCT_SIZE);
}

/* This function gets the file size in the file structure in shared memory */
INLINE UWORD16 get_shrd_mem_fstruct_fsize(UWORD8 *pu1_fstruct)
{
	UWORD16 u2_fsize = MAKE_WORD16(*(pu1_fstruct +
									 CS_SHRD_MEM_FILE_STRUCT_SIZE_OFFSET),
								   *(pu1_fstruct +
									 CS_SHRD_MEM_FILE_STRUCT_SIZE_OFFSET + 1));

	return u2_fsize;
}

/* This function sets the file size in the file structure in shared memory */
INLINE void set_shrd_mem_fstruct_fsize(UWORD8 *pu1_fstruct, UWORD16 u2_fsize)
{
	UWORD8 u1_fsize_byte_0 =  u2_fsize & 0x00FF;
	UWORD8 u1_fsize_byte_1 = (u2_fsize & 0xFF00) >> 8;

	*(pu1_fstruct + CS_SHRD_MEM_FILE_STRUCT_SIZE_OFFSET)     = u1_fsize_byte_0;
	*(pu1_fstruct + CS_SHRD_MEM_FILE_STRUCT_SIZE_OFFSET + 1) = u1_fsize_byte_1;
}

/* This function gets the file name in the file structure in shared memory */
INLINE void get_shrd_mem_fstruct_fname(UWORD8 *pu1_fstruct,
									   UWORD8 *pu1_fname)
{
	UWORD8 *pu1_mem_fname = pu1_fstruct + CS_SHRD_MEM_FILE_STRUCT_NAME_OFFSET;

	/* Copy the file name to the temporary array */
	memcpy(pu1_fname, pu1_mem_fname, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);
}

/* This function sets the file name in the file structure in shared memory */
INLINE void set_shrd_mem_fstruct_fname(UWORD8 *pu1_fstruct,
			  						   UWORD8 *pu1_fname)
{
	UWORD8 *pu1_mem_fname = pu1_fstruct + CS_SHRD_MEM_FILE_STRUCT_NAME_OFFSET;

	/* Copy the file name string to the file structure memeory */
	memcpy(pu1_mem_fname, pu1_fname, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);
}

/* This function corrupts the file name field */
INLINE void corrupt_shrd_mem_fstruct_fname(UWORD8 u1_oprtn_num)
{
	/* Get the file structure */
	UWORD8 *pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);
	UWORD8 u1_corrupt_file_name[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0xFF};

	/* Corrupt the file name by setting the corrupt file name */
	set_shrd_mem_fstruct_fname(pu1_fstruct, u1_corrupt_file_name);
}

/* This function gets the file data in the file structure in shared memory */
INLINE void get_shrd_mem_fstruct_fdata(UWORD8 *pu1_fstruct,
									   UWORD8 *pu1_fdata)
{
	UWORD8 *pu1_mem_fdata = pu1_fstruct + CS_SHRD_MEM_FILE_STRUCT_DATA_OFFSET;

	memcpy(pu1_fdata, pu1_mem_fdata, CS_SHRD_MEM_MAX_FILE_SIZE);
}

/* This function sets the file data in the file structure in shared memory */
INLINE void set_shrd_mem_fstruct_fdata(UWORD8 *pu1_fstruct,
									   UWORD8 *pu1_fdata)
{
	UWORD8 *pu1_mem_fdata = pu1_fstruct + CS_SHRD_MEM_FILE_STRUCT_DATA_OFFSET;

	/* Copy the file data into shared memory */
	memcpy(pu1_mem_fdata, pu1_fdata, CS_SHRD_MEM_MAX_FILE_SIZE);
}

/****************** Functions to Access File Structure Memory ****************/

/* This function returns the index of the file in the inode given the file   */
/* name 																     */
INLINE UWORD16 get_fmem_inode_fidx_fname(UWORD8 *pu1_fname)
{
	UWORD16 u2_i    = 0;
	UWORD16 u2_fidx = CS_FSTRUCT_INVALID_FIDX;

	for(u2_i = 0; u2_i < CS_FSTRUCT_MAX_FILES_SUPPORTED; u2_i++)
	{
		UWORD8 u1_mem_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0};
		UWORD8 *pu1_inode_entry =  g_pu1_fstruct_mem +
								  (u2_i * CS_FSTRUCT_FILE_NAME_FIELD_LENGTH);

		/* Copy the file name from the file structure memory to the temporary*/
		/* variable.                                                         */
		memcpy(u1_mem_fname, pu1_inode_entry, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

		/* Compare the file name given with the file name in memory */
		if(memcmp(pu1_fname, u1_mem_fname,
				  CS_SHRD_MEM_FILE_NAME_FIELD_LEN) == 0)
		{
			/* If a match is found, return the index */
			u2_fidx = u2_i;
			break;
		}
	}

	return u2_fidx;
}

/* This function sets the file name in the inode at the given entry given the*/
/* file index and the file name 											 */
INLINE void set_fmem_inode_fname_fidx(UWORD16 u2_fidx, UWORD8 *pu1_fname)
{
	/* Extract the pointer where the name needs to be stored in the inode */
	UWORD8 *pu1_inode_fname = g_pu1_fstruct_mem +
							 (u2_fidx * CS_FSTRUCT_FILE_NAME_FIELD_LENGTH);

	/* Copy the file name from the provided input to the File memory, byte by*/
	/* byte.                                                                 */
	memcpy(pu1_inode_fname, pu1_fname, CS_FSTRUCT_FILE_NAME_FIELD_LENGTH);
}

/* This function checks if the indoe is used in the file memory.             */
INLINE BOOL_T check_fmem_inode_used_idx(UWORD16 u2_fidx)
{
	BOOL_T  e_ret_val = FALSE;
	UWORD8  *pu1_inode_use_bmap = g_pu1_fstruct_mem +
								(CS_FSTRUCT_FILE_NAME_FIELD_LENGTH *
								 CS_FSTRUCT_MAX_FILES_SUPPORTED);
	UWORD16 u2_rem             = (u2_fidx - (u2_fidx >> 3));
	UWORD16 u2_quo             = (u2_fidx >> 3);

	/* If the bit indicated by the index is set in the bitmap, return TRUE */
	if(((1 << u2_rem) & pu1_inode_use_bmap[u2_quo]) == 1)
		e_ret_val = TRUE;

	return e_ret_val;
}

/* This function returns the index of the first free inode entry */
INLINE UWORD16 get_fmem_inode_free_fidx()
{
	UWORD16 u2_fidx 			= CS_FSTRUCT_INVALID_FIDX;
	UWORD16 u2_i    			= 0;

	/* Loop through the entire INODE file use bitmap and check */
	/* if any of them are empty. If empty, return.             */
	for(u2_i = 0; u2_i < CS_FSTRUCT_MAX_FILES_SUPPORTED; u2_i++)
	{
		if(check_fmem_inode_used_idx(u2_i) == FALSE)
		{
			u2_fidx = u2_i;
			break;
		}
	}

	return u2_fidx;
}

/* This function sets the usage status of the file in the file bitmap */
INLINE void set_fmem_inode_use_idx(UWORD16 u2_fidx, BOOL_T e_free_stat)
{
	UWORD8  *pu1_inode_use_bmap = g_pu1_fstruct_mem +
								(CS_FSTRUCT_FILE_NAME_FIELD_LENGTH *
								 CS_FSTRUCT_MAX_FILES_SUPPORTED);
	UWORD16 u2_rem             = (u2_fidx - (u2_fidx >> 3));
	UWORD16 u2_quo             = (u2_fidx >> 3);

	if(e_free_stat == TRUE)
		pu1_inode_use_bmap[u2_quo] |= (1 << u2_rem);
	else
		pu1_inode_use_bmap[u2_quo] &= ~(1 << u2_rem);
}

/* This function gets the fstruct from file memory given the index */
INLINE UWORD8 *get_fmem_fstruct(UWORD16 u2_fidx)
{
	UWORD8 *pu1_fstruct = NULL;

	/* Extract the file structure pointer */
	pu1_fstruct = g_pu1_fstruct_mem + CS_FSTRUCT_INODE_SIZE +
				  (u2_fidx * CS_FSTRUCT_ENTRY_MAX_SIZE);

	return pu1_fstruct;
}

/* This function returns the size of the file given the file name */
INLINE UWORD16 get_fmem_fstruct_fsize_fname(UWORD8 *pu1_fname)
{
	UWORD8  *pu1_fstruct = NULL;
	UWORD16 u2_fsize     = CS_FSTRUCT_INVALID_FSIZE;
	UWORD16 u2_fidx      = CS_FSTRUCT_INVALID_FIDX;

	/* Lookup file name in the inode */
	u2_fidx = get_fmem_inode_fidx_fname(pu1_fname);

	/* If lookup failed, return */
	if(u2_fidx == CS_FSTRUCT_INVALID_FIDX)
		return u2_fsize;

	/* Check if the index found is used in the inode or not */
	if(check_fmem_inode_used_idx(u2_fidx) == FALSE)
		return u2_fsize;

	/* Extract the file structure pointer */
	pu1_fstruct = g_pu1_fstruct_mem + CS_FSTRUCT_INODE_SIZE +
				  (u2_fidx * CS_FSTRUCT_ENTRY_MAX_SIZE);

	/* Extract the size from the file structure and return */
	u2_fsize =
		   MAKE_WORD16(*(pu1_fstruct +
						 CS_FSTRUCT_FILE_STRUCTS_FSIZE_OFFSET),
					   *(pu1_fstruct +
						 CS_FSTRUCT_FILE_STRUCTS_FSIZE_OFFSET + 1));
	return u2_fsize;
}

/* This function sets the size of the file given the file name */
INLINE void set_fmem_fstruct_fsize_fname(UWORD8 *pu1_fname, UWORD16 u2_fsize)
{
	UWORD8  u1_fsize_byte_0 =  u2_fsize & 0x00FF;
	UWORD8  u1_fsize_byte_1 = (u2_fsize & 0xFF00) >> 8;
	UWORD8  *pu1_fstruct    = NULL;
	UWORD16 u2_fidx         = CS_FSTRUCT_INVALID_FIDX;

	/* Lookup file name in the inode */
	u2_fidx = get_fmem_inode_fidx_fname(pu1_fname);

	/* If lookup failed, return */
	if(u2_fidx == CS_FSTRUCT_INVALID_FIDX)
		return;

	/* Check if the index found is used in the inode or not */
	if(check_fmem_inode_used_idx(u2_fidx) == FALSE)
		return;

	/* Extract the file structure pointer */
	pu1_fstruct = g_pu1_fstruct_mem + CS_FSTRUCT_INODE_SIZE +
				  (u2_fidx * CS_FSTRUCT_ENTRY_MAX_SIZE);

	*(pu1_fstruct + CS_FSTRUCT_FILE_STRUCTS_FSIZE_OFFSET) =
														u1_fsize_byte_0;

	*(pu1_fstruct + CS_FSTRUCT_FILE_STRUCTS_FSIZE_OFFSET + 1) =
														u1_fsize_byte_1;
}

/* This function returns the file data given the file name */
INLINE UWORD8 *get_fmem_fstruct_fdata_fname(UWORD8 *pu1_fname)
{
	UWORD8  *pu1_fdata   = NULL;
	UWORD8  *pu1_fstruct = NULL;
	UWORD16 u2_fidx      = CS_FSTRUCT_INVALID_FIDX;

	/* Lookup file name in the inode */
	u2_fidx = get_fmem_inode_fidx_fname(pu1_fname);

	/* If lookup failed, return */
	if(u2_fidx == CS_FSTRUCT_INVALID_FIDX)
		return pu1_fdata;

	/* Check if the index found is used in the inode or not */
	if(check_fmem_inode_used_idx(u2_fidx) == FALSE)
		return pu1_fdata;

	/* Extract the file structure pointer */
	pu1_fstruct = g_pu1_fstruct_mem + CS_FSTRUCT_INODE_SIZE +
				  (u2_fidx * CS_FSTRUCT_ENTRY_MAX_SIZE);

	/* Extract the file data from the file structure pointer */
	pu1_fdata = pu1_fstruct + CS_FSTRUCT_FILE_STRUCTS_FDATA_OFFSET;

	return pu1_fdata;
}

/* This function sets the file data given the file name */
INLINE void set_fmem_fstruct_fdata_fname(UWORD8 *pu1_fname, UWORD8 *pu1_fdata)
{
	UWORD8  *pu1_fstruct = NULL;
	UWORD16 u2_fidx = CS_FSTRUCT_INVALID_FIDX;

	/* Lookup file name in the inode */
	u2_fidx = get_fmem_inode_fidx_fname(pu1_fname);

	/* If lookup failed, return */
	if(u2_fidx == CS_FSTRUCT_INVALID_FIDX)
		return;

	/* Check if the index found is used in the inode or not */
	if(check_fmem_inode_used_idx(u2_fidx) == FALSE)
		return;

	/* Extract the file structure pointer */
	pu1_fstruct = g_pu1_fstruct_mem + CS_FSTRUCT_INODE_SIZE +
				  (u2_fidx * CS_FSTRUCT_ENTRY_MAX_SIZE);

	/* Set file data provided in the pointer to the shared memory */
	memcpy((pu1_fstruct + CS_FSTRUCT_FILE_STRUCTS_FDATA_OFFSET),
		   pu1_fdata, CS_FSTRUCT_MAX_FIlE_SIZE);
}

