/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tcd.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TCD

    implements a class to manage record control data

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2006

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    Helsinki University Library, the National Library of Finland

*/

#ifndef _TCD_H_
#define _TCD_H_

#include <stdio.h>
#include <string.h>
#include "typedef.h"
#include "defines.h"
#include "error.h"

#define CD_N        -1
#define CD_NT       -2
#define CD_NS       -3
#define CD_NO       -11
#define CD_NTO      -12
#define CD_NSO      -13

class TCD
{
public:
    TCD             (TError *ErrorHandler);
    TCD             (TCD *aCD);
    TCD             (TypeCD*, TError *ErrorHandler);
    virtual ~TCD    ();

    virtual int   FromString      (char *aString, TCD *Last, int InputOrOutput);
    virtual int   ToString        (char *String, int InputOrOutput);

    virtual TCD   *GetPrevious            (void)      { return itsPrevious; };
    virtual TCD   *GetNext                (void)      { return itsNext; };
    virtual char  *GetTag                 (void)      { return itsTag; };
    virtual char  *GetSubfield            (void)      { return itsSubfield; };
    virtual int   GetOccurenceNumber      (void)      { return itsOccurenceNumber; };
    virtual int   GetTagOccurenceNumber   (void)      { return itsTagOccurenceNumber; };
    virtual int   GetSubOccurenceNumber   (void)      { return itsSubOccurenceNumber; };
    virtual int   GetBegining             (void)      { return itsBegining; };
    virtual int   GetEnd                  (void)      { return itsEnd; };
    virtual int   GetIN                   ()      { return _IN; };

    virtual int   SetPrevious             (TCD *aCD)                  { itsPrevious=aCD; return 0; };
    virtual int   SetNext                 (TCD *aCD)                  { itsNext=aCD; return 0; };
    virtual int   SetTag                  (char *aTagString)          { memcpy(itsTag,aTagString, 3); itsTag[3]=0; return 0; };
    virtual int   SetTag                  (int aTagNumber)            { sprintf(itsTag,"%03d",aTagNumber); return 0; };
    virtual int   SetSubfield             (const char *aSubfield)     { memcpy(itsSubfield,aSubfield,2); itsSubfield[2]=0; return 0; };
    virtual int   SetOccurenceNumber      (int anOccurenceNumber)     { itsOccurenceNumber=anOccurenceNumber; return 0; };
    virtual int   SetTagOccurenceNumber   (int aTagOccurenceNumber)   { itsTagOccurenceNumber=aTagOccurenceNumber; return 0; };
    virtual int   SetSubOccurenceNumber   (int aSubOccurenceNumber)   { itsSubOccurenceNumber=aSubOccurenceNumber; return 0; };
    virtual int   SetBegining             (int aBegining)             { itsBegining=aBegining; return 0; };
    virtual int   SetEnd                  (int aEnd)                  { itsEnd=aEnd; return 0; };
    virtual int   SetFixedPos             (char *String);
    virtual int   SetIN                   (int aIN)   { _IN=aIN; return 0; };

    TError    *GetErrorHandler() { return itsErrorHandler; };

private:
    int           itsBegining;
    int           itsEnd;

protected:
    int           _IN;
    char          itsTag[4];
    char          itsSubfield[3];
    int           itsOccurenceNumber;
    int           itsTagOccurenceNumber;
    int           itsSubOccurenceNumber;
    TCD           *itsPrevious;
    TCD           *itsNext;
    TError        *itsErrorHandler;
};

#endif

