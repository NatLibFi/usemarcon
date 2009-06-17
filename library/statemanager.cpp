/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2009
 *
 *  File:  error.cpp
 *
 *  implements a class to manage errors
 *
 */

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "statemanager.h"
#include "tools.h"
#include "errcodes.h"

///////////////////////////////////////////////////////////////////////////////
//
// TStateManager
//
////////////////////////////////////////////////////////////////////////////////

TStateManager::TStateManager(TUMApplication* theApplication, const char *LogFileName)
{
    itsApplication=theApplication;

    itsTooManyErrors       = 0;
    itsHowManyErrors       = 0;
    itsErrorCode           = 0;
    itsDebugMode           = 0;
    itsLogError            = NULL;
    itsUTF8Mode            = false;
    itsConvertSubfieldCodesToLowercase = false;
    mRecordNumber = 0;
    *mRecordId = '\0';
    mHandleLinkedFields = false;
    mSortRecord = true;

    // Logfile opening
    if (*LogFileName)
        OpenErrorLogFile(LogFileName);

    itsLastErrorMessage[0] = '\0';
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TStateManager
//
////////////////////////////////////////////////////////////////////////////////

TStateManager::~TStateManager()
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

int TStateManager::SetErrorCode(int ErrorCode, short Severity, const char *FileName,
                         int LineNumber, const char *UserData)
{
    bool bFoundCode = false;
    short iIndice;
    for (iIndice = 0; iIndice < ErrorDescCount; iIndice++)
    {
        if (ErrorDesc[iIndice].errorcode == ErrorCode)
        {
            bFoundCode = true;
            break;
        }
    }

    const char* description = bFoundCode ? ErrorDesc[iIndice].description : "Unknown Error";
    char rec_info[100];
    if (*description == '*')
    {
        ++description;
        sprintf(rec_info, "(record %ld%s%s) ", mRecordNumber, *mRecordId ? ", ID " : "", mRecordId);
    }
    else
    {
        *rec_info = '\0';
    }

    typestr message;
    const char *category = NULL;
    switch(Severity)
    {
        case DISPLAY:
            message.allocstr(50);
            sprintf(message.str(), "WARNING (%d) - ", ErrorCode);
            message += description;
            message += "\n";
            message += rec_info;
            message += UserData;
            show_warning(message.str());
            return 0-ErrorCode;
        case WARNING: category = "WARNING"; break;
        case ERROR: category = "ERROR"; itsHowManyErrors++; break;
        case NOTICE: category = "NOTICE"; break;
        case NONERROR: category = "INFO"; break;
        default: category = "FATAL"; itsHowManyErrors++; break;
    }
    itsErrorCode = ErrorCode;
    
    if (itsLogError)
    {
        fprintf(itsLogError, "%s (%d) - %s %s%s%s\n", category, ErrorCode,
            description, rec_info, *UserData ? ": " : "", UserData);
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
    message += description;
    message += "\n";
    message += rec_info;
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
// OpenErrorLogFile
//
////////////////////////////////////////////////////////////////////////////////

int TStateManager::OpenErrorLogFile(const char *Name)
{
    time_t ns;
    time(&ns);

    if ((itsLogError=fopen(Name,"a"))==NULL)
        return SetErrorD(0,FATAL,Name);

    char *pcTime = asctime(localtime(&ns));
    if (pcTime)
    {
        fprintf(itsLogError,"\n\n-----------------------------------\nUSEMARCON v%s started at %s\n", 
            USEMARCON_VERSION, pcTime);
    }
    fflush(itsLogError);
    return 0;
}

void TStateManager::WriteError(char *message)
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
        printf("%s", message);
}

const char* TStateManager::GetPCRECompileErrorDesc(int a_index)
{
    if (a_index >= 0 && a_index <= sizeof(PCRECompileErrorDesc) / sizeof(char *))
        return PCRECompileErrorDesc[a_index];
    return "";
}

const char* TStateManager::GetPCREExecErrorDesc(int a_errorcode)
{
    for (int i = 0; i < PCREExecErrorDescCount; i++)
    {
        if (PCREExecErrorDesc[i].errorcode == a_errorcode)
        {
            return PCREExecErrorDesc[i].description;
        }
    }
    return NULL;
}
