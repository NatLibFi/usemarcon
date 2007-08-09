/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tmpplctn.h
 *
 *
 
CLASS
    TUMApplication

    implements the parent class for the application

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004
    Adapted by The National Library of Finland, 2004-2007

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

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

    virtual int           StartUp(const char *a_iniFileName, bool a_interactive, 
                                  const char *a_record, int a_recordLen, bool a_forceVerbose,
                                  const char *a_inputMarcFileName, const char *a_outputMarcFileName,
                                  bool a_disableCharacterConversion);
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
    int                   GetDebugRule(void) { return itsDebugRule; }
    unsigned long         GetOrdinal(void) { return itsOrdinal; }
    bool                  GetUTF8Mode(void) { return itsUTF8Mode; }
    bool                  GetConvertInFieldOrder(void) { return itsConvertInFieldOrder; }
    bool                  GetDisableCharacterConversion() { return m_disableCharacterConversion; }
    bool                  GetMarcRecordAvailable() { return m_record ? true : false; }
    int                   GetMarcRecordLength() { return m_recordLen; }
    void                  SetMarcRecord(const char *a_record, int a_length);
    void                  GetMarcRecord(char *&a_record, int &a_length);

private:
    typestr               itsIniFile;
    long                  itsMarcInputNumero;
    long                  itsRecordsOk;
    long                  itsRecordsFailed;
    TMarcDoc              *itsMarcDoc;
    TCheckDoc             *itsCheckDoc;
    TTransDoc             *itsTransDoc;
    TError                *itsErrorHandler;
    double                itsSize;
    long                  itsRecordsProcessed;
    int                   itsDebugRule;
    unsigned long         itsOrdinal;
    bool                  itsUpdateOrdinal;
    bool                  itsUTF8Mode;
    bool                  itsConvertInFieldOrder;
    bool                  m_interactive;
    char                  *m_record;
    int                   m_recordLen;
    bool                  m_forceVerbose;
    typestr               m_inputMarcFileName;
    typestr               m_outputMarcFileName;
    bool                  m_disableCharacterConversion;

    int                   ToHexa(const char* str);
};

#endif

