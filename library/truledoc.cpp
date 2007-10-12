/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  truledoc.cpp
 *
 *  implements a base class to manage file rule files
 *
 */

#include "error.h"
#include "defines.h"
#include "truledoc.h"
#include "tmpplctn.h"
#include "rulefile.h"

///////////////////////////////////////////////////////////////////////////////
//
// Document class constructor
//
// Create and initialize the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TRuleDoc::TRuleDoc(TUMApplication *Application)
{
    itsFile         = NULL;
    itsRuleSpec     = NULL;
    itsApplication  = Application;
    itsErrorHandler = Application->GetErrorHandler();
}

///////////////////////////////////////////////////////////////////////////////
//
// Document class destructor
//
// Delete the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TRuleDoc::~TRuleDoc( void )
{
    if (itsFile)            { delete itsFile;           itsFile             = NULL; }
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
// Called by DoOpen().  Do the actual work of restoring the object from
// a data source.
//
///////////////////////////////////////////////////////////////////////////////
bool TRuleDoc::Open(void)
{
    // Add code here to set internal data members of the class based
    // on information read from some data source
    if ((itsFile=new TRuleFile(itsFilePointer, itsApplication))==NULL)
    {
        itsErrorHandler->SetError(9001,ERROR);
        return false;
    }

    if (itsFile->OpenRuleFile())
    {
        // Rule file loading failure
        delete itsFile;
        itsFile = NULL;
        return false;
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// OpenRuleFile
//
///////////////////////////////////////////////////////////////////////////////
bool TRuleDoc::OpenRuleFile(char *DefaultFile)
{
    itsFilePointer = "";
    if (DefaultFile)
    {
        itsFilePointer = DefaultFile;
    }

    if (Open()) // Open the selected Rule file
    {
        itsRuleSpec = itsFilePointer;
        return true;
    }
    return false;
}


