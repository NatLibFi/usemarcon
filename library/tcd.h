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
    Adapted by The National Library of Finland, 2004-2007

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

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
#define CD_NEW      -20

class TCD
{
public:
    TCD             (TError *ErrorHandler);
    TCD             (TCD *aCD);
    TCD             (TypeCD*, TError *ErrorHandler);
    virtual ~TCD    ();

    int   FromString      (char *aString, TCD *Last, int InputOrOutput);
    bool  ToString        (typestr & a_string, int InputOrOutput);

    TCD   *GetPrevious            (void)      { return itsPrevious; };
    TCD   *GetNext                (void)      { return itsNext; };
    char  *GetTag                 (void)      { return itsTag; };
    char  *GetSubfield            (void)      { return itsSubfield; };
    int   GetOccurrenceNumber     (void)      { return itsOccurrenceNumber; };
    int   GetTagOccurrenceNumber  (void)      { return itsTagOccurrenceNumber; };
    int   GetSubOccurrenceNumber  (void)      { return itsSubOccurrenceNumber; };
    int   GetBeginning            (void)      { return itsBeginning; };
    int   GetEnd                  (void)      { return itsEnd; };
    int   GetIN                   ()      { return _IN; };

    void  SetPrevious             (TCD *aCD)                  { itsPrevious=aCD; };
    void  SetNext                 (TCD *aCD)                  { itsNext=aCD; };
    void  SetTag                  (char *aTagString)          { memcpy(itsTag, aTagString, 3); itsTag[3]=0; update_tag_wildcard(); };
    void  SetSubfield             (const char *aSubfield);
    void  SetOccurrenceNumber     (int anOccurrenceNumber)    { itsOccurrenceNumber=anOccurrenceNumber; };
    void  SetTagOccurrenceNumber  (int aTagOccurrenceNumber)  { itsTagOccurrenceNumber=aTagOccurrenceNumber; };
    void  SetSubOccurrenceNumber  (int aSubOccurrenceNumber)  { itsSubOccurrenceNumber=aSubOccurrenceNumber; };
    void  SetBeginning            (int aBeginning)            { itsBeginning=aBeginning; };
    void  SetEnd                  (int aEnd)                  { itsEnd=aEnd; };
    void  SetFixedPos             (char *String);
    void  SetIN                   (int aIN)   { _IN=aIN; };

    void ReplaceWildcards(const char *field, const char *subfield);

    bool TagContainsWildcard() { return itsTagContainsWildcard; } // There is at least one question mark in the tag
    bool TagIsWildcard() { return itsTagIsWildcard; } // Tag is three question marks
    bool SubfieldContainsWildcard() { return itsSubfieldContainsWildcard; } // There is at least one question mark in the subfield

    TError    *GetErrorHandler() { return itsErrorHandler; };

private:
    int           itsBeginning;
    int           itsEnd;

protected:
    int           _IN;
    char          itsTag[4];
    char          itsSubfield[3];
    int           itsOccurrenceNumber;
    int           itsTagOccurrenceNumber;
    int           itsSubOccurrenceNumber;
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

