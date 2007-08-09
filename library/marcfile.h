/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  marcfile.h
 *
 *
 
CLASS
    TMarcFile

    implements a class to manage MARC record files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2007

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _TMARC_FILE_H_
#define _TMARC_FILE_H_

#include "tfile.h"
#include "defines.h"
#include "umrecord.h"
#include "error.h"

//forward declaration
class TUMApplication;
class TUMRecord;
class TMarcDoc;

class TMarcFile : public TFile
{
public:
    TMarcFile               (typestr & FileSpec, TUMApplication *Application,
                             char Mode/*=FILE_READ*/,char Kind/*=FILE_ASCII*/,
                             MARC_FILE_FORMAT Format, short BlockSize, short MinFree,
                             char PaddingChar, bool LastBlock);
    ~TMarcFile              (void);

    // Overrides from the base class
    int       Open                    (void);
    int       Close                   (void);

    int       Read                    (TUMRecord *Record);
    int       Write                   (TUMRecord *Record);

    void      SetMarcInfoFormat       (MARC_FILE_FORMAT theFormat) { itsMarcInfo.Format=theFormat; };
    void      SetMarcInfoBlockSize    (short theBlockSize)    { itsMarcInfo.BlockSize=theBlockSize; };
    void      SetMarcInfoMinDataFree  (short theMinDataFree)  { itsMarcInfo.MinDataFree=theMinDataFree; };
    void      SetMarcInfoPaddingChar  (char thePaddingChar)   { itsMarcInfo.PaddingChar=thePaddingChar; };
    void      SetMarcInfoLastBlock    (bool theLastBlock)     { itsMarcInfo.LastBlock=theLastBlock; };
    MARC_FILE_FORMAT GetMarcInfoFormat (void)                 { return itsMarcInfo.Format; };
    unsigned short GetMarcInfoBlockSize (void)                { return itsMarcInfo.BlockSize; };
    unsigned short GetMarcInfoMinDataFree (void)              { return itsMarcInfo.MinDataFree; };
    char      GetMarcInfoPaddingChar  (void)                  { return itsMarcInfo.PaddingChar; };
    bool      GetMarcInfoLastBlock    (void)                  { return itsMarcInfo.LastBlock; };

    virtual long GetSize(void);
    virtual long GetPos(void);

private:
    typestr           m_filebuffer;
    unsigned long     PosCour;
    unsigned long     NumBloc;
    unsigned char     FinBande[6];
    int               TB;
    unsigned long     BufSize;
    unsigned long     BufPos;
    bool              EndOfFile;
    bool              itsEof;
    int               read_marc   (unsigned long, char*);
    int               read_marc_scw(bool a_first);
    bool              first_block (void);
    int               write_marc  (unsigned long, char*);
    int               write_marc_scw(short,unsigned long);
    int               val         (const char *, unsigned short *);
    int               longval     (const char *, unsigned long *);

    bool              xml_read_tag(const char *tag, typestr & a_xml);
    bool              xml_read_until_end(const char *tag, typestr & a_xml);

    TUMApplication    *itsApplication;
    TError            *itsErrorHandler;

    unsigned long     itsCharsRead;

protected:
    TMarcDoc          *itsDocument;
    struct MARCINFO
    {
      MARC_FILE_FORMAT  Format;
      unsigned short BlockSize;
      unsigned short MinDataFree;
      char  PaddingChar;
      bool  LastBlock;
    } itsMarcInfo;
};

#endif

