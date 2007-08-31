/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  error.cpp
 *
 *  implements a class to manage errors
 *
 */

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "tools.h"
#include "errcodes.h"

///////////////////////////////////////////////////////////////////////////////
//
// TError
//
////////////////////////////////////////////////////////////////////////////////

TError::TError(TUMApplication* theApplication, const char *LogFileName, const char *ErrorCodeFileName)
{
    itsApplication=theApplication;

    itsTooManyErrors       = 0;
    itsHowManyErrors       = 0;
    itsErrorCode           = 0;
    itsDebugMode           = 0;
    itsLogError            = NULL;
    itsUTF8Mode            = false;
    itsConvertSubfieldCodesToLowercase = false;

    // Logfile opening
    if (*LogFileName)
        OpenErrorLogFile(LogFileName);

    itsLastErrorMessage[0] = '\0';
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TError
//
////////////////////////////////////////////////////////////////////////////////

TError::~TError()
{
    if (itsLogError)
    {
        fclose(itsLogError);
        itsLogError = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// SetErrorCode
//
////////////////////////////////////////////////////////////////////////////////

int TError::SetErrorCode(int ErrorCode, short Severity, const char *FileName,
                         int LineNumber, const char *UserData)
{
    bool bFoundCode = false;
    for (short iIndice = 0; iIndice < ErrorDescCount; iIndice++)
    {
        if (ErrorDesc[iIndice].errorcode == ErrorCode)
        {
            bFoundCode = true;
            break;
        }
    }

    typestr message;
    const char *category = NULL;
    switch(Severity)
    {
        case FORCER:
        case DISPLAY:
            message.allocstr(50);
            sprintf(message.str(), "WARNING (%d) - ", ErrorCode);
            message += bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error";
            message += "\n";
            message += UserData;
            show_warning(message.str());
            return 0-ErrorCode;
        case WARNING: category = "WARNING"; break;
        case ERROR: category = "ERROR"; break;
        case NOTICE: category = "NOTICE"; break;
        case NONERROR: category = "NO ERROR"; break;
        default: category = "FATAL"; break;
    }

    // Error Code setting
    itsErrorCode = ErrorCode;
    itsHowManyErrors++;

    if (itsLogError)
    {
        fprintf(itsLogError, "%s (%d) - %s%s%s\n", category, ErrorCode,
            bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error", *UserData ? " : " : "",
            UserData);
        if (ErrorCode >= 9000 || itsDebugMode)
            fprintf(itsLogError, " in %s:%d\n", FileName, LineNumber);
        else 
            fprintf(itsLogError, "\n");
        fflush(itsLogError);
    }

    switch(Severity)
    {
    case ERROR:
    case WARNING:
    case NOTICE:
        if (itsMode != INTERACTIVE || !itsVerboseMode)
            return 0-ErrorCode;
        break;
    case NONERROR:
        // Don't show non-errors
        return 0-ErrorCode;
    default:
        if (itsMode != INTERACTIVE)
            return 0-ErrorCode;
        break;
    }
    
    // Write message to screen
    message.allocstr(100);
    sprintf(message.str(), "%s (%d) - ", category, ErrorCode);
    message += bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error";
    message += "\n";
    message += UserData;
    if (ErrorCode >= 9000 || itsDebugMode)
    {
        message += " in ";
        message += FileName;
        message += ":";
        char tmp[30];
        sprintf(tmp, "%d", LineNumber);
        message += tmp;
    }
    show_message(message.str());

    return 0-ErrorCode;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetErrorCode
//
////////////////////////////////////////////////////////////////////////////////

int TError::GetErrorCode(void)
{
    return itsErrorCode;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetMode
//
////////////////////////////////////////////////////////////////////////////////

void TError::SetMode(int Mode)
{
    itsMode=Mode;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetMode
//
////////////////////////////////////////////////////////////////////////////////

int TError::GetMode(void)
{
    return itsMode;
}

///////////////////////////////////////////////////////////////////////////////
//
// OpenErrorLogFile
//
////////////////////////////////////////////////////////////////////////////////

int TError::OpenErrorLogFile(const char *Name)
{
    time_t ns;
    time(&ns);

    if ((itsLogError=fopen(Name,"a"))==NULL)
        return SetErrorD(0,FATAL,Name);

    char *pcTime = asctime(localtime(&ns));
    if (pcTime)
    {
        fprintf(itsLogError,"\n\n ----------------------------------- \n Started at : %s\n",pcTime);
    }
    fflush(itsLogError);
    return 0;
}

void TError::WriteError(char *message)
{
    if (itsLogError)
    {
        fprintf(itsLogError,"%s",message);
        fflush(itsLogError);
    }

    if (message[0] == '\n')
        strncpy(itsLastErrorMessage, &message[1], 255);
    else
        strncpy(itsLastErrorMessage, message, 255);
    itsLastErrorMessage[254] = '\0';
    if (itsMode==INTERACTIVE)
        printf(message);
}
