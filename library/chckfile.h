/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  chckfile.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TCheckFile

    implements a class to manage check files

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

#ifndef _TCHECK_FILE_H_
#define _TCHECK_FILE_H_

#include "tfile.h"
#include "ctrlfld.h"
#include "umrecord.h"
#include "tmpplctn.h"

class TCheckFile : public TFile
{
public:
    TCheckFile          (FILE_SPEC *FileSpec, TError *ErrorHandler);
    ~TCheckFile         (void);
    
    virtual int           Open                (int IO);
    virtual int           Close               (void);
    virtual int           Verify              (int IO,TUMRecord *aRecord);
    
    virtual TControlField *FindControlField   (char *aTag);
    virtual TControlField *GetFirstCheckField (void) { return itsFirstCheckField; };
    virtual void          DelTreeCheckTag     (void);
    virtual void          ResetControl        (TControlField *Control);
    
protected:
    int                   TestIndicator       (char Ind,char *IndList);
    int                   TestSubfield        (char Balise,TCtrlSubfield *SubList);
    
private:
    TControlField         *itsFirstCheckField;
    TControlField         *itsLastCheckField;
};
#endif

