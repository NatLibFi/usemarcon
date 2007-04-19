@echo off

echo Creating Parser..
perl ../bison+.pl --input analyse2.y --class MarcParser --cpp ytab.cpp --hdr ytab.h --no-lines
if errorlevel 1 goto bisonerr
echo Creating Scanner..
flex -L -olex.yy.cpp Analyse2.l
if errorlevel 1 goto flexerr
goto end
:bisonerr
echo Parser creation failed
goto end
:flexerr
echo Scanner creation failed
goto end
:end