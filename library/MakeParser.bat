@echo off

rem Parser creation requires e.g. cygwin shell with perl, flex and bison
rem Note that FlexLexer.h needs to correspond to the flex version (copy the used version into library if necessary)

echo Creating Parser..
perl ../bison+.pl --input analyse.y --class MarcParser --cpp ytab.cpp --hdr ytab.h --no-lines
if errorlevel 1 goto bisonerr
echo Creating Scanner..
flex -L -olex.yy.cpp --nounistd Analyse.l 
echo Fixing Scanner Destructor (problem in older flex versions)..
perl -pi -e 's/delete yy_state_buf;/delete []yy_state_buf;/' lex.yy.cpp 
if errorlevel 1 goto flexerr
echo Removing "#define yyFlexLexer"..
perl -pi -e 's/#define yyFlexLexer/\/\/#define yyFlexLexer/' lex.yy.cpp
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