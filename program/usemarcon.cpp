#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "usemarconlib.h"
#include "defines.h"

// Adapted by ATP Library Systems Ltd, Finland, 2002-2004
// Adapted by The National Library of Finland, 2004-2008

// Below is an example of how to use the API
//#define USEMARCON_TEST

int main( int argc, char** argv )
{
    Usemarcon usemarcon;

#ifndef USEMARCON_TEST

    usemarcon.SetInteractive(true);

    /* sign-on message */
    printf("USEMARCON - MARC record converter\n");
    printf("Standalone Batch version %s\n", USEMARCON_VERSION);
    printf("Adapted by Crossnet Systems Limited under contract to The British Library\n");
    printf("Adapted by ATP Library Systems Ltd, Finland, 2002-2004\n");
    printf("Adapted by The National Library of Finland, 2004-2008\n"); 
    printf("Copyright 2001 - The British Library, The USEMARCON Consortium\n");
    printf("\n");

    bool ini_file_specified = false;
    int options = 0; 
    for (int argno = 1; argno < argc; argno++)
    {
        if (*argv[argno] == '-')
        {
            options++;
            if (strcmp(argv[argno], "-v") == 0)
                usemarcon.SetForceVerbose(true);
            else if (strcmp(argv[argno], "-dc") == 0)
                usemarcon.SetDisableCharacterConversion(true);
            else
            {
                printf("Error: Unknown option %s\n", argv[argno]);
                return -1;
            }
        }
        else
        {
            switch (argno-options)
            {
            case 1: usemarcon.SetIniFileName(argv[argno]); ini_file_specified = true; break;
            case 2: usemarcon.SetInputMarcFileName(argv[argno]); break;
            case 3: usemarcon.SetOutputMarcFileName(argv[argno]); break;
            }
        }
    }

    if (!ini_file_specified)
    {
        printf("Error: USAGE : usemarcon <options> xxxx.INI [input file] [output file]\n");
        printf("The INI file is mandatory.\n");
        printf("Possible options:\n");
        printf(" -v      verbose mode\n");
        printf(" -dc     disable character conversion\n\n");
        printf("Input and output files are optional. Defaults are read from the INI file.\n");
        return -1;
    }

    int rv = usemarcon.Convert();   // do the conversion
    return rv >= 0 ? rv : -rv;

#else // api based run example

    usemarcon.SetInteractive(false);
    usemarcon.SetIniFileName("c:\\usemarcon\\mafi2ma\\mafi2ma.ini"); // could use argv[1]

    // Load and convert 2 marc records - the first conversion will be slower than others because .ini file details are loaded
    for (int iLoop = 0; iLoop < 1; iLoop++)
    {
        // Load a marc record to convert
        FILE *fp;
        int iInLength = 0;
        char acInRecord[100001];
        memset(acInRecord, 0, sizeof(acInRecord));
        fp = fopen("c:\\usemarcon\\input.bib", "r");
        if (fp)
        {
            iInLength = fread( acInRecord, sizeof( char ), 100000, fp );
            fclose( fp );
        }
        printf("++++ input marc record ++++\n");
        printf(acInRecord);
        usemarcon.SetMarcRecord(acInRecord, iInLength);
        int res;
        res = usemarcon.Convert();          // do the conversion
        if (res)
        {
            printf("Conversion failed. Last error: %s\n", usemarcon.GetLastErrorMessage());
            return res;
        }

        // Get marc record produced by the conversion
        char *pcOutRecord;
        int iOutLength = 0;
        usemarcon.GetMarcRecord(pcOutRecord, iOutLength);

        printf("++++ output marc record ++++\n");
        printf(pcOutRecord);

        free(pcOutRecord);

        printf("\n\n");
    }
    return 0;
#endif
}


