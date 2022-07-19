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

import java.util.Arrays;

/**
 * A 4x4 matrix. This class is immutable.
 *
 * @author Matthew Ross
 */
public final class Matrix {
    private float[][] data;
    
    static {
        LibraryLoader.load("DXRenderer");
    }
    
    private Matrix() {
        data = new float[4][4];
    }
    
    /**
     * Creates a new Matrix initialised using the specified array.
     *
     * @throws IllegalArgumentException if <tt>in</tt> is not a non-null 
     *          array of array of floats (each of length 4) of length 4
     */
    public static Matrix fromArray(float[][] in) {
        if (!isMatrixValid(in)) {
            throw new IllegalArgumentException();
        }
        Matrix copy = new Matrix();
        
        // Copy array data
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                copy.data[i][j] = in[i][j];
            }
        }
        
        return copy;
    }
    
    /**
     * Creates a new Matrix initialised using the specified matrix.
     */
    public static Matrix fromExisting(Matrix m) {
        if (!isMatrixValid(m.data)) {
            throw new IllegalArgumentException();
        }
        Matrix copy = new Matrix();
        
        // Copy array data
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                copy.data[i][j] = m.data[i][j];
            }
        }
        
        return copy;
    }
    
    /**
     * Returns <tt>true</tt> if <tt>in</tt> is a non-null array of array of
     * floats (each of length 4) of length 4, <tt>false</tt> otherwise.
     *
     * @param in the float[] array to be verified
     * @return see above
     */
    private static boolean isMatrixValid(float[][] in) {
        return in != null && in.length == 4 
            && in[0] != null && in[0].length == 4
            && in[1] != null && in[1].length == 4
            && in[2] != null && in[2].length == 4
            && in[3] != null && in[3].length == 4;            
    }
    
    /**
     * Creates a new Matrix object initialised to the identity matrix.
     *
     * @return the identity matrix
     */
    public static Matrix identity() {
        Matrix m = new Matrix();
        m.data[0][0] = 1f;
        m.data[1][1] = 1f;
        m.data[2][2] = 1f;
        m.data[3][3] = 1f;
        return m;
    }
    
    /**
     * Returns the value at some row and column.
     * 
     * @param row the row number (0-3 inclusive)
     * @param col the column number (0-3 inclusive)
     * @return the value at row <tt>row</tt> and column <tt>col</tt>
     * @throws IllegalArgumentException if the row or col number
     *          is not valid
     */
    public float getValueAt(int row, int col) {
        if (row < 0) {
            throw new IllegalArgumentException("row ("+row+") < 0");
        }
        if (row > 3) {
            throw new IllegalArgumentException("row ("+row+") > 3");
        }
        if (col < 0) {
            throw new IllegalArgumentException("col ("+col+") < 0");
        }
        if (col > 3) {
            throw new IllegalArgumentException("col ("+col+") > 3");
        }
        return data[row][col];
    }
    
    /**
     * Creates a matrix using the specified offsets.
     *
     * @param x number of units to translate in the X-direction
     * @param y number of units to translate in the Y-direction
     * @param z number of units to translate in the Z-direction
     * @return the translation matrix
     */
    public static Matrix translate(float x, float y, float z) {
        Matrix m = new Matrix();
        _translate(m.data, x, y, z);
        return m;
    }
    
    /**
     * Creates a new matrix to represent a rotation around the X-axis.
     *
     * @param angle the rotation angle in radians
     * @return the rotation matrix
     */
    public static Matrix rotationX(float angle) {
        Matrix m = new Matrix();
        _rotationX(m.data, angle);
        return m;
    }
    
    /**
     * Creates a new matrix to represent a rotation around the Y-axis.
     *
     * @param angle the rotation angle in radians
     * @return the rotation matrix
     */
    public static Matrix rotationY(float angle) {
        Matrix m = new Matrix();
        _rotationY(m.data, angle);
        return m;
    }
    
    /**
     * Creates a new matrix to represent a rotation around the Z-axis.
     *
     * @param angle the rotation angle in radians
     * @return the rotation matrix
     */
    public static Matrix rotationZ(float angle) {
        Matrix m = new Matrix();
        _rotationZ(m.data, angle);
        return m;
    }
    
    /**
     * Creates a matrix that scales along the X, Y and Z-axes.
     *
     * @param x X-axis scaling factor
     * @param y Y-axis scaling factor
     * @param z Z-axis scaling factor
     * @return the scaling matrix
     */
    public static Matrix scale(float x, float y, float z) {
        Matrix m = new Matrix();
        _scale(m.data, x, y, z);
        return m;
    }
    
    /**
     * Calculates the product of two matrices and returns the result.
     * 
     * @param m1 the first matrix
     * @param m2 the second matrix
     * @return a matrix representing <tt>m1</tt> * <tt>m2</tt>
     * @throws NullPointerException if <tt>m1</tt> or <tt>m2</tt> 
     *                                  is <tt>null</tt>
     */
    public static Matrix multiply(Matrix m1, Matrix m2) {
        if (null == m1) {
            throw new NullPointerException("m1");
        }
        if (null == m2) {
            throw new NullPointerException("m2");
        }
        
        Matrix result = new Matrix();
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                float sum = 0;
                for (int z=0; z<4; z++) {
                    sum += m1.data[x][z] * m2.data[z][y];
                }
                result.data[x][y] = sum;
            }
        }

        return result;
    }
    
    /**
     * Creates a matrix with the specified yaw, pitch and roll.
     * The rotation around the z-axis is done first, 
     * followed by rotation around the x-axis, 
     * followed by rotation around the y-axis. Duplicates the semantics
     * of the D3DXMatrixRotationYawPitchRoll function in the
     * D3DX utility library.
     *
     * @param yaw   yaw around the Y-axis, in radians
     * @param pitch pitch around the X-axis, in radians
     * @param roll  roll around the Z-axis, in radians
     * @return the new rotation matrix
     */
    public static Matrix rotationYawPitchRoll(
                float yaw, float pitch, float roll) {
        
        Matrix m = new Matrix();
        _d3dxMatrixRotationYawPitchRoll(m.data, yaw, pitch, roll);
        return m;
    }
    
    /**
     * Creates a matrix that can be used as the view transform.
     * The "up direction" is the positive Y-axis direction (0, 1, 0).
     *
     * @param eyePt     the point at which the eye is placed
     * @param lookAtPt  the point at which the eye is directed
     */
    static Matrix createViewTransform(
            Vector3D eyePt, Vector3D lookAtPt) {
        
        return createViewTransform(eyePt, lookAtPt, new Vector3D(0f, 1f, 0f));
    }
    
    /**
     * Creates a matrix that can be used as the view transform.
     *
     * @param eyePt     the point at which the eye is placed
     * @param lookAtPt  the point at which the eye is directed
     * @param upVector  the point defining the "up" direction - usually
     *                   the positive Y-axis direction (0, 1, 0)
     */
    static Matrix createViewTransform(
            Vector3D eyePt, Vector3D lookAtPt, Vector3D upVector) {
        Matrix view = new Matrix();
        _createViewTransform(view.data, eyePt, lookAtPt, upVector);
        return view;
    }
    
    /**
     * Creates a matrix that can be used as the projection transform.
     *
     * @param fovy      the field of view in radians
     * @param aspect    the scene's aspect ratio
     * @param zn        location of the near clipping plane
     * @param zf        location of the far clipping plane
     * @return the projection transform
     */
    static Matrix createProjectionTransform(
            float fovy, float aspect, float zn, float zf) {
        float yScale = (float) (1.0f/Math.tan(fovy/2.0f));
        float xScale = aspect*yScale;
        
        float _33 = zf/(zf-zn);      // row 3, col 3
        float _43 = -zn*zf/(zf-zn);  // row 4, col 3
        
        float[][] data = new float[][] { 
            new float[] {xScale, 0f, 0f, 0f},
            new float[] {0f, yScale, 0f, 0f},
            new float[] {0f, 0f, _33, 1f},
            new float[] {0f, 0f, _43, 0f}
        };
        
        return Matrix.fromArray(data);
    }
    
    /////////////////////////////////////////////////////////////////
    // Native methods below
    /////////////////////////////////////////////////////////////////
    
    private static native float[][] _translate(
        float[][] data, float x, float y, float z);
    
    private static native float[][] _rotationX(float[][] data, float angle);
    
    private static native float[][] _rotationY(float[][] data, float angle);
    
    private static native float[][] _rotationZ(float[][] data, float angle);
    
    private static native float[][] _scale(
        float[][] data, float x, float y, float z);
    
    private static native void _createViewTransform(float[][] data, 
        Vector3D eyePt, Vector3D lookAtPt, Vector3D upVector);
    
    private static native void _d3dxMatrixRotationYawPitchRoll(
        float[][] data, float yaw, float pitch, float roll);
    
    /////////////////////////////////////////////////////////////////
    @Override
    public String toString() {
        StringBuilder buf = new StringBuilder();
        buf.append(Arrays.toString(data[0])).append("\n");
        buf.append(Arrays.toString(data[1])).append("\n");
        buf.append(Arrays.toString(data[2])).append("\n");
        buf.append(Arrays.toString(data[3]));
        return buf.toString();
    }
    
    public static void main(String[] args) {
        System.out.println("starting tests");
        Matrix $1 = new Matrix();
        $1.rotationX(2);
        System.out.println($1);
        System.out.println("tests ended");
    }
}

