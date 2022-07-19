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
 * Renders and transforms a set of pyramid shapes.
 *
 * @author Matthew Ross
 */
class PyramidFrame extends JFrame {
    private Environment3D env;
    
    /*
    private Primitive createTriangle() {
        Primitive p = new Primitive(RenderMode.TRIANGLE_LIST);
        p.addVertex(new Vertex(0f, 0f, 0f, Color.RED));
        p.addVertex(new Vertex(0.5f, -0.8f, 0f, Color.BLUE));
        p.addVertex(new Vertex(-0.5f, -0.8f, 0f, Color.GREEN));
        return p;
    }
    */
    
    private void transform(Primitive p) {
        //Matrix m = Matrix.scale(2f, 2f, 2f);
        //Matrix m = Matrix.translate(2f, 1f, 3f);
        //Matrix m = Matrix.rotationZ(0.78f);
        Matrix m = Matrix.rotationYawPitchRoll(0.78f, 0.78f, 0.78f);
        p.setTransform(m);
    }
    
    /**
     * Creates the 3D scene.
     */
    private void initScene() {
        // Create a 3D environment into which objects can be placed
        env = new Environment3D();
        
        /*
        // Create a triangle and add to the environment
        Primitive triangle = createTriangle();
        env.addPrimitive(triangle);
        */
        
        // Create a square-based pyramid
        SquareBasedPyramid py1 = new SquareBasedPyramid();
        env.addPrimitive(py1);
        transform(py1);
        
        // Eye: 3 units back on the Z-axis
        Vector3D eyePt = new Vector3D(0f, 0f, -3f);
        // Look at: the origin
        Vector3D lookAtPt = new Vector3D(0f, 0f, 0f);
        // Set the view using these two points
        env.setView(eyePt, lookAtPt);
        
        // 
        /*
        env.addRenderListener(new RenderListener() {
            public void frameRendering(FrameRenderingEvent e) {
                Matrix worldMat = env.getWorldTransform();
                Matrix rot = Matrix.rotationYawPitchRoll(0, 0.03f, 0);
                worldMat = Matrix.multiply(worldMat, rot);
                
                // Update the world transformation
                env.setWorldTransform(worldMat);
            }
        });
        */
        
        setLayout(new BorderLayout());
        add(env, BorderLayout.CENTER);
        setTitle("Pyramids");
        setSize(800, 600);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setVisible(true);
    }
    
    /**
     * Creates a new instance of PyramidFrame on the Swing event thread.
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                PyramidFrame frame = new PyramidFrame();
                frame.initScene();
            }
        });
    }
}

class SquareBasedPyramid extends Primitive {
    
    public SquareBasedPyramid() {
        super(RenderMode.TRIANGLE_LIST);
        
        // Front of pyramid
        addVertex(new Vertex(0f, 0f, 0f, Color.RED));
        addVertex(new Vertex(0.5f, -0.8f, -0.5f, Color.BLUE));
        addVertex(new Vertex(-0.5f, -0.8f, -0.5f, Color.BLUE));
        
        // Back of pyramid
        addVertex(new Vertex(0f, 0f, 0f, Color.RED));
        addVertex(new Vertex(-0.5f, -0.8f, 0.5f, Color.GREEN));
        addVertex(new Vertex(0.5f, -0.8f, 0.5f, Color.GREEN));
        
        // Left of pyramid
        addVertex(new Vertex(0f, 0f, 0f, Color.RED));
        addVertex(new Vertex(-0.5f, -0.8f, -0.5f, Color.YELLOW));
        addVertex(new Vertex(-0.5f, -0.8f, 0.5f, Color.YELLOW));
        
        // Right of pyramid
        addVertex(new Vertex(0f, 0f, 0f, Color.RED));
        addVertex(new Vertex(0.5f, -0.8f, 0.5f, Color.RED));
        addVertex(new Vertex(0.5f, -0.8f, -0.5f, Color.RED));
        
        // Base of pyramid (2 triangles)
        addVertex(new Vertex(-0.5f, -0.8f, -0.5f, Color.BLUE));
        addVertex(new Vertex(0.5f, -0.8f, -0.5f, Color.BLUE));
        addVertex(new Vertex(-0.5f, -0.8f, 0.5f, Color.BLUE));

        addVertex(new Vertex(0.5f, -0.8f, 0.5f, Color.BLUE));
        addVertex(new Vertex(-0.5f, -0.8f, 0.5f, Color.BLUE));        
        addVertex(new Vertex(0.5f, -0.8f, -0.5f, Color.BLUE));
    }
}
