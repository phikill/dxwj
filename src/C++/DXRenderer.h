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

#pragma once



#include <windows.h>
#include <jni.h>
#include <d3d9.h>
#include <dxerr9.h>
#include <d3dx9math.h>
#include <vector>

//
// Forward declarations
//
class Primitive;
class CMesh;
class CTexture;

struct CUSTOMVERTEX
{
    FLOAT x, y, z;
    DWORD color;        // Diffuse
};

struct CUSTOMVERTEX_RHW
{
    FLOAT x, y, z, rhw;
    DWORD color;        // Diffuse
};

struct CUSTOMVERTEX_BASIC
{
    FLOAT x, y, z;
};

struct CUSTOMVERTEX_RHW_ND
{
    FLOAT x, y, z, rhw;
};

struct CUSTOMVERTEX_TEX
{
    FLOAT x, y, z;
    DWORD color;        // Diffuse
    FLOAT tu, tv;
};

struct CUSTOMVERTEX_RHW_TEX
{
    FLOAT x, y, z, rhw;
    DWORD color;        // Diffuse
    FLOAT tu, tv;
};

struct CUSTOMVERTEX_BASIC_TEX
{
    FLOAT x, y, z;
    FLOAT tu, tv;
};

struct CUSTOMVERTEX_RHW_ND_TEX
{
    FLOAT x, y, z, rhw;
    FLOAT tu, tv;
};


// Representative for the Java jdw.graphics.Vertex class. 
// Vertex objects are passed by copy
class Vertex
{
public:
    FLOAT x;
    FLOAT y;
    FLOAT z;
    FLOAT rhw;
    DWORD color;
    FLOAT tu;
    FLOAT tv;
};

enum RenderMode
{
    POINT_LIST, 
    LINE_LIST, 
    LINE_STRIP, 
    TRIANGLE_LIST,
    TRIANGLE_STRIP, 
    TRIANGLE_FAN
};

//
// Thread safe wrapper for a D3DLIGHT9 structure
//
class Light
{
    D3DLIGHT9 dxLight;
    CRITICAL_SECTION csec;
    int dxLightIndex;
public:
    
    Light();
    ~Light();
    
    D3DLIGHT9 GetDxLight();
    void SetDxLight(D3DLIGHT9 newLight);
    int GetLightIndex();
    void SetLightIndex(int);
};

//
// Environment3D: Native representative for the Java gfx.Environment3D class
//
class Environment3D
{
    CRITICAL_SECTION m_critsec;
    HWND m_hWnd;   // handle of window in which to display graphics
    HRESULT m_hResult; 		           // HRESULT of last call
    IDirect3D9* m_pDirect3D; 	           // Direct3D object
    IDirect3DDevice9* m_pDirect3DDevice;    // Direct3D device object

    std::vector<Primitive*> m_primitiveList;
    std::vector<Light*> m_lightList;
    std::vector<CMesh*> m_meshList;
    bool m_lighting;             // true = use lighting
    bool m_transformCoords;      // true = apply 3 transformations
    DWORD m_ambientColor;
    DWORD m_backColor;
    CTexture* m_backImage;
    
    D3DXMATRIX m_WorldMatrix;
    D3DXMATRIX m_ViewMatrix;
    D3DXMATRIX m_ProjectionMatrix;
    
    //---------------------------------------------------------------
    VOID SetupTransformations(VOID);
    VOID SetupLights(BOOL enable);
public:
    Environment3D(VOID);
    ~Environment3D(VOID);

    HRESULT InitDirect3D(VOID);
    VOID SetHWND(HWND hwnd);
    VOID Render(VOID);
    VOID Dispose(VOID);
    VOID AddPrimitive(Primitive* p);
    VOID RemovePrimitive(Primitive* p);
    VOID AddMesh(CMesh* m);
    
    VOID SetLightingEnabled(bool lighting);
    bool IsLightingEnabled(VOID);
    VOID SetTransformationEnabled(bool transform);
    bool IsTransformationEnabled(VOID);
    VOID SetResultCode(HRESULT hr);
    HRESULT GetResultCode(VOID);
    
    VOID SetWorldMatrix(D3DXMATRIX*);
    VOID SetViewMatrix(D3DXMATRIX*);
    VOID SetProjectionMatrix(D3DXMATRIX*);
    VOID UpdateBackBufferSize(int w, int h);
    
    VOID AddSourceLight(Light*);
    VOID RemoveSourceLight(Light*);
    VOID SetAmbientLight(DWORD color);
    VOID SetBackground(DWORD color);
    VOID SetBackground(CTexture*);
};

//
// Primitive: Representative for the Java jdw.graphics.Primitive class
//
class Primitive
{
    CRITICAL_SECTION m_critsec;
    std::vector<Vertex> m_Vertices;
    RenderMode m_Mode;
    bool m_Dirty;
    IDirect3DVertexBuffer9* m_pVertexBuf;
    ID3DXMesh* m_pMesh;
    D3DMATERIAL9* m_Material;
    bool m_bColored;
    CTexture* m_pTexture;
    
    //---------------------------------------------------------------
    HRESULT CalculatePrimitiveTypeAndCount(
        D3DPRIMITIVETYPE* type, UINT* count);
    HRESULT CreateVertexBuffer(Environment3D* env3d, 
                               IDirect3DDevice9*,
                               DWORD fvf);
    HRESULT CreateMesh(Environment3D* env3d, 
                       IDirect3DDevice9*, 
                       DWORD fvf);
    
    HRESULT RenderNoTexture(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice);
    HRESULT RenderWithTexture(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice);
    
    HRESULT RenderWithLightsAndTransform(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice, DWORD fvf);
    HRESULT RenderWithTransform(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice,
        bool diffuse);
    HRESULT RenderNoTransform(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice,
        bool diffuse);
    
    HRESULT RenderWithLightsAndTransformTextured(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice, DWORD fvf);
    HRESULT RenderWithTransformTextured(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice,
        bool diffuse);
    HRESULT RenderNoTransformTextured(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice,
        bool diffuse);
    HRESULT CreateVertexBufferTextured(Environment3D* env3d, 
                               IDirect3DDevice9*,
                               DWORD fvf);
    HRESULT CreateMeshTextured(Environment3D* env3d, 
                       IDirect3DDevice9*, 
                       DWORD fvf);
    
    UINT GetVertexSize(DWORD fvf);
public:
    Primitive(RenderMode _mode);
    ~Primitive(VOID);

    VOID AddVertex(Vertex v);
    VOID SetVertexAt(size_t index, Vertex v);
    RenderMode GetRenderMode(VOID);
    VOID SetRenderMode(RenderMode m);
    VOID Dispose(VOID);
    HRESULT Render(Environment3D* env3d, IDirect3DDevice9*);
    VOID SetMaterial(D3DMATERIAL9*);
    VOID SetTexture(CTexture*);
    CTexture* GetTexture(VOID);
    VOID SetColored(bool);
    bool IsColored(VOID);
};

//
// CMesh: Wraps a Direct3D mesh
//
class CMesh
{
    ID3DXMesh* m_pMesh; // TODO: Need per-device mesh?
    LPCSTR m_filename;
    LPD3DXBUFFER materialBuffer;
    DWORD numMaterials;
    D3DMATERIAL9* meshMaterials;
    LPDIRECT3DTEXTURE9* meshTextures;
    
    HRESULT LoadFromFile(IDirect3DDevice9* pDirect3DDevice);
public:
    CMesh(VOID);
    ~CMesh(VOID);

    VOID SetFilename(LPCSTR filename);
    VOID SetMesh(ID3DXMesh*);
    HRESULT Render(Environment3D* env3d, IDirect3DDevice9*);
};

//
// CTexture: Wraps a Direct3D texture and image surface
//
class CTexture
{
    IDirect3DTexture9* m_pTexture;
    IDirect3DSurface9* m_pImageSurface;
    LPCSTR m_filename;
    UINT m_width;
    UINT m_height;
    
    HRESULT LoadFromFile(IDirect3DDevice9* pDirect3DDevice);
public:
    CTexture(VOID);
    CTexture(CTexture& rhs);
    ~CTexture(VOID);
    
    VOID SetFilename(LPCSTR filename);
    VOID SetDimensions(UINT width, UINT height);
    HRESULT GetDxTexture(Environment3D* env3d, IDirect3DDevice9*,
        IDirect3DTexture9**);
    HRESULT GetDxImage(Environment3D*,
                         IDirect3DDevice9*,
                         IDirect3DSurface9** ppSurface);
};

