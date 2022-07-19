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

import java.util.*;
import java.io.*;

/**
 * Loads required dynamic libraries.
 *
 * @author Matthew Ross
 */
class LibraryLoader {
    private List<String> filesExtracted;
    private Map<String,String> libToFileMap;
    private String tempPath;
    private static LibraryLoader inst = null;
    
    private LibraryLoader() {
        filesExtracted = new ArrayList<String>();
        libToFileMap = new HashMap<String,String>();
        tempPath = null;
        
        // Add a JVM shutdown hook to
        //  delete temp files beginning with __jd3d_
        //  and dispose of objects (Env3D, Primitive etc.)
        
        Runtime.getRuntime().addShutdownHook(new Thread() {
            @Override
            public void run() {
                deleteTempLibraries();
                disposeAllObjects();
            }
        });
    }
    
    private static LibraryLoader getInstance() {
        if (null == inst) {
            inst = new LibraryLoader();
        }
        return inst;
    }
    
    private static String getArch() {
        String arch = System.getProperty("os.arch");
        
        if (arch.equalsIgnoreCase("amd64")) {
            return "x64";
        } else if (arch.equalsIgnoreCase("x86")
                || arch.equalsIgnoreCase("i386")) {
            return "x86";
        }
        
        return "!!!Unknown!!!";
    }
    
    private String loadLibrary(String lib) {
        if (!filesExtracted.contains(lib)) {
            try {
                
                File tempFile = File.createTempFile("__jd3d_"+lib, ".dll");
                tempFile.deleteOnExit();
                tempPath = tempFile.getAbsolutePath();
                
                try {
                    tempPath = tempPath.substring(0, 
                        tempPath.lastIndexOf(File.separator));
                } catch (Exception _x) {}
                
                byte[] buf = new byte[1024];
                // Read from resource and copy to temp file
                
                String resName = "/bin/"+getArch()+"/"+lib+".dll";
                InputStream res = LibraryLoader.class.getResourceAsStream(
                    resName);
                if (null == res) {
                    System.err.println("Resource "+resName+" not found");
                    return "";
                }
                
                DataInputStream in = new DataInputStream(res);
                DataOutputStream out = new DataOutputStream(
                    new FileOutputStream(tempFile));
                
                while (true) {
                    
                    int count = in.read(buf, 0, buf.length);
                    if (count <= 0) {
                        break;
                    }
                    
                    out.write(buf, 0, count);
                }
                
                out.flush();
                out.close();
                in.close();
                
                filesExtracted.add(lib);
                
                // Load the library now!!!!
                System.load(tempFile.toString());
                
                libToFileMap.put(lib, tempFile.toString());
                
                return tempFile.toString();
                
            } catch (Exception x) {
                System.err.println(x);
                x.printStackTrace();
                
                try {
                    // Try loading the normal way
                    System.loadLibrary(lib);
                } catch (Exception x2) {
                    System.err.println(x2);
                }
            }
        }
        
        return libToFileMap.get(lib);
    }
    
    static synchronized String load(String lib) {
        LibraryLoader loader = LibraryLoader.getInstance();
        return loader.loadLibrary(lib);
    }
    
    
    //
    // Shutdown hook methods
    //
    
    private void deleteTempLibraries() {
        try {
            if (tempPath != null) {
                File[] files = new File(tempPath).listFiles();
                
                for (File f : files) {
                    // Delete temp files beginning with __jd3d_
                    
                    if (f != null && f.getName() != null) {
                        String fn = f.getName();
                    
                        if (fn.startsWith("__jd3d_")) {
                            f.delete();
                        }
                    }
                }
            }
        } catch (Exception x) {
        }
    }
    
    /*
     * Convenience method so the user doesn't have to dispose anything.
     */
    private void disposeAllObjects() {
        try {
            // 1. Suspend rendering and dispose all Environment3Ds
            
            Environment3D.disposeAll();
            
            // 2. Dispose all Primitives
            
     //       Primitive.disposeAll(); // TODO Broken on 32-bit Windows
            
            // TODO more
            
        } catch (Exception x) {
        }
    }
}

