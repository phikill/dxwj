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
// Class CTexture
//

CTexture::CTexture(VOID) : m_pTexture(NULL),
                           m_pImageSurface(NULL),
                           m_filename(NULL),
                           m_width(0),
                           m_height(0)
{
}

CTexture::CTexture(CTexture& rhs)
{
    m_pTexture = NULL;
    m_pImageSurface = NULL;
    m_filename = NULL;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    
    // Duplicate the interface pointers if not null
    if (rhs.m_pTexture)
    {
        m_pTexture = rhs.m_pTexture;
        m_pTexture->AddRef();
    }
    if (rhs.m_pImageSurface)
    {
        m_pImageSurface = rhs.m_pImageSurface;
        m_pImageSurface->AddRef();
    }
    if (rhs.m_filename)
    {
        m_filename = _strdup(rhs.m_filename);
    }
}

CTexture::~CTexture(VOID)
{
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
    if (m_pImageSurface)
    {
        m_pImageSurface->Release();
        m_pImageSurface = NULL;
    }
    if (m_filename)
    {
        free(const_cast<LPSTR>(m_filename));
        m_filename = NULL;
    }
}

VOID CTexture::SetFilename(LPCSTR filename)
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

VOID CTexture::SetDimensions(UINT width, UINT height)
{
    m_width = width;
    m_height = height;
}

HRESULT CTexture::LoadFromFile(IDirect3DDevice9* pDirect3DDevice)
{
    HRESULT hr = S_OK;
    
    // Load texture if not already loaded
    if (m_filename && pDirect3DDevice && !m_pTexture)
    {
        hr = D3DXCreateTextureFromFile(pDirect3DDevice,
            m_filename, &m_pTexture);
    }
    else
    {
        hr = E_FAIL;
    }
    
    return hr;
}

HRESULT CTexture::GetDxTexture(Environment3D*, 
    IDirect3DDevice9* pDirect3DDevice, IDirect3DTexture9** ppTex)
{
    HRESULT hr = S_OK;
    
    if (!ppTex)
    {
        return E_POINTER;
    }
    
    *ppTex = NULL;
    
    if (!m_pTexture)
    {
        hr = LoadFromFile(pDirect3DDevice);
        
        if (FAILED(hr))
        {
            printf("CTexture::LoadFromFile problem! %X\n", hr);
            return hr;
        }
        else
        {
            printf("CTexture::LoadFromFile OK! %X\n", hr);
            *ppTex = m_pTexture;
            // Conform to the COM interface pointer rules
            (*ppTex)->AddRef();
        }
    }
    else
    {
        *ppTex = m_pTexture;
        (*ppTex)->AddRef();
    }
    
    return hr;
}

HRESULT CTexture::GetDxImage(Environment3D*,
                             IDirect3DDevice9* pDirect3DDevice,
                             IDirect3DSurface9** ppSurface)
{
    HRESULT hr = S_OK;
    
    if (!ppSurface)
    {
        return E_POINTER;
    }
    
    // Init the out parameter
    *ppSurface = NULL;
    
    if (!m_pImageSurface)
    {
        // Create an image surface
        hr = pDirect3DDevice->CreateOffscreenPlainSurface(
            m_width, m_height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT,
            ppSurface, NULL);
        
        if (SUCCEEDED(hr))
        {
            // Load image data into the image surface
            hr = D3DXLoadSurfaceFromFile(*ppSurface,
                NULL, NULL, m_filename, NULL,
                D3DX_DEFAULT, 0, NULL);
            
            if (FAILED(hr))
            {
                printf("D3DXLoadSurfaceFromFile failed %X\n", hr);
            }
            else
            {
                m_pImageSurface = *ppSurface;
                m_pImageSurface->AddRef();
            }
        }
        else
        {
            printf("CreateOffscreenPlainSurface failed %X\n", hr);
        }
    }
    else
    {
        *ppSurface = m_pImageSurface;
        // Conform to the COM interface pointer rules
        (*ppSurface)->AddRef();
    }
    
    return hr;
}


