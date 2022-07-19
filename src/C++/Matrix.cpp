/*
 * Java-Direct3D Wrapper
 * Copyright (c) 2006 Matthew Ross
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 * OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "jdw_graphics_Matrix.h"
#include <assert.h>
#include <d3dx9math.h>

#pragma comment(lib, "advapi32.lib")

/////////////////////////////////////////////////////////////////////
// Function Prototypes
void JavaArrayToMatrix(JNIEnv* env, jobjectArray data, D3DXMATRIX* mat);
void MatrixToJavaArray(JNIEnv* env, jobjectArray data, D3DXMATRIX* mat);
void JavaToNativeVector(JNIEnv* env, jobject vector, D3DXVECTOR3* vec);
jobjectArray GetDataFromJavaMatrix(JNIEnv* env, jobject matrix);
/////////////////////////////////////////////////////////////////////

/*
 * Class:     gfx_Matrix
 * Method:    _translate
 * Signature: ([[FFFF)[[F
 */
JNIEXPORT jobjectArray JNICALL Java_jdw_graphics_Matrix__1translate
  (JNIEnv* env, jclass /*matrixCls*/, jobjectArray data, 
  jfloat x, jfloat y, jfloat z)
{
    // create D3DXMATRIX structure from data array
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    D3DXMatrixTranslation(&mat, x, y, z);
    // write results to Java array
    MatrixToJavaArray(env, data, &mat);
    return data;
}

/*
 * Class:     gfx_Matrix
 * Method:    _rotationX
 * Signature: ([[FF)[[F
 */
JNIEXPORT jobjectArray JNICALL Java_jdw_graphics_Matrix__1rotationX
  (JNIEnv* env, jclass /*matrixCls*/, jobjectArray data, jfloat angle)
{
    // create D3DXMATRIX structure from data array
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    D3DXMatrixRotationX(&mat, angle);
    // write results to Java array
    MatrixToJavaArray(env, data, &mat);
    return data;
}

/*
 * Class:     gfx_Matrix
 * Method:    _rotationY
 * Signature: ([[FF)[[F
 */
JNIEXPORT jobjectArray JNICALL Java_jdw_graphics_Matrix__1rotationY
  (JNIEnv* env, jclass /*matrixCls*/, jobjectArray data, jfloat angle)
{
    // create D3DXMATRIX structure from data array
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    D3DXMatrixRotationY(&mat, angle);
    // write results to Java array
    MatrixToJavaArray(env, data, &mat);
    return data;
}

/*
 * Class:     gfx_Matrix
 * Method:    _rotationZ
 * Signature: ([[FF)[[F
 */
JNIEXPORT jobjectArray JNICALL Java_jdw_graphics_Matrix__1rotationZ
  (JNIEnv* env, jclass /*matrixCls*/, jobjectArray data, jfloat angle)
{
    // Create D3DXMATRIX structure from data array
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    D3DXMatrixRotationZ(&mat, angle);
    // Write results to Java array
    MatrixToJavaArray(env, data, &mat);
    return data;
}

/*
 * Class:     gfx_Matrix
 * Method:    _scale
 * Signature: ([[FFFF)[[F
 */
JNIEXPORT jobjectArray JNICALL Java_jdw_graphics_Matrix__1scale
  (JNIEnv* env, jclass /*matrixCls*/, jobjectArray data, 
  jfloat x, jfloat y, jfloat z)
{
    // Create D3DXMATRIX structure from data array
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    D3DXMatrixScaling(&mat, x, y, z);
    // Write results to Java array
    MatrixToJavaArray(env, data, &mat);
    return data;
}

/*
 * Class:     gfx_Matrix
 * Method:    _createViewTransform
 * Signature: ([[FLgfx/Vector3D;Lgfx/Vector3D;Lgfx/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Matrix__1createViewTransform
  (JNIEnv* env, jclass /*matrixCls*/, jobjectArray data, 
  jobject eyePt, jobject lookatPt, jobject upVec)
{
    D3DXVECTOR3 vEyePt;
    D3DXVECTOR3 vLookatPt;
    D3DXVECTOR3 vUpVec;
    D3DXMATRIX viewMatrix;
    
    JavaToNativeVector(env, eyePt, &vEyePt);
    JavaToNativeVector(env, lookatPt, &vLookatPt);
    JavaToNativeVector(env, upVec, &vUpVec);
    
    D3DXMatrixLookAtLH(&viewMatrix, &vEyePt, &vLookatPt, &vUpVec);
    // Write results to Java array
    MatrixToJavaArray(env, data, &viewMatrix);
}

/*
 * Class:     jdw_graphics_Matrix
 * Method:    _d3dxMatrixRotationYawPitchRoll
 * Signature: ([[FFFF)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Matrix__1d3dxMatrixRotationYawPitchRoll
  (JNIEnv* env, jclass, jobjectArray data, 
      jfloat yaw, jfloat pitch, jfloat roll)
{
    // Create D3DXMATRIX structure from data array
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    
    D3DXMatrixRotationYawPitchRoll(&mat, yaw, pitch, roll);
    
    // Write results to Java array
    MatrixToJavaArray(env, data, &mat);
}


/////////////////////////////////////////////////////////////////////
// Private Functions
/////////////////////////////////////////////////////////////////////
void JavaArrayToMatrix(JNIEnv* env, jobjectArray data, D3DXMATRIX* mat)//data=float[][]
{
    jfloatArray row;
    jfloat* rowdata;
    
    row = static_cast<jfloatArray>(env->GetObjectArrayElement(data, 0));
    rowdata = env->GetFloatArrayElements(row, 0);
    
    mat->_11 = rowdata[0];
    mat->_12 = rowdata[1];
    mat->_13 = rowdata[2];
    mat->_14 = rowdata[3];
    
    row = static_cast<jfloatArray>(env->GetObjectArrayElement(data, 1));
    rowdata = env->GetFloatArrayElements(row, 0);
    
    mat->_21 = rowdata[0];
    mat->_22 = rowdata[1];
    mat->_23 = rowdata[2];
    mat->_24 = rowdata[3];
    
    row = static_cast<jfloatArray>(env->GetObjectArrayElement(data, 2));
    rowdata = env->GetFloatArrayElements(row, 0);
    
    mat->_31 = rowdata[0];
    mat->_32 = rowdata[1];
    mat->_33 = rowdata[2];
    mat->_34 = rowdata[3];
    
    row = static_cast<jfloatArray>(env->GetObjectArrayElement(data, 3));
    rowdata = env->GetFloatArrayElements(row, 0);
    
    mat->_41 = rowdata[0];
    mat->_42 = rowdata[1];
    mat->_43 = rowdata[2];
    mat->_44 = rowdata[3];
}

void MatrixToJavaArray(JNIEnv* env, jobjectArray data, D3DXMATRIX* mat)//data=float[][]
{
    jfloatArray row;
    jfloat rowdata[4];
    
    row = static_cast<jfloatArray>(env->GetObjectArrayElement(data, 0));
    rowdata[0] = mat->_11;
    rowdata[1] = mat->_12;
    rowdata[2] = mat->_13;
    rowdata[3] = mat->_14;
    env->SetFloatArrayRegion(row, 0, 4, rowdata);
    
    row = static_cast<jfloatArray>(env->GetObjectArrayElement(data, 1));
    rowdata[0] = mat->_21;
    rowdata[1] = mat->_22;
    rowdata[2] = mat->_23;
    rowdata[3] = mat->_24;
    env->SetFloatArrayRegion(row, 0, 4, rowdata);
    
    row = static_cast<jfloatArray>(env->GetObjectArrayElement(data, 2));
    rowdata[0] = mat->_31;
    rowdata[1] = mat->_32;
    rowdata[2] = mat->_33;
    rowdata[3] = mat->_34;
    env->SetFloatArrayRegion(row, 0, 4, rowdata);
    
    row = static_cast<jfloatArray>(env->GetObjectArrayElement(data, 3));
    rowdata[0] = mat->_41;
    rowdata[1] = mat->_42;
    rowdata[2] = mat->_43;
    rowdata[3] = mat->_44;
    env->SetFloatArrayRegion(row, 0, 4, rowdata);
}

void JavaToNativeVector(JNIEnv* env, jobject vector, D3DXVECTOR3* vec)
{
    jclass vecClass = env->GetObjectClass(vector);
    jfieldID xFld = env->GetFieldID(vecClass, "x", "F");
    jfieldID yFld = env->GetFieldID(vecClass, "y", "F");
    jfieldID zFld = env->GetFieldID(vecClass, "z", "F");
    
    jfloat x = env->GetFloatField(vector, xFld);
    jfloat y = env->GetFloatField(vector, yFld);
    jfloat z = env->GetFloatField(vector, zFld);
    
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

jobjectArray GetDataFromJavaMatrix(JNIEnv* env, jobject matrix)
{
    jclass matrixClass = env->GetObjectClass(matrix);
    jfieldID dataField = env->GetFieldID(matrixClass, "data", "[[F");
    assert(dataField != NULL);
    jobjectArray data = static_cast<jobjectArray>(
        env->GetObjectField(matrix, dataField));

    return data;
}

