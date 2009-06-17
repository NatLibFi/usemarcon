/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tcdlib.h
 *
 *
 
CLASS
    TCDLib

    implements a class to manage record control data

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2009

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/
#ifndef _TCDLIB_H_
#define _TCDLIB_H_

#include "tcd.h"
#include "statemanager.h"
#include "typedef.h"

class TCDLib : public TCD
{
public:
    TCDLib          (TStateManager *StateManager);
    TCDLib          (TCDLib *aCDLib,TCD *aCD=NULL);
    TCDLib          (TCD* aCD);
    ~TCDLib         (void);

    typestr2  GetContent      (TCD* aCD=NULL);
    int       SetContent      (const char *aContent, const char *aContent2, const char *aScript, TCD* aCD=NULL);
    int       SetContent      (const typestr2 & aContent, TCD* aCD=NULL);
    void      ResetContent    (void);
    int       IsEqual         (TCD* aCD, int aTagOccurrenceNumberOverride = -1);
    int       IsLess          (TCD* aCD);
    int       NextSubCDLib    (TCDLib** pCDLib, int*, int*, char*);

    friend  int  operator ==  (TCDLib& aCDLib, TCD& aCD) { return aCDLib.IsEqual(&aCD); };
    friend  int  operator !=  (TCDLib& aCDLib, TCD& aCD) { return !aCDLib.IsEqual(&aCD); };
    friend  int  operator <   (TCDLib& aCDLib, TCD& aCD) { return aCDLib.IsLess(&aCD); };

protected:
    typestr2 itsContent;
};

#endif

