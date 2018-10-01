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
/* This file contains the functionality for journal storage manager 		 */
/*																			 */
/*****************************************************************************/

/*****************************************************************************/
/* File Includes                                                             */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "journal_storage_manager.h"

/*****************************************************************************/
/* Global Variable Declarations                                              */
/*****************************************************************************/
sem_t  *g_ps_semaphore 		    = NULL;
UWORD8 *g_pu1_shrd_mem 		    = 0;
UWORD8  g_u1_first_oprtn_reqstd = 1;
WORD32  g_i4_shrd_mem_id        = 0;

/*****************************************************************************/
/* Function definitions                                                      */
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
/* This function signals the start semaphore provided the key for the       */
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
/* This function initiates a read operation by writing the appropriate       */
/* location in shared memory. It also takes care of waiting on semaphores    */
/* before access to the shared memory region is achieved.                    */
/*****************************************************************************/

void *initiate_cs_read(void *pv_input_data)
{
	read_input_t s_input_data       = *((read_input_t *)(pv_input_data));
	UWORD8  	 u1_curr_oprtn_req  = 0;
	UWORD8  	 u1_oprtn_num       = 0xFF;
	UWORD8  	 *pu1_fstruct       = NULL;
	UWORD8       *pu1_fdata         = NULL;
	UWORD16 	 u2_fsize           = 0;

	/* If the first operation hasn't been requested, then this is the */
	/* first operation. Set it to indicate that first operation has   */
	/* been requested.                                                */
	if(g_u1_first_oprtn_reqstd == 1)
		g_u1_first_oprtn_reqstd = 0;
	else if(g_u1_first_oprtn_reqstd == 0)
		wait_on_start_semaphore();

	/* Get current operation requested status */
	u1_curr_oprtn_req = get_shrd_mem_oprtn_type();

	/* if current operation request is not in INIT state */
	if(u1_curr_oprtn_req != CS_OPERTN_INIT)
	{
		printf("BUG: Got Initiate access. But, someone else also has access\n");
		return NULL;
	}

	/* Find the operation number that is free */
	u1_oprtn_num   = find_free_oprtn_num();

	/* If a free operation number couldn't be found, return */
	if(u1_oprtn_num == CS_FSTRUCT_INVALID_FOPRTN_NUM)
	{
		printf("Couldn't initiate a read request\n");
		return NULL;
	}

	/* Get Current Bitmap and set the operation number as being */
	/* used.                                                    */
	set_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Set current operation number */
	set_shrd_mem_oprtn_num(u1_oprtn_num);

	/* Initiate a read */
	set_shrd_mem_oprtn_type(CS_OPERTN_READ);

	/* Signal through the semaphore that the Read has been initiated */
	signal_start_semaphore();

	/* Read the file struct from shared memory */
	pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);

	/* Write the file name in shared memory */
	set_shrd_mem_fstruct_fname(pu1_fstruct, s_input_data.pu1_file_name);

	sleep(1);

	/* Signal Status semaphore to start the read process */
	signal_status_semaphore(u1_oprtn_num);

	/* Wait for the read to complete by waiting on the status semaphore */
	wait_on_status_semaphore(u1_oprtn_num);

	/* Once, the wait is over, complete the read procedure as follows */
	{
		UWORD8 pu1_check_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0xFF};
		UWORD8 *pu1_curr_fname = (UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

		/* Get current file name */
		get_shrd_mem_fstruct_fname(pu1_fstruct, pu1_curr_fname);

		/* Check if the file name has been corrupted */
		if(memcmp(pu1_check_fname, pu1_curr_fname,
				  CS_SHRD_MEM_FILE_NAME_FIELD_LEN) == 0)
		{
			printf("Operation Num: %d | Read Failed. Check CSS logs for details\n", u1_oprtn_num);
		}
		else
		{
			printf("Operation Num: %d | Read successful, (Hit Enter if you don't see the data)\
					\nFile data is as follows:\n", u1_oprtn_num);

			/* Read the file size */
			u2_fsize = get_shrd_mem_fstruct_fsize(pu1_fstruct);

			/* Allocate a temporary buffer to store the file data */
			pu1_fdata = (UWORD8 *) calloc(1, CS_SHRD_MEM_MAX_FILE_SIZE);

			/* Read the file data and print it out to the console byte by byte */
			get_shrd_mem_fstruct_fdata(pu1_fstruct, pu1_fdata);

			/* Print File data */
			{
				UWORD16 u2_i = 0;

				/* Traverse through the obtained data and print it out one char */
				/* at a time.                                                   */
				for(u2_i = 0; u2_i < u2_fsize; u2_i++)
				{
					if(((u2_i % 64) == 0) && (u2_i != 0))
						printf("\n");

					printf("%c", pu1_fdata[u2_i]);
				}
			}
		}
	}

	reset_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a read itself doesn't mandate    */
	/* return anything. So, NULL is being returned                           */
	return NULL;
}

/*****************************************************************************/
/* This function initiates a write operation by writing the appropriate  	 */
/* location in shared memory. It also takes care of waiting on semaphores    */
/* before access to the shared memory region is achieved.                    */
/*****************************************************************************/

void *initiate_cs_write(void *pv_input_data)
{
	write_input_t s_input_data       = *((write_input_t *)(pv_input_data));
	UWORD8  	  u1_curr_oprtn_req  = 0;
	UWORD8  	  u1_oprtn_num       = 0xFF;
	UWORD8  	  *pu1_fstruct       = NULL;

	/* If the first operation hasn't been requested, then this is the */
	/* first operation. Set it to indicate that first operation has   */
	/* been requested.                                                */
	if(g_u1_first_oprtn_reqstd == 1)
		g_u1_first_oprtn_reqstd = 0;
	else if(g_u1_first_oprtn_reqstd == 0)
		wait_on_start_semaphore();

	/* Get current operation requested status */
	u1_curr_oprtn_req = get_shrd_mem_oprtn_type();

	/* if current operation request is not in INIT state */
	if(u1_curr_oprtn_req != CS_OPERTN_INIT)
	{
		printf("BUG: Got Initiate access. But, someone else also has access\n");
		return NULL;
	}

	/* Find the operation number that is free */
	u1_oprtn_num   = find_free_oprtn_num();

	/* If a free operation number couldn't be found, return */
	if(u1_oprtn_num == CS_FSTRUCT_INVALID_FOPRTN_NUM)
	{
		printf("Couldn't initiate a write request, try again later\n");
		return NULL;
	}

	/* Get Current Bitmap and set the operation number as being */
	/* used.                                                    */
	set_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Set current operation number */
	set_shrd_mem_oprtn_num(u1_oprtn_num);

	/* Read the file struct from shared memory */
	pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);

	/* Write the file name in shared memory */
	set_shrd_mem_fstruct_fname(pu1_fstruct, s_input_data.pu1_file_name);

	/* Write the file sizein shared memory */
	/* Since the write operation only writes the request to shared     */
	/* memory and doesn't initiate a write request to the cell storage */
	/* system, the commit operation will initiate the write request to */
	/* CSS. In order for the commit operation to understand that there */
	/* was a write that was previously initiated, we need to somehow   */
	/* indicate to the commit operation that this operation is a write */
	/* In order to do this, we set BIT15 of file size in shared memory */
	/* fstruct because a valid fsize can never go beyond 512 bytes     */
	s_input_data.u2_file_size |= (1 << 15);

	/* Write the file sizein shared memory */
	set_shrd_mem_fstruct_fsize(pu1_fstruct, s_input_data.u2_file_size);

	/* Write the file data in shared memory */
	set_shrd_mem_fstruct_fdata(pu1_fstruct, s_input_data.pu1_file_data);

	/* Signal the start semaphore */
	signal_start_semaphore();

	printf("Operation Num: %d | Write successful\n", u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a write itself doesn't mandate   */
	/* return anything. So, NULL is being returned                           */
	return NULL;
}

/*****************************************************************************/
/* This function initiates a commit operation by writing the appropriate  	 */
/* location in shared memory. It also takes care of waiting on semaphores    */
/* before access to the shared memory region is achieved.                    */
/*****************************************************************************/

void *initiate_cs_commit(void *pv_input_data)
{
	commit_input_t s_input_data       = *((commit_input_t *)(pv_input_data));
	UWORD8  	  u1_curr_oprtn_req  = 0;
	UWORD8  	  u1_oprtn_num       = 0xFF;
#ifdef FAULTY_OPERATIONS
	UWORD8        u1_temp_oprtn_num  = 0xFF;
#endif /* FAULTY_OPERATIONS */
	UWORD8  	  *pu1_fstruct       = NULL;
	UWORD16       u2_fsize           = 0;

	/* If the first operation hasn't been requested, then this is the */
	/* first operation. Set it to indicate that first operation has   */
	/* been requested.                                                */
	if(g_u1_first_oprtn_reqstd == 1)
		g_u1_first_oprtn_reqstd = 0;
	else if(g_u1_first_oprtn_reqstd == 0)
		wait_on_start_semaphore();

	/* Get current operation requested status */
	u1_curr_oprtn_req = get_shrd_mem_oprtn_type();

	/* if current operation request is not in INIT state */
	if(u1_curr_oprtn_req != CS_OPERTN_INIT)
	{
		printf("BUG: Got Initiate access. But, someone else also has access\n");
		return NULL;
	}

	/* Read the operation number to be committed */
	u1_oprtn_num = s_input_data.u1_op_num;

#ifdef FAULTY_OPERATIONS
	/* Store the operation number temporarily */
	u1_temp_oprtn_num = u1_oprtn_num;

	if((u1_oprtn_num & (1 << 7)) != 0)
	{
		/* Reset BIT7 in operation  number */
		u1_oprtn_num &= ~(1 << 7);
	}
#endif /* FAULTY_OPERATIONS */

	/* If a free operation number couldn't be found, return */
	if(u1_oprtn_num == CS_FSTRUCT_INVALID_FOPRTN_NUM)
	{
		printf("Couldn't initiate a commit request, try again later\n");
		return NULL;
	}

	/* Read the file struct from shared memory */
	pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);

	/* Check if the operation requested to commit is a write operation by  */
	/* checking BIT15 of the file size.                                    */
	u2_fsize = get_shrd_mem_fstruct_fsize(pu1_fstruct);

	/* Return an error message if a write operation isn't found */
	if((u2_fsize & (1 << 15)) == 0)
	{
		printf("Couldn't find a write operation to commit\n");
		return NULL;
	}

	/* Reset BIT15 and set the size in shared memory before you initiate */
	/* the commit operation 											 */
	u2_fsize &= ~(1 << 15);

#ifdef FAULTY_OPERATIONS
	/* If BIT7 in operation number is set, set BIT14 to trigger a faulty */
	/* commit.                                                           */
	if((u1_temp_oprtn_num & (1 << 7)) != 0)
	{
		/* set BIT14 in fsize */
		u2_fsize |= (1 << 14);
	}
#endif /* FAULTY_OPERATIONS */

	set_shrd_mem_fstruct_fsize(pu1_fstruct, u2_fsize);

	/* Set current operation number */
	set_shrd_mem_oprtn_num(u1_oprtn_num);

	/* Initiate a write */
	set_shrd_mem_oprtn_type(CS_OPERTN_WRITE);

	/* Signal through the semaphore that the write has been initiated */
	signal_start_semaphore();

	sleep(1);

	/* Signal Status semaphore to start the write process */
	signal_status_semaphore(u1_oprtn_num);

	/* Wait for the read to complete by waiting on the status semaphore */
	wait_on_status_semaphore(u1_oprtn_num);

	/* Once, the wait is over, complete the write procedure as follows */
	{
		UWORD8 pu1_check_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0xFF};
		UWORD8 *pu1_curr_fname = (UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

		/* Get current file name */
		get_shrd_mem_fstruct_fname(pu1_fstruct, pu1_curr_fname);

		/* Check if the file name has been corrupted */
		if(memcmp(pu1_check_fname, pu1_curr_fname,
				  CS_SHRD_MEM_FILE_NAME_FIELD_LEN) == 0)
		{
			printf("Operation Num: %d | Commit Failed. Check CSS logs for details\n", u1_oprtn_num);
		}
		else
		{
			printf("Operation Num: %d | Commit successful\n", u1_oprtn_num);
		}
	}

	reset_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a write itself doesn't mandate   */
	/* return anything. So, NULL is being returned                           */
	return NULL;
}


/*****************************************************************************/
/* This function initiates an allocate operation by writing the appropriate  */
/* location in shared memory. It also takes care of waiting on semaphores    */
/* before access to the shared memory region is achieved.                    */
/*****************************************************************************/

void *initiate_cs_alloc(void *pv_input_data)
{
	alloc_input_t s_input_data       = *((alloc_input_t *)(pv_input_data));
	UWORD8  	  u1_curr_oprtn_req  = 0;
	UWORD8  	  u1_oprtn_num       = 0xFF;
	UWORD8  	  *pu1_fstruct       = NULL;

	/* If the first operation hasn't been requested, then this is the */
	/* first operation. Set it to indicate that first operation has   */
	/* been requested.                                                */
	if(g_u1_first_oprtn_reqstd == 1)
		g_u1_first_oprtn_reqstd = 0;
	else if(g_u1_first_oprtn_reqstd == 0)
		wait_on_start_semaphore();

	/* Get current operation requested status */
	u1_curr_oprtn_req = get_shrd_mem_oprtn_type();

	/* if current operation request is not in INIT state */
	if(u1_curr_oprtn_req != CS_OPERTN_INIT)
	{
		printf("BUG: Got Initiate access. But, someone else also has access\n");
		return NULL;
	}

	/* Find the operation number that is free */
	u1_oprtn_num   = find_free_oprtn_num();

	/* If a free operation number couldn't be found, return */
	if(u1_oprtn_num == CS_FSTRUCT_INVALID_FOPRTN_NUM)
	{
		printf("Couldn't initiate an alloc request, try again later\n");
		return NULL;
	}

	/* Get Current Bitmap and set the operation number as being */
	/* used.                                                    */
	set_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Set current operation number */
	set_shrd_mem_oprtn_num(u1_oprtn_num);

	/* Initiate an allloc */
	set_shrd_mem_oprtn_type(CS_OPERTN_ALLOC);

	/* Signal through the semaphore that the alloc has been initiated */
	signal_start_semaphore();

	/* Read the file struct from shared memory */
	pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);

	/* Write the file name in shared memory */
	set_shrd_mem_fstruct_fname(pu1_fstruct, s_input_data.pu1_file_name);

	sleep(1);

	/* Signal Status semaphore to start the read process */
	signal_status_semaphore(u1_oprtn_num);

	/* Wait for the read to complete by waiting on the status semaphore */
	wait_on_status_semaphore(u1_oprtn_num);

	/* Once, the wait is over, complete the alloc procedure as follows */
	{
		UWORD8 pu1_check_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0xFF};

		UWORD8 *pu1_curr_fname =
					(UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

		/* Get current file name */
		get_shrd_mem_fstruct_fname(pu1_fstruct, pu1_curr_fname);

		/* Check if the file name has been corrupted */
		if(memcmp(pu1_check_fname, pu1_curr_fname,
				  CS_SHRD_MEM_FILE_NAME_FIELD_LEN) == 0)
		{
			printf("Operation Num: %d | Alloc Failed. Check CSS logs for details\n", u1_oprtn_num);
		}
		else
		{
			printf("Operation Num: %d | Alloc successful\n", u1_oprtn_num);
		}
	}

	reset_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a read itself doesn't mandate    */
	/* return anything. So, NULL is being returned                           */
	return NULL;
}

/*****************************************************************************/
/* This function initiates an deallocate operation by writing the appropriate*/
/* location in shared memory. It also takes care of waiting on semaphores    */
/* before access to the shared memory region is achieved.                    */
/*****************************************************************************/

void *initiate_cs_dealloc(void *pv_input_data)
{
	dealloc_input_t s_input_data       = *((dealloc_input_t *)(pv_input_data));
	UWORD8  	    u1_curr_oprtn_req  = 0;
	UWORD8  	    u1_oprtn_num       = 0xFF;
	UWORD8  	    *pu1_fstruct       = NULL;

	/* If the first operation hasn't been requested, then this is the */
	/* first operation. Set it to indicate that first operation has   */
	/* been requested.                                                */
	if(g_u1_first_oprtn_reqstd == 1)
		g_u1_first_oprtn_reqstd = 0;
	else if(g_u1_first_oprtn_reqstd == 0)
		wait_on_start_semaphore();

	/* Get current operation requested status */
	u1_curr_oprtn_req = get_shrd_mem_oprtn_type();

	/* if current operation request is not in INIT state */
	if(u1_curr_oprtn_req != CS_OPERTN_INIT)
	{
		printf("BUG: Got Initiate access. But, someone else also has access\n");
		return NULL;
	}

	/* Find the operation number that is free */
	u1_oprtn_num   = find_free_oprtn_num();

	/* If a free operation number couldn't be found, return */
	if(u1_oprtn_num == CS_FSTRUCT_INVALID_FOPRTN_NUM)
	{
		printf("Couldn't initiate a de-alloc request, try again later\n");
		return NULL;
	}

	/* Get Current Bitmap and set the operation number as being */
	/* used.                                                    */
	set_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Set current operation number */
	set_shrd_mem_oprtn_num(u1_oprtn_num);

	/* Initiate a DeAlloc */
	set_shrd_mem_oprtn_type(CS_OPERTN_DALOC);

	/* Signal through the semaphore that the alloc has been initiated */
	signal_start_semaphore();

	/* Read the file struct from shared memory */
	pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);

	/* Write the file name in shared memory */
	set_shrd_mem_fstruct_fname(pu1_fstruct, s_input_data.pu1_file_name);

	sleep(1);

	/* Signal Status semaphore to start the read process */
	signal_status_semaphore(u1_oprtn_num);

	/* Wait for the read to complete by waiting on the status semaphore */
	wait_on_status_semaphore(u1_oprtn_num);

	/* Once, the wait is over, complete the alloc procedure as follows */
	{
		UWORD8 pu1_check_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0xFF};
		UWORD8 *pu1_curr_fname = (UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

		/* Get current file name */
		get_shrd_mem_fstruct_fname(pu1_fstruct, pu1_curr_fname);

		/* Check if the file name has been corrupted */
		if(memcmp(pu1_check_fname, pu1_curr_fname,
				  CS_SHRD_MEM_FILE_NAME_FIELD_LEN) == 0)
		{
			printf("Operation Num: %d | DeAlloc Failed. Check CSS logs for details\n", u1_oprtn_num);
		}
		else
		{
			printf("Operation Num: %d | DeAlloc successful\n", u1_oprtn_num);
		}
	}

	reset_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Since this function will be executed using a pthread, it has to return*/
	/* something. But, the functionality of a read itself doesn't mandate    */
	/* return anything. So, NULL is being returned                           */
	return NULL;
}

/*****************************************************************************/
/* This function initiates an abort operation by writing the appropriate     */
/* location in shared memory. It also takes care of waiting on semaphores    */
/* before access to the shared memory region is achieved.                    */
/*****************************************************************************/

void initiate_cs_abort(void *pv_input_data)
{
	abort_input_t s_input_data      = *((abort_input_t *)(pv_input_data));
	UWORD8  	  u1_curr_oprtn_req	= 0;
	UWORD8  	  u1_oprtn_num      = 0xFF;
	UWORD8  	  *pu1_fstruct     	= NULL;
	UWORD16		  u2_fsize          = 0;

	/* If the first operation hasn't been requested, then this is the */
	/* first operation. Set it to indicate that first operation has   */
	/* been requested.                                                */
	if(g_u1_first_oprtn_reqstd == 1)
		g_u1_first_oprtn_reqstd = 0;
	else if(g_u1_first_oprtn_reqstd == 0)
		wait_on_start_semaphore();

	/* Get current operation requested status */
	u1_curr_oprtn_req = get_shrd_mem_oprtn_type();

	/* if current operation request is not in INIT state */
	if(u1_curr_oprtn_req != CS_OPERTN_INIT)
	{
		printf("BUG: Got Initiate access. But, someone else also has access\n");
		return;
	}

	/* Read the operation number to be committed */
	u1_oprtn_num = s_input_data.u1_op_num;

	/* If a free operation number couldn't be found, return */
	if(u1_oprtn_num == CS_FSTRUCT_INVALID_FOPRTN_NUM)
	{
		printf("Couldn't initiate a write request, try again later\n");
		return;
	}

	/* Read the file struct from shared memory */
	pu1_fstruct = get_shrd_mem_fstruct(u1_oprtn_num);

	/* Check if the operation requested to commit is a write operation by  */
	/* checking BIT15 of the file size.                                    */
	u2_fsize = get_shrd_mem_fstruct_fsize(pu1_fstruct);

	/* Return an error message if a write operation isn't found */
	if((u2_fsize & (1 << 15)) == 0)
	{
		printf("Couldn't find a write operation to abort\n");
		return;
	}

	/* Reset the entire file structure corresponding to the operation number */
	/* requested for abortion.                                               */
	memset(pu1_fstruct, 0, CS_FSTRUCT_ENTRY_MAX_SIZE);

	/* Reset the operation number in the bitmap to indicate that the current */
	/* operation number can now be used. 									 */
	reset_shrd_mem_oprtn_bmap_bit(u1_oprtn_num);

	/* Signal through the semaphore that the abort has been initiated This   */
	/* will not initiate any tasks on the CSS side. It is only to ensure that*/
	/* the wait-signal protocol between the JSM and the CSS are maintained.  */
	signal_start_semaphore();

	printf("Operation Num: %d | Abort successful\n", u1_oprtn_num);
}

/*****************************************************************************/
/* This functions initializes the Journal storage manager.                   */
/*****************************************************************************/

WORD32 initialize_journal_storage_manager()
{
	WORD32 i4_ret_val = -1;

	/* Attach shared memory */
	g_pu1_shrd_mem   = (UWORD8 *) shmat(g_i4_shrd_mem_id, 0, 0);

	/* If accessing shared memory failed, return */
	if(g_pu1_shrd_mem == NULL)
		return i4_ret_val;

	/* Mark that initialization of the journal storage manager succeeded. */
	i4_ret_val = 0;
	return i4_ret_val;
}

/*****************************************************************************/
/* This is the entry point to execution of the journal storage manager.      */
/*****************************************************************************/

int main()
{
	WORD32  i4_init_jrnl_mngr 		= -1;
	UWORD32 u4_input          		= 0;

	printf("=================================================================\n");
	printf("NOTE: Please note that file names and file data are to be entered\
			\nwithout any spaces in them. The program is not written to\
			\nhandle such inputs\n");
	printf("=================================================================\n");

	printf("Enter the Cell Storage System ID:");
	scanf("%d", &g_i4_shrd_mem_id);
	printf("\n");

	i4_init_jrnl_mngr = initialize_journal_storage_manager();

	if(i4_init_jrnl_mngr == -1)
	{
		printf("Error: Failure accessing the shared memory\n");
		return 0;
	}

	while(1)
	{
		printf("What do you want to do?\n");
		printf("1.Allocate 2.De-Allocate 3.Read 4.Write 5.Commit\
				\n6.Faulty Commit 7.Abort 8.Exit\n");
		scanf("%d", &u4_input);

		switch(u4_input)
		{
			/* Perform an allocation */
			case 1:
			{
				char          u1_inp_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0};
				alloc_input_t *ps_input_data = NULL;
				pthread_t     t_alloc_thread;

				ps_input_data = (alloc_input_t *) calloc(1, sizeof(alloc_input_t));

				printf("Enter the file name (Max 28 characters/bytes): ");
				scanf("%s", u1_inp_fname);
				printf("\n");

				/* Set the input data structure parameters */
				ps_input_data->pu1_file_name =
						(UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

				/* Copy the input file name to the input data structure */
				memcpy(ps_input_data->pu1_file_name, u1_inp_fname,
										CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

				/* Create an alloc Thread and start execution of the same. */
				/* Here since the main function will run in an eternal   */
				/* while until a termination signal from the user is not */
				/* detected, the main thread will not wait until the     */
				/* created thread has been started.                      */
				if(pthread_create(&t_alloc_thread, NULL,
							      initiate_cs_alloc, (void *)ps_input_data))
				{
					printf("Error: Could not Initiate an alloc");
				}
			}
			break;

			/* Perform a de-allocation */
			case 2:
			{
				char            u1_inp_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0};
				dealloc_input_t *ps_input_data = NULL;
				pthread_t       t_dealloc_thread;

				ps_input_data = (dealloc_input_t *) calloc(1, sizeof(dealloc_input_t));

				printf("Enter the file name: ");
				scanf("%s", u1_inp_fname);
				printf("\n");

				/* Set the input data structure parameters */
				ps_input_data->pu1_file_name =
						(UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

				/* Copy the input file name to the input data structure */
				memcpy(ps_input_data->pu1_file_name, u1_inp_fname,
										CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

				/* Create a dealloc Thread and start execution of the same.  */
				/* Here since the main function will run in an eternal       */
				/* while until a termination signal from the user is not     */
				/* detected, the main thread will not wait until the         */
				/* created thread has been started.                          */
				if(pthread_create(&t_dealloc_thread, NULL,
							      initiate_cs_dealloc, (void *)ps_input_data))
				{
					printf("Error: Could not Initiate a dealloc");
				}
			}
			break;

			/* Perform a Read */
			case 3:
			{
				char         u1_inp_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0};
				read_input_t *ps_input_data = NULL;
				pthread_t    t_read_thread;

				ps_input_data = (read_input_t *) calloc(1, sizeof(read_input_t));

				printf("Enter the file name: ");
				scanf("%s", u1_inp_fname);
				printf("\n");

				/* Set the input data structure parameters */
				ps_input_data->pu1_file_name =
						(UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

				/* Copy the input file name to the input data structure */
				memcpy(ps_input_data->pu1_file_name, u1_inp_fname,
										CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

				/* Create a Read Thread and start execution of the same. */
				/* Here since the main function will run in an eternal   */
				/* while until a termination signal from the user is not */
				/* detected, the main thread will not wait until the     */
				/* created thread has been started.                      */
				if(pthread_create(&t_read_thread, NULL,
							      initiate_cs_read, (void *)ps_input_data))
				{
					printf("Error: Could not Initiate a read");
				}
			}
			break;

			/* Perform a Write */
			case 4:
			{
				char            u1_inp_fname[CS_SHRD_MEM_FILE_NAME_FIELD_LEN] = {0};
				char            u1_inp_fdata[CS_SHRD_MEM_MAX_FILE_SIZE]       = {0};
				write_input_t   *ps_input_data = NULL;
				pthread_t       t_write_thread;

				ps_input_data = (write_input_t *) calloc(1, sizeof(write_input_t));

				printf("Enter the file name: ");
				scanf("%s", u1_inp_fname);
				printf("\n");

				/* Set the input data structure parameters */
				ps_input_data->pu1_file_name =
						(UWORD8 *) calloc(1, CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

				/* Copy the input file name to the input data structure */
				memcpy(ps_input_data->pu1_file_name, u1_inp_fname,
										CS_SHRD_MEM_FILE_NAME_FIELD_LEN);

				printf("Enter file data (Max 512 characters/bytes): ");
				scanf("%s", u1_inp_fdata);
				printf("\n");

				/* Set the input data structure parameters */
				ps_input_data->pu1_file_data =
						(UWORD8 *) calloc(1, CS_SHRD_MEM_MAX_FILE_SIZE);

				/* Set the size of the file in the input data structure */
				ps_input_data->u2_file_size = strlen(u1_inp_fdata) + 1;

				/* Copy the input file name to the input data structure */
				memcpy(ps_input_data->pu1_file_data, u1_inp_fdata,
										CS_SHRD_MEM_MAX_FILE_SIZE);

				/* Create a write Thread and start execution of the same.  */
				/* Here since the main function will run in an eternal       */
				/* while until a termination signal from the user is not     */
				/* detected, the main thread will not wait until the         */
				/* created thread has been started.                          */
				if(pthread_create(&t_write_thread, NULL,
							      initiate_cs_write, (void *)ps_input_data))
				{
					printf("Error: Could not Initiate a write");
				}
			}
			break;

			/* Perform a commit */
			case 5:
			{
				/* Create the commit input structure */
				pthread_t      t_commit_thread;
				UWORD32        u4_inp_op_num   = 0;
				commit_input_t *ps_input_data  = NULL;

				ps_input_data =
						(commit_input_t *) calloc(1, sizeof(commit_input_t));

				/* Read the operation number to commit */
				printf("Enter operation number to commit: ");
				scanf("%x", &u4_inp_op_num);
				printf("\n");

				ps_input_data->u1_op_num = (UWORD8) (u4_inp_op_num & 0xFF);

				/* Create a commit Thread and start execution of the same.   */
				/* Here since the main function will run in an eternal       */
				/* while until a termination signal from the user is not     */
				/* detected, the main thread will not wait until the         */
				/* created thread has been started.                          */
				if(pthread_create(&t_commit_thread, NULL,
							      initiate_cs_commit, (void *)ps_input_data))
				{
					printf("Error: Could not Initiate a commit");
				}
			}
			break;

#ifdef FAULTY_OPERATIONS
			/* Perform a faulty commit */
			case 6:
			{
				/* Create the commit input structure */
				pthread_t      t_faulty_commit_thread;
				UWORD32        u4_inp_op_num   = 0;
				commit_input_t *ps_faulty_input_data  = NULL;

				ps_faulty_input_data =
						(commit_input_t *) calloc(1, sizeof(commit_input_t));

				/* Read the operation number to commit */
				printf("Enter operation number to commit: ");
				scanf("%x", &u4_inp_op_num);
				printf("\n");

				ps_faulty_input_data->u1_op_num = (UWORD8) (u4_inp_op_num & 0xFF);

				/* Set BIT 7 to trigger a faulty commit */
				ps_faulty_input_data->u1_op_num |= (1 << 7);

				/* Create a commit Thread and start execution of the same.   */
				/* Here since the main function will run in an eternal       */
				/* while until a termination signal from the user is not     */
				/* detected, the main thread will not wait until the         */
				/* created thread has been started.                          */
				if(pthread_create(&t_faulty_commit_thread, NULL,
							      initiate_cs_commit, (void *)ps_faulty_input_data))
				{
					printf("Error: Could not Initiate a commit");
				}
			}
			break;
#endif /* FAULTY_OPERATIONS */

			/* Abort */
			case 7:
			{
				/* Create the abort input structure */
				UWORD32       u4_inp_op_num  = 0;
				abort_input_t *ps_input_data = NULL;
				ps_input_data =
						(abort_input_t *) calloc(1, sizeof(abort_input_t));

				/* Read the operation number to abort */
				printf("Enter operation number to abort: ");
				scanf("%x", &u4_inp_op_num);
				printf("\n");

				ps_input_data->u1_op_num = (UWORD8) (u4_inp_op_num & 0xFF);

				/* Initiate an abort. The abort here isn't initiated as a */
				/* separate thread to ensure that the abort gets priority */
				/* and no other thread can initiate an operation until the*/
				/* abort has completed.                                   */
				initiate_cs_abort((void *)ps_input_data);
			}
			break;

			/* Exit */
			case 8:
			{
				printf("Journal Storage manager is Exiting...\n");
				return 0;
			}
			break;

			default:
			{
				printf("Choose an option between 1 and 5\n");
			}
			break;
		}
	}

	return 0;
}
