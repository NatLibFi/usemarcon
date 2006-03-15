/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  truledoc.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TRuleDoc

    implements a class to manage Rule files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/

#ifndef TRuleDoc_H
#define TRuleDoc_H
#include "rulefile.h"

// forward declaration
class TRuleFile;

class TRuleDoc
{
public:
    TRuleDoc        (TUMApplication *Application);
    virtual ~TRuleDoc   ();

    virtual bool          Open            (void);
    virtual bool          OpenRuleFile        (char *DefaultFile);

    virtual TRuleFile     *GetFile            (void)  { return itsFile; };
    virtual FILE_SPEC     *GetRuleSpec        (void)  { return itsRuleSpec; };

private:
    TRuleFile         *itsFile;
    FILE_SPEC         *itsRuleSpec;
    FILE_SPEC         *itsXVTFilePointer;

    TError            *itsErrorHandler;

    TUMApplication    *itsApplication;
};

#endif

