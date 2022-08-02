+-------------------------------------------------------------------------------+  
|⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|  
|⠀⠀⠀⠀⠀⠀DXWJ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|  
|⠀⠀⠀⠀⠀⠀DirectX Wrapper Java.⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|  
|⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|  
|⠀⠀⠀⠀⠀⠀a continuation of the jdw project⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|  
|⠀⠀⠀⠀⠀⠀jdw: A Java Wrapper Library for Direct3D⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|  
+--------------------------------------------------------------------------------+  


README
-----
Last modified 20 July 2022.


About
-----
this is a continuation of the "jdw" project It is designed to be
easy to use, with some of the most advanced Direct3D features
abstracted.

This project still uses DirectX 9.0

Features
--------
* Rendering 3D scenes within a java.awt.Canvas  
* Applying lighting and textures  
* Loading meshes from .X files  


Requirements
------------

* Java 8 (JDK).  
   http://java.sun.com/javase/downloads/index.jsp  

To compile and run Java programs that use the framework:  
* Microsoft DirectX 9 End-user runtime.  
   http://www.microsoft.com/windows/Directx/default.mspx  

To compile java-direct3D sources:  
* Microsoft DirectX SDK (June 2010).  
   https://www.microsoft.com/en-us/download/details.aspx?id=8109  
* Microsoft Visual C++ compiler (7.1 or newer).  
* Microsoft Platform SDK. 
   http://msdn.microsoft.com/visualc/downloads/msproducts/default.aspx  


How to build
------------

Use the batch files:  

compile_win32.bat:   
  
	Compiles the Direct3D Java class library followed by 
	the supporting 32-bit Windows binaries. Modify the
        variables DIR_JAVA and DIR_DX to refer to your JDK
	and DirectX installation respectively (inside the
	batch file).  
compile_win64.bat:  
  
	Compiles the Direct3D Java class library followed by 
	the supporting 64-bit Windows binaries. Modify the
        variables DIR_JAVA and DIR_DX to refer to your JDK
	and DirectX installation respectively (inside the
	batch file).  
compile_java_only.bat:   
  class library only.

Each of the above three batch files create the jdw.jar file
after compilation.  

Other batch files
-----------------

clean.bat:  
	Deletes compiled DLLs (bin folder), Java classes (classes folder),
	and jdw.jar.
make_javadoc.bat:  
	Creates Javadoc documentation.
run_tutorialXXX.bat:  
	Runs the tutorial program number XXX.


Folder structure
----------------
+bin\	        : Native libraries (DLLs)
+classes\       : Java classes
+documentation\ : Javadoc documentation
+src\	        : Java and C++ source code


EOF
