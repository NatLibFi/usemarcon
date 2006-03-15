/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2006
 *
 *  File:  trule.cpp
 *
 *  implements a class to manage rules
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include "trule.h"
#include "error.h"
#include "tools.h"

///////////////////////////////////////////////////////////////////////////////
//
// TRule
//
///////////////////////////////////////////////////////////////////////////////
TRule::TRule(TCD *LastInputCD, TCD *LastOutputCD, TError *ErrorHandler)
{
    itsInputCD      = NULL;
    itsOutputCD     = NULL;
    itsNextRule     = NULL;
    itsPreviousRule = NULL;
    itsLastInputCD      = LastInputCD;
    itsLastOutputCD = LastOutputCD;
    itsErrorHandler = ErrorHandler;
}

///////////////////////////////////////////////////////////////////////////////
//
// TRule copy constructor
//
///////////////////////////////////////////////////////////////////////////////
TRule::TRule(TRule *aRule)
{
    itsInputCD          = new TCD(aRule->GetInputCD());
    itsOutputCD     = new TCD(aRule->GetOutputCD());
    SetLib(aRule->GetLib());
    itsNextRule     = aRule->GetNextRule();
    itsPreviousRule = aRule->GetPreviousRule();
    itsLastInputCD      = aRule->itsLastInputCD;
    itsLastOutputCD = aRule->itsLastOutputCD;
    itsErrorHandler = aRule->itsErrorHandler;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TRule
//
///////////////////////////////////////////////////////////////////////////////
TRule::~TRule()
{
    if (itsInputCD) { delete itsInputCD;    itsInputCD  = NULL; }
    if(itsOutputCD) { delete itsOutputCD;   itsOutputCD = NULL; }
}

///////////////////////////////////////////////////////////////////////////////
//
// SetLib
//
///////////////////////////////////////////////////////////////////////////////
int TRule::SetLib(const char *aLib)
{
    if (!aLib || !*aLib)
        return 0;

    typestr lib;
    lib.str(aLib);

    char *c = strchr(lib.str(), '\t');
    while (c)
    {
        *c = ' ';
        c = strchr(lib.str(), '\t');
    }

    if (itsLib.str())
    {
        itsLib.append_char('\n');
        itsLib.append(lib.str());
    }
    else
    {
        itsLib = lib;
    }
    if (!itsLib.str())
        return itsErrorHandler->SetError(5502,ERROR);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// FromString
//
///////////////////////////////////////////////////////////////////////////////
int TRule::FromString(char *aString)
{
    char    *ItemPointer;
    int     NbPipes,
            MaxPos,
            CurrentPos,
            RealPos=0;
    int     ReturnCode;

    if (!*aString)
        // empty lines are ignored
        return 0;

    MaxPos=strlen(aString);

    // Count the number of '|' characters
    for (CurrentPos=RealPos,NbPipes=0; CurrentPos<MaxPos; CurrentPos++) {
        if (aString[CurrentPos]=='|') {
            NbPipes++;
        }
    }

    if (NbPipes>2)
        NbPipes=2;
    switch(NbPipes)
    {
    case 2 : // the rule is a complete rule ( CDin | CDOut | Rule )
        if ((ItemPointer=(char *)strtok(aString,"|"))!=NULL)
            if (RemoveSpace(ItemPointer))
                // the CDIn is not empty
            {
                itsInputCD = new TCD(itsErrorHandler);
                if (!itsInputCD)
                    return -5504;
                if ((ReturnCode=itsInputCD->FromString(ItemPointer,itsLastInputCD,INPUT))!=0)
                    return -ReturnCode;
            }
    case 1 : // the rule has no CDIn or has an empty CDIn
        // set the CDIn with the CDIn of the previous Rule ( itsLastInputCD )
        if (!itsInputCD)
        {
            itsInputCD = new TCD(itsErrorHandler);
            if (!itsInputCD)
                return -5504;
            if ((ReturnCode=itsInputCD->FromString("",itsLastInputCD,INPUT))!=0)
                return -ReturnCode;
        }
        if (NbPipes==1)
            // the Rule is a partial rule ( CDOut | Rule )
            ItemPointer=(char *)strtok(aString,"|");
        else
            // the Rule is a complete Rule with an empty CDIn
            ItemPointer=(char *)strtok(NULL,"|");
        if (ItemPointer)
        {
            itsOutputCD = new TCD(itsErrorHandler);
            if (!itsOutputCD)
                return -5504;
            RemoveSpace(ItemPointer);
            if ((ReturnCode=itsOutputCD->FromString(ItemPointer,itsLastOutputCD,OUTPUT))!=0)
                return -ReturnCode;
        }
    case 0:
        if (NbPipes==0)
            // This line is a new part of the previous Rule
        {
            if (RemoveSpace(aString,AT_BEGINING|AT_END))
                if (SetLib(aString))
                    return -5502;
            return 0;
        }
        else
        {
            if ((ItemPointer=(char *)strtok(NULL,"~")))
                if (RemoveSpace(ItemPointer,AT_BEGINING|AT_END))
                    if (SetLib(ItemPointer))
                        return -5502;
        }
        break;
    default:
        break;
    }

    return NbPipes;
}

///////////////////////////////////////////////////////////////////////////////
//
// ToString
//
///////////////////////////////////////////////////////////////////////////////
#define START_RULE_AT   30
int TRule::ToString(char *aString)
{
    int CurrentPos=0;
    int iIndice;
    int iLng;
    char    szStart[3+START_RULE_AT+1];

    memset(szStart,' ',3+START_RULE_AT);
    szStart[3+START_RULE_AT]=0;

    *aString = '\0';

    if ((!itsInputCD) || (!itsOutputCD))
        return 1;

    if (itsInputCD->ToString(&aString[CurrentPos],INPUT))
        // Absence du Tag d'entrȨe
        return 1;
    CurrentPos = strlen(aString);

    strcat(aString, " | ");
    CurrentPos += 3;

    if (itsOutputCD->ToString(&aString[CurrentPos],OUTPUT))
        // Absence du Tag de sortie
        return 1;
    CurrentPos = strlen(aString);

    if (CurrentPos<START_RULE_AT)
    {
        for (iIndice=CurrentPos; iIndice<START_RULE_AT; iIndice++)
            aString[iIndice]=' ';
        CurrentPos=iIndice;
        aString[CurrentPos]=0;
    }

    strcat(aString, " | ");
    CurrentPos += 3;

    if (itsLib.str())
    {
        strcat(aString,itsLib.str());
        iLng=strlen(aString);
        for (iIndice=0; iIndice<iLng; iIndice++)
            if (aString[iIndice]=='\n')
                iIndice+=InsertChar(aString,&iLng,iIndice+1,szStart,APRES);
            CurrentPos = strlen(aString);
    }

    return 0;
}

