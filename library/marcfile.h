/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  marcfile.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TMarcFile

    implements a class to manage MARC record files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2006

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
    TMarcFile               (FILE_SPEC *FileSpec, TUMApplication *Application,
                             char Mode/*=FILE_READ*/,char Kind/*=FILE_ASCII*/,
                             MARC_FILE_FORMAT Format, short BlockSize, short MinFree,
                             char PaddingChar, bool LastBlock);
    ~TMarcFile              (void);

    // Overrides from the base class
    int       Open                    (void);
    int       Close                   (void);

    int       Read                    (TUMRecord *Record);
    int       Write                   (TUMRecord *Record);
    long      GetRecordNumber         (void)                  { return NumNotice; };

    void      SetMarcInfoFormat       (MARC_FILE_FORMAT theFormat) { itsMarcInfo.Format=theFormat; };
    void      SetMarcInfoBlockSize    (short theBlockSize)    { itsMarcInfo.BlockSize=theBlockSize; };
    void      SetMarcInfoMinDataFree  (short theMinDataFree)  { itsMarcInfo.MinDataFree=theMinDataFree; };
    void      SetMarcInfoPaddingChar  (char thePaddingChar)   { itsMarcInfo.PaddingChar=thePaddingChar; };
    void      SetMarcInfoLastBlock    (bool theLastBlock)     { itsMarcInfo.LastBlock=theLastBlock; };
    MARC_FILE_FORMAT GetMarcInfoFormat       (void)                  { return itsMarcInfo.Format; };
    short     GetMarcInfoBlockSize    (void)                  { return itsMarcInfo.BlockSize; };
    short     GetMarcInfoMinDataFree  (void)                  { return itsMarcInfo.MinDataFree; };
    char      GetMarcInfoPaddingChar  (void)                  { return itsMarcInfo.PaddingChar; };
    bool      GetMarcInfoLastBlock    (void)                  { return itsMarcInfo.LastBlock; };

    virtual long GetSize(void);
    virtual long GetPos(void);

private:
    unsigned char     *Buffer;
    unsigned long     NumNotice;
    unsigned long     PosCour;
    unsigned long     NumBloc;
    unsigned long     LngNotice;
    unsigned long     FinBloc;
    unsigned char     *Buf;
    unsigned char     FinBande[6];
    int               TB;
    unsigned short    TBuf;
    unsigned short    PBuf;
    unsigned short    Reste;
    bool              EndOfFile;
    bool              itsEof;
    int               read_marc   (unsigned short, unsigned char*);
    int               read_marc_scw(char);
    int               debut_bloc  (void);
    int               write_marc  (unsigned short, unsigned char*);
    int               write_marc_scw(short,unsigned short);
    int               val         (unsigned char*, unsigned short *);
    int               longval     (unsigned char*, unsigned long *);

    TUMApplication    *itsApplication;
    TError            *itsErrorHandler;

    unsigned long     itsCharsRead;

protected:
    TMarcDoc          *itsDocument;
    struct MARCINFO
    {
      MARC_FILE_FORMAT  Format;
      short BlockSize;
      short MinDataFree;
      char  PaddingChar;
      bool  LastBlock;
    } itsMarcInfo;
};

#endif

