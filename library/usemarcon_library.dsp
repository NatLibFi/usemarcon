# Microsoft Developer Studio Project File - Name="usemarcon_library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=USEMARCON_LIBRARY - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "usemarcon_library.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "usemarcon_library.mak" CFG="USEMARCON_LIBRARY - WIN32 DEBUG"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "usemarcon_library - Win32 Release MinDependency" (based on "Win32 (x86) Static Library")
!MESSAGE "usemarcon_library - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "usemarcon_library - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "HAVE_STDLIB_H" /D "HAVE_FCNTL_H" /D "HAVE_IO_H" /D "HAVE_STRING_H" /YX /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "HAVE_STDLIB_H" /D "HAVE_FCNTL_H" /D "HAVE_IO_H" /D "HAVE_STRING_H" /YX /FD /c
# ADD BASE RSC /l 0x40b
# ADD RSC /l 0x40b
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "usemarcon_library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "HAVE_STDLIB_H" /D "HAVE_FCNTL_H" /D "HAVE_IO_H" /D "HAVE_STRING_H" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /WX /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "HAVE_STDLIB_H" /D "HAVE_FCNTL_H" /D "HAVE_IO_H" /D "HAVE_STRING_H" /FR /YX /FD /c
# ADD BASE RSC /l 0x40b
# ADD RSC /l 0x40b
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF

# Begin Target

# Name "usemarcon_library - Win32 Release MinDependency"
# Name "usemarcon_library - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\chckfile.cpp
# End Source File
# Begin Source File

SOURCE=.\codedata.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrlfld.cpp
# End Source File
# Begin Source File

SOURCE=.\error.cpp
# End Source File
# Begin Source File

SOURCE=.\lex.yy.cpp
# End Source File
# Begin Source File

SOURCE=.\lien_not.cpp
# End Source File
# Begin Source File

SOURCE=.\marcfile.cpp
# End Source File
# Begin Source File

SOURCE=.\mrcfield.cpp
# End Source File
# Begin Source File

SOURCE=.\rulefile.cpp
# End Source File
# Begin Source File

SOURCE=.\semi.cpp
# End Source File
# Begin Source File

SOURCE=.\tagnoind.cpp
# End Source File
# Begin Source File

SOURCE=.\tcd.cpp
# End Source File
# Begin Source File

SOURCE=.\tcdlib.cpp
# End Source File
# Begin Source File

SOURCE=.\tchckdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\tfile.cpp
# End Source File
# Begin Source File

SOURCE=.\tmarcdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\tmarcrec.cpp
# End Source File
# Begin Source File

SOURCE=.\tmpplctn.cpp
# End Source File
# Begin Source File

SOURCE=.\tools.cpp
# End Source File
# Begin Source File

SOURCE=.\trnsfile.cpp
# End Source File
# Begin Source File

SOURCE=.\trule.cpp
# End Source File
# Begin Source File

SOURCE=.\truledoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ttrnsdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\typedef.cpp
# End Source File
# Begin Source File

SOURCE=.\umrecord.cpp
# End Source File
# Begin Source File

SOURCE=.\ytab.cpp
# End Source File
# Begin Source File

SOURCE=.\regexp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\chckfile.h
# End Source File
# Begin Source File

SOURCE=.\codedata.h
# End Source File
# Begin Source File

SOURCE=.\ctrlfld.h
# End Source File
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\errcodes.h
# End Source File
# Begin Source File

SOURCE=.\error.h
# End Source File
# Begin Source File

SOURCE=.\lex.yy.h
# End Source File
# Begin Source File

SOURCE=.\lien_not.h
# End Source File
# Begin Source File

SOURCE=.\marcfile.h
# End Source File
# Begin Source File

SOURCE=.\mrcfield.h
# End Source File
# Begin Source File

SOURCE=.\objectlist.h
# End Source File
# Begin Source File

SOURCE=.\rulefile.h
# End Source File
# Begin Source File

SOURCE=.\semi.h
# End Source File
# Begin Source File

SOURCE=.\sort.h
# End Source File
# Begin Source File

SOURCE=.\tagnoind.h
# End Source File
# Begin Source File

SOURCE=.\tcd.h
# End Source File
# Begin Source File

SOURCE=.\tcdlib.h
# End Source File
# Begin Source File

SOURCE=.\tchckdoc.h
# End Source File
# Begin Source File

SOURCE=.\tfile.h
# End Source File
# Begin Source File

SOURCE=.\tmarcdoc.h
# End Source File
# Begin Source File

SOURCE=.\tmarcrec.h
# End Source File
# Begin Source File

SOURCE=.\tmpplctn.h
# End Source File
# Begin Source File

SOURCE=.\tools.h
# End Source File
# Begin Source File

SOURCE=.\trnsfile.h
# End Source File
# Begin Source File

SOURCE=.\trule.h
# End Source File
# Begin Source File

SOURCE=.\truledoc.h
# End Source File
# Begin Source File

SOURCE=.\ttrnsdoc.h
# End Source File
# Begin Source File

SOURCE=.\typedef.h
# End Source File
# Begin Source File

SOURCE=.\umrecord.h
# End Source File
# Begin Source File

SOURCE=.\ytab.h
# End Source File
# Begin Source File

SOURCE=.\regexp.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\analyse.l
# End Source File
# Begin Source File

SOURCE=.\analyse.y
# End Source File
# End Target
# End Project
