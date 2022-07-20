/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class jdw_graphics_Environment3D */

#ifndef _Included_jdw_graphics_Environment3D
#define _Included_jdw_graphics_Environment3D
#ifdef __cplusplus
extern "C" {
#endif
#undef jdw_graphics_Environment3D_FOCUS_TRAVERSABLE_UNKNOWN
#define jdw_graphics_Environment3D_FOCUS_TRAVERSABLE_UNKNOWN 0L
#undef jdw_graphics_Environment3D_FOCUS_TRAVERSABLE_DEFAULT
#define jdw_graphics_Environment3D_FOCUS_TRAVERSABLE_DEFAULT 1L
#undef jdw_graphics_Environment3D_FOCUS_TRAVERSABLE_SET
#define jdw_graphics_Environment3D_FOCUS_TRAVERSABLE_SET 2L
#undef jdw_graphics_Environment3D_TOP_ALIGNMENT
#define jdw_graphics_Environment3D_TOP_ALIGNMENT 0.0f
#undef jdw_graphics_Environment3D_CENTER_ALIGNMENT
#define jdw_graphics_Environment3D_CENTER_ALIGNMENT 0.5f
#undef jdw_graphics_Environment3D_BOTTOM_ALIGNMENT
#define jdw_graphics_Environment3D_BOTTOM_ALIGNMENT 1.0f
#undef jdw_graphics_Environment3D_LEFT_ALIGNMENT
#define jdw_graphics_Environment3D_LEFT_ALIGNMENT 0.0f
#undef jdw_graphics_Environment3D_RIGHT_ALIGNMENT
#define jdw_graphics_Environment3D_RIGHT_ALIGNMENT 1.0f
#undef jdw_graphics_Environment3D_serialVersionUID
#define jdw_graphics_Environment3D_serialVersionUID -7644114512714619750i64
#undef jdw_graphics_Environment3D_serialVersionUID
#define jdw_graphics_Environment3D_serialVersionUID -2284879212465893870i64
/*
 * Class:     jdw_graphics_Environment3D
 * Method:    getNativePtr
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_Environment3D_getNativePtr
  (JNIEnv *, jclass);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setLightingEnabled
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setLightingEnabled
  (JNIEnv *, jobject, jlong, jboolean);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _addLight
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1addLight
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _removeLight
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1removeLight
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setAmbientLight
 * Signature: (JZIII)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setAmbientLight
  (JNIEnv *, jobject, jlong, jboolean, jint, jint, jint);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _updateBackBufferSize
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1updateBackBufferSize
  (JNIEnv *, jobject, jlong, jint, jint);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setTransformationEnabled
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setTransformationEnabled
  (JNIEnv *, jobject, jlong, jboolean);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _addPrimitive
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1addPrimitive
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _removePrimitive
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1removePrimitive
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _addMesh
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1addMesh
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setWorldTransform
 * Signature: (JLjdw/graphics/Matrix;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setWorldTransform
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setViewTransform
 * Signature: (JLjdw/graphics/Matrix;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setViewTransform
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setProjectionTransform
 * Signature: (JLjdw/graphics/Matrix;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setProjectionTransform
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setBackground
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setBackground
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setTextureBackground
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setTextureBackground
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _dispose
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1dispose
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif