@echo off
cd library
msdev usemarcon_library.dsp /MAKE "usemarcon_library - Win32 Release MinDependency" 
if errorlevel 1 goto error
cd ..
cd program
msdev usemarcon.dsp /MAKE "usemarcon - Win32 Release" 
if errorlevel 1 goto error
copy runtime\usemarcon.exe ..
cd ..
goto end

:error
echo Build failed!

:end