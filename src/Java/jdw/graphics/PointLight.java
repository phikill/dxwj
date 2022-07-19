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
 * A point light is a light that has a position and a color
 * and radiates light in all directions.
 *
 * @author Matthew Ross
 */
public class PointLight extends Light {
    
    private Vector3D position;
    private Color color;
    
    /**
     * Creates a PointLight with position and color values.
     */
    public PointLight(Vector3D position, Color color) {
        if (null == position) {
            throw new NullPointerException("position");
        }
        if (null == color) {
            throw new NullPointerException("color");
        }
        
        this.position = position;
        this.color = color;
        trySetLightParams(position, color);
    }
    
    /**
     * Sets the color of this point light.
     */
    public void setColor(Color c) {
        if (null == c) {
            throw new NullPointerException("c");
        }
        // If trySetLightParams throws an exception, 
        // the member variables of this object will remain consistent
        trySetLightParams(this.position, c);
        this.color = c;
    }
    
    /**
     * Returns the color of this point light.
     */
    public Color getColor() {
        return color;
    }
    
    /**
     * Sets the position of this point light.
     */
    public void setPosition(Vector3D pos) {
        if (null == pos) {
            throw new NullPointerException("pos");
        }
        trySetLightParams(pos, this.color);
        this.position = pos;
    }
    
    /**
     * Returns the position of this point light.
     */
    public Vector3D getPosition() {
        return position;
    }
    
    private void trySetLightParams(Vector3D _position, Color _color) {
        super.setLightParams(D3DLIGHT_POINT, 
            _color.getRed(), _color.getGreen(), _color.getBlue(), 
            _color.getRed(), _color.getGreen(), _color.getBlue(), 
            _position.getX(), _position.getY(), _position.getZ(),
            0f, 0f, 0f);
    }
}

