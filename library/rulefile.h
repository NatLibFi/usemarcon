/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  rulefile.h
 *
 *
 
CLASS
    TRuleFile

    implements a class to manage Rule Files

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
#ifndef _TRULE_FILE_H_
#define _TRULE_FILE_H_

#include "defines.h"
#include "tfile.h"
#include "trule.h"
#include "codedata.h"
#include "tmpplctn.h"
#include "lien_not.h"
#include "umrecord.h"

class TRuleFile : public TFile
{
public:
    TRuleFile       (FILE_SPEC *FileSpec,TUMApplication *Application);
    ~TRuleFile      ();

    virtual int           OpenRuleFile    ();
    virtual int           CloseRuleFile   ();
    virtual int           ConvertInRuleOrder(TUMRecord *In, TUMRecord *Out);
    virtual int           ConvertInFieldOrder(TUMRecord *In, TUMRecord *Out);
    virtual void          DelTreeRule     (void);
    virtual void          DelTreeCodedData(void);
    virtual TRule         *GetFirstRule   (void)                  { return itsFirstRule; };
    virtual TCodedData    *GetCodedData   (char *theName);
    virtual void          SetFirstRule    (TRule *theRule)        { itsFirstRule=theRule; };

    virtual TEvaluateRule *GetEvaluateRule(void) { return &itsEvaluateRule; }

protected:
    TRuleDoc              *itsDocument;
    TRule                 *itsFirstRule;
    TCD                   *itsLastInputCD;
    TCD                   *itsLastOutputCD;
    TCodedData            *itsFirstCodedData;
    TCodedData            *itsLastCodedData;

    TEvaluateRule         itsEvaluateRule;

private:
    TUMApplication        *itsApplication;

    TError                *ErrorHandler;
};
#endif
