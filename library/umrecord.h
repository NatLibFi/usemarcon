/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  umrecord.h
 *
 *
 
CLASS
    TUMRecord

    implements a class to manage Records from MARC files

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
#ifndef _TUMREC_H_
#define _TUMREC_H_

#include "tmarcrec.h"
#include "tcdlib.h"

// forward declaration
class TRuleFile;

class TUMRecord : public TMarcRecord
{
public:
    TUMRecord       (TUMApplication *Application);
    TUMRecord       (const TUMRecord &aRecord);
    ~TUMRecord      (void);

    virtual int       SortCD          (void);
    virtual int       SortCD          (TCD*,char*);
    virtual int       PartialSort     (TCDLib *aFirst);
    virtual int       FromCD          (TRuleFile *RuleFile);
    virtual int       ToCD            (void);
    virtual int       NextCD          (TCDLib **CDLib, TCD *CD);
    virtual int       PreviousCD      (TCDLib **CDLib, TCD *CD);
    virtual int       InsereCDLib     (TCDLib *aCDLib, TCD *CD=NULL, int Replace=0);
    virtual TCDLib    *GetFirstCDLib (void) const           { return itsFirstCDLib; };
    virtual TCDLib    *GetLastCDLib  (void) const           { return itsLastCDLib; };
    virtual void      PrintCD(void);
    virtual int       DelTreeCDLib        (void);

protected:
    TCDLib            *itsFirstCDLib;
    TCDLib            *itsLastCDLib;

private:
    char              *itsSearchedField;
    bool              itsSearchedFieldFound;
    bool              itsExistingFields[1000];
};

#endif
