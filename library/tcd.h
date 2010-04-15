/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tcd.h
 *
 *
 
CLASS
    TCD

    implements a class to manage record control data

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2010

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _TCD_H_
#define _TCD_H_

#include "typedef.h"
#include "defines.h"
#include "statemanager.h"

#define CD_N        -1
#define CD_NT       -2
#define CD_NS       -3
#define CD_NO       -11
#define CD_NTO      -12
#define CD_NSO      -13
#define CD_NEW      -20
#define CD_NEWEST   -21

class TCD
{
public:
    TCD             (TStateManager *StateManager);
    TCD             (TCD *aCD);
    TCD             (TypeCD*, TStateManager *StateManager);
    virtual ~TCD    ();

    int   FromString      (const char *aString, const TCD *Last, int InputOrOutput);
    bool  ToString        (typestr & a_string, int InputOrOutput);

    TCD *GetPrevious              (void) const { return itsPrevious; };
    TCD *GetNext                  (void) const { return itsNext; };
    const char *GetTag            (void) const { return itsTag; };
    const char *GetSubfield       (void) const { return itsSubfield; };
    int   GetOccurrenceNumber     (void) const { return itsOccurrenceNumber; };
    int   GetTagOccurrenceNumber  (void) const { return itsTagOccurrenceNumber; };
    int   GetSubOccurrenceNumber  (void) const { return itsSubOccurrenceNumber; };
    int   GetBeginning            (void) const { return itsBeginning; };
    int   GetEnd                  (void) const { return itsEnd; };
    int   GetIN                   (void) const { return _IN; };

    void  SetPrevious             (TCD *aCD)                  { itsPrevious=aCD; };
    void  SetNext                 (TCD *aCD)                  { itsNext=aCD; };
    void  SetTag                  (const char *aTagString)    { memcpy(itsTag, aTagString, 3); itsTag[3]=0; update_tag_wildcard(); };
    void  SetSubfield             (const char *aSubfield);
    void  SetOccurrenceNumber     (int anOccurrenceNumber)    { itsOccurrenceNumber=anOccurrenceNumber; };
    void  SetTagOccurrenceNumber  (int aTagOccurrenceNumber)  { itsTagOccurrenceNumber=aTagOccurrenceNumber; };
    void  SetSubOccurrenceNumber  (int aSubOccurrenceNumber)  { itsSubOccurrenceNumber=aSubOccurrenceNumber; };
    void  SetBeginning            (int aBeginning)            { itsBeginning=aBeginning; };
    void  SetEnd                  (int aEnd)                  { itsEnd=aEnd; };
    void  SetFixedPos             (const char *a_str);
    void  SetIN                   (int aIN)   { _IN=aIN; };

    void ReplaceWildcards(const char *field, const char *subfield);

    bool TagContainsWildcard() { return itsTagContainsWildcard; } // There is at least one question mark in the tag
    bool TagIsWildcard() { return itsTagIsWildcard; } // Tag is three question marks
    bool SubfieldContainsWildcard() { return itsSubfieldContainsWildcard; } // There is at least one question mark in the subfield

    TStateManager    *GetStateManager() { return mStateManager; };

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
    TStateManager        *mStateManager;
    bool          itsTagContainsWildcard;
    bool          itsTagIsWildcard;
    bool          itsSubfieldContainsWildcard;

    void          update_tag_wildcard();
    void          update_subfield_wildcard();
};

#endif

