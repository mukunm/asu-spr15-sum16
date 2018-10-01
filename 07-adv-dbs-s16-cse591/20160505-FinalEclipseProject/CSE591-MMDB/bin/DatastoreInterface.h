/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class DatastoreInterface */

#ifndef _Included_DatastoreInterface
#define _Included_DatastoreInterface
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     DatastoreInterface
 * Method:    init_data_store
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_DatastoreInterface_init_1data_1store
  (JNIEnv *, jobject);

/*
 * Class:     DatastoreInterface
 * Method:    destroy_data_store
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_DatastoreInterface_destroy_1data_1store
  (JNIEnv *, jobject);

/*
 * Class:     DatastoreInterface
 * Method:    write_data
 * Signature: (ILjava/lang/String;CI)I
 */
JNIEXPORT jint JNICALL Java_DatastoreInterface_write_1data
  (JNIEnv *, jobject, jint, jstring, jchar, jint);

/*
 * Class:     DatastoreInterface
 * Method:    update_data
 * Signature: (IILjava/lang/String;CI)I
 */
JNIEXPORT jint JNICALL Java_DatastoreInterface_update_1data
  (JNIEnv *, jobject, jint, jint, jstring, jchar, jint);

/*
 * Class:     DatastoreInterface
 * Method:    read_data
 * Signature: (ILDataTuple;)V
 */
JNIEXPORT void JNICALL Java_DatastoreInterface_read_1data
  (JNIEnv *, jobject, jint, jobject);

#ifdef __cplusplus
}
#endif
#endif