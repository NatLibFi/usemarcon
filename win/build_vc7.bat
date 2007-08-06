@echo off
cd library
devenv usemarcon_library.vcproj /build "Release" 
if errorlevel 1 goto error
cd ..
cd program
devenv usemarcon.vcproj /build "Release" 
if errorlevel 1 goto error
copy release\usemarcon.exe ..
cd ..
goto end

:error
echo Build failed!

:end