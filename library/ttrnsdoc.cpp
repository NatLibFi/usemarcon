/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *
 *  File:  ttrnsdoc.cpp
 *
 *  implements a class to manage file transcoding files
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include "error.h"
#include "defines.h"
#include "ttrnsdoc.h"
#include "tmpplctn.h"

///////////////////////////////////////////////////////////////////////////////
//
// Document class constructor
//
// Create and initialize the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TTransDoc::TTransDoc(TError *ErrorHandler)
{
    itsFile         = NULL;
    itsTransSpec    = NULL;
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
TTransDoc::~TTransDoc( void )
{
    if (itsFile)        
    { 
        delete itsFile;       
        itsFile = NULL; 
    }

    // John Hough - added delete
    if (itsTransSpec)
        delete itsTransSpec;
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
// Called by DoOpen().  Do the actual work of restoring the object from
// a data source.
//
///////////////////////////////////////////////////////////////////////////////
bool TTransDoc::Open(void)
{
    // Add code here to set internal data members of the class based
    // on information read from some data source
    if ((itsFile=new TTransFile(itsXVTFilePointer, itsErrorHandler))==NULL)
    {
        itsErrorHandler->SetError(9013,ERROR);
        return false;
    }
    if (itsFile->Open())
    {
        delete itsFile;
        itsFile = NULL;
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// OpenTransFile
//
///////////////////////////////////////////////////////////////////////////////
bool TTransDoc::OpenTransFile(char *DefaultFile)
{
    if (!itsXVTFilePointer)
        itsXVTFilePointer = new FILE_SPEC;
    memset(itsXVTFilePointer,0,sizeof(FILE_SPEC));
    
    if (DefaultFile)
    {
        strcpy(itsXVTFilePointer->name,DefaultFile);
    }
    
    if (Open()) // Open the selected Rule file
    {
        itsTransSpec = itsXVTFilePointer;
        return true;
    }
    return false;
}
