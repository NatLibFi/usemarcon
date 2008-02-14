/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2008
 *
 *  File:  tmpplctn.cpp
 *
 *  implements the main operations of the software
 *
 */

#define _MAIN_
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "tools.h"
#include "defines.h"
#include "tmpplctn.h"
#include "truledoc.h"

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

    itsDebugRule    = false;
    itsOrdinal      = 0L;
    itsUpdateOrdinal = false;
    itsUTF8Mode     = false;

    itsRecordsOk    = 0L;
    itsRecordsFailed= 0L;

    m_record = NULL;
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

    free(m_record);
}

///////////////////////////////////////////////////////////////////////////////
//
// Startup
//
// Initialize everything
//
///////////////////////////////////////////////////////////////////////////////
int TUMApplication::StartUp(const char *a_iniFileName, bool a_interactive, 
                            const char *a_record, int a_recordLen, bool a_forceVerbose,
                            const char *a_inputMarcFileName, const char *a_outputMarcFileName,
                            bool a_disableCharacterConversion)
{
    m_interactive = a_interactive;

    if (!a_iniFileName || !*a_iniFileName)
    {
        if (a_interactive)
            printf("ERROR: INI file not specified\n");
        return -1;
    }

    itsIniFile = a_iniFileName;

    free(m_record);    
    if (a_record && a_recordLen)
    {
        m_recordLen = a_recordLen;
        m_record = reinterpret_cast<char *>(malloc(m_recordLen + 1));
        memset(m_record, 0, m_recordLen + 1);
        memcpy(m_record, a_record, m_recordLen);
    }
    else
    {
        m_recordLen = 0;
        m_record = NULL;
    }

    m_forceVerbose = a_forceVerbose;
    m_inputMarcFileName = a_inputMarcFileName;
    m_outputMarcFileName = a_outputMarcFileName;
    m_disableCharacterConversion = a_disableCharacterConversion;

    if (itsErrorHandler)
        delete itsErrorHandler;

    // Create an instance of TError
    typestr inistr;
    get_ini_filename("DEFAULT_FILES", "ErrorLogFile", "", inistr, itsIniFile.str());
    itsErrorHandler  = new TError(this, inistr.str());

    // Set itsErrorHandler mode according to run mode
    if (m_interactive)
        itsErrorHandler->SetMode(INTERACTIVE);
    else
        itsErrorHandler->SetMode(NONINTERACTIVE);

    if (!file_available(itsIniFile.str()))
    {
        itsErrorHandler->WriteError("Unable to open .ini file\n");
        return -1;
    }

    if (m_interactive)
        printf("Loading .ini file parameters...\n");

    itsDebugRule = false;

    get_ini_string("DEFAULT_STATES","IsVerboseExecutionModeChecked","",inistr,itsIniFile.str());
    if (!strcasecmp(inistr.str(),"true") || m_forceVerbose) 
    {
        itsErrorHandler->SetVerboseMode(1);
    } 
    else 
    {
        itsErrorHandler->SetVerboseMode(0);
    }

    // Set the maximum number of errors to be encountered before stop
    get_ini_string("DEFAULT_VALUES","MaxErrorsToBeEncountered","",inistr,itsIniFile.str());
    if (!*inistr.str())
        get_ini_string("DEFAULT_VALUES","MaxErrorsToBeEncoutered","",inistr,itsIniFile.str());
    if (*inistr.str()) 
    {
        itsErrorHandler->SetTooManyErrors(atoi(inistr.str()));
    }

    get_ini_string("DEBUG","IsDebugExecutionModeChecked","",inistr,itsIniFile.str());
    if (!strcasecmp(inistr.str(),"true")) {
        itsDebugRule = true;
    } else {
        itsDebugRule = false;
    }

    // Initialize itsRuleDoc
    if (itsRuleDoc)
        delete itsRuleDoc;
    itsRuleDoc = new TRuleDoc(this);
    if (!itsRuleDoc)
    {
        itsErrorHandler->WriteError("Failed to create rule structure\n");
        return -1;
    }

    get_ini_filename("DEFAULT_FILES", "RuleFile", "", inistr, itsIniFile.str());
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No RuleFile specified\n");
    }
    else if (itsRuleDoc->OpenRuleFile(inistr.str()) == false)
    {
        itsErrorHandler->WriteError("Unable to open rule file\n");
        return -1;
    }

    // Initialize itsTransDoc
    if (itsTransDoc)
        delete itsTransDoc;
    itsTransDoc = new TTransDoc(itsErrorHandler);
    if (!itsTransDoc)
    {
        itsErrorHandler->WriteError("Failed to create transcoding structure\n");
        return -1;
    }
    get_ini_filename("DEFAULT_FILES", "TranscodingCharacterTable", "", inistr, itsIniFile.str());
    if (*inistr.str() && itsTransDoc->OpenTransFile(inistr.str()) == false)
    {
        itsErrorHandler->WriteError("Unable to open transcoding file\n");
        return -1;
    }

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputFileCharacterSet", "", inistr, itsIniFile.str());
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
        itsErrorHandler->WriteError("Failed to create transcoding structure\n");
        return -1;
    }

    get_ini_filename("DEFAULT_FILES", "InputFormatCheckingTable", "", inistr, itsIniFile.str());
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No InputFormatCheckingTable specified\n");
    }
    else if (itsCheckDoc->OpenCheckFile(INPUT, inistr.str()) == false)
    {
        itsErrorHandler->WriteError("Unable to open input format checking file\n");
    }

    get_ini_filename("DEFAULT_FILES", "OutputFormatCheckingTable", "", inistr, itsIniFile.str());
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
        itsErrorHandler->WriteError("Failed to create marc structure\n");
        return -1;
    }

    get_ini_filename("DEFAULT_FILES", "InputMarcEditConfigurationFile", "", inistr, itsIniFile.str());
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No Input MARC edit configuration file specified\n");
    }
    itsMarcDoc->SetConfInputFileSpec(inistr);

    typestr filespec = "";
    if (!m_record)
    {
        if (!m_inputMarcFileName.is_empty())
        {
            filespec = m_inputMarcFileName;
        }
        else
        {
            get_ini_filename("DEFAULT_FILES", "MarcInputFile", "", inistr, itsIniFile.str());
            if (!*inistr.str())
            {
                itsErrorHandler->WriteError("Input MARC file not specified\n");
                return -1;
            }
            filespec = inistr;
        }
    }
    itsMarcDoc->SetMarcInputFileSpec(filespec);

    itsMarcDoc->SetMarcInputFileFormat(MFF_NONSEGMENTED); // nonsegmented by default
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "IsInputBlockSegmentedChecked", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsMarcDoc->SetMarcInputFileFormat(MFF_SEGMENTED);
    }

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputFileFormat", "", inistr, itsIniFile.str());
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
            itsErrorHandler->WriteError("Unknown input file format");
            return -1;
        }
    }

    itsMarcDoc->SetMarcInputFileBlockSize(2048);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputMarcSizeBlock", "", inistr, itsIniFile.str());
    if (*inistr.str() && strcmp(inistr.str(), "2048"))
    {
        itsMarcDoc->SetMarcInputFileBlockSize((short)atoi(inistr.str()));
    }

    itsMarcDoc->SetMarcInputFileMinDataFree(5);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputMarcMinDataFree", "", inistr, itsIniFile.str());
    if (*inistr.str() && strcmp(inistr.str(), "5"))
    {
        itsMarcDoc->SetMarcInputFileMinDataFree((short)atoi(inistr.str()));
    }

    itsMarcDoc->SetMarcInputFilePaddingChar(0x5E);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "InputMarcPaddingChar", "", inistr, itsIniFile.str());
    if (*inistr.str() && strcasecmp(inistr.str(), "5E"))
    {
        itsMarcDoc->SetMarcInputFilePaddingChar((char)ToHexa(inistr.str()));
    }

    itsMarcDoc->SetMarcInputFileLastBlock(false);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "IsInputLastBlockPaddedChecked", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsMarcDoc->SetMarcInputFileLastBlock(true);
    }

    itsUTF8Mode = false;
    get_ini_string("DEFAULT_STATES", "UTF8Mode", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsUTF8Mode = true;
    }
    itsErrorHandler->SetUTF8Mode(itsUTF8Mode);

    itsConvertInFieldOrder = false;
    get_ini_string("DEFAULT_STATES", "ConvertInFieldOrder", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsConvertInFieldOrder = true;
    }

    bool convertSubfieldCodes = false;
    get_ini_string("DEFAULT_STATES", "ConvertSubfieldCodesToLowercase", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        convertSubfieldCodes = true;
    }
    itsErrorHandler->SetConvertSubfieldCodesToLowercase(convertSubfieldCodes);

    filespec = "";
    if (!m_record)
    {
        if (!m_outputMarcFileName.is_empty())
        {
            filespec = m_outputMarcFileName;
        }
        else
        {
            get_ini_filename("DEFAULT_FILES", "MarcOutputFile", "", inistr, itsIniFile.str());
            if (!*inistr.str())
            {
                itsErrorHandler->WriteError("Output MARC file not specified\n");
                return -1;
            }
            filespec = inistr;
        }
    }
    itsMarcDoc->SetMarcOutputFileSpec(filespec);

    get_ini_filename("DEFAULT_FILES", "OutputMarcEditConfigurationFile", "", inistr, itsIniFile.str());
    if (!*inistr.str())
    {
        itsErrorHandler->WriteError("No Output MARC edit configuration file specified\n");
    }
    itsMarcDoc->SetConfOutputFileSpec(inistr);

    itsMarcDoc->SetMarcOutputFileFormat(MFF_NONSEGMENTED); // nonsegmented by default
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "IsOutputBlockSegmentedChecked", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsMarcDoc->SetMarcOutputFileFormat(MFF_SEGMENTED);
    }

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputFileFormat", "", inistr, itsIniFile.str());
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
            itsErrorHandler->WriteError("Unknown output file format");
            return -1;
        }
    }

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputXMLRecordFormat", "", inistr, itsIniFile.str());
    if (*inistr.str())
        itsErrorHandler->SetOutputXMLRecordFormat(inistr.str());
    else
        itsErrorHandler->SetOutputXMLRecordFormat(NULL);

    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputXMLRecordType", "", inistr, itsIniFile.str());
    if (*inistr.str())
        itsErrorHandler->SetOutputXMLRecordType(inistr.str());
    else
        itsErrorHandler->SetOutputXMLRecordType(NULL);

    itsMarcDoc->SetMarcOutputFileBlockSize(2048);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputMarcSizeBlock", "", inistr, itsIniFile.str());
    if (*inistr.str() && strcmp(inistr.str(),"2048"))
    {
        itsMarcDoc->SetMarcOutputFileBlockSize((short)atoi(inistr.str()));
    }

    itsMarcDoc->SetMarcOutputFileMinDataFree(5);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputMarcMinDataFree", "", inistr, itsIniFile.str());
    if (*inistr.str() && strcmp(inistr.str(), "5"))
    {
        itsMarcDoc->SetMarcOutputFileMinDataFree((short)atoi(inistr.str()));
    }

    itsMarcDoc->SetMarcOutputFilePaddingChar(0x5E);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "OutputMarcPaddingChar", "", inistr, itsIniFile.str());
    if (*inistr.str() && strcasecmp(inistr.str(), "5E"))
    {
        itsMarcDoc->SetMarcOutputFilePaddingChar((char)ToHexa(inistr.str()));
    }

    itsMarcDoc->SetMarcOutputFileLastBlock(false);
    get_ini_string("DEFAULT_MARC_ATTRIBUTES", "IsOutputLastBlockPaddedChecked", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(),"true"))
    {
        itsMarcDoc->SetMarcOutputFileLastBlock(true);
    }

    get_ini_string("DEFAULT_VALUES", "OrdinalNumber", "1", inistr, itsIniFile.str());
    itsOrdinal = atol(inistr.str());

    itsUpdateOrdinal = false;
    get_ini_string("DEFAULT_VALUES", "UpdateOrdinalNumber", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsUpdateOrdinal = true;
    }

    itsMarcDoc->SetDuplicateSubfields(DP_LEAVE);
    get_ini_string("DEFAULT_VALUES", "DuplicateSubfields", "", inistr, itsIniFile.str());
    if (!inistr.is_empty())
    {
        if (!strcasecmp(inistr.str(), "leave"))
        {
            // default, see above
        }
        else if (!strcasecmp(inistr.str(), "delete"))
        {
            itsMarcDoc->SetDuplicateSubfields(DP_DELETE);
        }
        else if (!strcasecmp(inistr.str(), "delete_ignore_case"))
        {
            itsMarcDoc->SetDuplicateSubfields(DP_DELETE_IGNORE_CASE);
        }
        else if (!strcasecmp(inistr.str(), "delete_smart"))
        {
            itsMarcDoc->SetDuplicateSubfields(DP_DELETE_SMART);
        }
        else
        {
            itsErrorHandler->WriteError("Unknown duplicate subfield processing mode");
            return -1;
        }
    }

    itsMarcDoc->SetDuplicateFields(DP_LEAVE);
    get_ini_string("DEFAULT_VALUES", "DuplicateFields", "", inistr, itsIniFile.str());
    if (!inistr.is_empty())
    {
        if (!strcasecmp(inistr.str(), "leave"))
        {
            // default, see above
        }
        else if (!strcasecmp(inistr.str(), "delete"))
        {
            itsMarcDoc->SetDuplicateFields(DP_DELETE);
        }
        else if (!strcasecmp(inistr.str(), "delete_ignore_case"))
        {
            itsMarcDoc->SetDuplicateFields(DP_DELETE_IGNORE_CASE);
        }
        else if (!strcasecmp(inistr.str(), "delete_smart"))
        {
            itsMarcDoc->SetDuplicateFields(DP_DELETE_SMART);
        }
        else
        {
            itsErrorHandler->WriteError("Unknown duplicate field processing mode");
            return -1;
        }
    }

    get_ini_string("DEFAULT_STATES", "HandleLinkedFields", "", inistr, itsIniFile.str());
    if (*inistr.str() && !strcasecmp(inistr.str(), "true"))
    {
        itsErrorHandler->SetHandleLinkedFields(true);
    }
    else
    {
        itsErrorHandler->SetHandleLinkedFields(false);
    }

    return 0;
}

int TUMApplication::Convert(void)
{
    bool ConvOk=true;

    itsMarcDoc->Reset();

    if (!itsMarcDoc->Open(INPUT))
    {
        itsErrorHandler->WriteError("Unable to open input MARC file\n");
        return -1;
    }

    if (!itsMarcDoc->Open(OUTPUT))
    {
        itsErrorHandler->WriteError("Unable to open output MARC file\n");
        return -1;
    }

    itsRecordsOk = 0L;
    itsRecordsFailed = 0L;
    itsRecordsProcessed = 0L;

    itsSize = (double)itsMarcDoc->GetInputFile()->GetSize();

    if (m_interactive)
    {
        printf("Size of input file: %ld bytes\n", (long)itsSize);
        printf("Conversion progress:\n");
    }

    while (true)
    {
        if (itsMarcDoc->Read())
            break;

        ConvOk=true;
        itsErrorHandler->SetRecordNumber(itsRecordsProcessed + 1);
        if (itsMarcDoc->Transcode(itsTransDoc)) ConvOk = false;
        if (itsMarcDoc->Convert(itsRuleDoc)) ConvOk = false;
        if (itsMarcDoc->ProcessDuplicateFields()) ConvOk = false;
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

        if (m_interactive)
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
            itsErrorHandler->WriteError("Maximum errors to be encountered has been reached\n");

            if (itsUpdateOrdinal)
            {
                char tmp[30];
                sprintf(tmp, "%ld", itsOrdinal);
                put_ini_string("DEFAULT_VALUES", "OrdinalNumber", tmp, itsIniFile.str());
            }

            return -1;
        }
        itsRecordsProcessed++;
        // We're done if converting a buffer (the data has been replaced)
        if (m_record)
            break;
    }

    if (itsUpdateOrdinal)
    {
        char tmp[30];
        sprintf(tmp, "%ld", itsOrdinal);
        put_ini_string("DEFAULT_VALUES", "OrdinalNumber", tmp, itsIniFile.str());
    }

    if (m_interactive)
    {
        printf("100%\n");
        char msg[1000];
        sprintf(msg, "%ld Converted Records: %ld without error and %ld with error(s)\n",
            itsRecordsProcessed, itsRecordsOk, itsRecordsFailed);
        itsErrorHandler->WriteError(msg);
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

int TUMApplication::ToHexa(const char* str)
{
    int     v=0,
            i,
            max=strlen(str)-1,
            d;
    char    *StrBis=l_strdup(str);

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

void TUMApplication::SetMarcRecord(const char *a_record, int a_length)
{
    free(m_record);

    m_recordLen = a_length;
    m_record = reinterpret_cast<char *>(malloc(m_recordLen + 1));
    memcpy(m_record, a_record, m_recordLen);
    m_record[m_recordLen] = '\0';
}

void TUMApplication::GetMarcRecord(char *&a_record, int &a_length)
{
    a_record = m_record ? l_strdup(m_record) : NULL;
    a_length = m_recordLen;
}
