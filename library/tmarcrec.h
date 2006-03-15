/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tmarcrec.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TMarcRecord

    implements a class to manage MARC record

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2006

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    Helsinki University Library, the National Library of Finland

*/

#ifndef _TMARCREC_H_
#define _TMARCREC_H_

#include "mrcfield.h"
#include "tagnoind.h"
#include "error.h"

// forward declaration
class CDetails;

class TMarcRecord
{
public:
    TMarcRecord         (TUMApplication *Application);
    TMarcRecord         (TMarcRecord*);
    virtual ~TMarcRecord        ();

    virtual int           NextField           (TMarcField** Field, char *Tag);
    virtual int           FromString          (char *MarcString);
    virtual int           ToString            (char* marc,long max_size);
    virtual void          DelTree             (void);
    virtual char          *GetLabel           (void)              { return itsLabel; };
    virtual int           SetLabel            (const char *aLabel);
    TMarcField            *GetFirstField      (void)              { return itsFirstField; };
    virtual void          SetFirstField       (TMarcField *Field) { itsFirstField=Field; };
    TTagNoInd             *GetFirstInputTNI   (void)              { return itsFirstInputTNI; };
    TTagNoInd             *GetFirstOutputTNI  (void)              { return itsFirstOutputTNI; };
    virtual void          SetFirstInputTNI    (TTagNoInd *aTNI)   { itsFirstInputTNI=aTNI; };
    virtual void          SetFirstOutputTNI   (TTagNoInd *aTNI)   { itsFirstOutputTNI=aTNI; };
    virtual void          SetIndSeparatorsID  (int IO,long ID);
    virtual long          GetIndSeparatorsID  (int IO);

private:
    int                   Val                 (char *buffer,unsigned long *valeur);
    int                   LongVal             (char *buffer,unsigned long *valeur);
    bool                  IsFieldWithIndicators(int IO, char *tag, char *field, unsigned long fieldlen);
    bool                  IsItAFieldWithIndicators(char *tag, int IO);

protected:
    char                  itsLabel[25];
    TMarcField            *itsFirstField;
    TTagNoInd             *itsFirstInputTNI;
    TTagNoInd             *itsFirstOutputTNI;
    long                  itsInputIndSeparatorsID;
    long                  itsOutputIndSeparatorsID;

    TError                *itsErrorHandler;

    CDetails              *itsDetails;
};

#endif
