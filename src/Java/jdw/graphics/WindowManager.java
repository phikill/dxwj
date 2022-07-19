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

import java.awt.Window;
import java.awt.Component;

/**
 * Provides some functions that operate on windows.
 *
 * @author Matthew Ross
 */
public class WindowManager {
    static {
        try {
            LibraryLoader.load("WinMan");
        } catch (UnsatisfiedLinkError x) {
            System.err.println("WindowManager library not found");
        }
    }

    private WindowManager() {}

    /*
     * Allows a the specified window to be dragged using the client
     * area of the window, not just the title bar.
     *
    public static native void setDraggable(Window w, boolean drag);
     */
    /**
     * Sets the opacity of a window. Valid opacity values range from
     * 0 (transparent) to 255 (opaque). -1 switches off any transparency.
     *
     * @param opacity the opacity value
     */
    public static native void setOpacity(Window w, int opacity);

    /*
     * Returns the native window handle (HWND) value.
     *
    public static native int getHWND(Component c);
     */
    /**
     * Fades in a window.
     */
    public static native void fadeIn(Window w);

    /**
     * Fades out a window.
     */
    public static native void fadeOut(Window w);
}
