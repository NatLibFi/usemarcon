/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tcdlib.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TCDLib

    implements a class to manage record control data

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/
#ifndef _TCDLIB_H_
#define _TCDLIB_H_

#include "tcd.h"
#include "error.h"
#include "typedef.h"

class TCDLib : public TCD
{
public:
    TCDLib          (TError *ErrorHandler);
    TCDLib          (TCDLib *aCDLib,TCD *aCD=NULL);
    TCDLib          (TCD* aCD);
    ~TCDLib         (void);

    typestr   GetContent      (TCD* aCD=NULL);
    int       SetContent      (const char *aContent, TCD* aCD=NULL);
    int       SetContent      (const typestr & aContent, TCD* aCD=NULL);
    void      ResetContent    (void);
    int       IsEqual         (TCD* aCD, int aTagOccurrenceNumberOverride = -1);
    int       IsLess          (TCD* aCD);
    int       NextSubCDLib    (TCDLib** pCDLib, int*, char*);

    friend  int  operator ==  (TCDLib& aCDLib, TCD& aCD) { return aCDLib.IsEqual(&aCD); };
    friend  int  operator !=  (TCDLib& aCDLib, TCD& aCD) { return !aCDLib.IsEqual(&aCD); };
    friend  int  operator <   (TCDLib& aCDLib, TCD& aCD) { return aCDLib.IsLess(&aCD); };

protected:
    typestr itsContent;
};

#endif

