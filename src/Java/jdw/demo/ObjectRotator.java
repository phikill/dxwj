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

package jdw.demo;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import jdw.graphics.*;

/**
 * Creates a cube and places it into an <tt>Environment3D</tt>.
 *
 * @author Matthew Ross
 */
public class ObjectRotator extends JFrame {
    private Environment3D env;
    private Primitive obj;
    
    /**
     * Creates the 3D scene.
     */
    private void initScene() {
        // Create a 3D environment into which objects can be placed
        env = new Environment3D();
        
        // Create a cube object and add to the environment
        obj = Primitive.createCube();
        env.addPrimitive(obj);
        
        // Eye: 3 units back on the Z-axis
        Vector3D eyePt = new Vector3D(0f, 0f, -3f);
        // Look at: the origin
        Vector3D lookAtPt = new Vector3D(0f, 0f, 0f);
        // Set the view using these two points
        env.setView(eyePt, lookAtPt);
        
        // Automatically rotate the cube
        env.addRenderListener(new RenderListener() {
            public void frameRendering(FrameRenderingEvent e) {
                Matrix worldMat = env.getWorldTransform();
                Matrix rot = Matrix.rotationYawPitchRoll(.005f, -.008f, .005f);
                worldMat = Matrix.multiply(worldMat, rot);
                
                // Update the world transformation
                env.setWorldTransform(worldMat);
            }
        });
        
        setLayout(new BorderLayout());
        add(env, BorderLayout.CENTER);
        setTitle("Tutorial 1: ObjectRotator");
        setSize(800, 600);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setVisible(true);
    }
    
    /**
     * Creates a new instance of ObjectRotator on the Swing event thread.
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                ObjectRotator o = new ObjectRotator();
                o.initScene();
            }
        });
    }
}

