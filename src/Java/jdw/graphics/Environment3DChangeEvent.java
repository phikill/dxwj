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

import java.util.EventObject;

/**
 * Represents the event that the state of an
 * <tt>Environment3D</tt> changed.
 *
 * @author Matthew Ross
 */
public class Environment3DChangeEvent extends EventObject {
    private Primitive primitive;
    private Mesh mesh;
    private Light light;

    public Environment3DChangeEvent(Environment3D source,
            Primitive p, Mesh m, Light l) {
        super(source);
        
        this.primitive = p;
        this.mesh = m;
        this.light = l;
    }
    
    @Override
    public Environment3D getSource() {
        return (Environment3D) super.getSource();
    }
    
    /**
     * Returns the primitive that was added/removed, 
     * or <tt>null</tt> if not applicable.
     */
    public Primitive getPrimitive() {
        return primitive;
    }
    
    /**
     * Returns the mesh that was added/removed,
     * or <tt>null</tt> if not applicable.
     */
    public Mesh getMesh() {
        return mesh;
    }
    
    /**
     * Returns the light that was added/removed,
     * or <tt>null</tt> if not applicable.
     */
    public Light getLight() {
        return light;
    }
}

