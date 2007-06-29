/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  error.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    Terror

    implements a class to error reporting and state storage

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004
    Adapted by The National Library of Finland, 2004-2007

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef TError_H
#define TError_H

#include <stdio.h>
#include "defines.h"
#include "typedef.h"

#define ERROR             0
#define WARNING           1
#define FATAL             2
#define DISPLAY           3
#define FORCER            4
#define NOTICE            5
#define NONERROR          6
#define STOP              0
#define CONTINUE          1
#define NONINTERACTIVE    0
#define INTERACTIVE       1
#define DEFAULT_ERR_FILE_NAME        "errcodes.txt"
#define DEFAULT_LOG_FILE_NAME        "errlog.txt"
#define EMPTY_STRING                 ""
#define DEFAULT_TOO_MANY_ERRORS      100
#define SetError(ErrCode,Severity)   SetErrorCode((ErrCode),(Severity),__FILE__,__LINE__)
#define SetErrorD(ErrCode,Severity,Data) SetErrorCode((ErrCode),(Severity),__FILE__,__LINE__,(Data))

// forward declaration
class TUMApplication;

class TError
{
private:
    // hide the default constructor
    TError();
public:
    TError(TUMApplication* theApplication, const char *LogFileName=DEFAULT_LOG_FILE_NAME,
        const char *ErrorCodeFileName=DEFAULT_ERR_FILE_NAME);
    ~TError(void);

    void  Reset                 (void) { itsErrorCode=0; };
    void  CloseErrorLogFile     (void) { fclose(itsLogError); };
    int   OpenErrorLogFile      (const char *Name);

    int   SetErrorCode          (int ErrorCode, short Severity, const char *FileName,
        int LineNumber, const char *UserData=EMPTY_STRING);

    void  SetMode               (int Mode);
    void  WriteError            (char *Message);
    void  SetTooManyErrors      (int max) { itsTooManyErrors = max; };
    void  SetVerboseMode        (int Mode) { itsVerboseMode=Mode; };
    void  SetDebugMode          (int Mode) { itsDebugMode=Mode; };

    int   GetErrorCode          (void);
    int   GetMode               (void);
    int   GetTooManyErrors      (void) { return itsTooManyErrors; };
    int   GetHowManyErrors      (void) { return itsHowManyErrors; };

    char  *GetLastErrorMessage  (void) { return itsLastErrorMessage; };

    // Just an easy way to distribute this information to others
    bool  GetUTF8Mode           (void) { return itsUTF8Mode; }
    void  SetUTF8Mode           (bool UTF8Mode) { itsUTF8Mode = UTF8Mode; }
    bool  GetConvertSubfieldCodesToLowercase(void) { return itsConvertSubfieldCodesToLowercase; }
    void  SetConvertSubfieldCodesToLowercase(bool a_value) { itsConvertSubfieldCodesToLowercase = a_value; }
    typestr GetOutputXMLRecordFormat() { return itsOutputXMLRecordFormat; }
    void  SetOutputXMLRecordFormat(const char *a_value) { itsOutputXMLRecordFormat = a_value; }
    typestr GetOutputXMLRecordType() { return itsOutputXMLRecordType; }
    void  SetOutputXMLRecordType(const char *a_value) { itsOutputXMLRecordType = a_value; }

    TUMApplication *GetApplication      (void) { return itsApplication; }

private:
    TUMApplication  *itsApplication;
    FILE            *itsLogError;
    int             itsMode;
    int             itsErrorCode;
    char            itsLastErrorMessage[255];
    bool            itsUTF8Mode;
    bool            itsConvertSubfieldCodesToLowercase;
    typestr         itsOutputXMLRecordFormat;
    typestr         itsOutputXMLRecordType;

protected:
    unsigned int    itsTooManyErrors;
    unsigned int    itsHowManyErrors;
    int             itsVerboseMode;
    int             itsDebugMode;
};

#endif // TError_H

