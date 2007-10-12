/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  tchckdoc.cpp
 *
 *  implements a class to manage the checking of MARC records
 *
 */

#include "error.h"
#include "defines.h"
#include "tchckdoc.h"
#include "tmpplctn.h"


///////////////////////////////////////////////////////////////////////////////
//
// Document class constructor
//
// Create and initialize the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TCheckDoc::TCheckDoc(TError *ErrorHandler)
{
    itsInputFile        = NULL;
    itsOutputFile       = NULL;
    itsCheckInputSpec   = NULL;
    itsCheckOutputSpec  = NULL;
    itsErrorHandler = ErrorHandler;
}

///////////////////////////////////////////////////////////////////////////////
//
// Document class destructor
//
// Delete the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TCheckDoc::~TCheckDoc( void )
{
    if (itsInputFile)       { delete itsInputFile;      itsInputFile        = NULL; }
    if (itsOutputFile)      { delete itsOutputFile;     itsOutputFile       = NULL; }
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
// Called by DoOpen().  Do the actual work of restoring the object from
// a data source.
//
///////////////////////////////////////////////////////////////////////////////
bool TCheckDoc::Open(int IO)
{
    // Add code here to set internal data members of the class based
    // on information read from some data source
    switch (IO)
    {
    case INPUT:
        if (!itsInputFile)      // there is no Input Check File
        {
            if ((itsInputFile = new TCheckFile(itsFilePointer, itsErrorHandler))==NULL)
            {
                itsErrorHandler->SetError(9011,ERROR);
                return false;
            }
        }
        if (itsInputFile->Open(IO))
        {
            delete itsInputFile;
            itsInputFile = NULL;
            return false;
        }
        itsCheckInputSpec = itsFilePointer;
        break;
    case OUTPUT:
        if (!itsOutputFile)     // there is no Output Check File
        {
            if ((itsOutputFile = new TCheckFile(itsFilePointer, itsErrorHandler))==NULL)
            {
                itsErrorHandler->SetError(9012,ERROR);
                return false;
            }
        }
        if (itsOutputFile->Open(IO))
        {
            delete itsOutputFile;
            itsOutputFile = NULL;
            return false;
        }
        itsCheckOutputSpec = itsFilePointer;
        break;
    default:
        return false;
        break;
    }
    return true;
}



///////////////////////////////////////////////////////////////////////////////
//
// OpenCheckFile
//
///////////////////////////////////////////////////////////////////////////////
bool TCheckDoc::OpenCheckFile(int IO,char *DefaultFile)
{
    itsFilePointer = "";
    if (DefaultFile)
    {
        itsFilePointer = DefaultFile;
    }

    return Open(IO);
}

