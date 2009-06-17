/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  ctrlfld.h
 *
 *
 
CLASS
    TCtrlSubfield

    implements a class to manage subfields

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2009

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _TCONTROLFIELD_H_
#define _TCONTROLFIELD_H_

#include "statemanager.h"
#include "typedef.h"

class TCtrlSubfield
{
public:
    TCtrlSubfield       (TStateManager *StateManager);
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
    
    TStateManager        *mStateManager;
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
    Adapted by The National Library of Finland, 2004-2009
    
AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland
    
*/


class TControlField
{
    
public:
    TControlField       (TStateManager *StateManager);
    ~TControlField      (void);
    
    void          DelTreeCheckSub     (void);
    void          ResetSubOccurency   (TCtrlSubfield *StartSub);
    
    char          *GetTag             (void)                          { return itsTag; };
    int           GetTagMandatory     (void)                          { return itsTagMandatory; };
    int           GetTagRepeatable    (void)                          { return itsTagRepeatable; };
    int           GetTagOccurrency    (void)                          { return itsTagOccurrency; };
    TCtrlSubfield *GetFirstSubfield   (void)                          { return itsFirstSubfield; };
    TCtrlSubfield *GetLastSubfield    (void)                          { return itsLastSubfield; };
    char          *GetFirstIndicators (void)                          { return itsFirstIndicators.str(); };
    char          *GetSecondIndicators(void)                          { return itsSecondIndicators.str(); };
    TControlField *GetNextTag         (void)                          { return itsNextTag; };
    
    void          SetTag              (char *aTag)                    { strcpy(itsTag,aTag); itsTag[3]=0; };
    void          SetTagMandatory     (int IsIt)                      { itsTagMandatory=IsIt; };
    void          SetTagRepeatable    (int IsIt)                      { itsTagRepeatable=IsIt; };
    void          SetTagOccurrency    (void)                          { itsTagOccurrency++; };
    void          SetTagOccurrency    (int Occ)                       { itsTagOccurrency=Occ; };
    int           SetFirstIndicators  (char *Indicators);
    int           SetSecondIndicators (char *Indicators);
    void          SetFirstSubfield    (TCtrlSubfield *Sub)            { itsFirstSubfield=Sub; };
    void          SetLastSubfield     (TCtrlSubfield *Sub)            { itsLastSubfield=Sub; };
    void          SetNextTag          (TControlField *theNextTag)     { itsNextTag=theNextTag; };
    
private:
    char          itsTag[4];
    int           itsTagMandatory;
    int           itsTagRepeatable;
    int           itsTagOccurrency;
    typestr       itsFirstIndicators;
    typestr       itsSecondIndicators;
    TCtrlSubfield *itsFirstSubfield;
    TCtrlSubfield *itsLastSubfield;
    TControlField *itsNextTag;
    
    TStateManager        *mStateManager;
};

#endif

