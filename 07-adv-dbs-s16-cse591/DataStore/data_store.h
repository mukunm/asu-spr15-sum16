/*****************************************************************************/
/*                                                                           */
/* Author - Mukund Manikarnike                                               */
/* Course - Advances in Databases (CSE 591)                                  */
/* Term   - Spring 2016                                                      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* This file includes definitions of all macros, inline functions required   */
/* for the creation of the data store.                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Typedefs                                                                  */
/*****************************************************************************/
typedef char           WORD8;
typedef short          WORD16;
typedef int            WORD32;

typedef unsigned char  UWORD8;
typedef unsigned short UWORD16;
typedef unsigned int   UWORD32;

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/
/* General Macros */
#define BYTE_LENGTH 8

/* Macro to combine bytes into words */
#define MAKE_WORD16(lsb, msb) ((((UWORD16)(msb) << 8) & 0xFF00) | (lsb))
#define MAKE_WORD32(lsw, msw) ((((UWORD32)(msw) << 16) & 0xFFFF0000) | (lsw))

#define INLINE static __inline

/* Data Store related macros */
#define DEFAULT_INDEX                    0
#define MAX_DATA_STORE_SZ_BYTES 438304768//1073741824

#define ID_FIELD_OFFSET                  0
#define NAME_FIELD_OFFSET                4
#define AGE_FIELD_OFSSET                36
#define SALARY_FIELD_OFSSET             37

#define ID_FIELD_LENGTH                  4
#define NAME_FIELD_LENGTH               23
#define AGE_FIELD_LENGTH                 1
#define SALARY_FIELD_LENGTH              4

#define TUPLE_LENGTH                    32
/*****************************************************************************/
/* Enums                                                                     */
/*****************************************************************************/
typedef enum {
	FALSE = 0,
	TRUE  = 1
} BOOL_T;

/*****************************************************************************/
/* Structures                                                                */
/*****************************************************************************/
typedef struct {
	UWORD32 u4_id;
	UWORD8  u1_name[NAME_FIELD_LENGTH];
	UWORD8  u1_age;
	UWORD32 u4_salary;
} s_tuple_data_t;

/*****************************************************************************/
/* Extern Variable Declarations                                              */
/*****************************************************************************/
extern UWORD8 *g_pu1_data_store;

/*****************************************************************************/
/* Inline Functions                                                          */
/*****************************************************************************/
INLINE void set_id_field(UWORD8 *pu1_curr_tuple, UWORD32 u4_id)
{
	UWORD8  u1_id_byte_0 =  u4_id & 0x000000FF;
	UWORD8  u1_id_byte_1 = (u4_id & 0x0000FF00) >> 8;
	UWORD8  u1_id_byte_2 = (u4_id & 0x00FF0000) >> 16;
	UWORD8  u1_id_byte_3 = (u4_id & 0xFF000000) >> 24;

	if(pu1_curr_tuple == NULL)
		return;

	*(pu1_curr_tuple + ID_FIELD_OFFSET) 	  = u1_id_byte_0;
	*(pu1_curr_tuple + ID_FIELD_OFFSET + 1) = u1_id_byte_1;
	*(pu1_curr_tuple + ID_FIELD_OFFSET + 2) = u1_id_byte_2;
	*(pu1_curr_tuple + ID_FIELD_OFFSET + 3) = u1_id_byte_3;
}

INLINE UWORD32 get_id_field(UWORD8 *pu1_curr_tuple)
{
	/* Create the LSW and MSW for the ID */
	UWORD16 u2_lsw = MAKE_WORD16(*(pu1_curr_tuple + ID_FIELD_OFFSET),
															 *(pu1_curr_tuple + ID_FIELD_OFFSET + 1));
	UWORD16 u2_msw = MAKE_WORD16(*(pu1_curr_tuple + ID_FIELD_OFFSET + 2),
															 *(pu1_curr_tuple + ID_FIELD_OFFSET + 3));

	/* Create the ID from the 2 Words */
	UWORD32 u4_id  = MAKE_WORD32(u2_lsw, u2_msw);

	/* Return ID */
	return u4_id;
}

INLINE void set_name_field(UWORD8 *pu1_curr_tuple, UWORD8 *pu1_name)
{
	memcpy(pu1_curr_tuple + NAME_FIELD_OFFSET, pu1_name, NAME_FIELD_LENGTH);
}

INLINE UWORD8 *get_name_field(UWORD8 *pu1_curr_tuple)
{
	UWORD8 *pu1_name = pu1_curr_tuple + NAME_FIELD_OFFSET;
	return pu1_name;
}

INLINE void set_age_field(UWORD8 *pu1_curr_tuple, UWORD8 u1_age)
{
	*(pu1_curr_tuple + AGE_FIELD_OFSSET) = u1_age;
}

INLINE UWORD8 get_age_field(UWORD8 *pu1_curr_tuple)
{
	UWORD8 u1_age = *(pu1_curr_tuple + AGE_FIELD_OFSSET);
	return u1_age;
}

INLINE void set_salary_field(UWORD8 *pu1_curr_tuple, UWORD32 u4_salary)
{
	UWORD8  u1_salary_byte_0 =  u4_salary & 0x000000FF;
	UWORD8  u1_salary_byte_1 = (u4_salary & 0x0000FF00) >> 8;
	UWORD8  u1_salary_byte_2 = (u4_salary & 0x00FF0000) >> 16;
	UWORD8  u1_salary_byte_3 = (u4_salary & 0xFF000000) >> 24;

	if(pu1_curr_tuple == NULL)
		return;

	*(pu1_curr_tuple + SALARY_FIELD_OFSSET)     = u1_salary_byte_0;
	*(pu1_curr_tuple + SALARY_FIELD_OFSSET + 1) = u1_salary_byte_1;
	*(pu1_curr_tuple + SALARY_FIELD_OFSSET + 2) = u1_salary_byte_2;
	*(pu1_curr_tuple + SALARY_FIELD_OFSSET + 3) = u1_salary_byte_3;
}

INLINE UWORD32 get_salary_field(UWORD8 *pu1_curr_tuple)
{
	/* Create the LSW and MSW for the ID */
	UWORD16 u2_lsw = MAKE_WORD16(*(pu1_curr_tuple + SALARY_FIELD_OFSSET),
															 *(pu1_curr_tuple + SALARY_FIELD_OFSSET + 1));
	UWORD16 u2_msw = MAKE_WORD16(*(pu1_curr_tuple + SALARY_FIELD_OFSSET + 2),
															 *(pu1_curr_tuple + SALARY_FIELD_OFSSET + 3));

	/* Create the ID from the 2 Words */
	UWORD32 u4_salary  = MAKE_WORD32(u2_lsw, u2_msw);

	/* Return ID */
	return u4_salary;
}
