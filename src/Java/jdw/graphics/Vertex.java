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

/**
 * A vertex is a point that defines where two edges of a shape come together.
 *
 * @author Matthew Ross
 */
public class Vertex {
    private float x, y, z;
    private Color color;
    private float tu, tv;
    private boolean hasTexCoords;
    private boolean isColored;
    
    /**
     * Creates a new <tt>Vertex</tt> with position (0,0,0) and
     * no color.
     */
    public Vertex() {
        this(0f, 0f, 0f, null, 0f, 0f, false);
        isColored = false;
    }
    
    /**
     * Creates a new <tt>Vertex</tt> with position (x,y,z) and
     * no color.
     *
     * @param x the x coordinate
     * @param y the y coordinate
     * @param z the z coordinate
     */
    public Vertex(float x, float y, float z) {
        this(x, y, z, null, 0f, 0f, false);
        isColored = false;
    }
    
    //PositionColored OR TransformedColored
    /**
     * Creates a new <tt>Vertex</tt> with position (x,y,z) and
     * a particular color.
     *
     * @param x the x coordinate
     * @param y the y coordinate
     * @param z the z coordinate
     * @param color the vertex's color (or <tt>null</tt> for no color)
     */
    public Vertex(float x, float y, float z, Color color) {
        this(x, y, z, color, 0f, 0f, false);
        isColored = color != null;
    }
    
    /**
     * Creates a new <tt>Vertex</tt> with position (x,y,z),
     * a color, and texture coordinates (u and v).
     *
     * @param x the x coordinate
     * @param y the y coordinate
     * @param z the z coordinate
     * @param color the vertex's color (or <tt>null</tt> for no color)
     * @param tu the u texture coordinate
     * @param tv the v texture coordinate
     */
    public Vertex(float x, float y, float z, Color color,
            float tu, float tv) {
        this(x, y, z, color, tu, tv, true);
        isColored = color != null;
    }
    
    private Vertex(float x, float y, float z, Color color,
            float tu, float tv, boolean hasTexCoords) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.color = color;
        this.tu = tu;
        this.tv = tv;
        this.hasTexCoords = hasTexCoords;
    }
    
    /**
     * Returns whether texture coordinates have been
     * specified for this vertex.
     */
    public boolean hasTextureCoords() {
        return hasTexCoords;
    }
    
    /**
     * Returns whether a color has been specified for this vertex.
     */
    public boolean isColored() {
        return isColored;
    }
    
    /**
     * Returns the x coordinate of this vertex.
     */
    public float getX() { return x; }
    
    /**
     * Returns the y coordinate of this vertex.
     */
    public float getY() { return y; }
    
    /**
     * Returns the z coordinate of this vertex.
     */
    public float getZ() { return z; }
    
    /**
     * Returns the color of this vertex.
     */
    public Color getColor() { return color; }

    /**
     * Returns the u texture coordinate of this vertex.
     */
    public float getU() { return tu; }
    
    /**
     * Returns the v texture coordinate of this vertex.
     */
    public float getV() { return tv; }
}

