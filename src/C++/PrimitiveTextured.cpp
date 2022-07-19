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

//
// PrimitiveTextured.cpp
//
// Contains functions to render textured primitives.
//
#include <assert.h>
#include "DXRenderer.h"

HRESULT Primitive::RenderWithLightsAndTransformTextured(Environment3D* env3d, 
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
        
        hr = CreateMeshTextured(env3d, pDirect3DDevice, fvf);
        
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
    if (m_pTexture)
    {
        IDirect3DTexture9* pTex = NULL;
        hr = m_pTexture->GetDxTexture(env3d, pDirect3DDevice, &pTex);
        if (SUCCEEDED(hr))
        {
            pDirect3DDevice->SetTexture(0, pTex);
            pTex->Release();
            pTex = NULL;
        }
        else
        {
            printf("Unable to set a texture\n");
        }
    }
    if (m_pMesh)
    {
        m_pMesh->DrawSubset(0);
    }
    
    return hr;
}

HRESULT Primitive::RenderWithTransformTextured(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice,
        bool diffuse)
{
    HRESULT hr = S_OK;
    
    DWORD fvf = D3DFVF_XYZ | D3DFVF_TEX1;
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
        hr = CreateVertexBufferTextured(env3d,
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
    
    if (m_pTexture)
    {
        IDirect3DTexture9* pTex = NULL;
        hr = m_pTexture->GetDxTexture(env3d, pDirect3DDevice, &pTex);
        if (SUCCEEDED(hr))
        {
            pDirect3DDevice->SetTexture(0, pTex);
            pTex->Release();
            pTex = NULL;
        }
        else
        {
            printf("Unable to set a texture\n");
        }
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

HRESULT Primitive::RenderNoTransformTextured(Environment3D* env3d, 
        IDirect3DDevice9* pDirect3DDevice,
        bool diffuse)
{
    HRESULT hr = S_OK;
    
    DWORD fvf = D3DFVF_XYZRHW | D3DFVF_TEX1;
    if (diffuse)
    {
        fvf |= D3DFVF_DIFFUSE;
    }
    UINT vertexSize = GetVertexSize(fvf);
    
    // If not dirty use saved vertex buffer 
    //   otherwise create new vertex buffer
    if (m_Dirty)
    {
        hr = CreateVertexBufferTextured(env3d,
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
    
    if (m_pTexture)
    {
        IDirect3DTexture9* pTex = NULL;
        hr = m_pTexture->GetDxTexture(env3d, pDirect3DDevice, &pTex);
        if (SUCCEEDED(hr))
        {
            pDirect3DDevice->SetTexture(0, pTex);
            pTex->Release();
            pTex = NULL;
        }
        else
        {
            printf("Unable to set a texture\n");
        }
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

HRESULT Primitive::CreateVertexBufferTextured(Environment3D* /*env3d*/, 
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
            CUSTOMVERTEX_RHW_TEX* verts 
                = new CUSTOMVERTEX_RHW_TEX[m_Vertices.size()];
            
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
                    verts[j].tu = next.tu;
                    verts[j].tv = next.tv;
                    
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
            CUSTOMVERTEX_TEX* verts = new CUSTOMVERTEX_TEX[m_Vertices.size()];
            
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
                    verts[j].tu = next.tu;
                    verts[j].tv = next.tv;
                    
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
            CUSTOMVERTEX_RHW_ND_TEX* verts 
                = new CUSTOMVERTEX_RHW_ND_TEX[m_Vertices.size()];
            
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
                    verts[j].tu = next.tu;
                    verts[j].tv = next.tv;
                    
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
            CUSTOMVERTEX_BASIC_TEX* verts 
                = new CUSTOMVERTEX_BASIC_TEX[m_Vertices.size()];
            
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
                    verts[j].tu = next.tu;
                    verts[j].tv = next.tv;
                    
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

HRESULT Primitive::CreateMeshTextured(Environment3D* env3d, 
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
        CUSTOMVERTEX_TEX* verts = new CUSTOMVERTEX_TEX[m_Vertices.size()];
        std::vector<Vertex>::iterator i = m_Vertices.begin();
        int j = 0;
        while (i != m_Vertices.end())
        {
            Vertex next = *i;
            verts[j].x = next.x;
            verts[j].y = next.y;
            verts[j].z = next.z;
            verts[j].color = next.color;
            verts[j].tu = next.tu;
            verts[j].tv = next.tv;
            
            i++;
            j++;
        }
        
        memcpy(pVertices, verts, m_Vertices.size()*sizeof(CUSTOMVERTEX_TEX));
        m_pMesh->UnlockVertexBuffer();
        delete verts;
    }
    else
    {
        CUSTOMVERTEX_BASIC_TEX* verts 
            = new CUSTOMVERTEX_BASIC_TEX[m_Vertices.size()];
        std::vector<Vertex>::iterator i = m_Vertices.begin();
        int j = 0;
        while (i != m_Vertices.end())
        {
            Vertex next = *i;
            verts[j].x = next.x;
            verts[j].y = next.y;
            verts[j].z = next.z;
            verts[j].tu = next.tu;
            verts[j].tv = next.tv;
            
            i++;
            j++;
        }
        
        memcpy(pVertices, verts, 
            m_Vertices.size()*sizeof(CUSTOMVERTEX_BASIC_TEX));
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


