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

int __stdcall ConvertFile(const char *inifile, const char *sourcefile, const char *targetfile, char *lasterror, int lasterrorsize)
{
    Usemarcon usemarcon;
    usemarcon.SetIniFileName(inifile);
    usemarcon.SetInputMarcFileName(sourcefile);
    usemarcon.SetOutputMarcFileName(targetfile);
    int result = usemarcon.Convert();
    if (lasterror)
    {
        *lasterror = '\0';
        if (result)
        {
#pragma warning(disable: 4996)
            strncpy(lasterror, usemarcon.GetLastErrorMessage(), lasterrorsize);
            lasterror[lasterrorsize - 1] = '\0';
        }
    }
    return result;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

