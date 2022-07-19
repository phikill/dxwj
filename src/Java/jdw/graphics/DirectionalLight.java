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
 * A directional light is a light with color and direction, but
 * no position.
 *
 * @author Matthew Ross
 */
public class DirectionalLight extends Light {
    
    private Color color;
    private Vector3D direction;

    /**
     * Creates a DirectionalLight with direction and color values.
     */
    public DirectionalLight(Vector3D direction, Color color) {
        if (null == direction) {
            throw new NullPointerException("direction");
        }
        if (null == color) {
            throw new NullPointerException("color");
        }
        
        this.direction = direction;
        this.color = color;
        trySetLightParams(direction, color);
    }

    /**
     * Sets the color of this directional light.
     */
    public void setColor(Color c) {
        if (null == c) {
            throw new NullPointerException("c");
        }
        trySetLightParams(this.direction, c);
        this.color = c;
    }
    
    /**
     * Returns the color of this directional light.
     */
    public Color getColor() {
        return color;
    }
    
    /**
     * Sets the direction of this light.
     */
    public void setDirection(Vector3D dir) {
        if (null == dir) {
            throw new NullPointerException("dir");
        }
        trySetLightParams(dir, this.color);
        this.direction = dir;
    }
    
    /**
     * Returns the direction of this light.
     */
    public Vector3D getDirection() {
        return direction;
    }
    
    private void trySetLightParams(Vector3D _direction, Color _color) {
        super.setLightParams(D3DLIGHT_DIRECTIONAL, 
            _color.getRed(), _color.getGreen(), _color.getBlue(), 
            _color.getRed(), _color.getGreen(), _color.getBlue(), 
            0f, 0f, 0f, 
            _direction.getX(), _direction.getY(), _direction.getZ());
    }
}

