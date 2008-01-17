/* 
 *  USEMARCON Software - Command Line version
 *  
 *  File:  tagnoind.h
 *
 *
 
CLASS
    TTagNoInd

  implements a class to manage tagged fields without indicators

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

#ifndef _TagNoInd_H_
#define _TagNoInd_H_ 
                 
#include <string.h>                 

class TTagNoInd
{
public:
    TTagNoInd (void);
    virtual ~TTagNoInd ();

    virtual TTagNoInd *GetNext  (void)          { return itsNext; };  
    virtual char      *GetTag   (void)          { return itsTag; };

    virtual int   SetNext   (TTagNoInd *aTagNoInd)  { itsNext=aTagNoInd; return 0; };
    virtual int   SetTag    (const char *aString) { memcpy(itsTag,aString,3); itsTag[3]=0; return 0; };                                                      
                                                       
protected:
    char        itsTag[4];
    TTagNoInd   *itsNext;                                                          
};

#endif 

