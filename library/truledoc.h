/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  truledoc.h
 *
 *
 
CLASS
    TRuleDoc

    implements a class to manage Rule files

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

#ifndef TRuleDoc_H
#define TRuleDoc_H

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
    virtual typestr       GetRuleSpec         (void)  { return itsRuleSpec; };

private:
    TRuleFile         *itsFile;
    typestr           itsRuleSpec;
    typestr           itsFilePointer;

    TError            *itsErrorHandler;

    TUMApplication    *itsApplication;
};

#endif

