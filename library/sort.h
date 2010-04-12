/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  sort.h
 *
 *
 
CLASS
    SortElem

    implements a class to manage sorting of elements

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

#include "tcd.h"

class SortElem
{
public:
    SortElem(TCD* aCD, const char* alist)
    {
        itsCD = new TCD(aCD->GetStateManager());
        itsCD->SetTag(aCD->GetTag());
        itsCD->SetTagOccurrenceNumber(aCD->GetTagOccurrenceNumber());
        itsLSubField=l_strdup(alist);
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
    const char*  GetSubFieldList()   { return itsLSubField; };

protected:
    SortElem*    itsNext;
    TCD          *itsCD;
    char*        itsLSubField;
};
