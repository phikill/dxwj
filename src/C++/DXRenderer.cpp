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


#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <d3d9.h>
#include <dxerr9.h>
#include <jawt_md.h>
#include <map>
#include "DXRenderer.h"
#include "jdw_graphics_Environment3D.h"
#include "jdw_graphics_Primitive.h"
#include "jdw_graphics_Light.h"
#include "jdw_graphics_Material.h"
#include "jdw_graphics_Texture.h"
#include "jdw_graphics_Mesh.h"

using namespace std;

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")


//////////////////////////////////////////////////////////////////////////
// Function prototypes
//////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport)
VOID Render2(JNIEnv* env, jobject canvas, 
	JAWT_Win32DrawingSurfaceInfo* drawinfo);
Environment3D* GetNativeEnv3D(JNIEnv* env, jobject canvas);

extern void JavaArrayToMatrix(JNIEnv* env, jobjectArray data, D3DXMATRIX* mat);
extern jobjectArray GetDataFromJavaMatrix(JNIEnv* env, jobject matrix);
FLOAT Limit(FLOAT val, FLOAT min, FLOAT max);


//////////////////////////////////////////////////////////////////////////

/*
 * Invoked on the Swing event thread by WinMan.dll when the AWT canvas
 * is to be painted.
 */
extern "C" __declspec(dllexport)
VOID Render2(JNIEnv* env, jobject canvas, 
	JAWT_Win32DrawingSurfaceInfo* drawinfo)
{
    Environment3D* currEnv = GetNativeEnv3D(env, canvas);
    if (currEnv)
    {
        currEnv->SetHWND(drawinfo->hwnd);
        currEnv->Render();
        
        // If HRESULT bad, throw exception
        HRESULT hr = currEnv->GetResultCode();
        if (FAILED(hr))
        {
            char buf[256];
            _snprintf(buf, 256, "An error occurred while rendering 0x%X", hr);
            env->ThrowNew(env->FindClass("java/lang/RuntimeException"), buf);
        }
    }
}

Environment3D* GetNativeEnv3D(JNIEnv* env, jobject canvas)
{
    // Get the nativePtr field ID
    jfieldID fid = env->GetFieldID(env->GetObjectClass(canvas),
        "nativePtr", "J");
    jlong val = env->GetLongField(canvas, fid);
    
    if (val != 0)
    {
        // Return nativePtr casted
        return reinterpret_cast<Environment3D*>(val);
    }
    return 0;
}

/*
 * Class:     gfx_NativeCanvas
 * Method:    getNativePtr
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_NativeCanvas_getNativePtr
  (JNIEnv *, jclass)
{
    return 0;
}

FLOAT Limit(FLOAT val, FLOAT min, FLOAT max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}



// ******************************************************************
// Implementation of jdw.graphics.Environment3D native methods
// ******************************************************************
/*
 * Class:     jdw_graphics_Environment3D
 * Method:    getNativePtr
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_Environment3D_getNativePtr
  (JNIEnv *, jclass)
{
    return reinterpret_cast<jlong>(new Environment3D);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _addLight
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1addLight
  (JNIEnv *, jobject, jlong ptr, jlong lightPtr)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    
    if (0 == lightPtr) return;
    Light* light = reinterpret_cast<Light*>(lightPtr);
    currEnv->AddSourceLight(light);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _removeLight
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1removeLight
  (JNIEnv *, jobject, jlong ptr, jlong lightPtr)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    
    if (0 == lightPtr) return;
    Light* light = reinterpret_cast<Light*>(lightPtr);
    currEnv->RemoveSourceLight(light);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setAmbientLight
 * Signature: (JZIII)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setAmbientLight
  (JNIEnv *, jobject, jlong ptr, 
      jboolean /*ambientPresent*/, jint ambR, jint ambG, jint ambB)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    
    currEnv->SetAmbientLight(D3DCOLOR_ARGB(0, ambR, ambG, ambB));
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _updateBackBufferSize
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1updateBackBufferSize
  (JNIEnv *, jobject, jlong ptr, jint w, jint h)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    currEnv->UpdateBackBufferSize(w, h);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _addPrimitive
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1addPrimitive
  (JNIEnv *, jobject, jlong ptr, jlong primitive)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    
    if (0 == primitive) return;
    Primitive* prim = reinterpret_cast<Primitive*>(primitive);
    currEnv->AddPrimitive(prim);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _removePrimitive
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1removePrimitive
  (JNIEnv *, jobject, jlong ptr, jlong primitive)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    
    if (0 == primitive) return;
    Primitive* prim = reinterpret_cast<Primitive*>(primitive);
    currEnv->RemovePrimitive(prim);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _addMesh
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1addMesh
  (JNIEnv *, jobject, jlong nativePtr, jlong meshPtr)
{
    if (0 == nativePtr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(nativePtr);
    
    if (0 == meshPtr) return;
    CMesh* mesh = reinterpret_cast<CMesh*>(meshPtr);
    currEnv->AddMesh(mesh);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setLightingEnabled
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setLightingEnabled
  (JNIEnv *, jobject, jlong ptr, jboolean lighting)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    currEnv->SetLightingEnabled(lighting == JNI_TRUE);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setTransformationEnabled
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setTransformationEnabled
  (JNIEnv *, jobject, jlong ptr, jboolean transform)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    currEnv->SetTransformationEnabled(transform == JNI_TRUE);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setWorldTransform
 * Signature: (JLgfx/Matrix;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setWorldTransform
  (JNIEnv* env, jobject, jlong ptr, jobject matrix)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    // get data array from the Java Matrix
    jobjectArray data = GetDataFromJavaMatrix(env, matrix);
    
    // convert data array to a Direct3D matrix
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    currEnv->SetWorldMatrix(&mat);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setViewTransform
 * Signature: (JLgfx/Matrix;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setViewTransform
  (JNIEnv* env, jobject, jlong ptr, jobject matrix)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    // get data array from the Java Matrix
    jobjectArray data = GetDataFromJavaMatrix(env, matrix);
    
    // convert data array to a Direct3D matrix
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    currEnv->SetViewMatrix(&mat);    
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setProjectionTransform
 * Signature: (JLgfx/Matrix;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setProjectionTransform
  (JNIEnv* env, jobject, jlong ptr, jobject matrix)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    // get data array from the Java Matrix
    jobjectArray data = GetDataFromJavaMatrix(env, matrix);
    
    // convert data array to a Direct3D matrix
    D3DXMATRIX mat;
    JavaArrayToMatrix(env, data, &mat);
    currEnv->SetProjectionMatrix(&mat);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setBackground
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setBackground
  (JNIEnv *, jobject, jlong ptr, jint rgb)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    
    currEnv->SetBackground(rgb);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _setTextureBackground
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1setTextureBackground
  (JNIEnv* , jobject, jlong ptr, jlong texturePtr)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    
    // tex CAN be null!
    CTexture* tex = reinterpret_cast<CTexture*>(texturePtr);
    
    currEnv->SetBackground(tex);
}

/*
 * Class:     jdw_graphics_Environment3D
 * Method:    _dispose
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Environment3D__1dispose
  (JNIEnv*, jobject, jlong ptr)
{
    if (0 == ptr) return;
    Environment3D* currEnv = reinterpret_cast<Environment3D*>(ptr);
    currEnv->Dispose();
    delete currEnv; // Would need to turn off rendering so you're not
          // inside the Render() method while deleting!
}


// ******************************************************************
// Implementation of jdw.graphics.Primitive native methods
// ******************************************************************

/*
 * Class:     gfx_Primitive
 * Method:    init
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_Primitive_init
  (JNIEnv *, jobject)
{
    Primitive* p = new Primitive(TRIANGLE_STRIP);
    return reinterpret_cast<jlong>(p);
}

/*
 * Class:     gfx_Primitive
 * Method:    _addPositionColored
 * Signature: (JFFFFIFF)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Primitive__1addPositionColored
  (JNIEnv*, jobject, jlong ptr,
  jfloat x, jfloat y, jfloat z, jfloat rhw, jint color,
  jfloat tu, jfloat tv)
{
    // Init the native vertex
    Vertex v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.rhw = rhw;
    v.color = color;
    v.tu = tu;
    v.tv = tv;
    
    if (0 == ptr) return;
    Primitive* prim = reinterpret_cast<Primitive*>(ptr);
    prim->AddVertex(v);
}

/*
 * Class:     jdw_graphics_Primitive
 * Method:    _setVertexAt
 * Signature: (JIFFFFIFF)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Primitive__1setVertexAt
  (JNIEnv *, jobject, jlong ptr, jint index, 
      jfloat x, jfloat y, jfloat z, jfloat rhw, jint color,
      jfloat tu, jfloat tv)
{
    // Init the native vertex
    Vertex v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.rhw = rhw;
    v.color = color;
    v.tu = tu;
    v.tv = tv;
    
    if (0 == ptr) return;
    Primitive* prim = reinterpret_cast<Primitive*>(ptr);
    prim->SetVertexAt(index, v);
}

/*
 * Class:     gfx_Primitive
 * Method:    _setRenderMode
 * Signature: (JLgfx/RenderMode;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Primitive__1setRenderMode
  (JNIEnv* env, jobject, jlong ptr, jobject rm)
{
    if (0 == ptr) return;
    Primitive* prim = reinterpret_cast<Primitive*>(ptr);
    
    // Get ID from RenderMode rm and convert to native RenderMode
    
    jclass rmClass = env->GetObjectClass(rm);
    jmethodID getIDmethod = env->GetMethodID(rmClass, "getID", "()I");
    assert(getIDmethod != NULL);

    jint modeID = env->CallIntMethod(rm, getIDmethod);
    
    prim->SetRenderMode(static_cast<RenderMode>(modeID));
}

/*
 * Class:     gfx_Primitive
 * Method:    _setMaterial
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Primitive__1setMaterial
  (JNIEnv *, jobject, jlong ptr, jlong rl)
{
    if (0 == ptr) return;
    Primitive* prim = reinterpret_cast<Primitive*>(ptr);
    
    if (rl)
    {
        D3DMATERIAL9* d3dm = reinterpret_cast<D3DMATERIAL9*>(rl);
        
        prim->SetMaterial(d3dm);
    }
}

/*
 * Class:     gfx_Primitive
 * Method:    _setTexture
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Primitive__1setTexture
  (JNIEnv *, jobject, jlong nativePtr, jlong texPtr)
{
    if (0 == nativePtr) return;
    Primitive* prim = reinterpret_cast<Primitive*>(nativePtr);
    
    CTexture* tex = reinterpret_cast<CTexture*>(texPtr);
    prim->SetTexture(tex);
}

/*
 * Class:     gfx_Primitive
 * Method:    _setColored
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Primitive__1setColored
  (JNIEnv *, jobject, jlong nativePtr, jboolean colored)
{
    if (0 == nativePtr) return;
    Primitive* prim = reinterpret_cast<Primitive*>(nativePtr);
    
    prim->SetColored(colored == JNI_TRUE);
}

/*
 * Class:     gfx_Primitive
 * Method:    _dispose
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Primitive__1dispose
  (JNIEnv *, jobject, jlong ptr)
{
    if (0 == ptr) return;
    Primitive* prim = reinterpret_cast<Primitive*>(ptr);

    prim->Dispose();
    delete prim;
}

// ******************************************************************
// Implementation of jdw.graphics.Light native methods
// ******************************************************************

/*
 * Class:     gfx_Light
 * Method:    getNativePtr
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_Light_getNativePtr
  (JNIEnv *, jobject)
{
    return reinterpret_cast<jlong>(new Light);
}

/*
 * Class:     gfx_Light
 * Method:    _dispose
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Light__1dispose
  (JNIEnv *, jobject, jlong nativePtr)
{
    if (!nativePtr)
    {
        return;
    }
    //   don't want to delete while in SetupLights()
    Light* light = reinterpret_cast<Light*>(nativePtr);
    delete light;
}

/*
 * Class:     gfx_Light
 * Method:    setLightParams
 * Signature: (JIIIIIIIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Light_setLightParams
  (JNIEnv *, jobject, jlong nativePtr, jint type, 
      jint diffuseR, jint diffuseG, jint diffuseB, 
      jint specularR, jint specularG, jint specularB, 
      jfloat positionX, jfloat positionY, jfloat positionZ, 
      jfloat directionX, jfloat directionY, jfloat directionZ)
{
    if (!nativePtr)
    {
        return;
    }

    Light* light = reinterpret_cast<Light*>(nativePtr);
    D3DLIGHT9 dxLight;
    dxLight.Type = static_cast<D3DLIGHTTYPE>(type);
    dxLight.Diffuse.r = Limit(diffuseR/255.0f, 0.0f, 1.0f);
    dxLight.Diffuse.g = Limit(diffuseG/255.0f, 0.0f, 1.0f);
    dxLight.Diffuse.b = Limit(diffuseB/255.0f, 0.0f, 1.0f);
    dxLight.Specular.r = Limit(specularR/255.0f, 0.0f, 1.0f);
    dxLight.Specular.g = Limit(specularG/255.0f, 0.0f, 1.0f);
    dxLight.Specular.b = Limit(specularB/255.0f, 0.0f, 1.0f);
    
    // Ambient missed out
    //dxLight.Ambient.r = 0;
    //dxLight.Ambient.g = 0;
    //dxLight.Ambient.b = 0;
    
    // Position
    D3DVECTOR position = {positionX, positionY, positionZ};
    dxLight.Position = position;
    
    // Direction
    D3DVECTOR direction = {directionX, directionY, directionZ};
    dxLight.Direction = direction;
    
    // Range
    dxLight.Range = 1000.0f;// might not be enough...
    
    // Set theta and phi and falloff if it's a spot light
    if (dxLight.Type == D3DLIGHT_SPOT)
    {
        dxLight.Phi = 2.0f;
        dxLight.Theta = 1.0f;
        dxLight.Falloff = 1.0f;
    }
    
    dxLight.Attenuation0 = 1.0f;
    
    // Update the D3DLIGHT9 inside the Light object
    light->SetDxLight(dxLight);
}

// ******************************************************************
// Implementation of jdw.graphics.Material native methods
// ******************************************************************

/*
 * Class:     gfx_Material
 * Method:    _initWithParams
 * Signature: (ZIIIZIIIZIIIZIIIF)J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_Material__1initWithParams
  (JNIEnv *, jobject, 
      jboolean diffusePresent, jint diffuseR, jint diffuseB, jint diffuseG, 
      jboolean ambientPresent, jint ambientR, jint ambientG, jint ambientB, 
      jboolean specularPresent, jint specularR, jint specularG, jint specularB, 
      jboolean emissivePresent, jint emissiveR, jint emissiveG, jint emissiveB, 
      jfloat specularIntensity)
{
    D3DMATERIAL9* d3dmaterial = new D3DMATERIAL9;
    if (!d3dmaterial) return 0;
    
    ZeroMemory(d3dmaterial, sizeof(D3DMATERIAL9));
    
    //
    // Init the D3DMATERIAL9 structure
    //
    if (diffusePresent)
    {
        d3dmaterial->Diffuse.r = Limit(diffuseR/255.0f, 0.0f, 1.0f);
        d3dmaterial->Diffuse.g = Limit(diffuseG/255.0f, 0.0f, 1.0f);
        d3dmaterial->Diffuse.b = Limit(diffuseB/255.0f, 0.0f, 1.0f);
    }
    if (ambientPresent)
    {
        d3dmaterial->Ambient.r = Limit(ambientR/255.0f, 0.0f, 1.0f);
        d3dmaterial->Ambient.g = Limit(ambientG/255.0f, 0.0f, 1.0f);
        d3dmaterial->Ambient.b = Limit(ambientB/255.0f, 0.0f, 1.0f);
    }
    if (specularPresent)
    {
        d3dmaterial->Specular.r = Limit(specularR/255.0f, 0.0f, 1.0f);
        d3dmaterial->Specular.g = Limit(specularG/255.0f, 0.0f, 1.0f);
        d3dmaterial->Specular.b = Limit(specularB/255.0f, 0.0f, 1.0f);
    }
    if (emissivePresent)
    {
        d3dmaterial->Emissive.r = Limit(emissiveR/255.0f, 0.0f, 1.0f);
        d3dmaterial->Emissive.g = Limit(emissiveG/255.0f, 0.0f, 1.0f);
        d3dmaterial->Emissive.b = Limit(emissiveB/255.0f, 0.0f, 1.0f);
    }
    
    d3dmaterial->Power = specularIntensity;
    
    return reinterpret_cast<jlong>(d3dmaterial);
}

/*
 * Class:     gfx_Material
 * Method:    _dispose
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Material__1dispose
  (JNIEnv *, jobject, jlong nativePtr)
{
    if (nativePtr)
    {
        D3DMATERIAL9* d3dmaterial = reinterpret_cast<D3DMATERIAL9*>(nativePtr);
        delete d3dmaterial;
    }
}

// ******************************************************************
// Implementation of jdw.graphics.Texture native methods
// ******************************************************************
/*
 * Class:     jdw_graphics_Texture
 * Method:    getNativePtr
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_Texture_getNativePtr
  (JNIEnv *, jobject)
{
    CTexture* tex = new CTexture;
    return reinterpret_cast<jlong>(tex);
}

/*
 * Class:     jdw_graphics_Texture
 * Method:    _loadFromFile
 * Signature: (JLjava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Texture__1loadFromFile
  (JNIEnv* env, jobject, jlong nativePtr, jstring jfilename, 
      jint width, jint height)
{
    const char* filename = env->GetStringUTFChars(jfilename, NULL);
    CTexture* tex = reinterpret_cast<CTexture*>(nativePtr);
    
    if (filename && tex)
    {
        tex->SetFilename(filename);
        tex->SetDimensions(width, height);
        env->ReleaseStringUTFChars(jfilename, filename);
    }
}

/*
 * Class:     jdw_graphics_Texture
 * Method:    _dispose
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Texture__1dispose
  (JNIEnv *, jobject, jlong nativePtr)
{
    if (nativePtr)
    {
        CTexture* tex = reinterpret_cast<CTexture*>(nativePtr);
        delete tex;
    }
}

// ******************************************************************
// Implementation of jdw.graphics.Mesh native methods
// ******************************************************************

/*
 * Class:     gfx_Mesh
 * Method:    getNativePtr
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_Mesh_getNativePtr
  (JNIEnv *, jobject)
{
    CMesh* mesh = new CMesh;
    return reinterpret_cast<jlong>(mesh);
}

/*
 * Class:     gfx_Mesh
 * Method:    _loadFromFile
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Mesh__1loadFromFile
  (JNIEnv* env, jobject, jlong nativePtr, jstring jfilename)
{
    const char* filename = env->GetStringUTFChars(jfilename, NULL);
    CMesh* mesh = reinterpret_cast<CMesh*>(nativePtr);
    
    if (filename && mesh)
    {
        mesh->SetFilename(filename);
        env->ReleaseStringUTFChars(jfilename, filename);
    }
}

/*
 * Class:     gfx_Mesh
 * Method:    d3dxCreateBox
 * Signature: (JFFF)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Mesh_d3dxCreateBox
  (JNIEnv *, jobject, jlong, jfloat, jfloat, jfloat)
{
}

/*
 * Class:     gfx_Mesh
 * Method:    d3dxCreateCylinder
 * Signature: (JFFFII)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Mesh_d3dxCreateCylinder
  (JNIEnv *, jobject, jlong, jfloat, jfloat, jfloat, jint, jint)
{
}

/*
 * Class:     gfx_Mesh
 * Method:    _dispose
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_jdw_graphics_Mesh__1dispose
  (JNIEnv *, jobject, jlong nativePtr)
{
    if (nativePtr)
    {
        CMesh* mesh = reinterpret_cast<CMesh*>(nativePtr);
        delete mesh;
    }
}


BOOL APIENTRY DllMain(HANDLE /*hModule*/, DWORD dwReason, LPVOID /*reserved*/)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

