/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  codedata.h
 *
 *
 
CLASS
    TCodedData

    implements a class to manage coded data

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2008

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/
#ifndef _TCODED_DATA_H_
#define _TCODED_DATA_H_

#include "trnsfile.h"

class TCodedData : public TTransFile
{
public:
    TCodedData          (typestr & FileSpec, TError *ErrorHandler);
    ~TCodedData         (void);
    
    virtual typestr       GetCodedDataName   (void) { return itsCodedDataName; };
    virtual TCodedData    *GetNextCodedData   (void) { return itsNextCodedData; };
    
    virtual void          SetNextCodedData    (TCodedData *theData) { itsNextCodedData=theData; };
    
protected:
    typestr               itsCodedDataName;
    TCodedData            *itsNextCodedData;
};
#endif

