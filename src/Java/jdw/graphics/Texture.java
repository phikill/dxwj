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

import java.io.*;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

/**
 * Represents either a texture surface that can be applied to a Primitive,
 * or an image surface that can be used as a background for a 3D scene.
 *
 * @author Matthew Ross
 */
public class Texture {
    private long nativePtr;
    private String filename;

    /**
     * Creates a new Texture.
     */
    private Texture(String filename) {
        if (null == filename) {
            throw new NullPointerException("filename");
        }
        
        this.nativePtr = getNativePtr();
        this.filename = filename;
        
        if (0L == nativePtr) {
            throw new OutOfMemoryError("Native heap exhausted");
        }
    }

    /**
     * Constructs a Texture object to represent a texture (or image) surface
     * from an image file.
     * 
     * @throws NullPointerException if <tt>filename</tt> is <tt>null</tt>
     * @throws FileNotFoundException if the texture denoted by <tt>filename</tt>
     *                                  does not exist
     * @throws IOException if the image dimensions could not be obtained
     */
    public static Texture fromFile(String filename)
            throws FileNotFoundException, IOException {
        Texture t = new Texture(filename);
        
        if (!new File(filename).exists()) {
            throw new FileNotFoundException(filename+" does not exist");
        }
        
        // Need to get the dimensions of the image
        BufferedImage img = ImageIO.read(new File(filename));
        int width = img.getWidth();
        int height = img.getHeight();
        
        t._loadFromFile(t.nativePtr, filename, width, height);
        return t;
    }
    
    long nativePtr() {
        return nativePtr;
    }
    
    /*
     * Frees native resources associated with this Texture.
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
    private native void _loadFromFile(long nativePtr, String filename,
        int width, int height);
    private native void _dispose(long nativePtr);
}

