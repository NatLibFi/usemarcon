/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tchckdoc.h
 *
 *
 
CLASS
    TCheckDoc

    implements a class to manage checking files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2010

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/
#ifndef TCheckDoc_H
#define TCheckDoc_H

#include "chckfile.h"
#include "defines.h"
#include "statemanager.h"

// forward declaration
class TCheckFile;

class TCheckDoc
{
public:
    TCheckDoc           (TStateManager *StateManager);
    virtual ~TCheckDoc   ();

    virtual bool          Open                (int IO);
    virtual bool          OpenCheckFile       (int IO,char *DefaultFile);
    virtual TCheckFile    *GetInputFile       (void)          { return itsInputFile; };
    virtual TCheckFile    *GetOutputFile      (void)          { return itsOutputFile; };
    virtual typestr       GetCheckInputSpec  (void)          { return itsCheckInputSpec; };
    virtual typestr       GetCheckOutputSpec (void)          { return itsCheckOutputSpec; };

private:
    TCheckFile            *itsInputFile;
    TCheckFile            *itsOutputFile;
    typestr               itsCheckInputSpec;
    typestr               itsCheckOutputSpec;
    typestr               itsFilePointer;
    TStateManager                *mStateManager;
};

#endif // TCheckDoc_H

