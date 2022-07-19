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


#include "DXRenderer.h"

//
// Class CMesh
//

CMesh::CMesh(VOID) : m_pMesh(NULL), m_filename(NULL),
                     materialBuffer(NULL), numMaterials(0), 
                     meshMaterials(NULL), meshTextures(NULL)
{
}

CMesh::~CMesh(VOID)
{
    if (m_pMesh)
    {
        m_pMesh->Release();
        m_pMesh = NULL;
    }
    if (m_filename)
    {
        free(const_cast<LPSTR>(m_filename));
        m_filename = NULL;
    }    
    if (materialBuffer)
    {
        materialBuffer->Release();
        materialBuffer = NULL;
    }
    if (meshMaterials)
    {
        delete [] meshMaterials;
        meshMaterials = NULL;
    }
    if (meshTextures)
    {
        delete [] meshTextures;
        meshTextures = NULL;
    }
}

VOID CMesh::SetFilename(LPCSTR filename)
{
    if (filename)
    {
        if (m_filename)
        {
            free(const_cast<LPSTR>(m_filename));
        }
        m_filename = _strdup(filename);
    }
}

VOID CMesh::SetMesh(ID3DXMesh* pMesh)
{
    // Used by Mesh.d3dxCreateBox, etc. native methods
    HRESULT hr = S_OK;
    
    if (!pMesh)
        hr = E_POINTER;
    
    if (pMesh)
    {
        m_pMesh = pMesh;
        m_pMesh->AddRef();
    }
}

HRESULT CMesh::LoadFromFile(IDirect3DDevice9* pDirect3DDevice)
{
    HRESULT hr = S_OK;
    
    // Load mesh if not already loaded
    if (m_filename && pDirect3DDevice && !m_pMesh)
    {
        // Load the mesh from the specified file
        hr = D3DXLoadMeshFromX(m_filename, D3DXMESH_SYSTEMMEM, 
                             pDirect3DDevice, NULL, 
                             &materialBuffer, NULL, &numMaterials, 
                             &m_pMesh);
        
        D3DXMATERIAL* d3dxMaterials 
            = static_cast<D3DXMATERIAL*>(materialBuffer->GetBufferPointer());
        
        meshMaterials = new D3DMATERIAL9[numMaterials];
        meshTextures = new LPDIRECT3DTEXTURE9[numMaterials];

        for (DWORD i=0; 
            meshMaterials && meshTextures && i<numMaterials; 
            i++)
        {
             // Copy the material
             meshMaterials[i] = d3dxMaterials[i].MatD3D;
            
             // Set the ambient color for the material (D3DX does not do this)
             meshMaterials[i].Ambient = meshMaterials[i].Diffuse;
                 
             // Create the texture if it exists - it may not
             meshTextures[i] = NULL;
             if (d3dxMaterials[i].pTextureFilename)
             {
                 D3DXCreateTextureFromFile(pDirect3DDevice, 
                     d3dxMaterials[i].pTextureFilename,
                     &meshTextures[i]);
             }
        }
    }
    else
    {
        hr = E_FAIL;
    }
    
    return hr;
}

HRESULT CMesh::Render(Environment3D*, IDirect3DDevice9* pDirect3DDevice)
{
    HRESULT hr = S_OK;
    
    if (!m_pMesh)
    {
        hr = LoadFromFile(pDirect3DDevice);
        
        if (FAILED(hr))
        {
            printf("CMesh::LoadFromFile problem! %X\n", hr);
            return hr;
        }
        else
        {
            printf("CMesh::LoadFromFile OK! %X\n", hr);
        }
    }
    
    if (m_pMesh && meshMaterials && meshTextures)
    {
        for (DWORD i=0; i<numMaterials; i++)
        {
            // Set the material and texture for this subset
            hr = pDirect3DDevice->SetMaterial(&meshMaterials[i]);
            if (FAILED(hr)) return hr;
            
            hr = pDirect3DDevice->SetTexture(0, meshTextures[i]);
            if (FAILED(hr)) return hr;
            
            // Draw the mesh subset
            hr = m_pMesh->DrawSubset(i);
            if (FAILED(hr)) return hr;
        }
    }
    return hr;
}

