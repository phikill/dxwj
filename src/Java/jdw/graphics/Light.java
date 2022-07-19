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

package jdw.graphics;

/**
 * Identifies a source of light. Multiple source lights
 * can be placed into Environment3Ds.
 *
 * @author Matthew Ross
 */
public abstract class Light {
    private long nativePtr;
    
    static final int D3DLIGHT_POINT = 1;
    static final int D3DLIGHT_SPOT = 2;
    static final int D3DLIGHT_DIRECTIONAL = 3;

    static {
        LibraryLoader.load("DXRenderer");
    }
    
    /**
     * Creates a new Light.
     */
    Light() {
        nativePtr = getNativePtr();
        if (0L == nativePtr) {
            throw new OutOfMemoryError("Native heap exhausted");
        }
    }
    
    /*
     * Frees native resources held by this object.
     */
    /*public*/ void dispose() {
        if (0L == nativePtr) {
            throw new RuntimeException("Light already disposed");
        }
        
        _dispose(nativePtr);
        nativePtr = 0L;
    }
    
    /**
     * Returns the pointer to the representative C++ Light object.
     */
    long nativePtr() {
        return nativePtr;
    }
    
    //
    // Native methods
    //
    
    /**
     * Creates and returns a pointer to a representative C++ Light object.
     */
    private native long getNativePtr();
    
    /**
     * Deletes the C++ Light object.
     */
    private native void _dispose(long nativePtr);
    
    /**
     * @param type Valid light types are:
     *                  D3DLIGHT_POINT = 1,
     *                  D3DLIGHT_SPOT = 2,
     *                  D3DLIGHT_DIRECTIONAL = 3.
     */
    native void setLightParams(
        long nativePtr,
        int type,
        int diffuseR,
        int diffuseG,
        int diffuseB,
        int specularR,
        int specularG,
        int specularB,
        float positionX,
        float positionY,
        float positionZ,
        float directionX,
        float directionY,
        float directionZ);
    
    /**
     * For use by subclasses (PointLight, DirectionalLight, SpotLight).
     */
    void setLightParams(int type,
        int diffuseR,
        int diffuseG,
        int diffuseB,
        int specularR,
        int specularG,
        int specularB,
        float positionX,
        float positionY,
        float positionZ,
        float directionX,
        float directionY,
        float directionZ)
    {
        if (0L == nativePtr) {
            throw new RuntimeException("Light has been disposed");
        }
        setLightParams(nativePtr, type,
            diffuseR, diffuseG, diffuseB,
            specularR, specularG, specularB,
            positionX, positionY, positionZ,
            directionX, directionY, directionZ);
    }
}

