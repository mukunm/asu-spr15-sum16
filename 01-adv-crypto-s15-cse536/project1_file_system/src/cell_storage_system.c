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
/* This file contains functions that read, write, allocate and delete files  */
/* in the emulated file system                                               */
/*																			 */
/*****************************************************************************/

/*****************************************************************************/
/* File Includes			                                                 */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include "cell_storage_system.h"

/*****************************************************************************/
/* Global Variable Declarations                                              */
/*****************************************************************************/
UWORD8 		   *g_pu1_shrd_mem          = NULL;
UWORD8 		   *g_pu1_fstruct_mem       = NULL;
WORD32 		 	g_i4_shrd_mem_id        = 0;
pthread_mutex_t g_l_write_lock;

UWORD8          g_u1_css_log[CS_FSTRUCT_MAX_FILES_SUPPORTED][CS_LOG_SIZE] 			   = {{0},};
UWORD8          g_u1_backup[CS_FSTRUCT_MAX_FILES_SUPPORTED][CS_FSTRUCT_ENTRY_MAX_SIZE] = {{0},};

/*****************************************************************************/
/* Function definitions			                                             */
/*****************************************************************************/

/*****************************************************************************/
/* This function signals the status semaphore provided the key for the       */
/* semaphore.                                                                */
/*****************************************************************************/

void wait_on_status_semaphore(UWORD8 u4_sem_num)
{
	WORD32 i4_ret_val = -1;
	WORD32 i4_sem_id  = -1;
	struct sembuf s_sem_operations[1] = {{0}, };

	/* Get the status semaphore providing the semaphore key */
	i4_sem_id = semget((g_i4_shrd_mem_id + 2), STATUS_OPRTN_SEM_NUM, 0666);

	/* Return with an error message if semaphore couldn't be obtained */
	if(i4_sem_id < 0)
	{
		printf("WAIT-ERROR: Status semaphore couldn't be obtained\n");
		return;
	}

	/* Perform a P operation on the semaphore */
	/* Set the operation  parameters */
	s_sem_operations[0].sem_num = u4_sem_num;
	s_sem_operations[0].sem_op  = -1;
	s_sem_operations[0].sem_flg = 0;

	/* Initiate the operation */
	i4_ret_val = semop(i4_sem_id, s_sem_operations, 1);

	if(i4_ret_val != 0)
	{
		printf("WAIT-ERROR: Wait operation on Status semaphore did not succeed\n");
		return;
	}

}

/*****************************************************************************/
/* This function waits on the status semaphore provided the key for the      */
/* semaphore.                                                                */
/*****************************************************************************/

void wait_on_start_semaphore()
{
	WORD32 i4_ret_val = -1;
	WORD32 i4_sem_id  = -1;
	struct sembuf s_sem_operations[INIT_OPRTN_SEM_NUM] = {{0}, };

	/* Get the start semaphore providing the semaphore key */
	i4_sem_id = semget((g_i4_shrd_mem_id + 1), INIT_OPRTN_SEM_NUM, 0666);

	/* Return with an error message if semaphore couldn't be obtained */
	if(i4_sem_id < 0)
	{
		printf("WAIT-ERROR: Start semaphore couldn't be obtained\n");
		return;
	}

	/* Perform a P operation on the semaphore */
	/* Set the operation  parameters */
	s_sem_operations[INIT_OPRTN_SEM_NUM - 1].sem_num = INIT_OPRTN_SEM_NUM - 1;
	s_sem_operations[INIT_OPRTN_SEM_NUM - 1].sem_op  = -1;
	s_sem_operations[INIT_OPRTN_SEM_NUM - 1].sem_flg =  0;

	/* Initiate the operation */
	i4_ret_val = semop(i4_sem_id, s_sem_operations, INIT_OPRTN_SEM_NUM);

	if(i4_ret_val != 0)
	{
		printf("WAIT-ERROR: Wait operation on Start semaphore did not succeed\n");
		return;
	}

}

/*****************************************************************************/
/* This function signals the start semaphore provided the key for the        */
/* semaphore.                                                                */
/*****************************************************************************/

void signal_start_semaphore()
{
	WORD32 i4_ret_val = -1;
	WORD32 i4_sem_id  = -1;
	struct sembuf s_sem_operations[INIT_OPRTN_SEM_NUM] = {{0}, };

	/* Get the start semaphore providing the semaphore key */
	i4_sem_id = semget((g_i4_shrd_mem_id + 1), INIT_OPRTN_SEM_NUM, 0666);

	/* Return with an error message if semaphore couldn't be obtained */
	if(i4_sem_id < 0)
	{
		printf("SIGNAL-ERROR: Start semaphore couldn't be obtained\n");
		return;
	}

	/* Perform a P operation on the semaphore */
	/* Set the operation  parameters */
	s_sem_operations[INIT_OPRTN_SEM_NUM - 1].sem_num = INIT_OPRTN_SEM_NUM - 1;
	s_sem_operations[INIT_OPRTN_SEM_NUM - 1].sem_op  = 1;
	s_sem_operations[INIT_OPRTN_SEM_NUM - 1].sem_flg = 0;

	/* Initiate the operation */
	i4_ret_val = semop(i4_sem_id, s_sem_operations, INIT_OPRTN_SEM_NUM);

	if(i4_ret_val != 0)
	{
		printf("SIGNAL-ERROR: Signal operation on Start semaphore did not succeed\n");
		return;
	}

}

/*****************************************************************************/
/* This function signals the status semaphore provided the key for the       */
/* semaphore.                                                                */
/*****************************************************************************/

void signal_status_semaphore(UWORD8 u4_sem_num)
{
	WORD32 i4_ret_val = -1;
	WORD32 i4_sem_id  = -1;
	struct sembuf s_sem_operations[1] = {{0}, };

	/* Get the status semaphore providing the semaphore key */
	i4_sem_id = semget((g_i4_shrd_mem_id + 2), STATUS_OPRTN_SEM_NUM, 0666);

	/* Return with an error message if semaphore couldn't be obtained */
	if(i4_sem_id < 0)
	{
		printf("SIGNAL-ERROR: Status semaphore couldn't be obtained\n");
		return;
	}

	/* Perform a P operation on the semaphore */
	/* Set the operation  parameters */
	s_sem_operations[0].sem_num = u4_sem_num;
	s_sem_operations[0].sem_op  = 1;
	s_sem_operations[0].sem_flg = 0;

	/* Initiate the operation */
	i4_ret_val = semop(i4_sem_id, s_sem_operations, 1);

	if(i4_ret_val != 0)
	{
		printf("SIGNAL-ERROR: Signal operation on Status semaphore did not succeed\n");
		return;
	}
}

/*****************************************************************************/
/* This function performs a read from file structure memory and writes the   */
/* file data to shared memory.                                               */
/*****************************************************************************/

void *cs_read(void *pv_oprtn_num)
{
	UWORD8  u1_oprtn_num     = *((UWORD8 *)pv_oprtn_num);
	UWORD8  *pu1_fstruct     = NULL;
	UWORD8  *pu1_file_name   = NULL;
	UWORD8  *pu1_fdata       = NULL;
	UWORD16  u2_fsize        = 0;

	printf("INFO: Read Attempted - Operation Number: %d\n", u1_oprtn_num);

	/* Wait for status semaphore to indicate that a read can be performed */
	wait_on_status_semaphore(u1_oprtn_num);

	/* If either the shared memory region or the file structure region have  */
	/* not been allocated, return.                                           */
	if(g_pu1_fstruct_mem == NULL || g_pu1_shrd_mem == NULL)
	{
		printf("READ-ERROR: CSS might not have initialized properly. Consider re-starting\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that read operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Extract the file struct pointer for the current operation */
	/* number from shared memory.                                */
	pu1_fstruct   = get_shrd_mem_fstruct(u1_oprtn_num);

	if(pu1_fstruct == NULL)
	{
		printf("READ-ERROR: Couldn't access file structure\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that read operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Allocate temporary memory to store the file name */
	pu1_file_name = (UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

	if(pu1_file_name == NULL)
	{
		printf("READ-ERROR: Couldn't obtain file name\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that read operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Read file name from Shared Memory */
	get_shrd_mem_fstruct_fname(pu1_fstruct, pu1_file_name);

	/* Before performing a read, check if all files has been appropriately*/
	/* written the last time it was committed. If it wasn't perform a     */
	/* restore, update the logs and clear the backup. Do this for each    */
	/* inconsistency that is found.                                       */
	{
		UWORD16 u2_i = 0;

		for(u2_i = 0; u2_i < CS_FSTRUCT_MAX_FILES_SUPPORTED; u2_i++)
		{
			/* Perform a recovery if a start log is present and an end log*/
			/* isn't present.                                             */
			if((g_u1_css_log[u2_i][CS_LOG_START_IDX] == 1) &&
			   (g_u1_css_log[u2_i][CS_LOG_END_IDX] == 0))
			{
				/* Get backup fstruct */
				UWORD8 *pu1_backup_fstruct = g_u1_backup[u2_i];

				/* Get the fstruct from file memory for the corresponding file*/
				UWORD8 *pu1_fmem_fstruct = get_fmem_fstruct(u2_i);

				printf("INFO: Read Recovery - File Index: %d\n", u2_i);

				/* Update the file memory with the backed up fstruct */
				memcpy(pu1_fmem_fstruct, pu1_backup_fstruct, CS_FSTRUCT_ENTRY_MAX_SIZE);

				/* Update the end log */
				g_u1_css_log[u2_i][CS_LOG_END_IDX] = 1;

				/* Reset the file back up */
				memset(pu1_backup_fstruct, 0, CS_FSTRUCT_ENTRY_MAX_SIZE);
			}
		}

	}

	/* Extract file size and data from the file memory */
	u2_fsize = get_fmem_fstruct_fsize_fname(pu1_file_name);

	/* Return if look-up failed */
	if(u2_fsize == CS_FSTRUCT_INVALID_FSIZE)
	{
		printf("READ-ERROR: Couldn't find file in the INODE\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that read operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Extract file data from the file memory */
	pu1_fdata = get_fmem_fstruct_fdata_fname(pu1_file_name);

	/* Return if look-up failed */
	if(pu1_fdata == NULL)
	{
		printf("READ-ERROR: Couldn't find file in the INODE\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that read operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Write the file size to shared memory */
	set_shrd_mem_fstruct_fsize(pu1_fstruct, u2_fsize);

	/* Copy file data from file memory to shared memory */
	set_shrd_mem_fstruct_fdata(pu1_fstruct, pu1_fdata);

	/* Signal Status semaphore that read operation is complete */
	signal_status_semaphore(u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a read itself doesn't mandate    */
	/* return anything. So, NULL is being returned                           */
	return NULL;
}

/*****************************************************************************/
/* This function performs a read from shared memory and writes the file data */
/* to file structure memory.                                                 */
/*****************************************************************************/

void *cs_write(void *pv_oprtn_num)
{
	UWORD8  u1_oprtn_num   = *((UWORD8 *)pv_oprtn_num);
	UWORD8  *pu1_fstruct   = NULL;
	UWORD8  *pu1_file_name = NULL;
	UWORD8  *pu1_fdata     = NULL;
	UWORD16  u2_fsize      = 0;
	UWORD16  u2_fidx       = 0;

	printf("INFO: Write Attempted - Operation Number: %d \n", u1_oprtn_num);

	/* Wait for status semaphore to indicate that a write can be performed */
	wait_on_status_semaphore(u1_oprtn_num);

	/* Acquire a lock before performing write to file memory */
	pthread_mutex_lock(&g_l_write_lock);

	/* If either the shared memory region or the file structure region have  */
	/* not been allocated, return.                                           */
	if(g_pu1_fstruct_mem == NULL || g_pu1_shrd_mem == NULL)
	{
		printf("WRITE-ERROR: CSS might not have initialized properly. Consider re-starting\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that write operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Extract the file struct pointer for the current operation */
	/* number from shared memory.                                */
	pu1_fstruct   = get_shrd_mem_fstruct(u1_oprtn_num);

	if(pu1_fstruct == NULL)
	{
		printf("WRITE-ERROR: Couldn't access file structure\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that write operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Allocate temporary memory to store the file name */
	pu1_file_name = (UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

	if(pu1_file_name == NULL)
	{
		printf("WRITE-ERROR: Couldn't obtain file name\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that write operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Read file name from Shared Memory */
	get_shrd_mem_fstruct_fname(pu1_fstruct, pu1_file_name);

	/* Lookup file name in file memory */
	u2_fidx = get_fmem_inode_fidx_fname(pu1_file_name);

	if(u2_fidx == CS_FSTRUCT_INVALID_FIDX)
	{
		printf("WRITE-ERROR: Couldn't find file name in INDOE\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that write operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Backup file data */
	{
		/* Get backup pointer */
		UWORD8 *pu1_backup_loc = g_u1_backup[u2_fidx];

		/* Get File memory pointer */
		UWORD8 *pu1_fmem_fstruct = get_fmem_fstruct(u2_fidx);

		/* Reset Backup for current file */
		memset(pu1_backup_loc, 0, CS_FSTRUCT_ENTRY_MAX_SIZE);

		/* Copy current file data to backup array */
		memcpy(pu1_backup_loc, pu1_fmem_fstruct, CS_FSTRUCT_ENTRY_MAX_SIZE);
	}

#ifdef THREAD_LOCKING_TEST
	if((u1_oprtn_num % 2) == 0)
	{
		sleep(15);
	}
#endif /* THREAD_LOCKING_TEST */

	/* Get File size from shared memory */
	u2_fsize = get_shrd_mem_fstruct_fsize(pu1_fstruct);

	/* Allocate temporary Variable from File data */
	pu1_fdata = (UWORD8 *) calloc(1, CS_SHRD_MEM_MAX_FILE_SIZE);

	/* Get File Data from shared memory */
	get_shrd_mem_fstruct_fdata(pu1_fstruct, pu1_fdata);

#ifdef FAULTY_OPERATIONS
	if((u2_fsize & (1 << 14)) != 0)
	{
		/* Create a temporary variable to store the size */
		UWORD16 u2_temp_fsize = u2_fsize;

		/* Reset BIT14 in the size */
		u2_temp_fsize &= ~(1 << 14);

		/* Set File size in File memory */
		set_fmem_fstruct_fsize_fname(pu1_file_name, u2_temp_fsize);
	}
	else
	{
		/* Set File size in File memory */
		set_fmem_fstruct_fsize_fname(pu1_file_name, u2_fsize);
	}
#else /* FAULTY_OPERATIONS */
	/* Set File size in File memory */
	set_fmem_fstruct_fsize_fname(pu1_file_name, u2_fsize);
#endif /* FAULTY_OPERATIONS */

	/* Reset Logs */
	g_u1_css_log[u2_fidx][CS_LOG_START_IDX] = 0;
	g_u1_css_log[u2_fidx][CS_LOG_END_IDX]   = 0;

	/* Log the write start */
	g_u1_css_log[u2_fidx][CS_LOG_START_IDX] = 1;

	/* Set File Data in File memory */
	set_fmem_fstruct_fdata_fname(pu1_file_name, pu1_fdata);

#ifdef FAULTY_OPERATIONS
	{
		/* Check if faulty write has been made by checking BIT14 of file */
		/* size. If it isn't a faulty write, add the end log as well. if */
		/* not, don't add the end log. The recover procedure should      */
		/* detect this while doing the next read and restore the previous*/
		/* backup.                                                       */
		if((u2_fsize & (1 << 14)) == 0)
			g_u1_css_log[u2_fidx][CS_LOG_END_IDX] = 1;
	}
#else /* FAULTY_OPERATIONS */
	/* Log the write end */
	g_u1_css_log[u2_fidx][CS_LOG_END_IDX] = 1;
#endif /* FAULTY_OPERATIONS */

	/* Release the write lock to indicate write completion */
	pthread_mutex_unlock(&g_l_write_lock);

	/* Signal Status semaphore that write operation is complete */
	signal_status_semaphore(u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a read itself doesn't mandate    */
	/* return anything. So, NULL is being returned                           */
	return NULL;
}

/*****************************************************************************/
/* This function looks up the file structure memory and creates an inode     */
/* entry for the file in the inode.                                          */
/*****************************************************************************/

void *cs_allocate(void *pv_oprtn_num)
{
	UWORD8  u1_oprtn_num   = *((UWORD8 *)pv_oprtn_num);
	UWORD8  *pu1_fstruct   = NULL;
	UWORD8  *pu1_file_name = NULL;
	UWORD16  u2_fidx       = 0;
	UWORD16  u2_free_fidx  = 0;

	printf("INFO: Allocate Attempted - Operation Number: %d\n", u1_oprtn_num);

	/* Wait for status semaphore to indicate that a alloc can be performed */
	wait_on_status_semaphore(u1_oprtn_num);

	/* If either the shared memory region or the file structure region have  */
	/* not been allocated, return.                                           */
	if(g_pu1_fstruct_mem == NULL || g_pu1_shrd_mem == NULL)
	{
		printf("ALLOC-ERROR: CSS might not have initialized properly. Consider re-starting\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that alloc operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Get file struct from shared memory */
	pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);

	/* Allocate memory for file name */
	pu1_file_name = (UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

	if(pu1_file_name == NULL)
	{
		printf("ALLOC-ERROR: Couldn't allocate memory for file name\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that alloc operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Get File name from shared memory */
	get_shrd_mem_fstruct_fname(pu1_fstruct, pu1_file_name);

	/* Look up file name in file memory */
	u2_fidx = get_fmem_inode_fidx_fname(pu1_file_name);

	/* If file found in file memory, return */
	if(u2_fidx != CS_FSTRUCT_INVALID_FIDX)
	{
		if(check_fmem_inode_used_idx(u2_fidx) == TRUE)
		{
			printf("ALLOC-ERROR: Trying to allocate a file that is alredy present\n");

			/* Corrupt the file name in shared memory to indicate failure status */
			corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

			/* Signal Status semaphore that alloc operation is complete */
			signal_status_semaphore(u1_oprtn_num);
		}

		return NULL;
	}

	/* Get first free inode in file memory for the file name */
	u2_free_fidx = get_fmem_inode_free_fidx();

	/* If an invalid file index is found, return */
	if(u2_free_fidx == CS_FSTRUCT_INVALID_FIDX)
	{
		printf("ALLOC-ERROR: Couldn't allocate an entry for the file\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that alloc operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Write file name in inode */
	set_fmem_inode_fname_fidx(u2_free_fidx, pu1_file_name);

	/* Write file usage status */
	set_fmem_inode_use_idx(u2_free_fidx, TRUE);

	/* Signal Status semaphore that alloc operation is complete */
	signal_status_semaphore(u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a read itself doesn't mandate    */
	/* return anything. So, NULL is being returned                           */
	return NULL;

}

/*****************************************************************************/
/* This function looks up the file structure memory for the file name given  */
/* in the shared memory and resets the file address in the inode entry to    */
/* ensure de-allocation.                                                     */
/*****************************************************************************/

void *cs_deallocate(void *pv_oprtn_num)
{
	UWORD8  u1_oprtn_num   = *((UWORD8 *)pv_oprtn_num);
	UWORD8  *pu1_fstruct   = NULL;
	UWORD8  *pu1_file_name = NULL;
	UWORD16  u2_fidx       = 0;

	printf("INFO: Allocate Attempted - Operation Number:%d", u1_oprtn_num);

	/* Wait for status semaphore to indicate that a de-aloc can be performed */
	wait_on_status_semaphore(u1_oprtn_num);

	/* Acquire a lock before performing de-allocation */
	pthread_mutex_lock(&g_l_write_lock);

	/* If either the shared memory region or the file structure region have  */
	/* not been allocated, return.                                           */
	if(g_pu1_fstruct_mem == NULL || g_pu1_shrd_mem == NULL)
	{
		printf("DEALLOC-ERROR: CSS might not have initialized properly. Consider re-starting\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that de-alloc operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Get file struct from shared memory */
	pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);

	/* Allocate memory for file name */
	pu1_file_name = (UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

	if(pu1_file_name == NULL)
	{
		printf("DEALLOC-ERROR: Couldn't allocate memory for file name\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that de-alloc operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Get File name from shared memory */
	get_shrd_mem_fstruct_fname(pu1_fstruct, pu1_file_name);

	/* Look up file name in file memory */
	u2_fidx = get_fmem_inode_fidx_fname(pu1_file_name);

	/* If file not found in file memory, return */
	if(u2_fidx == CS_FSTRUCT_INVALID_FIDX)
	{
		printf("DEALLOC-ERROR: Trying to de-allocate a file that is not present\n");

		/* Corrupt the file name in shared memory to indicate failure status */
		corrupt_shrd_mem_fstruct_fname(u1_oprtn_num);

		/* Signal Status semaphore that de-alloc operation is complete */
		signal_status_semaphore(u1_oprtn_num);

		return NULL;
	}

	/* Set the used status to False which marks the completion of de-allocate*/
	set_fmem_inode_use_idx(u2_fidx, FALSE);

	/* Release the lock to mark de-allocation completion */
	pthread_mutex_unlock(&g_l_write_lock);

	/* Signal Status semaphore that de-alloc operation is complete */
	signal_status_semaphore(u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a read itself doesn't mandate    */
	/* return anything. So, NULL is being returned                           */
	return NULL;
}

/*****************************************************************************/
/* This function initializes the shared memory structure that will be used   */
/* between the cell storage system and the Journal storage manager.          */
/*****************************************************************************/

void initialize_shared_memory()
{
	/* The shared memory size is chosen keeping in mind the number of files */
	/* the file metadata to be stored and so on. The details are descried in*/
	/* design document.                                                     */
	UWORD32 u4_shrd_mem_size = CS_SHRD_MEM_MAX_SIZE;
	g_i4_shrd_mem_id         = shmget(IPC_PRIVATE, u4_shrd_mem_size,
								      IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	/* If shared memory allocation failed, return */
	if(g_i4_shrd_mem_id == -1)
		return;

	/* Obtain a pointer to the shared memory region and store it in a global */
	/* pointer.                                                              */
	g_pu1_shrd_mem = (UWORD8 *)shmat(g_i4_shrd_mem_id, 0, 0);

	/* Reset the new shared memory that has been allocated with 0 */
	memset(g_pu1_shrd_mem, 0, u4_shrd_mem_size);

	/* Set the status of operation to be performed to INIT */
	set_shrd_mem_oprtn_type(CS_OPERTN_INIT);
}
/*****************************************************************************/
/* This function initializes the file structure memory that the cell storage */
/* system internally maintains.                                              */
/*****************************************************************************/

WORD32 initialize_file_structure_memory()
{
	WORD32 i4_ret_val = -1;

	/* Allocate memory to store the file structure */
	g_pu1_fstruct_mem = (UWORD8 *) calloc(1, CS_FSTRUCT_MAX_SIZE);

	/* If allocation was not a success, return */
	if(g_pu1_fstruct_mem == NULL)
		return i4_ret_val;

	/* Mark the return status as a success */
	i4_ret_val = 0;

	return i4_ret_val;
}

/*****************************************************************************/
/* This function initializes the operation status semaphore shared between   */
/* Journal storage manager and the cell storage system.                      */
/*****************************************************************************/

void initialize_operation_status_semaphores()
{
	/* Semaphore ID */
	WORD32 i4_sem_id = -1;
	UWORD8 u1_i      = 0;

	/* Declare a semun union to initialize the semaphores */
	union semun {
		WORD32          val;
		struct semid_ds *buf;
		ushort          *array;
	} u_argument;

	/* Set the initial value to be set in the semaphore */
	u_argument.val = STATUS_OPERTN_SEM_INIT_VALUE;

	/* Get new semaphore */
	i4_sem_id = semget((g_i4_shrd_mem_id + 2), STATUS_OPRTN_SEM_NUM, 0666 |
					    IPC_CREAT);

	/* If semaphore couldn't be obtained, return */
	if(i4_sem_id < 0)
	{
		printf("INIT-ERROR: Unable to obtain status semaphore.\n");
		return;
	}

	for(u1_i = 0; u1_i < STATUS_OPRTN_SEM_NUM; u1_i++)
	{
		/* Set initial value of semaphore */
		if(semctl(i4_sem_id, u1_i, SETVAL, u_argument) < 0)
			printf("INIT-ERROR: Status Semaphore initializing failed\n");
		else
			printf("Status Semaphore No %d, key = %d initialized.\n",
					u1_i, (g_i4_shrd_mem_id + 2));
	}
}

/*****************************************************************************/
/* This function initializes the operation start semaphore shared between the*/
/* Journal storage manager and the cell storage system.                      */
/*****************************************************************************/

void initialize_operation_start_semaphore()
{
	/* Semaphore ID */
	WORD32 i4_sem_id = -1;

	/* Declare a semun union to initialize the semaphores */
	union semun {
		WORD32          val;
		struct semid_ds *buf;
		ushort          *array;
	} u_argument;

	/* Set the initial value to be set in the semaphore */
	u_argument.val = INIT_OPERTN_SEM_INIT_VALUE;

	/* Get new semaphore */
	i4_sem_id = semget((g_i4_shrd_mem_id + 1), INIT_OPRTN_SEM_NUM, 0666 | IPC_CREAT);

	/* If semaphore couldn't be obtained, return */
	if(i4_sem_id < 0)
	{
		printf("INIT-ERROR: Unable to obtain start semaphore.\n");
		return;
	}

	/* Set initial value of semaphore */
	if(semctl(i4_sem_id, (INIT_OPRTN_SEM_NUM - 1), SETVAL, u_argument) < 0)
		printf("INIT-ERROR: Start Semaphore initializing failed\n");
	else
		printf("Start Semaphore, key = %d initialized.\n", (g_i4_shrd_mem_id + 1));
}

/*****************************************************************************/
/* This function initializes the shared semaphores between the journal strage*/
/* manager and the cell storage system.                                      */
/*****************************************************************************/

void intialize_semaphores()
{
	/* Initilize the operation start semaphore */
	initialize_operation_start_semaphore();

	/* Initialize the operation status semaphores */
	initialize_operation_status_semaphores();
}

/*****************************************************************************/
/* This function initializes the shared memory and the file structure for the*/
/* cell storage system.                                                      */
/*****************************************************************************/

void initialize_cell_storage_system()
{
	/* Set all the statuses to failure initially */
	WORD32 i4_fstruct_stat   = -1;

	/* Initialize the shared memory */
	initialize_shared_memory();

	/* If shared memory allocation failed, return */
	if(g_i4_shrd_mem_id == -1)
		return;

	/* Initialize the file structure memory */
	i4_fstruct_stat = initialize_file_structure_memory();

	if(i4_fstruct_stat == -1)
	{
		g_i4_shrd_mem_id = i4_fstruct_stat;
		return;
	}

	/* Intialize all the semaphores */
	intialize_semaphores();
}

/*****************************************************************************/
/* This is the entry point to execution of the cell storage system.          */
/*****************************************************************************/

WORD32 main()
{
	/* Initialize the cell storage system */
	initialize_cell_storage_system();

	/* If the shared memory ID returned is a -1, the initialization failed */
	/* Hence, return with an error message                                 */
	if(g_i4_shrd_mem_id == -1)
	{
		printf("ERROR: Initialization of the cell storage system failed\n");
		return 0;
	}

	printf("Cell storage system ID is: %d\n", g_i4_shrd_mem_id);
	printf("Please keep note of this ID. The Journal storage manager");
	printf("\nwill need this as an input to access the cell storage");
	printf("\nsystem\n");

	while(1)
	{
		UWORD8 u1_oprtn_requested = 0;
		UWORD8 u1_oprtn_num 	  = 0;

		/* Wait on start semaphore */
		wait_on_start_semaphore();

		/* Extract operation requested */
		u1_oprtn_requested = get_shrd_mem_oprtn_type();

		/* Extract the operation number */
		u1_oprtn_num = get_shrd_mem_oprtn_num();

		/* Perform a Read operation */
		if(u1_oprtn_requested == CS_OPERTN_READ)
		{
			pthread_t       t_read_thread;
			UWORD8 *pu1_oprtn_num     = calloc(1, 1);
			pu1_oprtn_num[0] = u1_oprtn_num;

			/* Create a Read Thread and start execution of the same. */
			/* Here since the main function will run in an eternal   */
			/* while until a termination signal from the user is not */
			/* detected, the main thread will not wait until the     */
			/* created thread has been started.                      */
			if(pthread_create(&t_read_thread, NULL,
							  cs_read, (void *)(pu1_oprtn_num)))
			{
				printf("Error: Could not perform a read");
			}
		}
		/* Perform a Write operation */
		else if(u1_oprtn_requested == CS_OPERTN_WRITE)
		{
			pthread_t       t_write_thread;
			UWORD8 *pu1_oprtn_num     = calloc(1, 1);
			pu1_oprtn_num[0] = u1_oprtn_num;

			/* Create a Write Thread and start execution of the same.*/
			/* Here since the main function will run in an eternal   */
			/* while until a termination signal from the user is not */
			/* detected, the main thread will not wait until the     */
			/* created thread has been started.                      */
			if(pthread_create(&t_write_thread, NULL,
							  cs_write, (void *)(pu1_oprtn_num)))
			{
				printf("Error: Could not perform a write");
			}
		}
		/* Perform an allocation operation */
		else if(u1_oprtn_requested == CS_OPERTN_ALLOC)
		{
			pthread_t       t_alloc_thread;
			UWORD8 *pu1_oprtn_num     = calloc(1, 1);
			pu1_oprtn_num[0] = u1_oprtn_num;

			/* Create a Alloc Thread and start execution of the same.*/
			/* Here since the main function will run in an eternal   */
			/* while until a termination signal from the user is not */
			/* detected, the main thread will not wait until the     */
			/* created thread has been started.                      */
			if(pthread_create(&t_alloc_thread, NULL,
							  cs_allocate, (void *)(pu1_oprtn_num)))
			{
				printf("Error: Could not perform an allocate");
			}

		}
		/* Perform a de-allocation operation */
		else if(u1_oprtn_requested == CS_OPERTN_DALOC)
		{
			pthread_t       t_dealloc_thread;
			UWORD8 *pu1_oprtn_num     = calloc(1, 1);
			pu1_oprtn_num[0] = u1_oprtn_num;

			/* Create a de-alloc Thread and start execution of the   */
			/* same. Here since the main function will run in an     */
			/* eternal while until a termination signal from the user*/
			/* is not detected, the main thread will not wait until  */
			/* the created thread has been started.                  */
			if(pthread_create(&t_dealloc_thread, NULL,
							  cs_deallocate, (void *)(pu1_oprtn_num)))
			{
				printf("Error: Could not perform a de-allocate\n");
			}
		}
		else if(u1_oprtn_requested != CS_OPERTN_INIT)
		{
			printf("Error: Cell Storage system recognized\n");
			printf(" undefined operation, State: %d \n", u1_oprtn_requested);
		}

		set_shrd_mem_oprtn_type(CS_OPERTN_INIT);

		/* Signal through the semaphore that the operation has been initiated*/
		signal_start_semaphore();
	}

	return 0;
}


