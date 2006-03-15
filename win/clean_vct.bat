@echo off

cd library

del chckfile.obj codedata.obj ctrlfld.obj error.obj marcfile.obj mrcfield.obj rulefile.obj semi.obj tagnoind.obj tcd.obj tcdlib.obj tfile.obj tmarcrec.obj trule.obj umrecord.obj lien_not.obj tchckdoc.obj tmarcdoc.obj tmpplctn.obj tools.obj trnsfile.obj truledoc.obj ttrnsdoc.obj ytab.obj lex.yy.obj >nul

cd ..\program

del usemarcon.obj usemarcon.exe >nul

cd ..