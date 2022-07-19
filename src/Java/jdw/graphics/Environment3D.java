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

import java.awt.Graphics;
import java.awt.Color;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.util.Timer;
import java.util.TimerTask;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * A 3D environment into which primitives are placed. Note that 
 * <tt>3DEnvironment</tt> is a heavyweight control which means it 
 * will overdraw Swing components.
 * 
 * @see Primitive
 * @author Matthew Ross
 */
public class Environment3D extends NativeCanvas {
    private static List<Environment3D> env3DList;
    private static String libName;
    
    // Periodically repaints the display
    private Timer timer;
    private TimerTask task;
    private volatile boolean running;
    private boolean renderingError;
    
    private boolean lighting;
    private boolean transformCoords;
    private List<RenderListener> rlList;
    private FrameRenderingEvent fre;
    private List<Primitive> primitiveList;
    private List<Mesh> meshes;
    
    // World transform -------
    private Matrix worldMat;
    
    // View transform --------
    private Vector3D eyePt;
    private Vector3D lookAtPt;
    
    // Projection transform --
    private float fieldOfViewY; // Field of view in radians
    private float aspectRatio;  // Aspect ratio
    private float nearClip;     // Near clipping plane location
    private float farClip;      // Far clipping plane location
    
    // Lights ----------------
    private List<Light> lightList;
    private Color ambientLight;
    
    private List<Environment3DChangeListener> listenerList;
    
    static {
        libName = LibraryLoader.load("DXRenderer");
        env3DList = new ArrayList<Environment3D>();
    }

    /**
     * Creates a new <tt>Environment3D</tt>. Rendering is started
     * immediately. Equivalent to calling <tt>new Environment3D(true)</tt>.
     */
    public Environment3D() {
        this(true);
    }
    
    /**
     * Creates a new <tt>Environment3D</tt>.
     *
     * @param startRendering <tt>true</tt> causes rendering to start 
     *                       immediately, <tt>false</tt> delays
     *                       rendering until resumeRendering() is invoked
     */
    public Environment3D(boolean startRendering) {
        super();
        super.initLibNameOnly(libName);
        
        nativePtr = getNativePtr();
        if (0L == nativePtr) {
            throw new OutOfMemoryError("Native heap exhausted");
        }
        
        lighting = false;
        // Use the world, view, projection transformations
        // (as opposed to specifying screen coordinates directly)
        setTransformationEnabled(true);
        
        worldMat = Matrix.identity();
        
        // View
        eyePt = new Vector3D();
        lookAtPt = new Vector3D();
        
        // Projection
        fieldOfViewY = (float) (Math.PI/4.0);
        aspectRatio = 1f;
        nearClip = 1f;
        farClip = 100f;
        
        setView(eyePt, lookAtPt);
        
        // Set a sensible default projection transform
        setProjectionTransform(Matrix.createProjectionTransform(
            fieldOfViewY, aspectRatio, nearClip, farClip));
        
        timer = new Timer("Repaint-Thread", true);
        if (startRendering) {
            startRepaintTask();
        }
        
        rlList = new ArrayList<RenderListener>();
        // We can reuse the FrameRenderingEvent object
        fre = new FrameRenderingEvent(this);
        lightList = new ArrayList<Light>();
        ambientLight = null;
        
        renderingError = false;
        
        addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                _updateBackBufferSize(nativePtr, getWidth(), getHeight());
            }
        });
        
        meshes = new ArrayList<Mesh>();
        primitiveList = new ArrayList<Primitive>();
        listenerList = new ArrayList<Environment3DChangeListener>();
        
        synchronized (Environment3D.class) {
            env3DList.add(this);
        }
    }
    
    private synchronized void startRepaintTask() {
        task = new RenderTask();
        timer.schedule(task, 0L, 15L); // 66 times/sec
        running = true;
    }
    
    private synchronized void stopRepaintTask() {
        task.cancel();
        running = false;
    }
    
    /**
     * Adds a listener for receiving events when primitives, meshes, lights
     * are added and removed to/from this environment.
     *
     * @param l the listener to be notified of the events
     * @throws NullPointerException if <tt>l</tt> is <tt>null</tt>
     */
    public synchronized void addEnvironment3DChangeListener(
            Environment3DChangeListener l) {
        if (null == l) {
            throw new NullPointerException("l");
        }
        listenerList.add(l);
    }
    
    /**
     * Removes a <tt>Environment3DChangeListener</tt> previously 
     * added in a call to addEnvironment3DChangeListener.
     *
     * @param l the listener to be notified of events no longer
     * @throws NullPointerException if <tt>l</tt> is <tt>null</tt>
     */
    public synchronized void removeEnvironment3DChangeListener(
            Environment3DChangeListener l) {
        if (null == l) {
            throw new NullPointerException("l");
        }
        listenerList.remove(l);
    }
    
    // Methods that fire events /////////////////////////////////////
    
    private synchronized void firePrimitiveAdded(Environment3DChangeEvent e) {
        for (Environment3DChangeListener l : listenerList) {
            l.primitiveAdded(e);
        }
    }

    private synchronized void firePrimitiveRemoved(Environment3DChangeEvent e) {
        for (Environment3DChangeListener l : listenerList) {
            l.primitiveRemoved(e);
        }
    }

    private synchronized void fireMeshAdded(Environment3DChangeEvent e) {
        for (Environment3DChangeListener l : listenerList) {
            l.meshAdded(e);
        }
    }

    private synchronized void fireMeshRemoved(Environment3DChangeEvent e) {
        for (Environment3DChangeListener l : listenerList) {
            l.meshRemoved(e);
        }
    }

    private synchronized void fireLightAdded(Environment3DChangeEvent e) {
        for (Environment3DChangeListener l : listenerList) {
            l.lightAdded(e);
        }
    }

    private synchronized void fireLightRemoved(Environment3DChangeEvent e) {
        for (Environment3DChangeListener l : listenerList) {
            l.lightRemoved(e);
        }
    }
    
    
    /**
     * Adds a primitive to the list of primitives to be rendered.
     *
     * @param p the primitive to be added
     * @throws NullPointerException if <tt>p</tt> is <tt>null</tt>
     */
    public synchronized void addPrimitive(Primitive p) {
        if (null == p) {
            throw new NullPointerException("p");
        }
        _addPrimitive(nativePtr, p.getNativePtr());
        primitiveList.add(p);
        
        firePrimitiveAdded(new Environment3DChangeEvent(this, p, null, null));
    }
    
    /**
     * Removes a primitive from the list of primitives to be rendered.
     *
     * @param p the primitive to remove
     * @throws NullPointerException if <tt>p</tt> is <tt>null</tt>
     */
    public synchronized void removePrimitive(Primitive p) {
        if (null == p) {
            throw new NullPointerException("p");
        }
        _removePrimitive(nativePtr, p.getNativePtr());
        primitiveList.remove(p);
        
        firePrimitiveRemoved(new Environment3DChangeEvent(this, p, null, null));
    }
    
    public synchronized Iterator<Primitive> getPrimitives() {
        return primitiveList.iterator();
    }
    
    /**
     * Adds a mesh to this environment.
     *
     * @param m the mesh to add
     * @throws NullPointerException if <tt>m</tt> is <tt>null</tt>
     */
    public synchronized void addMesh(Mesh m) {
        if (null == m) {
            throw new NullPointerException("m");
        }
        _addMesh(nativePtr, m.nativePtr());
        meshes.add(m);
        fireMeshAdded(new Environment3DChangeEvent(this, null, m, null));
    }
    
    /**
     * Removes a mesh from this environment.
     *
     * @param m the mesh to remove
     * @throws NullPointerException if <tt>m</tt> is <tt>null</tt>
     */
    public synchronized void removeMesh(Mesh m) {
        if (null == m) {
            throw new NullPointerException("m");
        }
        
        // TODO
        
        meshes.remove(m);
        fireMeshRemoved(new Environment3DChangeEvent(this, null, m, null));
    }
    
    /**
     * Returns an Iterator containing all meshes in this environment.
     */
    public synchronized Iterator<Mesh> getMeshes() {
        return meshes.iterator();
    }
    
    /**
     * Causes rendering to be paused. Useful when adding primitives where
     * you want to suspend rendering until all the primitives have been added.
     */
    public synchronized void suspendRendering() {
        if (running) {
            stopRepaintTask();
        }
    }
    
    /**
     * Causes rendering to be resumed. There will be regular paint operations
     * to keep the display up-to-date.
     */
    public synchronized void resumeRendering() {
        if (!running) {
            startRepaintTask();
        }        
    }
    
    /**
     * Returns whether lighting is enabled.
     */
    public synchronized boolean isLightingEnabled() {
        return lighting;
    }
    
    /**
     * Changes whether lighting is enabled.
     */
    public synchronized void setLightingEnabled(boolean lighting) {
        _setLightingEnabled(nativePtr, lighting);
        this.lighting = lighting;
    }
    
    /**
     * Returns whether transformations are enabled.
     * The initial value when an Environment3D is created is true.
     */
    public synchronized boolean isTransformationEnabled() {
        return transformCoords;
    }
    
    /**
     * Changes whether transformations are enabled.
     */
    public synchronized void setTransformationEnabled(boolean transform) {
        _setTransformationEnabled(nativePtr, transform);
        this.transformCoords = transform;
    }
    
    /**
     * Sets the world transform. The world transform modifies the
     * position of all objects in the environment. To change the 
     * position of a single primitive, use Primitive.setTransform().
     * 
     * @param m the world transform
     * @throws NullPointerException if <tt>m</tt> is <tt>null</tt>
     * @see jdw.graphics.Primitive#setTransform(jdw.graphics.Matrix)
     */
    public void setWorldTransform(Matrix m) {
        if (null == m) {
            throw new NullPointerException();
        }
        _setWorldTransform(nativePtr, m);
        worldMat = m;
    }
    
    /**
     * Returns the current world transform.
     */
    public Matrix getWorldTransform() {
        return worldMat;
    }
    
    /**
     * Specifies how the 3D scene appears to the viewer.
     *
     * @param eyePt the location in 3D space of the eye/camera
     * @param lookAtPt the point on which the eye is focused i.e. where to look
     * @throws NullPointerException if either parameter is <tt>null</tt>
     */
    public void setView(Vector3D eyePt, Vector3D lookAtPt) {
        if (null == eyePt) {
            throw new NullPointerException("eyePt");
        }
        if (null == lookAtPt) {
            throw new NullPointerException("lookAtPt");
        }
        
        this.eyePt = eyePt;
        this.lookAtPt = lookAtPt;
        Matrix view = Matrix.createViewTransform(eyePt, lookAtPt);
        
        if (null == view) {
            throw new NullPointerException();
        }
        _setViewTransform(nativePtr, view);
    }
    
    /**
     * Sets the location in 3D space of the eye/camera.
     */
    public void setEyePoint(Vector3D eyePt) {
        if (null == eyePt) {
            throw new NullPointerException("eyePt");
        }
        
        this.eyePt = eyePt;
        Matrix view = Matrix.createViewTransform(eyePt, this.lookAtPt);
        
        if (null == view) {
            throw new NullPointerException();
        }
        _setViewTransform(nativePtr, view);
    }
    
    /**
     * Returns the location in 3D space of the eye/camera.
     */
    public Vector3D getEyePoint() {
        return eyePt;
    }
    
    /**
     * Sets the point on which the eye is focused.
     */
    public void setLookAtPoint(Vector3D lookAtPt) {
        if (null == lookAtPt) {
            throw new NullPointerException("lookAtPt");
        }
        
        this.lookAtPt = lookAtPt;
        Matrix view = Matrix.createViewTransform(this.eyePt, lookAtPt);
        
        if (null == view) {
            throw new NullPointerException();
        }
        _setViewTransform(nativePtr, view);
    }
    
    /**
     * Returns the point on which the eye is focused.
     */
    public Vector3D getLookAtPoint() {
        return lookAtPt;
    }
    
    /**
     * Sets the projection transform.
     */
    private void setProjectionTransform(Matrix m) {
        if (null == m) {
            throw new NullPointerException();
        }
        _setProjectionTransform(nativePtr, m);
    }
    
    /**
     * Returns the field of view in radians.
     */
    public float getFieldOfView() {
        return fieldOfViewY;
    }
    
    /**
     * Sets the field of view. Units are radians.
     */
    public void setFieldOfView(float fovy) {
        fieldOfViewY = fovy;
        setProjectionTransform(Matrix.createProjectionTransform(
            fieldOfViewY, aspectRatio, nearClip, farClip));
    }
    
    /**
     * Returns the aspect ratio.
     */
    public float getAspectRatio() {
        return aspectRatio;
    }
    
    /**
     * Sets the aspect ratio.
     */
    public void setAspectRatio(float ar) {
        aspectRatio = ar;
        setProjectionTransform(Matrix.createProjectionTransform(
            fieldOfViewY, aspectRatio, nearClip, farClip));
    }
    
    /**
     * Returns the location of the near clipping plane.
     */
    public float getNearClip() {
        return nearClip;
    }
    
    /**
     * Sets the location of the near clipping plane.
     */
    public void setNearClip(float nc) {
        nearClip = nc;
        setProjectionTransform(Matrix.createProjectionTransform(
            fieldOfViewY, aspectRatio, nearClip, farClip));
    }
    
    /**
     * Returns the location of the far clipping plane.
     */
    public float getFarClip() {
        return farClip;
    }
    
    /**
     * Sets the location of the near clipping plane.
     */
    public void setFarClip(float fc) {
        farClip = fc;
        setProjectionTransform(Matrix.createProjectionTransform(
            fieldOfViewY, aspectRatio, nearClip, farClip));
    }
    
    /**
     * Frees native resources associated with this environment.
     */
    void dispose() {
        synchronized (this) {
            task.cancel();
        }
        _dispose(nativePtr);
    }
    
    static synchronized void disposeAll() {
        for (Environment3D env : env3DList) {
            
            try {
                synchronized (env) {
                    env.suspendRendering();
                    env.dispose();
                }
            } catch (Exception x) {}
        }
        env3DList.clear();
    }
    
    /**
     * Sets the background color of this component.
     *
     * @param c the color to become the background color; if this parameter 
     *          is <tt>null</tt>, then Color.BLACK will be used
     */
    @Override
    public synchronized void setBackground(Color c) {
        if (null == c) {
            c = Color.BLACK;
        }
        
        super.setBackground(c);
        
        _setBackground(nativePtr, c.getRGB());
    }
    
    /**
     * Sets the background image of this component.
     *
     * @param t the Texture object to be used as the background image,
     *          or <tt>null</tt> to remove the current texture
     */
    public synchronized void setBackground(Texture t) {
        _setTextureBackground(nativePtr, t.nativePtr());
    }

    @Override
    public void paint(Graphics g) {
        try {
            if (running && !renderingError) {
                // Fire rendering event
                synchronized (this) {
                    fireFrameRendering(fre);
                }
                super.paint(g);
            }
        } catch (Throwable t) {
            renderingError = true;
            throw new RuntimeException(t);
        }
    }
    
    /**
     * Adds a listener to be notified of rendering events.
     *
     * @param l the listener to be added - <tt>null</tt> values ignored
     */
    public synchronized void addRenderListener(RenderListener l) {
        if (l != null) {
            rlList.add(l);
        }
    }
    
    /**
     * Removes a listener being notified of rendering events.
     *
     * @param l the listener to be removed - <tt>null</tt> values ignored
     */
    public synchronized void removeRenderListener(RenderListener l) {
        if (l != null) {
            rlList.remove(l);
        }
    }
    
    private synchronized void fireFrameRendering(FrameRenderingEvent e) {
        if (null == e) {
            throw new NullPointerException("e");
        }
        for (RenderListener l : rlList) {
            l.frameRendering(e);
        }
    }
    
    /**
     * Adds a light source to this environment.
     *
     * @param light the light source to be added
     * @throws NullPointerException if <tt>light</tt> is <tt>null</tt>
     */
    public synchronized void addLight(Light light) {
        if (null == light) {
            throw new NullPointerException("light");
        }
        
        _addLight(nativePtr, light.nativePtr());
        // TODO Check that we don't exceed the max. number of lights
        lightList.add(light);
        
        fireLightAdded(new Environment3DChangeEvent(this, null, null, light));
    }
    
    /**
     * Removes a light source from this environment.
     *
     * @param light the light source to be removed
     * @throws NullPointerException if <tt>light</tt> is <tt>null</tt>
     */
    public synchronized void removeLight(Light light) {
        if (null == light) {
            throw new NullPointerException("light");
        }
        
        _removeLight(nativePtr, light.nativePtr());
        lightList.remove(light);
        
        fireLightRemoved(new Environment3DChangeEvent(this, null, null, light));
    }
    
    /**
     * Returns an iterator containing all lights in this environment.
     * For thread safety, if an Environment3D instance is used by
     * more than one thread, use the Iterator within a synchronized block
     * like so:
     * <pre>
     * Environment3D env = ...;
     * synchronized (env) {
     *     Iterator&lt;Light&gt; i = env.getLights();
     *     // use Iterator...
     * }
     * </pre>
     *
     * @return an iterator containing all lights in this environment
     */
    public synchronized Iterator<Light> getLights() {
        return lightList.iterator();
    }
    
    /**
     * Sets an ambient light.
     *
     * @param light the ambient light color to use or <tt>null</tt> to use
     *                  no ambient lighting
     */
    public synchronized void setAmbientLight(Color light) {
        _setAmbientLight(nativePtr, light != null,
            light != null ? light.getRed() : 0,
            light != null ? light.getGreen() : 0,
            light != null ? light.getBlue() : 0);
        this.ambientLight = light;
    }
    
    /**
     * Returns the current ambient light color, or <tt>null</tt>
     * if no ambient light is being used.
     */
    public synchronized Color getAmbientLight() {
        return ambientLight;
    }
    
    //
    // Native methods
    //
    private static native synchronized long getNativePtr();
    private native void _setLightingEnabled(long nativePtr, 
        boolean lighting);
    private native void _addLight(long nativePtr, long lightPtr);
    private native void _removeLight(long nativePtr, long lightPtr);
    private native void _setAmbientLight(long nativePtr, 
        boolean ambientPresent, int ambR, int ambG, int ambB);
    private native void _updateBackBufferSize(long nativePtr, int width, 
        int height);
    private native void _setTransformationEnabled(long nativePtr, 
        boolean transform);
    private native void _addPrimitive(long nativePtr, long primitivePtr);
    private native void _removePrimitive(long nativePtr, long primitivePtr);
    private native void _addMesh(long nativePtr, long meshPtr);
    private native void _setWorldTransform(long nativePtr, Matrix m);
    private native void _setViewTransform(long nativePtr, Matrix m);
    private native void _setProjectionTransform(long nativePtr, Matrix m);
    private native void _setBackground(long nativePtr, int rgb);
    private native void _setTextureBackground(long nativePtr, long texPtr);
    private native void _dispose(long nativePtr);
    
    //
    // Private classes
    //
    
    private class RenderTask extends TimerTask {
        public void run() {
            repaint();
        }
    }
}

