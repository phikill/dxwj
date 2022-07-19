@echo off
REM Change the two lines below as needed
SET DIR_JAVA=C:\Program Files\Java\jdk1.6.0
SET DIR_DX=C:\Program Files (x86)\Microsoft DirectX SDK (June 2006)

mkdir bin\x64 1>NUL 2>NUL

mkdir classes
javac -d classes -target 1.5 src/Java/jdw/graphics/*.java src/Java/jdw/util/*.java src/Java/jdw/demo/*.java
javah -classpath classes -d src/C++ jdw.graphics.NativeCanvas ^
  jdw.graphics.WindowManager ^
  jdw.graphics.Environment3D jdw.graphics.Primitive ^
  jdw.graphics.Matrix jdw.graphics.Light ^
  jdw.graphics.Material jdw.graphics.Texture ^
  jdw.graphics.Mesh

cl /nologo /LD /W4 /EHsc /Febin/x64/WinMan.dll ^
  "/I%DIR_JAVA%\include" "/I%DIR_JAVA%\include\win32" ^
  "src/C++/WinMan.cpp" user32.lib psapi.lib /link /MACHINE:X64

REM cl /nologo /LD /W4 /Febin/x64/Renderer.dll "src/C++/Renderer.cpp" ^
REM  /link /MACHINE:X64

cl /nologo /c /W4 /EHsc "/I%DIR_DX%\Include" ^
 "/I%DIR_JAVA%\include" "/I%DIR_JAVA%\include\win32" ^
 "src/C++/DXRenderer.cpp" "src/C++/Environment3D.cpp" ^
 "src/C++/Primitive.cpp" "src/C++/Matrix.cpp" "src/C++/Light.cpp" ^
 "src/C++/Mesh.cpp" "src/C++/Texture.cpp" "src/C++/PrimitiveTextured.cpp"

link /nologo /DLL DXRenderer.obj Environment3D.obj Primitive.obj ^
 Matrix.obj Light.obj Mesh.obj Texture.obj PrimitiveTextured.obj ^
 "%DIR_DX%\lib\x64\d3d9.lib" "%DIR_DX%\lib\x64\d3dx9.lib" ^
 "%DIR_DX%\lib\x64\dxerr9.lib" ^
 /MACHINE:X64 /out:bin/x64/DXRenderer.dll

del *.obj
del bin\x86\*.exp 1>NUL 2>NUL
del bin\x86\*.lib 1>NUL 2>NUL
del bin\x64\*.exp 1>NUL 2>NUL
del bin\x64\*.lib 1>NUL 2>NUL
jar cvf jdw.jar bin -C classes jdw 1>NUL 2>NUL
