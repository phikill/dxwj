+-------------------------------------------------------------------------------+  
|⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀DXWJ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀DirectX Wrapper Java.⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀a continuation of the jdw project⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀jdw: A Java Wrapper Library for Direct3D⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
+--------------------------------------------------------------------------------+  


README
-----
Last modified | August  2  2022 


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

* Java SE (JDK).  
   https://www.oracle.com/java/technologies/downloads/  

To compile and run Java programs that use the framework:  
* Microsoft DirectX End-user runtime.  
   https://www.microsoft.com/en-us/download/details.aspx?id=8109  

-To compile java-direct3D sources:  
* Microsoft DirectX SDK (June 2010).  
   https://www.microsoft.com/en-us/download/details.aspx?id=8109  
   ___
* Microsoft Visual C++ compiler (7.1 or newer).  

* Microsoft Platform SDK. 
   http://msdn.microsoft.com/visualc/downloads/msproducts/default.aspx  


How to build
------------

Use the batch files  
___
compile_win32.bat :   
  
Compiles the Direct3D Java class library followed by the supporting 32-bit Windows binaries.  

Modify the variables DIR_JAVA and DIR_DX to refer to your JDK  
	and DirectX installation respectively (inside the batch file).  
___	
compile_win64.bat :  
  
Compiles the Direct3D Java class library followed by the supporting 64-bit Windows binaries.  

Modify the variables DIR_JAVA and DIR_DX to refer to your JDK  
	and DirectX installation respectively (inside the batch file).  
___
compile_java_only.bat :   

  java class library only.

Each of the above three batch files create the (COMPILED\java\dxwj.jar) file
after compilation.  

Other batch files
-----------------

clean.bat:  

Deletes the COMPILED folder and all contents that is in it including the compiled java files and compiled C++ files.  
___	
make_javadoc.bat:  

Creates Javadoc documentation.  
___	
run_tutorialXXX.bat:  

Runs the tutorial program number XXX.  


Folder structure
----------------
+COMPILED\java		: Java classes
+COMPILED\native\ 	: Native libraries (DLLs)  
+documentation\ 	: Javadoc documentation  
+src\	        	: Java and C++ source code  
  
+----------------------------------------------------------------------------------------------------------------+
