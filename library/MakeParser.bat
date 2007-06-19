@echo off

echo Creating Parser..
perl ../bison+.pl --input analyse.y --class MarcParser --cpp ytab.cpp --hdr ytab.h --no-lines
if errorlevel 1 goto bisonerr
echo Creating Scanner..
flex -L -olex.yy.cpp Analyse.l
echo Fixing Scanner Destructor..
perl -pi -e 's/delete yy_state_buf;/delete []yy_state_buf;/' lex.yy.cpp
if errorlevel 1 goto flexerr
echo All done.
goto end
:bisonerr
echo Parser creation failed
goto end
:flexerr
echo Scanner creation failed
goto end
:end