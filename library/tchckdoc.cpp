/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *
 *  File:  tchckdoc.cpp
 *
 *  implements a class to manage the checking of MARC records
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
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
    itsXVTFilePointer   = NULL;
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
    if (itsCheckInputSpec)  { delete itsCheckInputSpec; itsCheckInputSpec   = NULL; }
    if (itsCheckOutputSpec){ delete itsCheckOutputSpec;itsCheckOutputSpec   = NULL; }

    // John Hough - added delete
    if (itsXVTFilePointer)
        delete itsXVTFilePointer;
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
            if ((itsInputFile = new TCheckFile(itsXVTFilePointer, itsErrorHandler))==NULL)
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
        if (!itsCheckInputSpec)
            itsCheckInputSpec = new FILE_SPEC;
        memcpy(itsCheckInputSpec,itsXVTFilePointer,sizeof(FILE_SPEC));
        break;
    case OUTPUT:
        if (!itsOutputFile)     // there is no Output Check File
        {
            if ((itsOutputFile = new TCheckFile(itsXVTFilePointer, itsErrorHandler))==NULL)
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
        if (!itsCheckOutputSpec)
            itsCheckOutputSpec = new FILE_SPEC;
        memcpy(itsCheckOutputSpec,itsXVTFilePointer,sizeof(FILE_SPEC));
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
    if (!itsXVTFilePointer)
        itsXVTFilePointer = new FILE_SPEC;
    memset(itsXVTFilePointer,0,sizeof(FILE_SPEC));

    if (DefaultFile)
    {
        strcpy(itsXVTFilePointer->name,DefaultFile);
    }

    return Open(IO);
}

