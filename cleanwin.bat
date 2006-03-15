@echo off
cd library
devenv usemarcon_library.vcproj /clean "Release MinDependency" 
if errorlevel 1 goto error
cd ..
cd program
devenv usemarcon.vcproj /clean "Release"
if errorlevel 1 goto error
cd ..
goto end

:error
echo Clean failed!

:end