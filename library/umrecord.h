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
    Adapted by The National Library of Finland, 2004-2009

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
    virtual int       SortField       (TCD*);
    virtual int       FromCD          (TRuleFile *RuleFile);
    virtual int       ToCD            (void);
    virtual bool      NextCD          (TCDLib **CDLib, TCD *CD);
    virtual bool      PreviousCD      (TCDLib **CDLib, TCD *CD);
    virtual TCDLib    *InsertCDLib    (TCDLib *aCDLib, TCD *CD=NULL, bool aReplace = false);
    virtual TCDLib    *GetFirstCDLib  (void) const { return itsFirstCDLib; };
    virtual TCDLib    *GetLastCDLib   (void) const { return itsLastCDLib; };
    virtual int       DelTreeCDLib    (void);
    virtual void      RemoveCDLib     (TCDLib* aField);
    virtual void      InsertCDLibAfter(TCDLib* aField, TCDLib* aAfter);
    virtual void      InsertCDLibBefore(TCDLib* aField, TCDLib* aBefore);

protected:
    TCDLib            *itsFirstCDLib;
    TCDLib            *itsLastCDLib;

    void GetOccurrenceNumbersForNew(const char *a_tag, int a_tag_occurrence, const char *a_subfield, 
                                    int &a_occurrence, int &a_sub_occurrence);

    void MergeLinkedFields();
    void SeparateLinkedFields();

private:
    char              *itsSearchedField;
    bool              itsSearchedFieldFound;
    bool              itsExistingFields[1000];
};

#endif
