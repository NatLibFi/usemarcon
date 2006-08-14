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

    int   FromString      (char *aString, TCD *Last, int InputOrOutput);
    int   ToString        (char *String, int InputOrOutput);

    TCD   *GetPrevious            (void)      { return itsPrevious; };
    TCD   *GetNext                (void)      { return itsNext; };
    char  *GetTag                 (void)      { return itsTag; };
    char  *GetSubfield            (void)      { return itsSubfield; };
    int   GetOccurenceNumber      (void)      { return itsOccurenceNumber; };
    int   GetTagOccurenceNumber   (void)      { return itsTagOccurenceNumber; };
    int   GetSubOccurenceNumber   (void)      { return itsSubOccurenceNumber; };
    int   GetBegining             (void)      { return itsBegining; };
    int   GetEnd                  (void)      { return itsEnd; };
    int   GetIN                   ()      { return _IN; };

    int   SetPrevious             (TCD *aCD)                  { itsPrevious=aCD; return 0; };
    int   SetNext                 (TCD *aCD)                  { itsNext=aCD; return 0; };
    int   SetTag                  (char *aTagString)          { memcpy(itsTag, aTagString, 3); itsTag[3]=0; update_tag_wildcard(); return 0; };
    int   SetSubfield             (const char *aSubfield)     { memcpy(itsSubfield, aSubfield, 2); itsSubfield[2]=0; update_subfield_wildcard(); return 0; };
    int   SetOccurenceNumber      (int anOccurenceNumber)     { itsOccurenceNumber=anOccurenceNumber; return 0; };
    int   SetTagOccurenceNumber   (int aTagOccurenceNumber)   { itsTagOccurenceNumber=aTagOccurenceNumber; return 0; };
    int   SetSubOccurenceNumber   (int aSubOccurenceNumber)   { itsSubOccurenceNumber=aSubOccurenceNumber; return 0; };
    int   SetBegining             (int aBegining)             { itsBegining=aBegining; return 0; };
    int   SetEnd                  (int aEnd)                  { itsEnd=aEnd; return 0; };
    int   SetFixedPos             (char *String);
    int   SetIN                   (int aIN)   { _IN=aIN; return 0; };

    void ReplaceWildcards(const char *field, const char *subfield);

    bool TagContainsWildcard() { return itsTagContainsWildcard; } // There is at least one question mark in the tag
    bool TagIsWildcard() { return itsTagIsWildcard; } // Tag is three question marks
    bool SubfieldContainsWildcard() { return itsSubfieldContainsWildcard; } // There is at least one question mark in the subfield

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
    bool          itsTagContainsWildcard;
    bool          itsTagIsWildcard;
    bool          itsSubfieldContainsWildcard;

    void          update_tag_wildcard();
    void          update_subfield_wildcard();
};

#endif

