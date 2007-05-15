/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  sort.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    SortElem

    implements a class to manage sorting of elements

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/

#include "tcd.h"

class SortElem
{
public:
    SortElem(TCD* aCD, char* alist)
    {
        itsCD = new TCD(aCD->GetErrorHandler());
        itsCD->SetTag(aCD->GetTag());
        itsCD->SetTagOccurrenceNumber(aCD->GetTagOccurrenceNumber());
        itsLSubField=strdup(alist);
        itsNext=NULL;
    };
    ~SortElem()
    {
        free(itsLSubField);
        if (itsNext)
            delete itsNext;
        delete itsCD;
    };

    void         SetNext(SortElem* aElem) { itsNext=aElem; };
    SortElem*    GetNext()           { return itsNext; };
    TCD*         GetCD()             { return itsCD; };
    char*        GetSubFieldList()   { return itsLSubField; };

protected:
    SortElem*    itsNext;
    TCD          *itsCD;
    char*        itsLSubField;
};
