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

import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.util.Iterator;

/**
 * Displays information about an Environment3D object and
 * the primitives, meshes and lights it contains.
 *
 * @author Matthew Ross
 */
public class Environment3DInfo extends JPanel {
    private Environment3D env;
    private boolean allowChanges;
    
    // GUI components
    private JList objectTypeJList;
    private JList primitiveJList;
    private JList meshJList;
    private JList lightJList;
    private DefaultListModel primitiveJListModel;
    private DefaultListModel meshJListModel;
    private DefaultListModel lightJListModel;
    private JPanel middlePanel;
    private JPanel bottomPanel;

    /**
     * Creates a new <tt>Environment3DInfo</tt>.
     *
     * @param env       the <tt>Environment3D</tt> whose state is to be viewed
     *                  and possibly changed
     * @param allowChanges whether to allow the user to change
     *                  the state of the <tt>Environment3D</tt> under focus
     *                  through the GUI provided by this object
     */
    public Environment3DInfo(Environment3D env, boolean allowChanges) {
        if (null == env) {
            throw new NullPointerException("env");
        }
        
        this.env = env;
        this.allowChanges = allowChanges;
        
        env.addEnvironment3DChangeListener(new EnvChangeListener());
        
        createGUI();
        
        // Populate list models with primitives etc. from the Env3D
        
        Iterator<Primitive> primIter = env.getPrimitives();
        while (primIter.hasNext()) {
            Primitive prim = primIter.next();
            
            primitiveJListModel.addElement(prim);
        }
        
        Iterator<Mesh> meshIter = env.getMeshes();
        while (meshIter.hasNext()) {
            Mesh mesh = meshIter.next();
            
            meshJListModel.addElement(mesh);
        }
        
        Iterator<Light> lightIter = env.getLights();
        while (lightIter.hasNext()) {
            Light light = lightIter.next();
            
            lightJListModel.addElement(light);
        }
    }

    private void createGUI() {
        // Make 3 panels
        // Top Panel ////////////////////////////////////////////////
        JPanel topPanel = new JPanel();
        topPanel.setLayout(new BorderLayout());
        topPanel.setBorder(BorderFactory.createTitledBorder("Object Type"));
        
        DefaultListModel objectTypeListModel = new DefaultListModel();
        
        objectTypeListModel.addElement("(General)");
        objectTypeListModel.addElement("Primitives");
        objectTypeListModel.addElement("Meshes");
        objectTypeListModel.addElement("Lights");
        objectTypeJList = new JList(objectTypeListModel);
        objectTypeJList.addListSelectionListener(new ListSelectionListener() {
            public void valueChanged(ListSelectionEvent e) {
                String sel = (String) objectTypeJList.getSelectedValue();
                
                if (null == sel) {
                    return;
                }
                
                if ("(General)".equals(sel)) {
                    objectTypeGeneralSelected();
                } else if ("Primitives".equals(sel)) {
                    objectTypePrimitivesSelected();
                } else if ("Meshes".equals(sel)) {
                    objectTypeMeshesSelected();
                } else if ("Lights".equals(sel)) {
                    objectTypeLightsSelected();
                }
            }
        });
        
        topPanel.add(new JScrollPane(objectTypeJList));
        
        
        // Middle Panel /////////////////////////////////////////////
        middlePanel = new JPanel(new CardLayout());
        middlePanel.setBorder(BorderFactory.createTitledBorder("---"));
        
        primitiveJListModel = new DefaultListModel();
        primitiveJList = new JList(primitiveJListModel);
        meshJListModel = new DefaultListModel();
        meshJList = new JList(meshJListModel);
        lightJListModel = new DefaultListModel();
        lightJList = new JList(lightJListModel);
        
        middlePanel.add(new JScrollPane(primitiveJList), "primitiveJList");
        middlePanel.add(new JScrollPane(meshJList), "meshJList");
        middlePanel.add(new JScrollPane(lightJList), "lightJList");

        middlePanel.setVisible(false);

        // Bottom Panel /////////////////////////////////////////////
        bottomPanel = new JPanel();
        bottomPanel.setBorder(BorderFactory.createTitledBorder("Detail"));
        
        
        // Set the layout etc. of this panel
        setLayout(new GridLayout(3, 1)); // 3 rows, 1 column
        setBorder(BorderFactory.createTitledBorder(
            "Environment3D Information"));
        add(topPanel);
        add(middlePanel);
        add(bottomPanel);
    }
    
    @Override
    public Dimension getPreferredSize() {
        Dimension d = super.getPreferredSize();
        return new Dimension(220, d.height);
    }
    
    private void objectTypeGeneralSelected() {
        middlePanel.setBorder(BorderFactory.createTitledBorder("---"));
        
        middlePanel.setVisible(false);
    }
    
    private void objectTypePrimitivesSelected() {
        middlePanel.setBorder(BorderFactory.createTitledBorder("Primitives"));
        middlePanel.setVisible(true);
        CardLayout cl = (CardLayout) middlePanel.getLayout();
        cl.show(middlePanel, "primitiveJList");
    }

    private void objectTypeMeshesSelected() {
        middlePanel.setBorder(BorderFactory.createTitledBorder("Meshes"));
        middlePanel.setVisible(true);
        CardLayout cl = (CardLayout) middlePanel.getLayout();
        cl.show(middlePanel, "meshJList");
    }

    private void objectTypeLightsSelected() {
        middlePanel.setBorder(BorderFactory.createTitledBorder("Lights"));
        middlePanel.setVisible(true);
        CardLayout cl = (CardLayout) middlePanel.getLayout();
        cl.show(middlePanel, "lightJList");
    }
    
    //
    // Private classes
    //
    
    private class EnvChangeListener implements Environment3DChangeListener {
        /**
         * Called when a primitive is added to an <tt>Environment3D</tt>.
         */
        public void primitiveAdded(Environment3DChangeEvent e) {
            primitiveJListModel.addElement(e.getPrimitive());
        }
        
        /**
         * Called when a primitive is removed from an <tt>Environment3D</tt>.
         */
        public void primitiveRemoved(Environment3DChangeEvent e) {
            primitiveJListModel.removeElement(e.getPrimitive());
        }
        
        /**
         * Called when a mesh is added to an <tt>Environment3D</tt>.
         */
        public void meshAdded(Environment3DChangeEvent e) {
            meshJListModel.addElement(e.getMesh());
        }
        
        /**
         * Called when a mesh is removed from an <tt>Environment3D</tt>.
         */
        public void meshRemoved(Environment3DChangeEvent e) {
            meshJListModel.removeElement(e.getMesh());
        }
        
        /**
         * Called when a light is added to an <tt>Environment3D</tt>.
         */
        public void lightAdded(Environment3DChangeEvent e) {
            lightJListModel.addElement(e.getLight());
        }
        
        /**
         * Called when a light is removed from an <tt>Environment3D</tt>.
         */
        public void lightRemoved(Environment3DChangeEvent e) {
            lightJListModel.removeElement(e.getLight());
        }
    }
}

