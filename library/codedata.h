/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  codedata.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TCodedData

    implements a class to manage coded data

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/
#ifndef _TCODED_DATA_H_
#define _TCODED_DATA_H_

#include "trnsfile.h"

class TCodedData : public TTransFile
{
public:
    TCodedData          (FILE_SPEC *FileSpec, TError *ErrorHandler);
    ~TCodedData         (void);
    
    virtual FILE_SPEC     *GetCodedDataName   (void) { return itsCodedDataName; };
    virtual TCodedData    *GetNextCodedData   (void) { return itsNextCodedData; };
    
    virtual void          SetNextCodedData    (TCodedData *theData) { itsNextCodedData=theData; };
    
protected:
    FILE_SPEC             *itsCodedDataName;
    TCodedData            *itsNextCodedData;
};
#endif

