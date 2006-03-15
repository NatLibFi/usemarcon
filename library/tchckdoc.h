/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tchckdoc.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TCheckDoc

    implements a class to manage checking files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/
#ifndef TCheckDoc_H
#define TCheckDoc_H

#include "chckfile.h"
#include "defines.h"
#include "error.h"

// forward declaration
class TCheckFile;

class TCheckDoc
{
public:
    TCheckDoc           (TError *ErrorHandler);
    virtual ~TCheckDoc   ();

    virtual bool          Open                (int IO);
    virtual bool          OpenCheckFile       (int IO,char *DefaultFile);
    virtual TCheckFile    *GetInputFile       (void)          { return itsInputFile; };
    virtual TCheckFile    *GetOutputFile      (void)          { return itsOutputFile; };
    virtual FILE_SPEC     *GetCheckInputSpec  (void)          { return itsCheckInputSpec; };
    virtual FILE_SPEC     *GetCheckOutputSpec (void)          { return itsCheckOutputSpec; };

private:
    TCheckFile            *itsInputFile;
    TCheckFile            *itsOutputFile;
    FILE_SPEC             *itsCheckInputSpec;
    FILE_SPEC             *itsCheckOutputSpec;
    FILE_SPEC             *itsXVTFilePointer;
    TError                *itsErrorHandler;
};

#endif // TCheckDoc_H

