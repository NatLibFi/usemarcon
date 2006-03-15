/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  ctrlfld.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TCtrlSubfield

    implements a class to manage subfields

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/

#ifndef _TCONTROLFIELD_H_
#define _TCONTROLFIELD_H_

#include <stdio.h>
#include <string.h>
#include "error.h"
#include "typedef.h"

class TCtrlSubfield
{
public:
    TCtrlSubfield       (TError *ErrorHandler);
    ~TCtrlSubfield      (void);
    
    char          GetSub              (void)              { return itsSub; };
    int           GetSubMandatory     (void)              { return itsSubMandatory; };
    int           GetSubRepeatable    (void)              { return itsSubRepeatable; };
    int           GetSubOccurency     (void)              { return itsSubOccurency; };
    TCtrlSubfield *GetNextSub         (void)              { return itsNextSub; };
    void          SetSub              (char aSub)         { itsSub=aSub; };
    void          SetSubMandatory     (int IsIt)          { itsSubMandatory=IsIt; };
    void          SetSubRepeatable    (int IsIt)          { itsSubRepeatable=IsIt; };
    void          SetSubOccurency     (void)              { itsSubOccurency++; };
    void          SetSubOccurency     (int Occ)           { itsSubOccurency=Occ; };
    void          SetNextSub          (TCtrlSubfield *theNextSub)     { itsNextSub=theNextSub; };
    
private:
    char          itsSub;
    int           itsSubMandatory;
    int           itsSubRepeatable;
    int           itsSubOccurency;
    TCtrlSubfield *itsNextSub;
    
    TError        *itsErrorHandler;
};


/*
CLASS
    TControlField

    implements a class to manage control fields

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    
AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    
*/


class TControlField
{
    
public:
    TControlField       (TError *ErrorHandler);
    ~TControlField      (void);
    
    void          DelTreeCheckSub     (void);
    void          ResetSubOccurency   (TCtrlSubfield *StartSub);
    
    char          *GetTag             (void)                          { return itsTag; };
    int           GetTagMandatory     (void)                          { return itsTagMandatory; };
    int           GetTagRepeatable    (void)                          { return itsTagRepeatable; };
    int           GetTagOccurency     (void)                          { return itsTagOccurency; };
    TCtrlSubfield *GetFirstSubfield   (void)                          { return itsFirstSubfield; };
    TCtrlSubfield *GetLastSubfield    (void)                          { return itsLastSubfield; };
    char          *GetFirstIndicators (void)                          { return itsFirstIndicators.str(); };
    char          *GetSecondIndicators(void)                          { return itsSecondIndicators.str(); };
    TControlField *GetNextTag         (void)                          { return itsNextTag; };
    
    void          SetTag              (char *aTag)                    { strcpy(itsTag,aTag); itsTag[3]=0; };
    void          SetTagMandatory     (int IsIt)                      { itsTagMandatory=IsIt; };
    void          SetTagRepeatable    (int IsIt)                      { itsTagRepeatable=IsIt; };
    void          SetTagOccurency     (void)                          { itsTagOccurency++; };
    void          SetTagOccurency     (int Occ)                       { itsTagOccurency=Occ; };
    int           SetFirstIndicators  (char *Indicators);
    int           SetSecondIndicators (char *Indicators);
    void          SetFirstSubfield    (TCtrlSubfield *Sub)            { itsFirstSubfield=Sub; };
    void          SetLastSubfield     (TCtrlSubfield *Sub)            { itsLastSubfield=Sub; };
    void          SetNextTag          (TControlField *theNextTag)     { itsNextTag=theNextTag; };
    
private:
    char          itsTag[4];
    int           itsTagMandatory;
    int           itsTagRepeatable;
    int           itsTagOccurency;
    typestr       itsFirstIndicators;
    typestr       itsSecondIndicators;
    TCtrlSubfield *itsFirstSubfield;
    TCtrlSubfield *itsLastSubfield;
    TControlField *itsNextTag;
    
    TError        *itsErrorHandler;
};

#endif

