package jdw.demo;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.TimerTask;
import java.io.*;
import jdw.graphics.*;

/**
 * Creates a cube and places it into an <tt>Environment3D</tt>.
 *
 * @author Matthew Ross
 */
public class DXTester4 extends JFrame {
    private Environment3D env;
    private Primitive cube;
    private Primitive cube2;
    
    public DXTester4() {
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
        
        
        Environment3DInfo e3di = new Environment3DInfo(env, true);
        add(e3di, BorderLayout.LINE_END);
        
        
        setTitle("Environment3D Demo");
        setSize(800, 600);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setVisible(true);
    }
    
    private void initKeyboard() {
        env.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                
                Matrix worldMat = env.getWorldTransform();
                Matrix m2 = Matrix.rotationYawPitchRoll(0f, 0.1f, 0f);
                env.setWorldTransform(Matrix.multiply(worldMat, m2));
                
                
            }
        });
    }
    
    private void initScene() {
        // Create a 3D environment into which objects can be placed
        env = new Environment3D();
        env.addRenderListener(new RenderListener() {
            public void frameRendering(FrameRenderingEvent e) {
                Matrix m = cube.getTransform();
                m = Matrix.multiply(m,
                    Matrix.rotationYawPitchRoll(-0.01f, -0.01f, -0.01f));
                cube.setTransform(m);
            }
        });
        
        env.setBackground(Color.GRAY);
        
        
        // Create a cube object and add to the environment
        cube = Primitive.createCube();
        
        cube2 = Primitive.createCube();
        cube2.setTransform(Matrix.translate(0f, 1.5f, 1.5f));
        
        
        env.addPrimitive(cube2);
        env.addPrimitive(cube);

        
        
        // Eye: 10 units back on the Z-axis
        Vector3D eyePt = new Vector3D(0f, 0f, -10f);
        // Look at: the origin
        Vector3D lookAtPt = new Vector3D(0f, 0f, 0f);
        // Set the view using these two points
        env.setView(eyePt, lookAtPt);
        
        // Spot light
        SpotLight spotLight = new SpotLight(
            new Vector3D(1f, 1f, 0f), 
            new Vector3D(-15f, -15f, 0f), 
            new Color(110, 10, 0));
        //env.addLight(spotLight);
        
        // Add a directional light
        DirectionalLight dirLight2 = new DirectionalLight(
            new Vector3D(0f, 0f, 1f), Color.RED);
        env.addLight(dirLight2);
        
        // Point light
        PointLight pointLight = new PointLight(
            new Vector3D(0f, 10f, 0f), Color.BLUE);
        env.addLight(pointLight);
        
        // Ambient light
        //env.setAmbientLight(Color.GRAY);
        //env.setLightingEnabled(true);
        try {
            
            Texture bg = Texture.fromFile("Background.bmp");
            env.setBackground(bg);
        
        
        
            Mesh mesh = Mesh.fromFile("private\\meshes\\tiger.x");
            env.addMesh(mesh);
        
        } catch (IOException x) {
            
        }
    }
    
    /**
     * Cleans up resources and terminates the demo.
     */
    private void close() {
        System.exit(0);
    }
    
    /**
     * Creates a new instance of DXTester4 on the Swing event thread.
     */
    private static void createAndShowGUI() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new DXTester4();
            }
        });
    }
    
    public static void main(String[] args) {
        createAndShowGUI();
    }
}

