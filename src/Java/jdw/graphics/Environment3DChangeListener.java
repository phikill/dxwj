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

import java.util.EventListener;

/**
 * This interface receives events as objects (primitives, meshes, lights)
 * are added and removed to/from an <tt>Environment3D</tt>.
 *
 * @author Matthew Ross
 */
public interface Environment3DChangeListener extends EventListener {

    /**
     * Called when a primitive is added to an <tt>Environment3D</tt>.
     */
    void primitiveAdded(Environment3DChangeEvent e);
    
    /**
     * Called when a primitive is removed from an <tt>Environment3D</tt>.
     */
    void primitiveRemoved(Environment3DChangeEvent e);
    
    /**
     * Called when a mesh is added to an <tt>Environment3D</tt>.
     */
    void meshAdded(Environment3DChangeEvent e);
    
    /**
     * Called when a mesh is removed from an <tt>Environment3D</tt>.
     */
    void meshRemoved(Environment3DChangeEvent e);
    
    /**
     * Called when a light is added to an <tt>Environment3D</tt>.
     */
    void lightAdded(Environment3DChangeEvent e);
    
    /**
     * Called when a light is removed from an <tt>Environment3D</tt>.
     */
    void lightRemoved(Environment3DChangeEvent e);
}

