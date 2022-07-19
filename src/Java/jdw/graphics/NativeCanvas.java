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

import java.awt.Canvas;
import java.awt.Graphics;

/**
 * Allows some native code to render the graphics for this canvas.
 *
 * @author Matthew Ross
 */
public class NativeCanvas extends Canvas {
    protected long nativePtr;
    private String libName;
    
    static {
        try {
            LibraryLoader.load("WinMan");
        } catch (UnsatisfiedLinkError x) {
            System.err.println("NativeCanvas library not found");
        }
    }
    
    public NativeCanvas(String libName) {
        initLibNameOnly(libName);
        
        nativePtr = getNativePtr();
    }
    
    protected NativeCanvas() {
    }
    
    protected void initLibNameOnly(String libName) {
        if (null == libName) {
            throw new NullPointerException("libName");
        }
        this.libName = libName;
    }
    
    private static native synchronized long getNativePtr();
    
    /**
     * Overriden such that the background is not erased before painting.
     */
    @Override
    public void update(Graphics g) {
        paint(g);
    }
    
    /**
     * Paints this canvas.
     * <br><br>
     * Most applications that subclass <tt>Canvas</tt> should override 
     * this method in order to perform some useful operation (typically, 
     * custom painting of the canvas). The default operation is simply 
     * to clear the canvas. Applications that override this method need 
     * not call <tt>super.paint(g)</tt>.
     */
    @Override
    public native void paint(Graphics g);
    
    public String getLibName() {
        return libName;
    }
}

