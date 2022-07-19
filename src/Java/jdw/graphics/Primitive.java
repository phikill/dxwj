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

import java.awt.Color;
import java.util.List;
import java.util.ArrayList;

/**
 * Primitives are objects that can be placed into an <tt>Environment3D</tt>.
 * Positions of objects are based on a left-handed coordinate system.
 *
 * @author Matthew Ross
 */
public class Primitive {
    private static List<Primitive> primitiveList;
    
    private long nativePtr;
    private RenderMode renderMode;
    private Material refLight;
    private boolean colored;
    private Texture texture;
    private Matrix transform;
    private List<Vertex> defaultVertices;
    
    static { 
        LibraryLoader.load("DXRenderer");
        primitiveList = new ArrayList<Primitive>();
    }

    /**
     * Creates a new <tt>Primitive</tt> with a user specified render mode.
     *
     * @param rm the render mode specifiying how the vertices of the primitive
     *                  should be interpreted
     * @see RenderMode
     */
    public Primitive(RenderMode rm) {
        if (null == rm) {
            throw new NullPointerException("rm");
        }
        
        nativePtr = init();
        if (0L == nativePtr) {
            throw new OutOfMemoryError("Native heap exhausted");
        }
        
        colored = true;
        texture = null;
        setRenderMode(rm);
        setMaterial(Material.getDefault());
        transform = Matrix.identity();
        defaultVertices = new ArrayList<Vertex>();
        
        // Add to a list so we can dispose of all Primitives later
        synchronized (Primitive.class) {
            primitiveList.add(this);
        }
    }
    
    /**
     * Used by the Environment3D class when adding primitives to 3D environments.
     */
    long getNativePtr() {
        return nativePtr;
    }
    
    /**
     * Sets the transformation matrix for this primitive.
     * This method can be used to manipulate the positions of individual
     * primtives.
     *
     * @param m the matrix to be used as this primitive's transformation,
     *           or <tt>null</tt> for no transformation
     */
    public synchronized void setTransform(Matrix m) {
        if (null == m) {
            // Set back to default values
            setDefaultVertices();
            transform = Matrix.identity();
        } else {
            transformVertices(m);
            transform = m;
        }
    }
    
    /**
     * Returns the current primitive transformation matrix.
     * If no transformation is being used (e.g. null was passed
     * to setTransform) the returned matrix is the identity matrix.
     *
     * @return the current primitive transformation matrix
     */
    public synchronized Matrix getTransform() {
        return transform;
    }
    
    private synchronized void setDefaultVertices() {
        for (int i=0; i<defaultVertices.size(); i++) {
            Vertex vertex = defaultVertices.get(i);
            
            Color c = vertex.getColor();
            _setVertexAt(nativePtr, i,
                vertex.getX(), vertex.getY(), vertex.getZ(), 1.0f, 
                c != null ? c.getRGB() : 0,
                vertex.getU(), vertex.getV());
        }
    }
    
    private synchronized void transformVertices(Matrix m) {
        for (int i=0; i<defaultVertices.size(); i++) {
            Vertex vertex = defaultVertices.get(i);
            
            // Transform the current vertex according
            // to the matrix
            
            float x1 = vertex.getX();
            float y1 = vertex.getY();
            float z1 = vertex.getZ();
            float x2 = x1 * m.getValueAt(0, 0)
                       + y1 * m.getValueAt(1, 0)
                       + z1 * m.getValueAt(2, 0)
                       + m.getValueAt(3, 0);
            float y2 = x1 * m.getValueAt(0, 1)
                       + y1 * m.getValueAt(1, 1)
                       + z1 * m.getValueAt(2, 1)
                       + m.getValueAt(3, 1);
            float z2 = x1 * m.getValueAt(0, 2)
                       + y1 * m.getValueAt(1, 2)
                       + z1 * m.getValueAt(2, 2)
                       + m.getValueAt(3, 2);
            
            Color c = vertex.getColor();
            _setVertexAt(nativePtr, i,
                x2, y2, z2, 1.0f, 
                c != null ? c.getRGB() : 0,
                vertex.getU(), vertex.getV());
        }
    }
    
    /**
     * Adds a vertex to this primitive. The Vertex object data is copied
     * into this primitive.
     *
     * @param vertex the vertex to add
     * @throws NullPointerException if <tt>vertex</tt> was <tt>null</tt>
     */
    public synchronized void addVertex(Vertex vertex) {
        if (null == vertex) {
            throw new NullPointerException("vertex");
        }
        
        Color c = vertex.getColor();
        _addPositionColored(nativePtr, 
            vertex.getX(), vertex.getY(), vertex.getZ(), 1.0f, 
            c != null ? c.getRGB() : 0,
            vertex.getU(), vertex.getV());
        defaultVertices.add(vertex);
    }
    
    ///**
    // * Removes a single vertex from this primitive.
    // */
    //public void removeVertex(Vertex v) {
    //    _removeVertex(v);
    //}
    
    /**
     * Returns the render mode i.e. how edges are calculated
     * during the rendering process.
     *
     * @return the render mode
     */
    public RenderMode getRenderMode() {
        return renderMode;
    } 
    
    /*
     * Changes the render mode.
     *
     * @param mode the new render mode
     * @throws NullPointerException if <tt>mode</tt> is <tt>null</tt>
     */
    private void setRenderMode(RenderMode mode) {
        if (null == mode) {
            throw new NullPointerException();
        }

        _setRenderMode(nativePtr, mode);
        renderMode = mode;
    }
    
    /**
     * Returns the current Material.
     *
     * @return the current material
     */
    public synchronized Material getMaterial() {
        return refLight;
    }
    
    /**
     * Sets the current Material.
     *
     * @param m the new material
     */
    public synchronized void setMaterial(Material m) {
        if (null == m) {
            throw new NullPointerException("m");
        }
        
        _setMaterial(nativePtr, m.getNativePtr());
        refLight = m;
    }
    
    /**
     * Sets whether the vertices of this primitive should
     * be colored according to the vertex colors.
     * It may be the case that some or all of the vertices of
     * this primitive have a null color specified. In this case
     * the color black is used. The default value for this
     * property is true.
     */
    public synchronized void setColored(boolean c) {
        _setColored(nativePtr, c);
        colored = c;
    }
    
    /**
     * Returns whether the vertices of this primitive are
     * colored according to the vertex colors.
     */
    public synchronized boolean isColored() {
        return colored;
    }
    
    /**
     * The texture/skin to use for this primitive, or <tt>null</tt>
     * if no texture is required.
     */
    public synchronized void setTexture(Texture t) {
        _setTexture(nativePtr, t.nativePtr());
        texture = t;
    }
    
    /**
     * Returns the current texture, or <tt>null</tt> if no
     * texture is in use.
     */
    public synchronized Texture getTexture() {
        return texture;
    }
    
    /**
     * Frees native resources held by this object. Call this method
     * after the Environment3D containing it has been disposed.
     */
    void dispose() {
        _dispose(nativePtr);
    }
    
    /*
     * For use by the VM shutdown hook.
     * Disposes of all primitive objects.
     */
    static synchronized void disposeAll() {
        for (Primitive p : primitiveList) {
            if (p != null) {
                p.dispose();
            }
        }
    }
    
    //
    // Some convenience methods for creating common primitives
    //
    /**
     * Creates a triangle with width 1, centered around the origin.
     */
    public static Primitive createTriangle() {
        Primitive triangle = new Primitive(RenderMode.TRIANGLE_STRIP);
        triangle.addVertex(new Vertex(0.5f, -0.5f, 0f, Color.RED));
        triangle.addVertex(new Vertex(-0.5f, -0.5f, 0f, Color.GREEN));
        triangle.addVertex(new Vertex(0f, 0.5f, 0f, Color.BLUE));
        return triangle;
    }
    
    /**
     * Creates a cube with side length 1, centered around the origin.
     */
    public static Primitive createCube() {
        Primitive cube = new Primitive(RenderMode.TRIANGLE_LIST);
        
        // Front of cube
        cube.addVertex(new Vertex( -0.5f, 0.5f, -0.5f, Color.BLUE));
        cube.addVertex(new Vertex(0.5f, -0.5f, -0.5f, Color.BLUE));
        cube.addVertex(new Vertex(-0.5f,  -0.5f, -0.5f, Color.YELLOW));
        cube.addVertex(new Vertex( -0.5f, 0.5f, -0.5f, Color.BLUE));
        cube.addVertex(new Vertex(0.5f,  0.5f, -0.5f, Color.BLUE));
        cube.addVertex(new Vertex(0.5f, -0.5f, -0.5f, Color.BLUE));

        // Right side of cube
        cube.addVertex(new Vertex(0.5f, 0.5f, -0.5f, Color.RED));
        cube.addVertex(new Vertex(0.5f, -0.5f, 0.5f, Color.RED));
        cube.addVertex(new Vertex(0.5f,  -0.5f, -0.5f, Color.BLUE));
        cube.addVertex(new Vertex(0.5f, 0.5f, -0.5f, Color.RED));
        cube.addVertex(new Vertex(0.5f,  0.5f, 0.5f, Color.RED));
        cube.addVertex(new Vertex(0.5f, -0.5f, 0.5f, Color.RED));

        // Back of cube
        cube.addVertex(new Vertex(0.5f,  0.5f, 0.5f, Color.YELLOW));
        cube.addVertex(new Vertex(-0.5f, -0.5f, 0.5f, Color.YELLOW));
        cube.addVertex(new Vertex(0.5f,  -0.5f, 0.5f, Color.RED));
        cube.addVertex(new Vertex(0.5f, 0.5f, 0.5f, Color.YELLOW));
        cube.addVertex(new Vertex(-0.5f,  0.5f, 0.5f, Color.YELLOW));
        cube.addVertex(new Vertex(-0.5f, -0.5f, 0.5f, Color.YELLOW));

        // Left side of cube
        cube.addVertex(new Vertex(-0.5f,  0.5f, 0.5f, Color.GREEN));
        cube.addVertex(new Vertex(-0.5f, -0.5f, -0.5f, Color.GREEN));
        cube.addVertex(new Vertex(-0.5f,  -0.5f, 0.5f, Color.GREEN));
        cube.addVertex(new Vertex(-0.5f, 0.5f, 0.5f, Color.GREEN));
        cube.addVertex(new Vertex(-0.5f,  0.5f, -0.5f, Color.GREEN));
        cube.addVertex(new Vertex(-0.5f, -0.5f, -0.5f, Color.GREEN));

        // Top of cube
        cube.addVertex(new Vertex(-0.5f,  0.5f, -0.5f, new Color(3, 187, 116)));
        cube.addVertex(new Vertex(-0.5f, 0.5f, 0.5f, new Color(3, 187, 116)));
        cube.addVertex(new Vertex(0.5f,  0.5f, -0.5f, new Color(3, 187, 116)));
        cube.addVertex(new Vertex(-0.5f, 0.5f, 0.5f, new Color(3, 187, 116)));
        cube.addVertex(new Vertex(0.5f,  0.5f, 0.5f, new Color(3, 187, 116)));
        cube.addVertex(new Vertex(0.5f, 0.5f, -0.5f, new Color(3, 187, 116)));

        // Bottom of cube
        cube.addVertex(new Vertex(-0.5f,  -0.5f, -0.5f, new Color(169, 167, 245)));
        cube.addVertex(new Vertex(0.5f, -0.5f, -0.5f, new Color(169, 167, 245)));
        cube.addVertex(new Vertex(-0.5f,  -0.5f, 0.5f, new Color(169, 167, 245)));
        cube.addVertex(new Vertex(0.5f, -0.5f, -0.5f, new Color(169, 167, 245)));
        cube.addVertex(new Vertex(0.5f,  -0.5f, 0.5f, new Color(169, 167, 245)));
        cube.addVertex(new Vertex(-0.5f, -0.5f, 0.5f, new Color(169, 167, 245)));

        return cube;
    }
    
    //
    // Native methods
    //
    private native long init();
    private native void _addPositionColored(long nativePtr, 
        float x, float y, float z, float rhw, int color,
        float tu, float tv);
    private native void _setVertexAt(long nativePtr, int index, 
        float x, float y, float z, float rhw, int color,
        float tu, float tv);
    private native void _setRenderMode(long nativePtr, RenderMode mode);
    //private native void _removeVertex(Vertex v);
    private native void _setMaterial(long nativePtr, 
        long refLightPtr);
    private native void _setTexture(long nativePtr, long texPtr);
    private native void _setColored(long nativePtr, boolean colored);
    private native void _dispose(long nativePtr);
}

