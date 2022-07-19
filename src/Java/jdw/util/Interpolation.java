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

package jdw.util;

/**
 * Provides a smooth transition between two numerical values.
 *
 * @author Matthew Ross
 */
public class Interpolation {
    private float start;
    private float end;
    private long duration;
    private long startTime;
    
    /**
     * Creates a new <tt>Interpolation</tt>.
     *
     * @param start the starting value
     * @param end the ending value
     * @param duration the time it should take to go from
     * the start value to the end value
     */
    public Interpolation(float start, float end, long duration) {
        this.start = start;
        this.end = end;
        this.duration = duration;
        this.startTime = System.currentTimeMillis();
    }
    
    /**
     * Returns the current value of the interpolation.
     */
    public float currentValue() {
        long elapsedTime = System.currentTimeMillis()-startTime;
        elapsedTime %= duration;
        
        return start*(float) elapsedTime/(end-start);
    }
    
    public static void main(String[] args) {
        System.out.println("starting tests");
// TODO test this class
        System.out.println("tests ended");
    }
}
