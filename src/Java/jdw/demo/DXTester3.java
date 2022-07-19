package jdw.demo;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.TimerTask;
import java.io.IOException;
import jdw.graphics.*;

/**
 * Creates a cube and places it into an <tt>Environment3D</tt>.
 *
 * @author Matthew Ross
 */
public class DXTester3 extends JFrame {
    private Environment3D env;
    private Primitive cube;
    
    public DXTester3() {
        // Create the 3D scene
        initScene();
        
        initKeyboard();
        
        // Do standard Swing stuff
        initWindow();
    }
    
    private void initWindow() {
        JButton closeButton = new JButton("Close");
        closeButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                close();
            }
        });
        
        setLayout(new BorderLayout());
        add(env, BorderLayout.CENTER);
        
        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(
            new BoxLayout(bottomPanel, BoxLayout.LINE_AXIS));
        bottomPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        bottomPanel.add(Box.createHorizontalGlue());
        bottomPanel.add(closeButton);
        
        add(bottomPanel, BorderLayout.PAGE_END);
        
        setTitle("Environment3D Demo");
        setSize(800, 600);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setVisible(true);
    }
    
    /**
     * Creates a cube with side length 1, centered around the origin.
     */
    public static Primitive createCubeTex() {
        Primitive cube = new Primitive(RenderMode.TRIANGLE_LIST);
        
        // Front of cube
        cube.addVertex(new Vertex( -0.5f, 0.5f, -0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(0.5f, -0.5f, -0.5f, null, 1f, 1f));
        cube.addVertex(new Vertex(-0.5f,  -0.5f, -0.5f, null, 0f, 1f));
        cube.addVertex(new Vertex( -0.5f, 0.5f, -0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(0.5f,  0.5f, -0.5f, null, 1f, 0f));
        cube.addVertex(new Vertex(0.5f, -0.5f, -0.5f, null, 1f, 1f));

        // Right side of cube
        cube.addVertex(new Vertex(0.5f, 0.5f, -0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(0.5f, -0.5f, 0.5f, null, 1f, 1f));
        cube.addVertex(new Vertex(0.5f,  -0.5f, -0.5f, null, 0f, 1f));
        cube.addVertex(new Vertex(0.5f, 0.5f, -0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(0.5f,  0.5f, 0.5f, null, 1f, 0f));
        cube.addVertex(new Vertex(0.5f, -0.5f, 0.5f, null, 1f, 1f));

        // Back of cube
        cube.addVertex(new Vertex(0.5f,  0.5f, 0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(-0.5f, -0.5f, 0.5f, null, 1f, 1f));
        cube.addVertex(new Vertex(0.5f,  -0.5f, 0.5f, null, 0f, 1f));
        cube.addVertex(new Vertex(0.5f, 0.5f, 0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(-0.5f,  0.5f, 0.5f, null, 1f, 0f));
        cube.addVertex(new Vertex(-0.5f, -0.5f, 0.5f, null, 1f, 1f));

        // Left side of cube
        cube.addVertex(new Vertex(-0.5f,  0.5f, 0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(-0.5f, -0.5f, -0.5f, null, 1f, 1f));
        cube.addVertex(new Vertex(-0.5f,  -0.5f, 0.5f, null, 0f, 1f));
        cube.addVertex(new Vertex(-0.5f, 0.5f, 0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(-0.5f,  0.5f, -0.5f, null, 1f, 0f));
        cube.addVertex(new Vertex(-0.5f, -0.5f, -0.5f, null, 1f, 1f));

        // Top of cube
        cube.addVertex(new Vertex(-0.5f,  0.5f, -0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(-0.5f, 0.5f, 0.5f, null, 1f, 0f));
        cube.addVertex(new Vertex(0.5f,  0.5f, -0.5f, null, 0f, 1f));
        cube.addVertex(new Vertex(-0.5f, 0.5f, 0.5f, null, 1f, 0f));
        cube.addVertex(new Vertex(0.5f,  0.5f, 0.5f, null, 1f, 1f));
        cube.addVertex(new Vertex(0.5f, 0.5f, -0.5f, null, 0f, 1f));

        // Bottom of cube
        cube.addVertex(new Vertex(-0.5f,  -0.5f, -0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(0.5f, -0.5f, -0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(-0.5f,  -0.5f, 0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(0.5f, -0.5f, -0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(0.5f,  -0.5f, 0.5f, null, 0f, 0f));
        cube.addVertex(new Vertex(-0.5f, -0.5f, 0.5f, null, 0f, 0f));
        
        try {
            cube.setTexture(Texture.fromFile("Texture.bmp"));

        } catch (IOException x) {
            System.err.println(x);
        }
        return cube;
    }
    
    private void initKeyboard() {
        env.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                Matrix worldMat = env.getWorldTransform();
                Matrix m2 = Matrix.rotationYawPitchRoll(0.1f, 0f, 0f);
                env.setWorldTransform(Matrix.multiply(worldMat, m2));
            }
        });
    }
    
    private void initScene() {
        // Create a 3D environment into which objects can be placed
        env = new Environment3D();
        
        // Create a cube object and add to the environment
        cube = createCubeTex();
        env.addPrimitive(cube);      
        
        // Eye: 3 units back on the Z-axis
        Vector3D eyePt = new Vector3D(0f, 0f, -3f);
        // Look at: the origin
        Vector3D lookAtPt = new Vector3D(0f, 0f, 0f);
        // Set the view using these two points
        env.setView(eyePt, lookAtPt);
        
        env.setAmbientLight(Color.WHITE);
        env.setLightingEnabled(true);
        env.setWorldTransform(Matrix.rotationYawPitchRoll(0.0f, -0.3f, 0f));
    }
    
    /**
     * Cleans up resources and terminates the demo.
     */
    private void close() {
        System.exit(0);
    }
    
    /**
     * Creates a new instance of DXTester3 on the Swing event thread.
     */
    private static void createAndShowGUI() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new DXTester3();
            }
        });
    }
    
    public static void main(String[] args) {
        createAndShowGUI();
    }
}

