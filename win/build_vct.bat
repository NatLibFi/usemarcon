@echo off

set USEMARCON_FLAGS=-O2 -Og -Ob2 -DWIN32 -D_WINDOWS -DHAVE_STDLIB_H -DHAVE_FCNTL_H -DHAVE_IO_H -DHAVE_STRING_H

cd library

cl -c %USEMARCON_FLAGS% chckfile.cpp codedata.cpp ctrlfld.cpp error.cpp marcfile.cpp mrcfield.cpp rulefile.cpp semi.cpp tagnoind.cpp tcd.cpp tcdlib.cpp tfile.cpp tmarcrec.cpp trule.cpp umrecord.cpp lien_not.cpp tchckdoc.cpp tmarcdoc.cpp tmpplctn.cpp tools.cpp trnsfile.cpp truledoc.cpp ttrnsdoc.cpp ytab.cpp lex.yy.cpp regexp.cpp

cd ..\program

cl %USEMARCON_FLAGS% usemarcon.cpp -I..\library -link ..\library\chckfile.obj ..\library\codedata.obj ..\library\ctrlfld.obj ..\library\error.obj ..\library\marcfile.obj ..\library\mrcfield.obj ..\library\rulefile.obj ..\library\semi.obj ..\library\tagnoind.obj ..\library\tcd.obj ..\library\tcdlib.obj ..\library\tfile.obj ..\library\tmarcrec.obj ..\library\trule.obj ..\library\umrecord.obj ..\library\lien_not.obj ..\library\tchckdoc.obj ..\library\tmarcdoc.obj ..\library\tmpplctn.obj ..\library\tools.obj ..\library\trnsfile.obj ..\library\truledoc.obj ..\library\ttrnsdoc.obj ..\library\ytab.obj ..\library\lex.yy.obj ..\library\regexp.obj

copy usemarcon.exe ..

cd ..