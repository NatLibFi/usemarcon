/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  ttrnsdoc.h
 *
 *
 
CLASS
    TTransDoc

    implements a class to manage Transcoding files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2008

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef TTransDoc_H
#define TTransDoc_H

#include "trnsfile.h"
#include "statemanager.h"

enum CHARSET 
{
    CHARSET_DEFAULT = 0,
    CHARSET_MARC8 = 1
};

class TTransDoc
{
public:
    TTransDoc      (TStateManager *StateManager) ;
    virtual ~TTransDoc    ();

    virtual bool          Open            (void);
    virtual bool          OpenTransFile   (char *DefaultFile);
    virtual TTransFile    *GetFile        (void)  { return itsFile; }
    virtual typestr GetTransSpec   (void)  { return itsTransSpec; }
    bool                  SetInputFileCharacterSet(const char *charset);
    int                   Convert (TMarcRecord* In, TMarcRecord* Out);
    bool                  NeedTranscoding (void)  { return itsFile || itsCharset != CHARSET_DEFAULT ? true : false; };

private:
    TTransFile            *itsFile;
    typestr               itsTransSpec;
    typestr               itsFilePointer;
    CHARSET               itsCharset;

    TStateManager                *mStateManager;

    int ConvertToUTF8(TMarcRecord* MarcIn, TMarcRecord* MarcOut);
    const char* Transcode(char* In, typestr *Out, char *Notice, char *Field);
};

#endif // TTransDoc_H
