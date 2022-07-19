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
 * A RenderMode is applied to a primitive and specifies how the 
 * vertices of that primitive are interpreted.
 *
 * @author Matthew Ross
 */
public enum RenderMode {
    
    /** Specifies that the vertices of a primitive are not linked. */
    POINT_LIST(0),

    /** Specifies that pairs of vertices of a primitive are linked 
        to form lines. */
    LINE_LIST(1),

    /** Specifies that each vertex of a primitive is linked in a
        chain. */
    LINE_STRIP(2),

    /** Specifies that three adjacent vertices of a primitive
        make up a triangle. */
    TRIANGLE_LIST(3),

    /** Specifies that the first three vertices of a primitive
        make up a triangle, and each subsequent vertex along
        with the previous two, make up additional triangles. */
    TRIANGLE_STRIP(4),

    /** Like TRIANGLE_STRIP but each triangle has one common point
        which is the centre of a fan. */
    TRIANGLE_FAN(5);
    

    private int id;
    
    private RenderMode(int id) { 
        this.id = id; 
    }
    
    public int getID() { 
        return id; 
    }
}

