/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  tmpplctn.cpp
 *
 *  implements the main operations of the software
 *
 */

#define _MAIN_
#include <stdlib.h>
#include <stdio.h>

#include "tools.h"
#include "defines.h"
#include "tmpplctn.h"
#include "truledoc.h"
#include "objectlist.h"

int debug_umrecord;
int ToHexa(const char* str);

///////////////////////////////////////////////////////////////////////////////
//
// Application class constructor
//
// Defines the initial state of the application object.
//
///////////////////////////////////////////////////////////////////////////////
TUMApplication::TUMApplication(void) /*: CApplication()*/
{
    itsRuleDoc      = NULL;
    itsTransDoc     = NULL;
    itsCheckDoc     = NULL;
    itsMarcDoc      = NULL;

    itsErrorHandler = NULL;

    itsMarcInputNumero = 0L;

    itsSize         = 0;
    itsRecordsProcessed = 0L;

    itsDebugRule    = 0;
    itsOrdinal      = 0L;
    itsUpdateOrdinal = false;
    itsUTF8Mode     = false;

    itsRecordsOk    = 0L;
    itsRecordsFailed= 0L;
}

///////////////////////////////////////////////////////////////////////////////
//
// Application class destructor
//
// Delete the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TUMApplication::~TUMApplication( void )
{
    // John Hough - added test and delete calls
    if (itsRuleDoc)
        delete itsRuleDoc;

    if (itsTransDoc)
        delete itsTransDoc;

    if (itsCheckDoc)
        delete itsCheckDoc;

    if (itsMarcDoc)
        delete itsMarcDoc;

    if (itsErrorHandler)
    {
        delete itsErrorHandler;
        itsErrorHandler = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Startup
//
// Initialize everything
//
///////////////////////////////////////////////////////////////////////////////
int TUMApplication::StartUp(CDetails *Details)
{
    itsDetails = Details;

    if (!itsDetails->GetIniFileName())
    {
        if (itsDetails->GetInteractive())
        {
            printf("ERROR: INI file not specified\n");
        }
        return -1;
    }

    strcpy(itsIniFile, itsDetails->GetIniFileName());

    if (itsErrorHandler)
        delete itsErrorHandler;

    // Create an instance of TError
    typestr inistr;
    get_ini_filename("DEFAULT_FILES","ErrorLogFile","",inistr,itsIniFile);
    itsErrorHandler  = new TError(this, inistr.str());

    // Set itsErrorHandler mode according to run mode
    if (itsDetails->GetInteractive())
        itsErrorHandler->SetMode(INTERACTIVE);
    else
        itsErrorHandler->SetMode(NONINTERACTIVE);


    if (!*itsIniFile)
    {
        itsErrorHandler->WriteError("END non OK : no .ini file specified\n");
        return -1;
    }

    if (itsDetails->GetInteractive())
        printf("Loading .ini file parameters...\n");

    debug_umrecord=0;
    itsDebugRule=0;

    get_ini_string("DEFAULT_STATES","IsVerboseExecutionModeChecked","",inistr,itsIniFile);
    if (!strcasecmp(inistr.str(),"true") || itsDetails->GetForceVerbose()) {
        itsErrorHandler->SetVerboseMode(1);
    } else {
        itsErrorHandler->SetVerboseMode(0);
    }

    // Set the maximum number of errors to be encoutered before stop
    get_ini_string("DEFAULT_VALUES","MaxErrorsToBeEncoutered","",inistr,itsIniFile);
    if (!*inistr.str())
        get_ini_string("DEFAULT_VALUES","MaxErrorsToBeEncountered","",inistr,itsIniFile);
    if (*inistr.str()) 
    {
        itsErrorHandler->SetTooManyErrors(atoi(inistr.str()));
    }

    get_ini_string("DEBUG","IsDebugExecutionModeChecked","",inistr,itsIniFile);
    if (!strcasecmp(inistr.str(),"true")) {
        itsDebugRule = 1;
    } else {
        itsDebugRule = 0;
    }

    // Initialize itsRuleDoc
    if (itsRuleDoc)
        delete itsRuleDoc;
    itsRuleDoc = new TRuleDoc(this);
    if (!itsRuleDoc)
    {
        itsErrorHandler->WriteError("END non OK : No rule structure created\n");
        return -1;
    }

    get_ini_filename("DEFAULT_FILES", "RuleFile", "", inistr, itsIniFile);
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No RuleFile specified\n");
    }
    else if (itsRuleDoc->OpenRuleFile(inistr.str()) == false)
    {
        itsErrorHandler->WriteError("Unable to open rule file\n");
    }

    // Initialize itsTransDoc
    if (itsTransDoc)
        delete itsTransDoc;
    itsTransDoc = new TTransDoc(itsErrorHandler);
    if (!itsTransDoc)
    {
        itsErrorHandler->WriteError("END non OK : No transcoding structure created\n");
        return -1;
    }
    get_ini_filename("DEFAULT_FILES", "TranscodingCharacterTable", "", inistr, itsIniFile);
    if (*inistr.str() && itsTransDoc->OpenTransFile(inistr.str()) == false)
    {
        itsErrorHandler->WriteError("Unable to open transcoding file\n");
        return -1;
    }

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputFileCharacterSet", "", inistr, itsIniFile);
    if (itsTransDoc->SetInputFileCharacterSet(inistr.str()) == false)
    {
        itsErrorHandler->WriteError("Specified input file character set not supported\n");
        return -1;
    }

    // Initialize itsCheckDoc
    if (itsCheckDoc)
        delete itsCheckDoc;
    itsCheckDoc = new TCheckDoc(itsErrorHandler);

    if (!itsCheckDoc)
    {
        itsErrorHandler->WriteError("END non OK : No transcoding structure created\n");
        return -1;
    }

    get_ini_filename("DEFAULT_FILES", "InputFormatCheckingTable", "", inistr, itsIniFile);
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No InputFormatCheckingTable specified\n");
    }
    else if (itsCheckDoc->OpenCheckFile(INPUT, inistr.str()) == false)
    {
        itsErrorHandler->WriteError("Unable to open input format checking file\n");
    }

    get_ini_filename("DEFAULT_FILES", "OutputFormatCheckingTable", "", inistr, itsIniFile);
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No OutputFormatCheckingTable specified\n");
    }
    else if (itsCheckDoc->OpenCheckFile(OUTPUT,inistr.str()) == false)
    {
        itsErrorHandler->WriteError("Unable to open output format checking file\n");
    }

    // Initialize itsMarcDoc
    if (itsMarcDoc)
        delete itsMarcDoc;
    itsMarcDoc = new TMarcDoc(this);
    if (!itsMarcDoc)
    {
        itsErrorHandler->WriteError("END non OK : No marc structure created\n");
        return -1;
    }

    get_ini_filename("DEFAULT_FILES", "InputMarcEditConfigurationFile", "", inistr, itsIniFile);
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No Input MARC edit configuration file specified\n");
    }

    FILE_SPEC FileSpec;
    memset(&FileSpec, 0, sizeof(FILE_SPEC));
    strcpy(FileSpec.name, inistr.str());
    itsMarcDoc->SetConfInputFileSpec(&FileSpec);

    memset(&FileSpec, 0, sizeof(FILE_SPEC));
    if (!itsDetails->GetMarcRecordAvailable())
    {
        if (itsDetails->GetInputMarcFileName())
        {
            strcpy(FileSpec.name, itsDetails->GetInputMarcFileName());
        }
        else
        {
            get_ini_filename("DEFAULT_FILES", "MarcInputFile", "", inistr, itsIniFile);
            if (!*inistr.str())
            {
                itsErrorHandler->WriteError("END non OK : Input MARC file not specified\n");
                return -1;
            }
            strcpy(FileSpec.name, inistr.str());
        }
    }
    itsMarcDoc->SetMarcInputFileSpec(&FileSpec);

    itsMarcDoc->SetMarcInputFileFormat(MFF_NONSEGMENTED); // nonsegmented by default
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "IsInputBlockSegmentedChecked", "", inistr, itsIniFile);
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsMarcDoc->SetMarcInputFileFormat(MFF_SEGMENTED);
    }

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputFileFormat", "", inistr, itsIniFile);
    if (*inistr.str())
    {
        if (!strcasecmp(inistr.str(),"iso2709"))
        {
            // default, see above
        }
        else if (!strcasecmp(inistr.str(),"marcxml"))
        {
            itsMarcDoc->SetMarcInputFileFormat(MFF_MARCXML);
        }
        else if (!strcasecmp(inistr.str(),"marcxchange"))
        {
            itsMarcDoc->SetMarcInputFileFormat(MFF_MARCXCHANGE);
        }
        else
        {
            itsErrorHandler->WriteError("END non OK : Unknown input file format");
            return -1;
        }
    }

    itsMarcDoc->SetMarcInputFileBlockSize(2048);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputMarcSizeBlock", "", inistr, itsIniFile);
    if (*inistr.str() && strcmp(inistr.str(), "2048"))
    {
        itsMarcDoc->SetMarcInputFileBlockSize((short)atoi(inistr.str()));
    }

    itsMarcDoc->SetMarcInputFileMinDataFree(5);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputMarcMinDataFree", "", inistr, itsIniFile);
    if (*inistr.str() && strcmp(inistr.str(), "5"))
    {
        itsMarcDoc->SetMarcInputFileMinDataFree((short)atoi(inistr.str()));
    }

    itsMarcDoc->SetMarcInputFilePaddingChar(0x5E);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputMarcPaddingChar", "", inistr, itsIniFile);
    if (*inistr.str() && strcasecmp(inistr.str(), "5E"))
    {
        itsMarcDoc->SetMarcInputFilePaddingChar((char)ToHexa(inistr.str()));
    }

    itsMarcDoc->SetMarcInputFileLastBlock(false);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "IsInputLastBlockPaddedChecked", "", inistr, itsIniFile);
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsMarcDoc->SetMarcInputFileLastBlock(true);
    }

    itsUTF8Mode = false;
    get_ini_string("DEFAULT_STATES", "UTF8Mode", "", inistr, itsIniFile);
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsUTF8Mode = true;
    }
    itsErrorHandler->SetUTF8Mode(itsUTF8Mode);

    itsConvertInFieldOrder = false;
    get_ini_string("DEFAULT_STATES", "ConvertInFieldOrder", "", inistr, itsIniFile);
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsConvertInFieldOrder = true;
    }

    bool convertSubfieldCodes = false;
    get_ini_string("DEFAULT_STATES", "ConvertSubfieldCodesToLowercase", "", inistr, itsIniFile);
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        convertSubfieldCodes = true;
    }
    itsErrorHandler->SetConvertSubfieldCodesToLowercase(convertSubfieldCodes);

    memset(&FileSpec,0,sizeof(FILE_SPEC));
    if (!itsDetails->GetMarcRecordAvailable())
    {
        if (itsDetails->GetOutputMarcFileName())
        {
            strcpy(FileSpec.name, itsDetails->GetOutputMarcFileName());
        }
        else
        {
            get_ini_filename("DEFAULT_FILES", "MarcOutputFile", "", inistr, itsIniFile);
            if (!*inistr.str())
            {
                itsErrorHandler->WriteError("END non OK : Output MARC file not specified\n");
                return -1;
            }
            strcpy(FileSpec.name, inistr.str());
        }
    }
    itsMarcDoc->SetMarcOutputFileSpec(&FileSpec);

    get_ini_filename("DEFAULT_FILES", "OutputMarcEditConfigurationFile", "", inistr, itsIniFile);
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No Output MARC edit configuration file specified\n");
    }

    memset(&FileSpec, 0, sizeof(FILE_SPEC));
    strcpy(FileSpec.name, inistr.str());
    itsMarcDoc->SetConfOutputFileSpec(&FileSpec);

    itsMarcDoc->SetMarcOutputFileFormat(MFF_NONSEGMENTED); // nonsegmented by default
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "IsOutputBlockSegmentedChecked", "", inistr, itsIniFile);
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsMarcDoc->SetMarcOutputFileFormat(MFF_SEGMENTED);
    }

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputFileFormat", "", inistr, itsIniFile);
    if (*inistr.str())
    {
        if (!strcasecmp(inistr.str(), "iso2709"))
        {
            // default, see above
        }
        else if (!strcasecmp(inistr.str(), "marcxml"))
        {
            itsMarcDoc->SetMarcOutputFileFormat(MFF_MARCXML);
        }
        else if (!strcasecmp(inistr.str(), "marcxchange"))
        {
            itsMarcDoc->SetMarcOutputFileFormat(MFF_MARCXCHANGE);
        }
        else
        {
            itsErrorHandler->WriteError("END non OK : Unknown output file format");
            return -1;
        }
    }

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputXMLRecordFormat", "", inistr, itsIniFile);
    if (*inistr.str())
        itsErrorHandler->SetOutputXMLRecordFormat(inistr.str());
    else
        itsErrorHandler->SetOutputXMLRecordFormat(NULL);

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputXMLRecordType", "", inistr, itsIniFile);
    if (*inistr.str())
        itsErrorHandler->SetOutputXMLRecordType(inistr.str());
    else
        itsErrorHandler->SetOutputXMLRecordType(NULL);

    itsMarcDoc->SetMarcOutputFileBlockSize(2048);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputMarcSizeBlock", "", inistr, itsIniFile);
    if (*inistr.str() && strcmp(inistr.str(),"2048"))
    {
        itsMarcDoc->SetMarcOutputFileBlockSize((short)atoi(inistr.str()));
    }

    itsMarcDoc->SetMarcOutputFileMinDataFree(5);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputMarcMinDataFree", "", inistr, itsIniFile);
    if (*inistr.str() && strcmp(inistr.str(), "5"))
    {
        itsMarcDoc->SetMarcOutputFileMinDataFree((short)atoi(inistr.str()));
    }

    itsMarcDoc->SetMarcOutputFilePaddingChar(0x5E);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputMarcPaddingChar", "", inistr, itsIniFile);
    if (*inistr.str() && strcasecmp(inistr.str(), "5E"))
    {
        itsMarcDoc->SetMarcOutputFilePaddingChar((char)ToHexa(inistr.str()));
    }

    itsMarcDoc->SetMarcOutputFileLastBlock(false);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "IsOutputLastBlockPaddedChecked", "", inistr, itsIniFile);
    if (*inistr.str() && !strcasecmp(inistr.str(),"true"))
    {
        itsMarcDoc->SetMarcOutputFileLastBlock(true);
    }

    get_ini_string("DEFAULT_VALUES", "OrdinalNumber", "1", inistr, itsIniFile);
    itsOrdinal = atol(inistr.str());

    itsUpdateOrdinal = false;
    get_ini_string("DEFAULT_VALUES", "UpdateOrdinalNumber", "", inistr, itsIniFile);
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsUpdateOrdinal = true;
    }

    return 0;
}

int TUMApplication::Convert(void)
{
    bool ConvOk=true;

    itsMarcDoc->Reset();

    if (!itsMarcDoc->Open(INPUT))
    {
        itsErrorHandler->WriteError("END non OK : Unable to open input MARC file\n");
        return -1;
    }

    if (!itsMarcDoc->Open(OUTPUT))
    {
        itsErrorHandler->WriteError("END non OK : Unable to open output MARC file\n");
        return -1;
    }

    itsRecordsOk = 0L;
    itsRecordsFailed = 0L;
    itsRecordsProcessed = 0L;

    itsSize = (double)itsMarcDoc->GetInputFile()->GetSize();

    if (itsDetails->GetInteractive())
    {
        printf("Size of input file: %ld bytes\n", (long)itsSize);
        printf("Conversion progress:\n");
    }

    while (true)
    {
        if (itsMarcDoc->Read())
            break;

        ConvOk=true;
        if (itsMarcDoc->Transcode(itsTransDoc)) ConvOk=false;
        if (itsMarcDoc->Convert(itsRuleDoc)) ConvOk=false;
        itsOrdinal++;

        // Mise a jour des compteurs
        if (ConvOk)
        {
            ++itsRecordsOk;
        }
        else
        {
            ++itsRecordsFailed;
        }

        if (itsDetails->GetInteractive())
        {
            int readcentile = (int)(itsMarcDoc->GetInputFile()->GetPos() / itsSize * 100);
            printf("%d%%   %d input records\r",(int)readcentile, itsRecordsOk + itsRecordsFailed);
        }

        // Controle du fichier de sortie si un fichier de controle a ete charge
        if (itsCheckDoc->GetOutputFile())
        {
            itsCheckDoc->GetOutputFile()->Verify(OUTPUT,itsMarcDoc->GetOutputRecord());
        }

        DoConvertWrite();

        if ((itsErrorHandler->GetTooManyErrors()) && (itsErrorHandler->GetHowManyErrors()>itsErrorHandler->GetTooManyErrors()))
        {
            // Maximum number of errors exceeded
            itsErrorHandler->WriteError("END non OK : Maximum errors to be encountered has been reached\n");

            if (itsUpdateOrdinal)
            {
                char tmp[30];
                sprintf(tmp, "%ld", itsOrdinal);
                put_ini_string("DEFAULT_VALUES", "OrdinalNumber", tmp, itsIniFile);
            }

            return -1;
        }
        itsRecordsProcessed++;
        // We're done if converting a buffer (the data has been replaced)
        if (itsDetails->GetMarcRecordAvailable())
            break;
    }

    if (itsUpdateOrdinal)
    {
        char tmp[30];
        sprintf(tmp, "%ld", itsOrdinal);
        put_ini_string("DEFAULT_VALUES", "OrdinalNumber", tmp, itsIniFile);
    }

    if (itsDetails->GetInteractive())
    {
        printf("100%\n");
        char mess[1000];
        sprintf(mess,"END OK %ld Converted Records : %ld without error and %ld with error(s)\n",
            itsRecordsProcessed, itsRecordsOk, itsRecordsFailed);
        itsErrorHandler->WriteError(mess);
    }

    return 0;
}

void TUMApplication::End(void)
{
    itsMarcDoc->Close(OUTPUT);
    itsMarcDoc->Close(INPUT);

    // John Hough - replaced exit with return
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DoConvertWrite
//
///////////////////////////////////////////////////////////////////////////////
int TUMApplication::DoConvertWrite(void)
{
    // On ecrit la notice marc itsMarcDoc->itsOutputRecord dans le fichier de sortie
    itsMarcDoc->Write();
    return 0;
}

int ToHexa(const char* str)
{
    int     v=0,
            i,
            max=strlen(str)-1,
            d;
    char    *StrBis=strdup(str);

    for (i=max,d=1;i>=0;--i)
    {
        StrBis[i]=(char)toupper((int)StrBis[i]);
        if (StrBis[i]>='A')
            v+=d*(StrBis[i]-'A'+10);
        else
            v+=d*(StrBis[i]-'0');
        d*=16;
    }
    free (StrBis);
    return v;
}


TError *TUMApplication::GetErrorHandler(void)
{
    return itsErrorHandler;
}

CDetails *TUMApplication::GetDetails(void)
{
    return itsDetails;
}
