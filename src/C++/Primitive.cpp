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



#include <assert.h>
#include "DXRenderer.h"

Primitive::Primitive(RenderMode _mode) : m_Mode(_mode), 
        m_Dirty(false), m_pVertexBuf(NULL), m_pMesh(NULL), 
        m_Material(NULL), m_bColored(true), m_pTexture(NULL)
{
    InitializeCriticalSection(&m_critsec);
}

Primitive::~Primitive(VOID)
{
    if (m_pVertexBuf)
    {
        m_pVertexBuf->Release();
        m_pVertexBuf = NULL;
    }
    if (m_Material)
    {
        delete m_Material;
        m_Material = NULL;
    }
    DeleteCriticalSection(&m_critsec);
}

VOID Primitive::AddVertex(Vertex v)
{
    __try
    {
        EnterCriticalSection(&m_critsec);
        m_Dirty = true;
        m_Vertices.push_back(v);
    }
    __finally
    {
        LeaveCriticalSection(&m_critsec);
    }
}

VOID Primitive::SetVertexAt(size_t index, Vertex v)
{
    EnterCriticalSection(&m_critsec);
    
    m_Dirty = true;
    m_Vertices[index] = v;
    
    LeaveCriticalSection(&m_critsec);
}

RenderMode Primitive::GetRenderMode(VOID)
{
    RenderMode ret;
    EnterCriticalSection(&m_critsec);
    ret = m_Mode;
    LeaveCriticalSection(&m_critsec);
    return ret;
}

VOID Primitive::SetRenderMode(RenderMode m)
{
    EnterCriticalSection(&m_critsec);
    m_Mode = m;
    LeaveCriticalSection(&m_critsec);
}

VOID Primitive::Dispose()
{
}

/* Should only be called by the Swing event thread. */
HRESULT Primitive::Render(Environment3D* env3d, 
    IDirect3DDevice9* pDirect3DDevice)
{
    HRESULT hr = S_OK;
    __try
    {
        EnterCriticalSection(&m_critsec);
        bool textured = m_pTexture != NULL;
        
        if (!textured)
        {
            hr = RenderNoTexture(env3d, pDirect3DDevice);
        }
        else
        {
            hr = RenderWithTexture(env3d, pDirect3DDevice);
        }
    }
    __finally
    {
        if (FAILED(hr))
        {
            DXTRACE_ERR("Error occurred while rendering", hr);
        }
        LeaveCriticalSection(&m_critsec);
    }
    
    return hr;
}

HRESULT Primitive::RenderNoTexture(Environment3D* env3d, 
    IDirect3DDevice9* pDirect3DDevice)
{
    bool transform = env3d->IsTransformationEnabled();
    bool lights = env3d->IsLightingEnabled();
    bool colored = m_bColored;
    HRESULT hr = S_OK;
    
    if (!transform && !lights && !colored)     // 0
    {
        hr = RenderNoTransform(env3d, pDirect3DDevice, false /*diffuse*/);
    }
    else if (transform && !lights && !colored) // 1
    {
        hr = RenderWithTransform(env3d, pDirect3DDevice, false /*diffuse*/);
    }
    else if (!transform && lights && !colored) // 2
    {
        // Not allowed
        hr = E_FAIL;
    }
    else if (transform && lights && !colored) // 3
    {
        hr = RenderWithLightsAndTransform(env3d, pDirect3DDevice,
            D3DFVF_XYZ);
    }
    else if (!transform && !lights && colored) // 4
    {
        hr = RenderNoTransform(env3d, pDirect3DDevice, true /*diffuse*/);
    }
    else if (transform && !lights && colored) // 5
    {
        hr = RenderWithTransform(env3d, pDirect3DDevice, true /*diffuse*/);
    }
    else if (!transform && lights && colored) // 6
    {
        // Not allowed
        hr = E_FAIL;
    }
    else if (transform && lights && colored) // 7
    {
        hr = RenderWithLightsAndTransform(env3d, pDirect3DDevice,
            D3DFVF_XYZ | D3DFVF_DIFFUSE);
    }
    
    return hr;
}

HRESULT Primitive::RenderWithTexture(Environment3D* env3d, 
    IDirect3DDevice9* pDirect3DDevice)
{
    bool transform = env3d->IsTransformationEnabled();
    bool lights = env3d->IsLightingEnabled();
    bool colored = m_bColored;
    HRESULT hr = S_OK;
    
    if (!transform && !lights && !colored) // 8
    {
        hr = RenderNoTransformTextured(env3d, pDirect3DDevice, false /*diffuse*/);
    }
    else if (transform && !lights && !colored) // 9
    {
        hr = RenderWithTransformTextured(env3d, pDirect3DDevice, false /*diffuse*/);
    }
    else if (!transform && lights && !colored) // 10
    {
        // Not allowed
        hr = E_FAIL;
    }
    else if (transform && lights && !colored) // 11
    {
        hr = RenderWithLightsAndTransformTextured(env3d, pDirect3DDevice,
            D3DFVF_XYZ | D3DFVF_TEX1);
    }
    else if (!transform && !lights && colored) // 12
    {
        hr = RenderNoTransformTextured(env3d, pDirect3DDevice, true /*diffuse*/);
    }
    else if (transform && !lights && colored) // 13
    {
        hr = RenderWithTransformTextured(env3d, pDirect3DDevice, true /*diffuse*/);
    }
    else if (!transform && lights && colored) // 14
    {
        // Not allowed
        hr = E_FAIL;
    }
    else if (transform && lights && colored) // 15
    {
        hr = RenderWithLightsAndTransformTextured(env3d, pDirect3DDevice,
            D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    }
    
    return hr;
}



HRESULT Primitive::RenderWithLightsAndTransform(Environment3D* env3d, 
    IDirect3DDevice9* pDirect3DDevice, DWORD fvf)
{
    HRESULT hr = S_OK;
    
    // use the non-RHW vertex type 
    //
    // if not dirty use saved mesh 
    // otherwise create new mesh
    if (m_Dirty)
    {
        // Calculate vertex normals and render as a mesh
        
        hr = CreateMesh(env3d, pDirect3DDevice, fvf);
        
        m_Dirty = false;
        env3d->SetResultCode(hr);
        if (FAILED(hr))
        {
            return hr;
        }
    }

    if (m_Material)
    {
        pDirect3DDevice->SetMaterial(m_Material);
    }
    
    if (m_pMesh)
    {
        m_pMesh->DrawSubset(0);
    }
    
    return hr;
}

HRESULT Primitive::RenderWithTransform(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice,
        bool diffuse)
{
    HRESULT hr = S_OK;
    
    DWORD fvf = D3DFVF_XYZ;
    if (diffuse)
    {
        fvf |= D3DFVF_DIFFUSE;
    }
    UINT vertexSize = GetVertexSize(fvf);
    
    // Use the non-RHW vertex type
    // if not dirty use saved vertex buffer 
    //   otherwise create new vertex buffer
    if (m_Dirty)
    {
        hr = CreateVertexBuffer(env3d,
            pDirect3DDevice, 
            fvf);
        
        m_Dirty = false;
        env3d->SetResultCode(hr);
        if (FAILED(hr))
        {
            return hr;
        }
    }
    
    pDirect3DDevice->SetStreamSource(0, m_pVertexBuf,
        0, vertexSize);
    pDirect3DDevice->SetFVF(fvf);
    
    D3DPRIMITIVETYPE primitiveType;
    UINT primitiveCount;
    
    // Convert RenderMode to D3DPT_* constant
    hr = CalculatePrimitiveTypeAndCount(
        &primitiveType, &primitiveCount);
    env3d->SetResultCode(hr);
    if (FAILED(hr))
    {
        return hr;
    }
    
    hr = pDirect3DDevice->DrawPrimitive(
        primitiveType, 0, primitiveCount);
    if (FAILED(hr))
    {
        DXTRACE_ERR("Error occurred while drawing a primitive", hr);
        return hr;
    }
    
    return hr;
}

HRESULT Primitive::RenderNoTransform(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice,
        bool diffuse)
{
    HRESULT hr = S_OK;
    
    DWORD fvf = D3DFVF_XYZRHW;
    if (diffuse)
    {
        fvf |= D3DFVF_DIFFUSE;
    }
    UINT vertexSize = GetVertexSize(fvf);
    
    // If not dirty use saved vertex buffer 
    //   otherwise create new vertex buffer
    if (m_Dirty)
    {
        hr = CreateVertexBuffer(env3d,
            pDirect3DDevice, 
            fvf);
        
        m_Dirty = false;
        env3d->SetResultCode(hr);
        if (FAILED(hr))
        {
            return hr;
        }
    }
    
    pDirect3DDevice->SetStreamSource(0, m_pVertexBuf,
        0, vertexSize);
    pDirect3DDevice->SetFVF(fvf);
    
    D3DPRIMITIVETYPE primitiveType;
    UINT primitiveCount;
    
    // Convert RenderMode to D3DPT_* constant
    hr = CalculatePrimitiveTypeAndCount(
        &primitiveType, &primitiveCount);
    env3d->SetResultCode(hr);
    if (FAILED(hr))
    {
        return hr;
    }
    
    hr = pDirect3DDevice->DrawPrimitive(
        primitiveType, 0, primitiveCount);
    if (FAILED(hr))
    {
        DXTRACE_ERR("Error occurred while drawing a primitive", hr);
        return hr;
    }
    
    return hr;
}

HRESULT Primitive::CalculatePrimitiveTypeAndCount(
        D3DPRIMITIVETYPE* primitiveType, UINT* primitiveCount)
{
    if (NULL == primitiveType || NULL == primitiveCount)
    {
        return E_POINTER;
    }
    switch (m_Mode)
    {
    case POINT_LIST:
        *primitiveType = D3DPT_POINTLIST;
        *primitiveCount = static_cast<UINT>(m_Vertices.size());
        break;
    case LINE_LIST:
        *primitiveType = D3DPT_LINELIST;
        *primitiveCount = static_cast<UINT>(m_Vertices.size()/2);
        break;
    case LINE_STRIP:
        *primitiveType = D3DPT_LINESTRIP;
        *primitiveCount = static_cast<UINT>(m_Vertices.size() > 0 
            ? m_Vertices.size()-1
            : 0);
        break;
    case TRIANGLE_LIST:
        *primitiveType = D3DPT_TRIANGLELIST;
        *primitiveCount = static_cast<UINT>(m_Vertices.size()/3);
        break;
    case TRIANGLE_STRIP:
        *primitiveType = D3DPT_TRIANGLESTRIP;
        *primitiveCount = static_cast<UINT>(m_Vertices.size() > 1 
            ? m_Vertices.size()-2
            : 0);
        break;
    case TRIANGLE_FAN:
        *primitiveType = D3DPT_TRIANGLEFAN;
        *primitiveCount = static_cast<UINT>(m_Vertices.size() > 1 
            ? m_Vertices.size()-2
            : 0);
        break;
    default:
        return E_FAIL;
    }

    return D3D_OK;
}

UINT Primitive::GetVertexSize(DWORD fvf)
{
    UINT vertexSize = 0;
    
    if ((fvf & D3DFVF_DIFFUSE) == D3DFVF_DIFFUSE)
    {
        if ((fvf & D3DFVF_XYZ) == D3DFVF_XYZ)
        {
            vertexSize = sizeof(CUSTOMVERTEX);
        }
        else if ((fvf & D3DFVF_XYZRHW) == D3DFVF_XYZRHW)
        {
            vertexSize = sizeof(CUSTOMVERTEX_RHW);
        }
    }
    else // No diffuse
    {
        if ((fvf & D3DFVF_XYZ) == D3DFVF_XYZ)
        {
            vertexSize = sizeof(CUSTOMVERTEX_BASIC);
        }
        else if ((fvf & D3DFVF_XYZRHW) == D3DFVF_XYZRHW)
        {
            vertexSize = sizeof(CUSTOMVERTEX_RHW_ND);
        }
    }
    
    if ((fvf & D3DFVF_TEX1) == D3DFVF_TEX1)
    {
        vertexSize += 2 * sizeof(FLOAT);
    }
    
    return vertexSize;
}

HRESULT Primitive::CreateVertexBuffer(Environment3D* /*env3d*/, 
        IDirect3DDevice9* pDirect3DDevice, DWORD fvf)
{
    UINT vertexSize = GetVertexSize(fvf);
    
    // Delete old vertex buffer if it exists and create a new one
    if (m_pVertexBuf)
    {
        m_pVertexBuf->Release();
        m_pVertexBuf = NULL;
    }
    
    HRESULT hr = pDirect3DDevice->CreateVertexBuffer(
        static_cast<UINT>(m_Vertices.size()*vertexSize),
        0,
        fvf,
        D3DPOOL_DEFAULT,
        &m_pVertexBuf,
        NULL);
    
    if (FAILED(hr))
    {
        DXTRACE_ERR("Error creating the vertex buffer", hr);
        return E_FAIL;
    }
    
    // Convert m_Vertices to CUSTOMVERTEX[_RHW] or whatever
    
    VOID* pVertices;
    hr = m_pVertexBuf->Lock(0, 0, &pVertices, 0);
    if (FAILED(hr))
    {
        DXTRACE_ERR("Error locking the vertex buffer", hr);
        return hr;
    }
    
    if ((fvf & D3DFVF_DIFFUSE) == D3DFVF_DIFFUSE)
    {
        //
        // Using diffuse
        //
        if ((fvf & D3DFVF_XYZRHW) == D3DFVF_XYZRHW)
        {
            // Transformed vertices
            CUSTOMVERTEX_RHW* verts = new CUSTOMVERTEX_RHW[m_Vertices.size()];
            
            if (verts)
            {
                std::vector<Vertex>::iterator i = m_Vertices.begin();
                int j = 0;
                while (i != m_Vertices.end())
                {
                    Vertex next = *i;
                    verts[j].x = next.x;
                    verts[j].y = next.y;
                    verts[j].z = next.z;
                    verts[j].rhw = next.rhw;
                    verts[j].color = next.color;
                    
                    i++;
                    j++;
                }
                
                memcpy(pVertices, verts, m_Vertices.size()*vertexSize);
                m_pVertexBuf->Unlock();
                
                delete [] verts;
            }
        }
        else if ((fvf & D3DFVF_XYZ) == D3DFVF_XYZ)
        {
            // Untransformed vertices
            CUSTOMVERTEX* verts = new CUSTOMVERTEX[m_Vertices.size()];
            
            if (verts)
            {
                std::vector<Vertex>::iterator i = m_Vertices.begin();
                int j = 0;
                while (i != m_Vertices.end())
                {
                    Vertex next = *i;
                    verts[j].x = next.x;
                    verts[j].y = next.y;
                    verts[j].z = next.z;
                    verts[j].color = next.color;
                    
                    i++;
                    j++;
                }
                
                memcpy(pVertices, verts, m_Vertices.size()*vertexSize);
                m_pVertexBuf->Unlock();
                
                delete [] verts;
            }
        }
    }
    else
    {
        //
        // Not using diffuse
        //
        if ((fvf & D3DFVF_XYZRHW) == D3DFVF_XYZRHW)
        {
            // Transformed vertices
            CUSTOMVERTEX_RHW_ND* verts = new CUSTOMVERTEX_RHW_ND[m_Vertices.size()];
            
            if (verts)
            {
                std::vector<Vertex>::iterator i = m_Vertices.begin();
                int j = 0;
                while (i != m_Vertices.end())
                {
                    Vertex next = *i;
                    verts[j].x = next.x;
                    verts[j].y = next.y;
                    verts[j].z = next.z;
                    verts[j].rhw = next.rhw;
                    
                    i++;
                    j++;
                }
                
                memcpy(pVertices, verts, m_Vertices.size()*vertexSize);
                m_pVertexBuf->Unlock();
                
                delete [] verts;
            }
        }
        else if ((fvf & D3DFVF_XYZ) == D3DFVF_XYZ)
        {
            // Untransformed vertices
            CUSTOMVERTEX_BASIC* verts = new CUSTOMVERTEX_BASIC[m_Vertices.size()];
            
            if (verts)
            {
                std::vector<Vertex>::iterator i = m_Vertices.begin();
                int j = 0;
                while (i != m_Vertices.end())
                {
                    Vertex next = *i;
                    verts[j].x = next.x;
                    verts[j].y = next.y;
                    verts[j].z = next.z;
                    
                    i++;
                    j++;
                }
                
                memcpy(pVertices, verts, m_Vertices.size()*vertexSize);
                m_pVertexBuf->Unlock();
                
                delete [] verts;
            }
        }
    }
    
    return D3D_OK;
}

HRESULT Primitive::CreateMesh(Environment3D* env3d, 
    IDirect3DDevice9* pDirect3DDevice, DWORD fvf)
{
    HRESULT hr = S_OK;
    
    // Delete old mesh if it exists and create a new one
    if (m_pMesh)
    {
        m_pMesh->Release();
        m_pMesh = NULL;
    }
    
    D3DPRIMITIVETYPE primitiveType;
    UINT primitiveCount;
    
    // Convert RenderMode to D3DPT_* constant
    hr = CalculatePrimitiveTypeAndCount(
        &primitiveType, &primitiveCount);
    env3d->SetResultCode(hr);
    if (FAILED(hr))
    {
        return hr;
    }
    
    hr = D3DXCreateMeshFVF(primitiveCount, 
        static_cast<DWORD>(m_Vertices.size()),
        D3DXMESH_MANAGED, fvf, pDirect3DDevice, &m_pMesh);
    env3d->SetResultCode(hr);
    if (FAILED(hr))
    {
        return hr;
    }
    //
    // Load vertices
    //
    LPVOID pVertices = NULL;
    m_pMesh->LockVertexBuffer(0, &pVertices);
    
    if ((fvf & D3DFVF_DIFFUSE) == D3DFVF_DIFFUSE)
    {
        CUSTOMVERTEX* verts = new CUSTOMVERTEX[m_Vertices.size()];
        std::vector<Vertex>::iterator i = m_Vertices.begin();
        int j = 0;
        while (i != m_Vertices.end())
        {
            Vertex next = *i;
            verts[j].x = next.x;
            verts[j].y = next.y;
            verts[j].z = next.z;
            verts[j].color = next.color;
            
            i++;
            j++;
        }
        
        memcpy(pVertices, verts, m_Vertices.size()*sizeof(CUSTOMVERTEX));
        m_pMesh->UnlockVertexBuffer();
        delete verts;
    }
    else
    {
        CUSTOMVERTEX_BASIC* verts = new CUSTOMVERTEX_BASIC[m_Vertices.size()];
        std::vector<Vertex>::iterator i = m_Vertices.begin();
        int j = 0;
        while (i != m_Vertices.end())
        {
            Vertex next = *i;
            verts[j].x = next.x;
            verts[j].y = next.y;
            verts[j].z = next.z;
            
            i++;
            j++;
        }
        
        memcpy(pVertices, verts, m_Vertices.size()*sizeof(CUSTOMVERTEX_BASIC));
        m_pMesh->UnlockVertexBuffer();
        delete verts;
    }

    //
    // Load indices
    //
    WORD* pIndices = NULL;
    m_pMesh->LockIndexBuffer(0, reinterpret_cast<void**>(&pIndices));
    
    // Do something different depending on render mode
    switch (primitiveType)
    {
    case D3DPT_POINTLIST:
        break;
    case D3DPT_LINELIST:
        break;
    case D3DPT_LINESTRIP:
        break;
    case D3DPT_TRIANGLELIST:
        for (WORD i=0; (i+2)<m_Vertices.size(); i+=3)
        {
            pIndices[i] = i;
            pIndices[i+1] = i+1;
            pIndices[i+2] = i+2;
        }
        break;
    case D3DPT_TRIANGLESTRIP:
        for (WORD i=0, j=0; (i+2)<m_Vertices.size(); i+=3, j++)
        {
            pIndices[i] = j;
            pIndices[i+1] = j+1;
            pIndices[i+2] = j+2;
        }
        break;
    case D3DPT_TRIANGLEFAN:
        for (WORD i=0, j=0; (i+2)<m_Vertices.size(); i+=3, j++)
        {
            pIndices[i] = 0;
            pIndices[i+1] = j+1;
            pIndices[i+2] = j+2;
        }
        break;
    }
    m_pMesh->UnlockIndexBuffer();
    
    //
    // Generate normals
    //
    if (!(fvf & D3DFVF_NORMAL))
    {
        fvf |= D3DFVF_NORMAL;
        
        ID3DXMesh* normMesh;
        hr = m_pMesh->CloneMeshFVF(0, fvf, pDirect3DDevice, &normMesh);
        
        env3d->SetResultCode(hr);
        if (FAILED(hr))
        {
            return hr;
        }
        
        m_pMesh->Release();
        m_pMesh = normMesh;
    }
    D3DXComputeNormals(m_pMesh, NULL);
    
    return hr;
}

VOID Primitive::SetMaterial(D3DMATERIAL9* d3dm)
{
    if (!d3dm)
    {
        return;
    }
    
    EnterCriticalSection(&m_critsec);
    // Delete the previous material if there is one
    if (m_Material)
    {
        delete m_Material;
    }
    
    // Take a copy of the material (to be deleted in the destructor)
    m_Material = new D3DMATERIAL9;
    memcpy(m_Material, d3dm, sizeof(D3DMATERIAL9));
    
    LeaveCriticalSection(&m_critsec);
}

VOID Primitive::SetTexture(CTexture* t)
{
    if (!t) return;//TODO: allow a null texture???
    
    EnterCriticalSection(&m_critsec);
    m_pTexture = t;
    LeaveCriticalSection(&m_critsec);
}

CTexture* Primitive::GetTexture(VOID)
{
    CTexture* pTexture;
    EnterCriticalSection(&m_critsec);
    pTexture = m_pTexture;
    LeaveCriticalSection(&m_critsec);
    return pTexture;
}

VOID Primitive::SetColored(bool c)
{
    EnterCriticalSection(&m_critsec);
    m_bColored = c;
    LeaveCriticalSection(&m_critsec);
}

bool Primitive::IsColored(VOID)
{
    bool ret;
    EnterCriticalSection(&m_critsec);
    ret = m_bColored;
    LeaveCriticalSection(&m_critsec);
    return ret;
}

