@echo off
cd library
msdev usemarcon_library.dsp /MAKE "usemarcon_library - Win32 Release MinDependency" /CLEAN
if errorlevel 1 goto error
cd ..
cd program
msdev usemarcon.dsp /MAKE "usemarcon - Win32 Release" /CLEAN
if errorlevel 1 goto error
cd ..
goto end

:error
echo Clean failed!

:end