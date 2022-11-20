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

Last modified | **August/31/2022**

tests
----
testing the compilers  

Visual Studio_cl  
CLANG  
Watcom v2   

About
-----
this is a continuation of the "jdw" project It is designed to be
easy to use, with some of the most advanced Direct3D features
abstracted.


Features
--------
* Rendering 3D scenes within a java.awt.Canvas  
-  problems loading Directx 12 in Windows.10.SDK


Requirements
------------

* Java SE (JDK).  (sdk 8 +)  
   https://www.oracle.com/java/technologies/downloads/  

>***To compile and run Java programs that use the framework*** :  

>***To compile javah-direct3D sources*** :  
* Microsoft DirectX SDK (June 2010).  
   https://www.microsoft.com/en-us/download/details.aspx?id=8109  
   
* Microsoft Visual C++ compiler (7.1 or newer).  

* Microsoft Platform SDK. 
   http://msdn.microsoft.com/visualc/downloads/msproducts/default.aspx  


How to build
------------

Use the batch files  
___
```compile_win32.bat``` :   
  
Compiles the DirectX | ```x32``` Windows binaries.  

Modify the variables DIR_DX to refer to your DirectX installation respectively (inside the batch file).  
___	
```compile_win64.bat``` :  
  
Compiles the DirectX | ```x64``` Windows binaries.  

Modify the variables DIR_DX to refer to your DirectX installation respectively (inside the batch file).  
___
```compile_java_only.bat``` :   

  java class library only.

Each of the above three batch files create the (dxwj.jar) file in ```COMPILED/java``` folder
after compilation.  
___
```compile_full_win64.bat``` :   

  Compile DirectX natives and Java library .

Each of the above three batch files create the (dxwj.jar) file in ```COMPILED/java``` folder
after compilation. 
___
```clean.bat``` :  

Deletes the COMPILED folder and all contents that is in it including the compiled java files and compiled C++ files.  
___	

Folder structure
----------------

>```COMPILED```  
⠀⠀⠀⠀⠀⠀⠀⠀L>```/java```  | Java Compiled Program  
⠀⠀⠀⠀⠀⠀⠀⠀L>```/native``` | Native Compiled (DLLs)  
___
>```src```  
⠀⠀⠀L>```/java```  | Java Source Code  
⠀⠀⠀L>```/native``` | C/C++ Source Code  
  
___c----------------------------------------------------------------------------------------------------------------ↄ___
