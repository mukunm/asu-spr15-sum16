/*****************************************************************************/
/*                                                                           */
/* Author - Mukund Manikarnike                                               */
/* Course - Advances in Databases (CSE 591)                                  */
/* Term   - Spring 2016                                                      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* This file includes APIs to initialize the data store, read/write to it    */
/* and destroy the data store.                                               */
/*****************************************************************************/

/*****************************************************************************/
/* File Includes                                                             */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data_store.h"

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/

/***************************************************************************/
/* The allocated memory is as shown below                                  */
/* With this partition, one can fit 26,188,824 records in this data store  */
/* +--------------+--------------+------------------------+--------------+ */
/* | Data Tuple 1 | Data Tuple 2 |           ...          | Data Tuple n | */
/* +--------------+--------------+------------------------+--------------+ */
/* |    41 bytes  |   41 bytes   |           ...          |    41 bytes  | */
/* +--------------+--------------+------------------------+--------------+ */
/*                                                                         */
/***************************************************************************/
UWORD8 *g_pu1_data_store = NULL;

/* Stores the current index to which a new tuples should be inserted */
UWORD32 g_u4_current_idx = DEFAULT_INDEX;

/*****************************************************************************/
/* This function initializes the data store.                                 */
/* It allocates sufficient amount of memory to store the entire database     */
/* and stores it in the global pointer that can be accessed throughout the   */
/* existence of the data store.                                              */
/*****************************************************************************/
void init_data_store()
{
  /* Allocates 1GB for the Data Store */
  g_pu1_data_store = (UWORD8 *) malloc(MAX_DATA_STORE_SZ_BYTES);

  /* Reset the entire structure to 0s */
  memset(g_pu1_data_store, 0, MAX_DATA_STORE_SZ_BYTES);
}

/*****************************************************************************/
/* This function destroys the data store.                                    */
/* It frees the memory allocated for the data store pointed to by the global */
/* pointer.                                                                  */
/*****************************************************************************/
void destroy_data_store()
{
  /* Free the entire structure */
  free(g_pu1_data_store);
}

/***************************************************************************/
/* Each data tuple is partiioned as shown below                            */
/* With this partition, one can fit 33,554,432 records in this data store  */
/* +------------+--------------+---------+----------+                      */
/* | ID         | Name         | Age     | Salary   |                      */
/* +------------+--------------+---------+----------+                      */
/* |  4 bytes   |   23 bytes   | 1 byte  |  4 bytes |                      */
/* +------------+--------------+---------+----------+                      */
/*                                                                         */
/***************************************************************************/

/* Writes each of these fields into the allocated memory */
UWORD8 *write_data(UWORD32 u4_id, UWORD8 *pu1_name,
                   UWORD8 u1_age, UWORD32 u4_salary)
{
  UWORD8 *pu1_curr_tuple = NULL;

  if(g_pu1_data_store == NULL)
    return NULL;


  pu1_curr_tuple = g_pu1_data_store + (g_u4_current_idx * TUPLE_LENGTH);

  set_id_field(pu1_curr_tuple, u4_id);
  set_name_field(pu1_curr_tuple, pu1_name);
  set_age_field(pu1_curr_tuple, u1_age);
  set_salary_field(pu1_curr_tuple, u4_salary);

  g_u4_current_idx++;
  return pu1_curr_tuple;
}

/* Reads each field from memory, puts it them into a structure and returns. */
s_tuple_data_t read_data(UWORD8 *pu1_curr_tuple)
{
  s_tuple_data_t s_tuple_data = {0};
  UWORD8 *pu1_name = NULL;

  /* Return empty structure */
  if(pu1_curr_tuple == NULL)
    return s_tuple_data;

  s_tuple_data.u4_id     = get_id_field(pu1_curr_tuple);

  pu1_name               = get_name_field(pu1_curr_tuple);
  memcpy(s_tuple_data.u1_name, pu1_name, NAME_FIELD_LENGTH);

  s_tuple_data.u1_age    = get_age_field(pu1_curr_tuple);
  s_tuple_data.u4_salary = get_salary_field(pu1_curr_tuple);

  return s_tuple_data;
}

/* Test Main Function */
int main()
{
  /* Initialize Data Store */
  init_data_store();
  printf("Data Store Created\n");

  /* Create dummy records in the data store for testing */
  {
    UWORD32 u4_i = 0;
    for(u4_i = 0; u4_i < 10; u4_i++)
    {

      UWORD8  u1_name[NAME_FIELD_LENGTH] = {0};
      UWORD8  u1_j      = 0;
      UWORD8  u1_age    = (u4_i + 100) % 75;
      UWORD32 u4_id     = u4_i;
      UWORD32 u4_salary = (u4_i + 1) * 10;

      for(u1_j = 0; u1_j < NAME_FIELD_LENGTH; u1_j++)
        u1_name[u1_j] = 'a' + (((u1_j + 1) * (u4_i + 1)) % 26);

      write_data(u4_id, u1_name, u1_age, u4_salary);
    }
  }

  /* Read dummy records in the data store for testing */
  {
    UWORD32 u4_i = 0;
    s_tuple_data_t s_tuple_data[10] = {{0}};

    for(u4_i = 0; u4_i < 10; u4_i++)
    {
      UWORD8 *pu1_curr_tuple = g_pu1_data_store + (u4_i * TUPLE_LENGTH);
      s_tuple_data[u4_i] = read_data(pu1_curr_tuple);
    }

    printf("%10s\t%10s\t%10s\t%10s\n", "ID", "Name", "Age", "Salary");
    for(u4_i = 0; u4_i < 10; u4_i++)
    {
      printf("%10d\t%.10s\t%10d\t%10d\n",
             s_tuple_data[u4_i].u4_id, s_tuple_data[u4_i].u1_name,
             s_tuple_data[u4_i].u1_age, s_tuple_data[u4_i].u4_salary);
    }
  }

  sleep(10);

  /* Destroy Data Store before exit */
  destroy_data_store();
  printf("Data Store Destroyed\n");
}
