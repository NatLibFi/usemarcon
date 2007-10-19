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
    char       *GetLib1      (void)            { return itsLib.str(); };
    char       *GetLib2      (void)            { return itsLib.s2.str(); };
    TMarcField *GetNextField (void)            { return itsNextField; };

    int     SetTag        (const char *aTagString);
    int     SetTag        (int aTagNumber);
    int     SetIndicators (const char *theIndicators);
    int     SetI1         (char anIndicator)      { itsIndicators[0]=anIndicator; return 0; };
    int     SetI2         (char anIndicator)      { itsIndicators[1]=anIndicator; return 0; };  
    void    SetLib        (const typestr2& a_lib)  { itsLib = a_lib; }
    void    SetLib1       (const char *aLib);
    void    SetLib1       (const char *aLib, unsigned int aSize);
    void    SetLib2       (const char *aLib);
    void    SetLib2       (const char *aLib, unsigned int aSize);
    TMarcField  *SetNextField   (TMarcField *theNextField)  { itsNextField=theNextField; return itsNextField; };
       
protected:
    char      itsTag[4];
    char      itsIndicators[3];
    typestr2  itsLib;
    TMarcField  *itsNextField;
};
 
#endif
