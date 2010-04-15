/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2010
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

    itsMaximumErrors       = 0;
    itsErrorCount          = 0;
    itsWarningCount        = 0;
    itsErrorCode           = 0;
    itsDebugMode           = 0;
    itsLogError            = NULL;
    itsUTF8Mode            = false;
    itsConvertSubfieldCodesToLowercase = false;
    mRecordNumber = 0;
    *mRecordId = '\0';
    mHandleLinkedFields = false;
    mSortRecord = true;

    itsLastErrorMessage = "";
    itsLastWarningMessage = "";

    // Logfile opening
    if (*LogFileName)
        OpenErrorLogFile(LogFileName);
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
    switch (Severity)
    {
        case NONERROR: 
            category = "INFO"; 
            break;
        case NOTICE: 
            category = "NOTICE"; 
            break;
        case DISPLAY: 
        case WARNING: 
            category = "WARNING"; 
            itsWarningCount++; 
            break;
        case ERROR: 
            category = "ERROR"; 
            itsErrorCount++; 
            itsErrorCode = ErrorCode; 
            break;
        default: 
            category = "FATAL"; 
            itsErrorCount++; 
            itsErrorCode = ErrorCode; 
            break;
    }
    
    message.allocstr(50);
    sprintf(message.str(), "%s (%d) - ", category, ErrorCode);
    message += description;
    message += " ";
    message += rec_info;
    if (*UserData)
    {
        if (*rec_info)
            message += ": ";
        message += UserData;
    }

    if (itsLogError && Severity != DISPLAY)
    {
        fprintf(itsLogError, "%s\n", message.str());
        fflush(itsLogError);
    }

    switch (Severity)
    {
    case DISPLAY:
        show_message(message.str());
        itsLastWarningMessage = message;
        break;
    case ERROR:
        itsLastErrorMessage = message;
        if (itsMode == INTERACTIVE && itsVerboseMode)
            show_message(message.str());
        break;
    case WARNING:
        itsLastWarningMessage = message;
        if (itsMode == INTERACTIVE && itsVerboseMode)
            show_message(message.str());
        break;
    case NOTICE:
        if (itsMode == INTERACTIVE && itsVerboseMode)
            show_message(message.str());
        break;
    case NONERROR:
        // Don't show non-errors
        break;
    default:
        itsLastErrorMessage = message;
        if (itsMode == INTERACTIVE)
            show_message(message.str());
        break;
    }
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

void TStateManager::WriteError(const char *message)
{
    itsErrorCount++;
    if (itsLogError)
    {
        fprintf(itsLogError, "%s", message);
        fflush(itsLogError);
    }

    if (message[0] == '\n')
        itsLastErrorMessage = &message[1];
    else
        itsLastErrorMessage = message;

    if (itsMode == INTERACTIVE)
        printf("%s", message);
}

void TStateManager::WriteMessage(const char *message)
{
    if (itsLogError)
    {
        fprintf(itsLogError, "%s", message);
        fflush(itsLogError);
    }

    if (itsMode == INTERACTIVE)
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
