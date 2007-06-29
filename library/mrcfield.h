/* 
 *  USEMARCON Software - Command Line version
 *  
 *  File:  mrcfield.h
 *
 *
 
CLASS
    TMarcField

    implements a class to manage MARC fields

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2007

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _TMARCFIELD_H_
#define _TMARCFIELD_H_ 

#include <stdio.h>            
#include <string.h>
#include "typedef.h"
                                 
class TMarcField
{
public:
    TMarcField      (void);
    TMarcField      (TMarcField*);
    ~TMarcField     (void);
    
    int     NextSubField    (int *_Position, char* _SubField);
  
    char       *GetTag       (void)            { return itsTag; };
    char       *GetIndicators(void)            { return itsIndicators; };
    char       GetI1         (void)            { return itsIndicators[0]; };
    char       GetI2         (void)            { return itsIndicators[1]; };
    char       *GetLib       (void)            { return itsLib.str(); };
    TMarcField *GetNextField (void)            { return itsNextField; };

    int     SetTag        (const char *aTagString);
    int     SetTag        (int aTagNumber);
    int     SetIndicators (const char *theIndicators);
    int     SetI1         (char anIndicator)      { itsIndicators[0]=anIndicator; return 0; };
    int     SetI2         (char anIndicator)      { itsIndicators[1]=anIndicator; return 0; };  
    int     SetLib        (const char *aLib);
    int     SetLib        (const char *aLib, unsigned int aSize);
    TMarcField  *SetNextField   (TMarcField *theNextField)  { itsNextField=theNextField; return itsNextField; };
       
protected:
    char      itsTag[4];
    char      itsIndicators[3];
    typestr   itsLib;
    TMarcField  *itsNextField;
};
 
#endif
