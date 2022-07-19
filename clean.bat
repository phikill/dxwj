@echo off
REM
REM Deletes the contents of the bin folder
REM	and all of the Java .class files
REM

del bin\x64\*.exp 1>NUL 2>NUL
del bin\x64\*.lib 1>NUL 2>NUL
del bin\x64\*.dll 1>NUL 2>NUL
del bin\x86\*.exp 1>NUL 2>NUL
del bin\x86\*.lib 1>NUL 2>NUL
del bin\x86\*.dll 1>NUL 2>NUL
rmdir /S /Q classes\gfx 1>NUL 2>NUL
del jdw.jar 1>NUL 2>NUL
