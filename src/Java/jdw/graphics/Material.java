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
 * A material specifies the types and colors of light
 * reflected by a primitive. 
 *
 * @author Matthew Ross
 */
public class Material {
    private long nativePtr;
    private Color diffuse;
    private Color ambient;
    private Color specular;
    private float specularIntensity;
    private Color emissive;

    /**
     * Creates a new Material. Parameters can be null.
     *
     * @param diffuse the color of diffuse light to reflect
     * @param ambient the color of ambient light to reflect
     * @param specular the color of specular light to reflect
     * @param emissive the color of light to emit
     * @param specularIntensity the intensity of the specular reflection -
     *          higher values have higher intensity
     */
    public Material(Color diffuse, Color ambient, Color specular,
                Color emissive, float specularIntensity) {
        this.diffuse = diffuse;
        this.ambient = ambient;
        this.specular = specular;
        this.emissive = emissive;
        this.specularIntensity = specularIntensity;
        
        // Establish native DX material
        boolean diffusePresent = diffuse != null;
        boolean ambientPresent = ambient != null;
        boolean specularPresent = specular != null;
        boolean emissivePresent = emissive != null;
        
        int diffuseR = 0;
        int diffuseG = 0;
        int diffuseB = 0;
        int ambientR = 0;
        int ambientG = 0;
        int ambientB = 0;
        int specularR = 0;
        int specularG = 0;
        int specularB = 0;
        int emissiveR = 0;
        int emissiveG = 0;
        int emissiveB = 0;
        
        if (diffusePresent) {
            diffuseR = diffuse.getRed();
            diffuseG = diffuse.getGreen();
            diffuseB = diffuse.getBlue();
        }
        if (ambientPresent) {
            ambientR = ambient.getRed();
            ambientG = ambient.getGreen();
            ambientB = ambient.getBlue();
        }
        if (specularPresent) {
            specularR = specular.getRed();
            specularG = specular.getGreen();
            specularB = specular.getBlue();
        }
        if (emissivePresent) {
            emissiveR = emissive.getRed();
            emissiveG = emissive.getGreen();
            emissiveB = emissive.getBlue();
        }
        
        nativePtr = _initWithParams(
            diffusePresent, diffuseR, diffuseG, diffuseB,
            ambientPresent, ambientR, ambientG, ambientB,
            specularPresent, specularR, specularG, specularB,
            emissivePresent, emissiveR, emissiveG, emissiveB,
            specularIntensity);
        
        if (0L == nativePtr) {
            throw new OutOfMemoryError("Native heap exhausted");
        }
    }
    
    public static Material getDefault() {
        return new Material(Color.WHITE,
            Color.WHITE, Color.WHITE, Color.BLACK, 1f);
    }
    
    long getNativePtr() {
        return nativePtr;
    }
    
    public Color getDiffuseColor() {
        return diffuse;
    }
    
    public Color getAmbientColor() {
        return ambient;
    }
    
    public Color getSpecularColor() {
        return specular;
    }
    
    public float getSpecularColorIntensity() {
        return specularIntensity;
    }
    
    public Color getEmissiveColor() {
        return emissive;
    }
    
    /*public*/ synchronized void dispose() {
        if (0L == nativePtr) {
            throw new RuntimeException("Object already disposed");
        }
        
        _dispose(nativePtr);
        nativePtr = 0L;
    }
    
    //
    // Native methods
    //
    
    private native long _initWithParams(boolean diffusePresent,
        int diffuseR, int diffuseB, int diffuseG,
        boolean ambientPresent, 
        int ambientR, int ambientG, int ambientB,
        boolean specularPresent,
        int specularR, int specularG, int specularB,
        boolean emissivePresent,
        int emissiveR, int emissiveG, int emissiveB,
        float specularIntensity);
    
    private native void _dispose(long nativePtr);
}

