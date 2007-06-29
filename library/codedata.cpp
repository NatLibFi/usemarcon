/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  codedata.cpp
 *
 *  implements a class to manage Code data file
 *
 */

#include <memory.h>
#include "trnsfile.h"
#include "error.h"
#include "codedata.h"

///////////////////////////////////////////////////////////////////////////////
//
// TCodedData
//
///////////////////////////////////////////////////////////////////////////////
TCodedData::TCodedData( FILE_SPEC *FileSpec, TError *ErrorHandler)
:TTransFile( FileSpec, ErrorHandler)
{
    itsCodedDataName=new FILE_SPEC;
    if (itsCodedDataName)
        memcpy(itsCodedDataName,FileSpec,sizeof(FILE_SPEC));
    itsNextCodedData = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TCodedData
//
///////////////////////////////////////////////////////////////////////////////
TCodedData::~TCodedData()
{
    if (itsCodedDataName) { delete itsCodedDataName; itsCodedDataName=NULL; }
}








