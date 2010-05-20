// win32dll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

BOOL __stdcall ConvertFile(const char *inifile, const char *sourcefile, const char *targetfile)//LPCTSTR szIniFile, LPCTSTR szSourceFile, LPCTSTR szTargetFile)
{
    Usemarcon usemarcon;
    usemarcon.SetIniFileName(inifile);
    usemarcon.SetInputMarcFileName(sourcefile);
    usemarcon.SetOutputMarcFileName(targetfile);
    return usemarcon.Convert();
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

