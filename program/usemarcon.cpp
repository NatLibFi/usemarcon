#include <stdio.h>
#include <string.h>
#include "objectlist.h"

// Adapted by ATP Library Systems Ltd, Finland, 2002-2004
// Adapted by The National Library of Finland, 2004-2007

// Define USEMARCON_BATCHRUN for a run which uses files
// Define nothing (comment out #define USEMARCON_BATCHRUN) for a run which uses the api 
#define USEMARCON_BATCHRUN

int main( int argc, char** argv )
{
    CDetails myrun;

#ifdef USEMARCON_BATCHRUN // batch run

    myrun.SetInteractive(true);

    /* sign-on message */
    printf("USEMARCON - MARC record converter\n");
    printf("Standalone Batch version %s\n", USEMARCON_VERSION);
    printf("Adapted by Crossnet Systems Limited under contract to The British Library\n");
    printf("Adapted by ATP Library Systems Ltd, Finland, 2002-2004\n");
    printf("Adapted by The National Library of Finland, 2004-2007\n"); 
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
                myrun.SetForceVerbose(true);
            else if (strcmp(argv[argno], "-dc") == 0)
                myrun.SetDisableCharacterConversion(true);
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
            case 1: myrun.SetIniFileName(argv[argno]); ini_file_specified = true; break;
            case 2: myrun.SetInputMarcFileName(argv[argno]); break;
            case 3: myrun.SetOutputMarcFileName(argv[argno]); break;
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

    return myrun.Start();   // do the conversion

#else // api based run

    myrun.SetInteractive(false);
    myrun.SetIniFileName("c:\\usemarcon\\mafi2ma\\mafi2ma.ini"); // could use argv[1]

    // Load and convert 2 marc records - the first conversion will be slower than others because .ini file details are loaded
    for (int iLoop = 0; iLoop < 1; iLoop++)
    {
        // Load a marc record to convert
        FILE *fp;
        int iInLength = 0;
        char acInRecord[TBUF + 1];
        fp = fopen("input.bib", "r");
        if (fp)
        {
            iInLength = fread( acInRecord, sizeof( char ), TBUF, fp );
            fclose( fp );
        }
        printf("++++ input marc record ++++\n");
        printf(acInRecord);
        myrun.SetMarcRecord(acInRecord, iInLength);
        int res;
        res = myrun.Start();          // do the conversion
        if (res)
        {
            printf("Conversion failed. Last error: %s\n", myrun.GetLastErrorMessage());
            return res;
        }

        // Get marc record produced by the conversion
        char *pcOutRecord;
        int iOutLength = 0;
        myrun.GetMarcRecord(pcOutRecord, iOutLength);

        printf("++++ output marc record ++++\n");
        printf(pcOutRecord);

        free(pcOutRecord);

        printf("\n\n");
    }
    return 0;
#endif
}


