/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tmpplctn.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TUMApplication

    implements the parent class for the application

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/

#ifndef TUMApplication_H
#define TUMApplication_H

#include "defines.h"
#include "tfile.h"
#include "truledoc.h"
#include "tmarcdoc.h"
#include "tchckdoc.h"
#include "ttrnsdoc.h"
#include "error.h"

// forward declaration
class TCheckDoc;

class TUMApplication
{
public:
    TUMApplication(void);
    virtual ~TUMApplication();

    virtual int           StartUp(CDetails *Details);
    virtual int           Convert(void);
    virtual void          End(void);
    virtual int           DoConvertWrite(void);
    virtual void          SetMarcDoc(TMarcDoc *aDoc) { itsMarcDoc=aDoc; };
    virtual void          SetCheckDoc(TCheckDoc *aDoc) { itsCheckDoc=aDoc; };
    virtual void          SetRuleDoc(TRuleDoc *aDoc) { itsRuleDoc=aDoc; };
    virtual void          SetTransDoc(TTransDoc *aDoc) { itsTransDoc=aDoc; };
    virtual TMarcDoc      *GetMarcDoc(void) { return itsMarcDoc; };
    virtual TCheckDoc     *GetCheckDoc(void) { return itsCheckDoc; };
    virtual TRuleDoc      *GetRuleDoc(void) { return itsRuleDoc; };
    virtual TTransDoc     *GetTransDoc(void) { return itsTransDoc; };
    TRuleDoc              *itsRuleDoc;
    TError                *GetErrorHandler(void);
    CDetails              *GetDetails(void);
    int                   GetDebugRule(void) { return itsDebugRule; }
    unsigned long         GetOrdinal(void) { return itsOrdinal; }
    bool                  GetUTF8Mode(void) { return itsUTF8Mode; }
    bool                  GetConvertInFieldOrder(void) 
                            { return itsConvertInFieldOrder; }

private:
    char                  itsIniFile[MAXPATH];
    long                  itsMarcInputNumero;
    long                  itsRecordsOk;
    long                  itsRecordsFailed;
    TMarcDoc              *itsMarcDoc;
    TCheckDoc             *itsCheckDoc;
    TTransDoc             *itsTransDoc;
    TError                *itsErrorHandler;
    CDetails              *itsDetails;
    double                itsSize;
    long                  itsRecordsProcessed;
    int                   itsDebugRule;
    unsigned long         itsOrdinal;
    bool                  itsUpdateOrdinal;
    bool                  itsUTF8Mode;
    bool                  itsConvertInFieldOrder;
};

#endif

