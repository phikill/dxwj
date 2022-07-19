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

using std::vector;

Environment3D::Environment3D(VOID) : m_hWnd(0),
        m_hResult(D3D_OK),
        m_pDirect3D(NULL),
        m_pDirect3DDevice(NULL),
        m_lighting(false),
        m_transformCoords(false),
        m_ambientColor(0),
        m_backColor(0),
        m_backImage(NULL)
{
    InitializeCriticalSection(&m_critsec);
    
    // Init 3 matrices to the identity
    D3DXMatrixIdentity(&m_WorldMatrix);
    D3DXMatrixIdentity(&m_ViewMatrix);
    D3DXMatrixIdentity(&m_ProjectionMatrix);
}

Environment3D::~Environment3D(VOID)
{
    if (m_pDirect3DDevice)
    {
        m_pDirect3DDevice->Release();
        m_pDirect3DDevice = NULL;
    }
    if (m_pDirect3D)
    {
        m_pDirect3D->Release();
        m_pDirect3D = NULL;
    }
    if (m_backImage)
    {
        delete m_backImage;
        m_backImage = NULL;
    }
    DeleteCriticalSection(&m_critsec);
}

/*
 * Initialises Direct3D.
 */
HRESULT Environment3D::InitDirect3D(VOID)
{
    m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (NULL == m_pDirect3D)
    {
        return E_FAIL;
    }
    D3DDISPLAYMODE d3ddisplaymode;
    HRESULT hr = m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, 
        &d3ddisplaymode);

    if (hr != D3D_OK)
    {
        return E_FAIL;
    }

    D3DPRESENT_PARAMETERS D3DPresentParams;
    ZeroMemory(&D3DPresentParams, sizeof(D3DPRESENT_PARAMETERS));
    D3DPresentParams.Windowed = TRUE;
    D3DPresentParams.BackBufferCount = 1;
    D3DPresentParams.BackBufferFormat = d3ddisplaymode.Format;
    D3DPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    D3DPresentParams.hDeviceWindow = m_hWnd;
    D3DPresentParams.EnableAutoDepthStencil = TRUE;
    D3DPresentParams.AutoDepthStencilFormat = D3DFMT_D16;

    // Hardware vertex processing
    hr = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &D3DPresentParams, &m_pDirect3DDevice);

    printf("CreateDevice returned %X\n", hr);
    
    if (FAILED(hr))
    {
        // Sofware vertex processing
        hr = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &D3DPresentParams, &m_pDirect3DDevice);
        
        printf("CreateDevice returned %X\n", hr);
        
        if (FAILED(hr))
        {
            // Create the reference rasterizer
            hr = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
                D3DDEVTYPE_REF, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                &D3DPresentParams, &m_pDirect3DDevice);
            
            printf("CreateDevice returned %X\n", hr);
        
            if (FAILED(hr))
            {
                return hr;
            }
        }
    }
    m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, m_lighting);
    m_pDirect3DDevice->SetRenderState(D3DRS_SPECULARENABLE, m_lighting);
    m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, m_ambientColor);
    m_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    
    return D3D_OK;
}

VOID Environment3D::UpdateBackBufferSize(int w, int h)
{
    __try
    {
        EnterCriticalSection(&m_critsec);
        
    }
    __finally
    {
        LeaveCriticalSection(&m_critsec);
    }
    
    UNREFERENCED_PARAMETER(w);
    UNREFERENCED_PARAMETER(h);
}

VOID Environment3D::SetHWND(HWND hwnd)
{
    __try
    {
        EnterCriticalSection(&m_critsec);
        if (0 == m_hWnd)            // Init only if not yet inited
        {
            m_hWnd = hwnd;
            m_hResult = InitDirect3D();
        }
    }
    __finally
    {
        LeaveCriticalSection(&m_critsec);
    }
}

VOID Environment3D::Render(VOID)
{
    EnterCriticalSection(&m_critsec);
    if (FAILED(m_hResult))
    {
        LeaveCriticalSection(&m_critsec);
        return;
    }

    m_pDirect3DDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
        m_backColor, 1.0f, 0);

    if (m_backImage)
    {
        // Get the image surface from the CTexture object
        IDirect3DSurface9* pImgSurface = NULL;
        m_hResult = m_backImage->GetDxImage(this, 
            m_pDirect3DDevice, &pImgSurface);
        
        if (FAILED(m_hResult))
        {
            printf("Getting background image failed %X\n", m_hResult);
            
            m_hResult = S_OK; // Continue regardless
        }
        else if (pImgSurface)
        {
            // Get the back buffer
            IDirect3DSurface9* pBackBuffer = NULL;
            
            m_hResult = m_pDirect3DDevice->GetBackBuffer(0, 0,
                D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
            
            if (FAILED(m_hResult))
            {
                printf("GetBackBuffer failed %X\n", m_hResult);
                m_hResult = S_OK; // Continue regardless
            }
            else if (pBackBuffer)
            {
                // Got back buffer
                // Display the image as the background

                m_pDirect3DDevice->StretchRect(pImgSurface, NULL, 
                    pBackBuffer, NULL, D3DTEXF_NONE);
            }
            pImgSurface->Release();
        }
    }
    
    if (IsTransformationEnabled())
    {
        SetupTransformations();
    }
    else
    {
        // Make sure there are no transformations enabled
        // TODO
    }
    
    SetupLights(IsLightingEnabled());
    
    m_pDirect3DDevice->BeginScene();
    //
    // Ask each primitive in the list to render itself
    //
    std::vector<Primitive*>::iterator i = m_primitiveList.begin();
    while (i != m_primitiveList.end())
    {
        Primitive* next = *i;
        m_hResult = next->Render(this, m_pDirect3DDevice);
        
        //printf("Primitive->Render: %X\n", m_hResult);
        
        if (FAILED(m_hResult))
        {
            m_pDirect3DDevice->EndScene();
            m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
        
            LeaveCriticalSection(&m_critsec);
            return;
        }
        i++;
    }
    
    //
    // Ask each mesh in the list to render itself
    //
    std::vector<CMesh*>::iterator j = m_meshList.begin();
    while (j != m_meshList.end())
    {
        CMesh* next = *j;
        m_hResult = next->Render(this, m_pDirect3DDevice);
        
        if (FAILED(m_hResult))
        {
            m_pDirect3DDevice->EndScene();
            m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
        
            LeaveCriticalSection(&m_critsec);
            return;
        }
        j++;
    }
    
    m_pDirect3DDevice->EndScene();
    m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);

    LeaveCriticalSection(&m_critsec);
}

VOID Environment3D::Dispose(VOID)
{
}

VOID Environment3D::AddPrimitive(Primitive* p)
{
    __try
    {
        EnterCriticalSection(&m_critsec);
        m_primitiveList.push_back(p);// TODO: Copy the primitive????
    }
    __finally
    {
        LeaveCriticalSection(&m_critsec);
    }
}

VOID Environment3D::RemovePrimitive(Primitive* p)
{
    EnterCriticalSection(&m_critsec);
    std::vector<Primitive*>::iterator i = m_primitiveList.begin();
    while (i != m_primitiveList.end())
    {
        if (*i == p)
        {
            break;
        }
        i++;
    }
    if (i != m_primitiveList.end())
    {
        m_primitiveList.erase(i);
    }

    LeaveCriticalSection(&m_critsec);
}

VOID Environment3D::AddMesh(CMesh* m)
{
    __try
    {
        EnterCriticalSection(&m_critsec);
    
        if (m)
        {
            m_meshList.push_back(m);
        }
    }
    __finally
    {
        LeaveCriticalSection(&m_critsec);
    }
}

VOID Environment3D::SetLightingEnabled(bool lighting)
{
    EnterCriticalSection(&m_critsec);
    m_lighting = lighting;
    if (m_pDirect3DDevice)
    {
        m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, lighting);
        m_pDirect3DDevice->SetRenderState(D3DRS_SPECULARENABLE, lighting);
        m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, m_ambientColor);
    }
    LeaveCriticalSection(&m_critsec);
}

VOID Environment3D::SetAmbientLight(DWORD color)
{
    EnterCriticalSection(&m_critsec);
    m_ambientColor = color;
    if (m_pDirect3DDevice)
    {
        m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, color);
    }
    LeaveCriticalSection(&m_critsec);
}

bool Environment3D::IsLightingEnabled(VOID)
{
    bool ret;
    EnterCriticalSection(&m_critsec);
    ret = m_lighting;
    LeaveCriticalSection(&m_critsec);
    return ret;
}

VOID Environment3D::SetTransformationEnabled(bool transform)
{
    EnterCriticalSection(&m_critsec);
    m_transformCoords = transform;
    LeaveCriticalSection(&m_critsec);
}

bool Environment3D::IsTransformationEnabled(VOID)
{
    bool ret;
    EnterCriticalSection(&m_critsec);
    ret = m_transformCoords;
    LeaveCriticalSection(&m_critsec);
    return ret;
}

VOID Environment3D::SetResultCode(HRESULT hr)
{
    EnterCriticalSection(&m_critsec);
    m_hResult = hr;
    LeaveCriticalSection(&m_critsec);
}

HRESULT Environment3D::GetResultCode(VOID)
{
    HRESULT ret;
    EnterCriticalSection(&m_critsec);
    ret = m_hResult;
    LeaveCriticalSection(&m_critsec);
    return ret;
}

VOID Environment3D::SetWorldMatrix(D3DXMATRIX* mat)
{
    EnterCriticalSection(&m_critsec);
    m_WorldMatrix = *mat;
    LeaveCriticalSection(&m_critsec);
}

VOID Environment3D::SetViewMatrix(D3DXMATRIX* mat)
{
    EnterCriticalSection(&m_critsec);
    m_ViewMatrix = *mat;
    LeaveCriticalSection(&m_critsec);
}

VOID Environment3D::SetProjectionMatrix(D3DXMATRIX* mat)
{
    EnterCriticalSection(&m_critsec);
    m_ProjectionMatrix = *mat;
    LeaveCriticalSection(&m_critsec);
}

VOID Environment3D::SetupTransformations(VOID)
{
    // Note: The call to this method is inside the critical section
    m_pDirect3DDevice->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
    m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &m_ProjectionMatrix);
}

VOID Environment3D::SetupLights(BOOL enable)
{
    // Note: The call to this method is inside the critical section
    
    vector<Light*>::iterator i = m_lightList.begin();
    int lightIdx = 0; // Each light needs an index value
    
    while (i != m_lightList.end())
    {
        Light* light = *i;
        // Get a local copy of the Direct3D light
        D3DLIGHT9 dxLight = light->GetDxLight();
        
        if (m_pDirect3DDevice != NULL)
        {
            m_pDirect3DDevice->SetLight(lightIdx, &dxLight);
            m_pDirect3DDevice->LightEnable(lightIdx, enable);
            light->SetLightIndex(lightIdx);
        }
        i++;
        lightIdx++;
    }
}

VOID Environment3D::AddSourceLight(Light* light)
{
    if (light)
    {
        EnterCriticalSection(&m_critsec);
        m_lightList.push_back(light);
        LeaveCriticalSection(&m_critsec);
    }
}

VOID Environment3D::RemoveSourceLight(Light* light)
{
    if (light)
    {
        EnterCriticalSection(&m_critsec);
        
        vector<Light*>::iterator i = m_lightList.begin();
        
        while (i != m_lightList.end())
        {
            if (*i == light)
            {
                break;
            }
            i++;
        }
        
        if (i != m_lightList.end())
        {
            m_lightList.erase(i);
        }
        
        //
        // Java Light object has been removed
        // so disable the light in Direct3D
        //
        int lightIdx = light->GetLightIndex();
        
        if (lightIdx >= 0 && m_pDirect3DDevice != NULL)
        {
            m_pDirect3DDevice->LightEnable(lightIdx, FALSE);
        }
        
        LeaveCriticalSection(&m_critsec);
    }
}

VOID Environment3D::SetBackground(DWORD color)
{
    EnterCriticalSection(&m_critsec);
    m_backColor = color;
    LeaveCriticalSection(&m_critsec);
}

VOID Environment3D::SetBackground(CTexture* pTex)
{
    EnterCriticalSection(&m_critsec);
    
    if (m_backImage)
    {
        delete m_backImage;
        m_backImage = NULL;
    }
    
    // Create a copy of pTex
    if (pTex)
    {
        m_backImage = new CTexture(*pTex);
    }
    
    LeaveCriticalSection(&m_critsec);
}

