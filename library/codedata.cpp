/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2008
 *
 *  File:  codedata.cpp
 *
 *  implements a class to manage Code data file
 *
 */

#include <memory.h>
#include "statemanager.h"
#include "codedata.h"

///////////////////////////////////////////////////////////////////////////////
//
// TCodedData
//
///////////////////////////////////////////////////////////////////////////////
TCodedData::TCodedData(typestr & FileSpec, TStateManager *StateManager)
: TTransFile(FileSpec, StateManager)
{
    itsCodedDataName = FileSpec;
    itsNextCodedData = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TCodedData
//
///////////////////////////////////////////////////////////////////////////////
TCodedData::~TCodedData()
{
}
