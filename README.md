__DXWJ__
--------
c-------------------------------------------------------------------------------ↄ  
|⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀DXWJ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀DirectX Wrapper Java.⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀a continuation of the jdw project⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
|⠀⠀⠀⠀⠀⠀jdw: A Java Wrapper Library for Direct3D⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  
c-------------------------------------------------------------------------------ↄ  

Last modified | **August/27/2022**


About
-----
this is a continuation of the "jdw" project It is designed to be
easy to use, with some of the most advanced Direct3D features
abstracted.

This project still uses DirectX 9.0

Features
--------
* Rendering 3D scenes within a java.awt.Canvas  
* Bases dds Textures  
* included .fx reader  
-  more hlsl problems :( |


Requirements
------------

* Java SE (JDK).  
   https://www.oracle.com/java/technologies/downloads/  

>***To compile and run Java programs that use the framework*** :  
* Microsoft DirectX End-user runtime.  
   https://www.microsoft.com/en-us/download/details.aspx?id=8109  

>***To compile java-direct3D sources*** :  
* Microsoft DirectX SDK (June 2010).  
   https://www.microsoft.com/en-us/download/details.aspx?id=8109  
   
* Microsoft Visual C++ compiler (7.1 or newer).  

* Microsoft Platform SDK. 
   http://msdn.microsoft.com/visualc/downloads/msproducts/default.aspx  


How to build
------------

Use the batch files  
___
>```compile_win32.bat``` :   
  
Compiles the Direct3D Java class library followed by the supporting 32-bit Windows binaries.  

Modify the variables DIR_JAVA and DIR_DX to refer to your JDK  
	and DirectX installation respectively (inside the batch file).  
___	
>```compile_win64.bat``` :  
  
Compiles the Direct3D Java class library followed by the supporting 64-bit Windows binaries.  

Modify the variables DIR_JAVA and DIR_DX to refer to your JDK  
	and DirectX installation respectively (inside the batch file).  
___
>```compile_java_only.bat``` :   

  java class library only.

Each of the above three batch files create the (dxwj.jar) file in COMPILED/java folder
after compilation.  
___
>```clean.bat``` :  

Deletes the COMPILED folder and all contents that is in it including the compiled java files and compiled C++ files.  
___	


Folder structure
----------------
```COMPILED```  
⠀⠀⠀⠀⠀⠀⠀⠀L```/java```  | Java Compiled Program  
⠀⠀⠀⠀⠀⠀⠀⠀L```/native``` | Native Compiled (DLLs)  
___
```src```  
⠀⠀⠀L```/java```  | Java Source Code  
⠀⠀⠀L```/native``` | C/C++ Source Code  
  
___c----------------------------------------------------------------------------------------------------------------ↄ___
