/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *
 *  File:  ctrlfld.cpp
 *
 *  implements a class to manage MARC control fields
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include <ctype.h>
#include <stdlib.h>

#include "ctrlfld.h"
#include "error.h"

///////////////////////////////////////////////////////////////////////////////
//
// TCtrlSubfield
//
///////////////////////////////////////////////////////////////////////////////
TCtrlSubfield::TCtrlSubfield(TError *itsErrorHandler)
{
    itsSub              = '?';
    itsSubMandatory = false;
    itsSubRepeatable    = false;
    itsSubOccurency = 0;
    itsNextSub          = NULL;
    itsErrorHandler = itsErrorHandler;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TCtrlSubfield
//
///////////////////////////////////////////////////////////////////////////////
TCtrlSubfield::~TCtrlSubfield(void)
{

}

///////////////////////////////////////////////////////////////////////////////
//
// TControlField
//
///////////////////////////////////////////////////////////////////////////////
TControlField::TControlField(TError *ErrorHandler)
{
    *itsTag         = 0;
    itsTagMandatory = false;
    itsTagRepeatable    = false;
    itsTagOccurency = 0;
    itsFirstSubfield    = NULL;
    itsLastSubfield = NULL;
    itsNextTag          = NULL;
    itsErrorHandler    = ErrorHandler;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TControlField
//
///////////////////////////////////////////////////////////////////////////////
TControlField::~TControlField(void)
{
    DelTreeCheckSub();
}

///////////////////////////////////////////////////////////////////////////////
//
// SetFirstIndicators
//
///////////////////////////////////////////////////////////////////////////////
int TControlField::SetFirstIndicators(char *Indicators)
{
    itsFirstIndicators.freestr();
    if (*Indicators)
        if (itsFirstIndicators.str(Indicators)==NULL)
            return itsErrorHandler->SetError(2503,ERROR);
        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetSecondIndicators
//
///////////////////////////////////////////////////////////////////////////////
int TControlField::SetSecondIndicators(char *Indicators)
{
    itsSecondIndicators.freestr();
    if (*Indicators)
        if (itsSecondIndicators.str(Indicators)==NULL)
            return itsErrorHandler->SetError(2504,ERROR);
        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// DelTreeCheckSub
//
///////////////////////////////////////////////////////////////////////////////
void TControlField::DelTreeCheckSub(void)
{
    TCtrlSubfield   *Courant;
    TCtrlSubfield   *Suivant;

    Courant = itsFirstSubfield;
    while (Courant)
    {
        Suivant = Courant->GetNextSub();
        delete Courant;
        Courant = Suivant;
    }
    itsFirstSubfield = itsLastSubfield = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// TControlField
//
///////////////////////////////////////////////////////////////////////////////
void TControlField::ResetSubOccurency(TCtrlSubfield *StartSub)
{
    TCtrlSubfield   *Current;

    Current=StartSub;
    while (Current)
    {
        Current->SetSubOccurency(0);
        Current=Current->GetNextSub();
    }
}




