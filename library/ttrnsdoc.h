/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  ttrnsdoc.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TTransDoc

    implements a class to manage Transcoding files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/

#ifndef TTransDoc_H
#define TTransDoc_H

#include "tmpplctn.h"
#include "trnsfile.h"
#include "error.h"

enum CHARSET 
{
    CHARSET_DEFAULT = 0,
    CHARSET_MARC8 = 1
};

class TTransDoc
{
public:
    TTransDoc      (TError *ErrorHandler) ;
    virtual ~TTransDoc    ();

    virtual bool          Open            (void);
    virtual bool          OpenTransFile   (char *DefaultFile);
    virtual TTransFile    *GetFile        (void)  { return itsFile; };
    virtual FILE_SPEC     *GetTransSpec   (void)  { return itsTransSpec; };
    bool                  SetInputFileCharacterSet(const char *charset);
    int                   Convert (TMarcRecord* In, TMarcRecord* Out);
    bool                  NeedTranscoding (void)  { return itsFile || itsCharset != CHARSET_DEFAULT ? true : false; };

private:
    TTransFile            *itsFile;
    FILE_SPEC             *itsTransSpec;
    FILE_SPEC             *itsXVTFilePointer;
    CHARSET               itsCharset;

    TError                *itsErrorHandler;

    int ConvertToUTF8(TMarcRecord* MarcIn, TMarcRecord* MarcOut);
    const char* Transcode(char* In, typestr *Out, char *Notice, char *Field);
};

#endif // TTransDoc_H
