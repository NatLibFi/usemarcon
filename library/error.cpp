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
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
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

int TError::SetErrorCode(int ErrorCode, short Severity, const char *FileName,int
                         LineNumber, const char *UserData)
{
    short          iIndice;
    bool           bFoundCode=false;
    char           categorie[10];
    char           Message[1000];
    char           ShortUserData[500];

    for (iIndice = 0; iIndice < ErrorDescCount; iIndice++)
    {
        if (ErrorDesc[iIndice].errorcode == ErrorCode)
        {
            bFoundCode = true;
            break;
        }
    }

    strncpy(ShortUserData, UserData, 500);
    ShortUserData[499] = '\0';

    // Write message to logfile ...
    switch(Severity)
    {
        case FORCER:
        case DISPLAY:
            sprintf(Message,"WARNING (%d)-%2s\n%s",ErrorCode,bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error",ShortUserData);
            Message[199] = '\0';
            show_warning(Message);
            return 0-ErrorCode;
        case WARNING: strcpy(categorie,"WARNING "); break;
        case ERROR: strcpy(categorie,"ERROR "); break;
        case NOTICE: strcpy(categorie,"NOTICE "); break;
        case NONERROR: strcpy(categorie,"NO ERROR "); break;
        default: strcpy(categorie,"FATAL "); break;
    }

    // Error Code setting
    itsErrorCode=ErrorCode;
    itsHowManyErrors++;

    if (itsLogError)
    {
        if ((ErrorCode>=9000) || (itsDebugMode))
            fprintf(itsLogError,"%s(%d)-%s%s%s in %s:%d\n",categorie,ErrorCode,
                bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error",*UserData ? " : " : "",
                UserData,FileName,LineNumber);
        else
            fprintf(itsLogError,"%s(%d)-%s%s%s\n",categorie,ErrorCode,
                bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error",*UserData ? " : " : "",
                UserData);
        fflush(itsLogError);
    }

    // Write message to screen

    if ((ErrorCode>=9000) || (itsDebugMode))
        sprintf(Message,"%s(%d)-%s\n%s\nin %s:%d",categorie,ErrorCode,
        bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error",ShortUserData,FileName,LineNumber);
    else
        sprintf(Message,"%s(%d)-%s\n%s",categorie,ErrorCode,
        bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error",ShortUserData);
    Message[199] = '\0';
    switch(Severity)
    {
    case ERROR:
    case WARNING:
    case NOTICE:
        if (itsMode==INTERACTIVE && itsVerboseMode)
            show_message(Message);
        break;
    case NONERROR:
        // Don't show non-errors
        break;
    default:
        if (itsMode==INTERACTIVE)
            show_message(Message);
        break;
    }
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
