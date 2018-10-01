#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datastore_DatastoreInterface.h"

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
/* Function Definitions                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* This function initializes the data store.                                 */
/* It allocates sufficient amount of memory to store the entire database     */
/* and stores it in the global pointer that can be accessed throughout the   */
/* existence of the data store.                                              */
/*****************************************************************************/

JNIEXPORT jint JNICALL
Java_datastore_DatastoreInterface_init_1data_1store(JNIEnv *env, jobject obj)
{
	/* Allocates 1GB for the Data Store */
	g_pu1_data_store = (UWORD8 *) malloc(MAX_DATA_STORE_SZ_BYTES);

	printf("Allocated %d\n", MAX_DATA_STORE_SZ_BYTES);

	/* Reset the entire structure to 0s */
	memset(g_pu1_data_store, 0, MAX_DATA_STORE_SZ_BYTES);

	printf("Global Address is 0x%x\n\r", (UWORD32) g_pu1_data_store);
	return (jint) g_pu1_data_store;
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
JNIEXPORT jint JNICALL
Java_datastore_DatastoreInterface_write_1data(JNIEnv *env, jobject obj,
									jint id, jstring name,
									jchar age, jint salary)
{
	UWORD32 u4_id     = (UWORD32) id;
	UWORD8  *pu1_name = (UWORD8 *)(*env)->GetStringUTFChars(env, name, NULL);
	UWORD8  u1_age    = (UWORD8) age;
	UWORD32 u4_salary = (UWORD32) salary;

	UWORD8 *pu1_curr_tuple = NULL;

	if(g_u4_current_idx >= MAX_DATA_STORE_SZ_BYTES)
		return (jint) NULL;

	if(g_pu1_data_store == NULL)
		return (jint) NULL;

	pu1_curr_tuple = g_pu1_data_store + (g_u4_current_idx * TUPLE_LENGTH);

	set_id_field(pu1_curr_tuple, u4_id);
	set_name_field(pu1_curr_tuple, pu1_name);
	set_age_field(pu1_curr_tuple, u1_age);
	set_salary_field(pu1_curr_tuple, u4_salary);

	g_u4_current_idx++;

	return (jint) pu1_curr_tuple;
}

/* Updates each of these fields in the allocated memory. */
JNIEXPORT jint JNICALL
Java_datastore_DatastoreInterface_update_1data(JNIEnv *env, jobject obj,
									 jint tuple_addr, jint id, jstring name,
									 jchar age, jint salary)
{
	UWORD32 u4_id     = (UWORD32) id;
	UWORD8  *pu1_name = (UWORD8 *)(*env)->GetStringUTFChars(env, name, NULL);
	UWORD8  u1_age    = (UWORD8) age;
	UWORD32 u4_salary = (UWORD32) salary;

	UWORD8 *pu1_curr_tuple = (UWORD8 *) tuple_addr;

	if(g_pu1_data_store == NULL)
		return (jint) NULL;

	//pu1_curr_tuple = g_pu1_data_store + (g_u4_current_idx * TUPLE_LENGTH);

	set_id_field(pu1_curr_tuple, u4_id);
	set_name_field(pu1_curr_tuple, pu1_name);
	set_age_field(pu1_curr_tuple, u1_age);
	set_salary_field(pu1_curr_tuple, u4_salary);

	g_u4_current_idx++;

	return (jint) pu1_curr_tuple;
}

/* Reads each field from memory, puts it them into a structure and returns. */
JNIEXPORT void JNICALL
Java_datastore_DatastoreInterface_read_1data(JNIEnv *env, jobject this_obj,
								   jint tuple_addr, jobject tuple_obj)
{
	jclass tuple_class;

	jfieldID id_field_number;
	jfieldID name_field_number;
	jfieldID age_field_number;
	jfieldID salary_field_number;

	jint    id;
	jstring name;
	jchar   age;
	jint    salary;

	UWORD8 *pu1_name;
	UWORD8 *pu1_curr_tuple;
	UWORD8 *pu1_name_str;

	tuple_class = (*env)->GetObjectClass(env, tuple_obj);

	id_field_number = (*env)->GetFieldID(env, tuple_class, "id", "I");
	name_field_number = (*env)->GetFieldID(env, tuple_class, "name", "Ljava/lang/String;");
	age_field_number = (*env)->GetFieldID(env, tuple_class, "age", "C");
	salary_field_number = (*env)->GetFieldID(env, tuple_class, "salary", "I");

	id = (*env)->GetIntField(env, tuple_obj, id_field_number);
	name = (*env)->GetObjectField(env, tuple_obj, name_field_number);
	age = (*env)->GetCharField(env, tuple_obj, age_field_number);
	salary = (*env)->GetIntField(env, tuple_obj, salary_field_number);

	pu1_name       = (UWORD8 *) malloc(NAME_FIELD_LENGTH);
	memset(pu1_name, 0, NAME_FIELD_LENGTH);
	pu1_curr_tuple = (UWORD8 *) tuple_addr;

	/* Return empty structure */
	if(pu1_curr_tuple == NULL)
	{
		free(pu1_name);
		return;
	}

	/* Set ID */
	id = (jint) get_id_field(pu1_curr_tuple);
	(*env)->SetIntField(env, tuple_obj, id_field_number, id);

	/* Set Name */
	memcpy(pu1_name, get_name_field(pu1_curr_tuple), NAME_FIELD_LENGTH);
	name = (*env)->NewStringUTF(env, pu1_name);
	(*env)->SetObjectField(env, tuple_obj, name_field_number, name);
	free(pu1_name);

	/* Set Age */
	age = (jchar) get_age_field(pu1_curr_tuple);
	(*env)->SetCharField(env, tuple_obj, age_field_number, age);

	/* Set Salary */
	salary  = get_salary_field(pu1_curr_tuple);
	(*env)->SetIntField(env, tuple_obj, salary_field_number, salary);

	return;
}

/*****************************************************************************/
/* This function destroys the data store.                                    */
/* It frees the memory allocated for the data store pointed to by the global */
/* pointer.                                                                  */
/*****************************************************************************/

JNIEXPORT void JNICALL
Java_datastore_DatastoreInterface_destroy_1data_1store(JNIEnv *env, jobject obj)
{
	/* Free the entire structure */
	free(g_pu1_data_store);
	g_pu1_data_store = NULL;
	printf("Freed\n");
	return;
}

