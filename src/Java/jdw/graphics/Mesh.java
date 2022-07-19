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

import java.io.File;
import java.io.FileNotFoundException;

/**
 * Represents a Direct3D mesh. 
 * <!-- A Mesh object contains a list of Primitives; each representing
 * a subset of the mesh. -->
 *
 * @author Matthew Ross
 */
public class Mesh {
    private long nativePtr;
    private String filename;
    
    private Mesh() {
    }

    /**
     * Creates a new Mesh.
     */
    private Mesh(String filename) {
        if (null == filename) {
            throw new NullPointerException("filename");
        }
        
        this.nativePtr = getNativePtr();
        this.filename = filename;
        
        if (0L == nativePtr) {
            throw new OutOfMemoryError("Native heap exhausted");
        }
    }
    
    long nativePtr() {
        return nativePtr;
    }

    /**
     * Constructs a Mesh object from a .X file. 
     * 
     * @throws NullPointerException if <tt>filename</tt> is <tt>null</tt>
     * @throws FileNotFoundException if the mesh denoted by <tt>filename</tt>
     *                                  does not exist
     */
    public static Mesh fromFile(String filename)
            throws FileNotFoundException {
        Mesh t = new Mesh(filename);
        
        if (!new File(filename).exists()) {
            throw new FileNotFoundException(filename+" does not exist");
        }
        
        t._loadFromFile(t.nativePtr, filename);
        return t;
    }
    
    /**
     * Wrapper for D3DXCreateBox.
     */
    public static Mesh createBox(float width, 
            float height, float depth) {
        Mesh temp = new Mesh();
        temp.d3dxCreateBox(temp.nativePtr, width, height, depth);
        return temp;
    }
    
    /**
     * Wrapper for D3DXCreateCylinder.
     */
    public static Mesh createCylinder(float radius1, 
            float radius2, float length,
            int slices, int stacks) {
        Mesh temp = new Mesh();
        temp.d3dxCreateCylinder(temp.nativePtr, radius1, radius2, length,
            slices, stacks);
        return temp;
    }
    
    /**
     * Wrapper for D3DXCreatePolygon.
     */
    public static Mesh createPolygon() {
        Mesh temp = new Mesh();
        return temp;
    }
    
    /**
     * Wrapper for D3DXCreateSphere.
     */
    public static Mesh createSphere() {
        Mesh temp = new Mesh();
        return temp;
    }
    
    /**
     * Wrapper for D3DXCreateTeapot.
     */
    public static Mesh createTeapot() {
        Mesh temp = new Mesh();
        return temp;
    }
    
    /**
     * Wrapper for D3DXCreateText.
     */
    public static Mesh createText() {
        Mesh temp = new Mesh();
        return temp;
    }
    
    /**
     * Wrapper for D3DXCreateTorus.
     */
    public static Mesh createTorus() {
        Mesh temp = new Mesh();
        return temp;
    }
    
    /*
     * Frees native resources associated with this Mesh.
     */
    /*public*/ void dispose() {
        if (0L == nativePtr) {
            throw new RuntimeException("Object already disposed");
        }
        
        _dispose(nativePtr);
        nativePtr = 0L;
    }
    
    //
    // Native methods
    //
    
    private native long getNativePtr();
    private native void _loadFromFile(long nativePtr, String filename);
    private native void d3dxCreateBox(long nativePtr, 
        float width, float height, float depth);
    private native void d3dxCreateCylinder(long nativePtr, 
        float radius1, float radius2, float length,
        int slices, int stacks);
    //private native void d3dxCreatePolygon();
    //private native void d3dxCreateSphere();
    //private native void d3dxCreateTeapot();
    //private native void d3dxCreateText();
    //private native void d3dxCreateTorus();
    private native void _dispose(long nativePtr);
}

