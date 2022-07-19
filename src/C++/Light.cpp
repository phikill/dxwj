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
// Class Light
//

Light::Light()
{
    ZeroMemory(&dxLight, sizeof(dxLight));
    InitializeCriticalSection(&csec);//TODO error check
    dxLightIndex = -1; // Set to a value >= 0 when enabling lights
}

Light::~Light()
{
    DeleteCriticalSection(&csec);
}

D3DLIGHT9 Light::GetDxLight()
{
    D3DLIGHT9 tempLight;
    EnterCriticalSection(&csec);
    tempLight = dxLight;
    LeaveCriticalSection(&csec);
    return tempLight;
}

void Light::SetDxLight(D3DLIGHT9 newLight)
{
    EnterCriticalSection(&csec);
    dxLight = newLight;
    LeaveCriticalSection(&csec);
}

// The two methods below are called within the Environment3D critical section
int Light::GetLightIndex()
{
    // dxLightIndex is the index of the light
    // used when calling IDirect3DDevice9::SetLight, LightEnable etc.
    return dxLightIndex;
}

void Light::SetLightIndex(int lightIdx)
{
    dxLightIndex = lightIdx;
}

