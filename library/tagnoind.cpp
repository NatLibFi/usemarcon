/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  tagnoind.cpp
 *
 *  implements a class to manage MARC tags that do not have indicators
 *
 */

#include "tagnoind.h"

///////////////////////////////////////////////////////////////////////////////
//
// TTagNoInd
//
///////////////////////////////////////////////////////////////////////////////
TTagNoInd::TTagNoInd(void)
{
    *itsTag     = 0;
    itsNext     = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TTagNoInd
//
///////////////////////////////////////////////////////////////////////////////
TTagNoInd::~TTagNoInd()
{
}

