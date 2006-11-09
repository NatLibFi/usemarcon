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

    implements a class to error reporting

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004
    Adapted by The National Library of Finland, 2004-2006

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    Helsinki University Library, the National Library of Finland

*/

#ifndef TError_H
#define TError_H

#include <stdio.h>
#include "defines.h"

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
    virtual ~TError(void);

    virtual void  Reset                 (void) { itsErrorCode=0; };
    virtual void  CloseErrorLogFile     (void) { fclose(itsLogError); };
    virtual int   OpenErrorLogFile      (const char *Name);

    virtual int   SetErrorCode          (int ErrorCode, short Severity, const char
        *FileName,int LineNumber, const char *UserData=EMPTY_STRING);

    virtual void  SetMode               (int Mode);
    virtual void  WriteError            (char *Message);
    virtual void  SetTooManyErrors      (int max) { itsTooManyErrors = max; };
    virtual void  SetVerboseMode        (int Mode) { itsVerboseMode=Mode; };
    virtual void  SetDebugMode          (int Mode) { itsDebugMode=Mode; };

    virtual int   GetErrorCode          (void);
    virtual int   GetMode               (void);
    virtual int   GetTooManyErrors      (void) { return itsTooManyErrors; };
    virtual int   GetHowManyErrors      (void) { return itsHowManyErrors; };

    virtual char  *GetLastErrorMessage  (void) { return itsLastErrorMessage; };

    // Just an easy way to distribute this information to others
    bool          GetUTF8Mode           (void) { return itsUTF8Mode; }
    void          SetUTF8Mode           (bool UTF8Mode) { itsUTF8Mode = UTF8Mode; }

    TUMApplication *GetApplication      (void) { return itsApplication; }

    // Temporary buffers for all to use
    unsigned char Temporary[TBUF];
    unsigned char Temporary2[TBUF];

private:
    TUMApplication  *itsApplication;
    FILE            *itsLogError;
    int             itsMode;
    int             itsErrorCode;
    char            itsLastErrorMessage[255];
    bool            itsUTF8Mode;

protected:
    unsigned int    itsTooManyErrors;
    unsigned int    itsHowManyErrors;
    int             itsVerboseMode;
    int             itsDebugMode;
};

#endif // TError_H

