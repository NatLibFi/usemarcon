/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2006
 *
 *  File:  tcd.cpp
 *
 *  implements a class to manage MARC record control data
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include "tcd.h"
#include "error.h"
#include "tools.h"


///////////////////////////////////////////////////////////////////////////////
//
// TCD
//
///////////////////////////////////////////////////////////////////////////////
TCD::TCD(TError *ErrorHandler)
{
    _IN                     = 0;
    *itsTag             = 0;
    *itsSubfield            = 0;
    itsOccurenceNumber      = 0;
    itsTagOccurenceNumber   = 0;
    itsSubOccurenceNumber   = 0;
    itsPrevious         = NULL;
    itsNext             = NULL;
    itsBegining         = 0;
    itsEnd                  = 0;
    itsErrorHandler     = ErrorHandler;
}

///////////////////////////////////////////////////////////////////////////////
//
// TCD : Copie constructeur a partir d'un TypeCD
//
///////////////////////////////////////////////////////////////////////////////
TCD::TCD(TypeCD* atcd, TError *ErrorHandler)
{
    strcpy(itsTag,atcd->Field);
    strcpy(itsSubfield,atcd->SubField);
    itsOccurenceNumber  = 0;
    itsTagOccurenceNumber   = atcd->nt;
    itsSubOccurenceNumber   = atcd->ns;
    itsPrevious = NULL;
    itsNext     = NULL;
    itsErrorHandler = ErrorHandler;
    SetFixedPos(atcd->Fixed.str());
}


///////////////////////////////////////////////////////////////////////////////
//
// TCD : copie constructeur recursif
//
///////////////////////////////////////////////////////////////////////////////
TCD::TCD(TCD *aCD)
{
    _IN=aCD->GetIN();
    strcpy(itsTag,aCD->GetTag());
    strcpy(itsSubfield,aCD->GetSubfield());
    itsOccurenceNumber=aCD->GetOccurenceNumber();
    itsTagOccurenceNumber=aCD->GetTagOccurenceNumber();
    itsSubOccurenceNumber=aCD->GetSubOccurenceNumber();
    itsBegining=aCD->GetBegining();
    itsEnd=aCD->GetEnd();
    if (aCD->GetNext())
    {
        itsNext = (TCD *)new TCD(aCD->GetNext());
        itsNext->SetPrevious(this);
    }
    else
        itsNext = NULL;
    itsPrevious = NULL;
    itsErrorHandler = aCD->itsErrorHandler;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TCD
//
///////////////////////////////////////////////////////////////////////////////
TCD::~TCD()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// SetFixedPos
//
///////////////////////////////////////////////////////////////////////////////
int TCD::SetFixedPos(char *String)
{
    int     MaxPos,
        CurrentPos,
        RealPos=0;
    char        *Pointer;

    if (!String)
    {
        SetBegining(0);
        SetEnd(0);
        return 0;
    }

    // Remove all spaces
    if ((MaxPos=RemoveSpace(String))==0)
    {
        SetBegining(0);
        SetEnd(0);
        return 0;
    }

    // Test if the String is a Fixed position and returns if not
    if (*String!='/')
        return 0;

    RealPos++;                  // skip the '/' character
    for (CurrentPos=RealPos; CurrentPos<MaxPos && String[CurrentPos]!='/'; )
        CurrentPos++;             // CurrentPos points to the last '/' or the end of string
    if (CurrentPos-RealPos)
        // The Fixed position is not empty
    {
        memcpy(itsErrorHandler->Temporary2,&String[RealPos],CurrentPos-RealPos);
        itsErrorHandler->Temporary2[CurrentPos-RealPos]=0;
        Pointer=(char *)strchr((char *)itsErrorHandler->Temporary2,'-');
        if (Pointer!=NULL)
        {
            if (Pointer==(char *)itsErrorHandler->Temporary2)
            {
                if (*(Pointer+1)!=0)
                {
                    SetBegining(1);
                    SetEnd(atoi(++Pointer));
                }
            }
            else
                if (*(Pointer+1)==0)
                {
                    SetEnd(-1);
                    *Pointer=0;
                    SetBegining(atoi((char *)itsErrorHandler->Temporary2));
                }
                else
                {
                    SetEnd(atoi(++Pointer));
                    *Pointer=0;
                    SetBegining(atoi((char *)itsErrorHandler->Temporary2));
                }
        }
        else
        {
            SetBegining(atoi((char *)itsErrorHandler->Temporary2));
            SetEnd(itsBegining);
        }
        return 0;
    }
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
// FromString
//
///////////////////////////////////////////////////////////////////////////////
int TCD::FromString(char *aString, TCD *Last, int InputOrOutput)
{
    int MaxPos,
        CurrentPos,
        IsOccTagPresent=0,
        IsOccSubPresent=0,
        RealPos=0;
    char *String = aString;

    // All spaces are removed
    MaxPos=RemoveSpace(String);

    // Search for the "Send to Input" character for the output CD
    SetIN(0);
    if (InputOrOutput==OUTPUT)
    {
        if (*String=='<')
        {
            SetIN(1);
            String++;
            MaxPos--;
        }
    }

    // Try to read a Tag
    for (CurrentPos=RealPos; CurrentPos<MaxPos && CurrentPos<3 && (isalnum(String[CurrentPos]) || String[CurrentPos]=='?'); )
        CurrentPos++;
    if (CurrentPos-RealPos==3)
        // This is the Tag
    {
        memcpy(itsErrorHandler->Temporary2,&String[RealPos],CurrentPos-RealPos);
        itsErrorHandler->Temporary2[CurrentPos-RealPos]=0;
        SetTag((char *)itsErrorHandler->Temporary2);
        RealPos=CurrentPos;
        // Search for the Occurence Tag
        if (String[CurrentPos]=='(')
            // This is the Occurence Tag
        {
            // Try to read the occurence Tag
            RealPos++;
            for (CurrentPos=RealPos; CurrentPos<MaxPos && String[CurrentPos]!=')'; )
                CurrentPos++;
            if (CurrentPos-RealPos)
                // the occurence Tag is not empty
            {
                IsOccTagPresent=1;
                memcpy(itsErrorHandler->Temporary2,&String[RealPos],CurrentPos-RealPos);
                itsErrorHandler->Temporary2[CurrentPos-RealPos]=0;
                ToUpperCase((char *)itsErrorHandler->Temporary2);
                if (!strcmp((char *)itsErrorHandler->Temporary2,"N"))
                    SetTagOccurenceNumber(CD_N);
                else if (!strcmp((char *)itsErrorHandler->Temporary2,"NT"))
                    SetTagOccurenceNumber(CD_NT);
                else if (!strcmp((char *)itsErrorHandler->Temporary2,"NS"))
                    SetTagOccurenceNumber(CD_NS);
                else if (!strcmp((char *)itsErrorHandler->Temporary2,"NTO"))
                    SetTagOccurenceNumber(CD_NTO);
                else if (!strcmp((char *)itsErrorHandler->Temporary2,"NO"))
                    SetTagOccurenceNumber(CD_NO);
                else 
                    SetTagOccurenceNumber(atoi((char *)itsErrorHandler->Temporary2));
            }
            else
                // the occurence Tag is empty
                SetTagOccurenceNumber(CD_NT);
            CurrentPos++;
            RealPos=CurrentPos;
        }
        else
            // there is no occurence Tag
            SetTagOccurenceNumber(CD_NT);
    }
    else
    {
        // There is no Tag ==> set the current Tag with the last tag
        if (Last)
        {
            IsOccTagPresent=1;
            SetTag(Last->GetTag());
            SetSubfield(Last->GetSubfield());
            SetTagOccurenceNumber(Last->GetTagOccurenceNumber());
        }
        else
            return 5200;
    }

    switch(String[RealPos])
    {
    case '$' :
    case 'I' :
        // This is SS
        // Try to read SS
        for (CurrentPos=RealPos+1; CurrentPos<MaxPos && String[CurrentPos]!='(' && String[CurrentPos]!='/'; )
            CurrentPos++;
        if (CurrentPos-RealPos==2)
            // this is a SS
        {
            memcpy(itsErrorHandler->Temporary2,&String[RealPos],CurrentPos-RealPos);
            itsErrorHandler->Temporary2[CurrentPos-RealPos]=0;
            SetSubfield((char *)itsErrorHandler->Temporary2);
        }
        else
            // Invalid SS
            return 5203;
        RealPos=CurrentPos;
        // Search for the Occurence Sub
        if (String[RealPos]=='(')
            // There is an ocuurence Sub
        {
            RealPos++;
            // Try to read the occurence Sub
            for (CurrentPos=RealPos; CurrentPos<MaxPos && String[CurrentPos]!=')'; )
                CurrentPos++;
            if (CurrentPos-RealPos)
                // the occurence Sub is no empty
            {
                IsOccSubPresent=1;
                memcpy(itsErrorHandler->Temporary2,&String[RealPos],CurrentPos-RealPos);
                itsErrorHandler->Temporary2[CurrentPos-RealPos]=0;
                ToUpperCase((char *)itsErrorHandler->Temporary2);
                if (!strcmp((char *)itsErrorHandler->Temporary2,"N"))
                    SetSubOccurenceNumber(CD_N);
                else if (!strcmp((char *)itsErrorHandler->Temporary2,"NT"))
                    SetSubOccurenceNumber(CD_NT);
                else if (!strcmp((char *)itsErrorHandler->Temporary2,"NS"))
                    SetSubOccurenceNumber(CD_NS);
                else if (!strcmp((char *)itsErrorHandler->Temporary2,"NSO"))
                    SetSubOccurenceNumber(CD_NSO);
                else if (!strcmp((char *)itsErrorHandler->Temporary2,"NO"))
                    SetSubOccurenceNumber(CD_NO);
                else
                    SetTagOccurenceNumber(atoi((char *)itsErrorHandler->Temporary2));
            }
            else
                // the occurence Sub is empty
                SetSubOccurenceNumber(CD_NS);
            CurrentPos++;
            RealPos=CurrentPos;
        }

    case '/' :
        if (String[RealPos]!='/') break;
        SetFixedPos(&String[RealPos]);
        break;

    case 0: // empty string
        break;

    default: // invalid character
        return 5212;
        break;
    }

    if (InputOrOutput==INPUT)
    {
        // No occurence N, NT etc. ( < 0) is allowed in Input CDs
        if (GetTagOccurenceNumber() < 0)
            SetTagOccurenceNumber(0);
        if (GetSubOccurenceNumber() < 0)
            SetSubOccurenceNumber(0);
        SetOccurenceNumber(0);
    }
    else
    {
        if (!IsOccTagPresent && !IsOccSubPresent)
            // Tag Ocurence and Sub Occurence are respectively set to nt and ns if there is no defined occurence
        {
            SetTagOccurenceNumber(CD_NT);
            SetSubOccurenceNumber(CD_NS);
        }
        else
        {
            if (IsOccTagPresent  && !IsOccSubPresent)
                SetSubOccurenceNumber(1);
            if (!IsOccTagPresent  && IsOccSubPresent)
                SetTagOccurenceNumber(1);
        }
    }
    if (!*itsSubfield)
        SetSubOccurenceNumber(0);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// ToString
//
///////////////////////////////////////////////////////////////////////////////
int TCD::ToString(char *String, int InputOrOutput)
{
    int CurrentPos=0;
    int Occurence;

    bool        bIsOccTagNT=false;
    bool        bIsOccSubNS=false;
    char            szOccTag[10];
    char            szOccSub[10];

    *szOccTag=*szOccSub='\0';

    // Le Tag est obligatoire
    if (!(*GetTag()))
        return 5200;

    // TTT[(n1)]SS[(n2)][/p1-p2/]

    // Ecriture du caractere "Send to Input CD" si n‰cessaire devant le Tag de sortie
    if (InputOrOutput==OUTPUT)
    {
        if (GetIN())
            strcpy(&String[CurrentPos++],"<");
    }

    // TTT
    strcpy(&String[CurrentPos],GetTag());
    CurrentPos+=strlen(GetTag());

    // [(n1)]
    if (InputOrOutput==OUTPUT)
    {
        Occurence=GetTagOccurenceNumber();
        switch(Occurence)
        {
        case CD_N  : strcpy(szOccTag,"(n)");                break;
        case CD_NS : strcpy(szOccTag,"(ns)");               break;
        case CD_NTO: strcpy(szOccTag,"(nto)");              break;
        case CD_NO : strcpy(szOccTag,"(no)");               break;
        case CD_NT : bIsOccTagNT=true;
        case 1      :
        case 0     : strcpy(szOccTag,"");                   break;
        default : sprintf(szOccTag,"(%d)",Occurence);   break;
        }
    }

    strcpy((char *)itsErrorHandler->Temporary2,GetSubfield());
    if (strlen((char *)itsErrorHandler->Temporary2) && memcpy((char *)itsErrorHandler->Temporary2,"I",1))
    {
        if (InputOrOutput==INPUT)
        {
            sprintf(&String[CurrentPos],"%-3s",GetSubfield());
            CurrentPos=strlen(String);
        }
        else
        {
            // [(n2)]
            Occurence=GetSubOccurenceNumber();
            switch(Occurence)
            {
            case CD_N  : strcpy(szOccSub,"(n)");            break;
            case CD_NT : strcpy(szOccSub,"(nt)");           break;
            case CD_NSO: strcpy(szOccSub,"(nso)");          break;
            case CD_NO : strcpy(szOccSub,"(no)");           break;
            case CD_NS : bIsOccSubNS=true;
            case 1      :
            case 0  : strcpy(szOccSub,"");                  break;
            default : sprintf(szOccSub,"(%d)",Occurence);   break;
            }

            if ((bIsOccTagNT==true) && (bIsOccSubNS==true))
                // OccTag et OccSub sont les valeurs par d‰faut
                // ==> pas d'affichage de ces occurences
            {
                sprintf(&String[CurrentPos],"%-3s",GetSubfield());
                CurrentPos=strlen(String);
            }
            else
            {
                if ((bIsOccTagNT==true) && (bIsOccSubNS==false))
                    // OccTag est la valeur par d‰faut mais pas OccSub
                    // ==> Affichage de OccTag avec '(nt)' et de OccSub ( sauf si OccSub est € 1 )
                {
                    strcpy(&String[CurrentPos],"(nt)");
                    CurrentPos=strlen(String);
                    sprintf(&String[CurrentPos],"%-3s",GetSubfield());
                    CurrentPos=strlen(String);
                    if (*szOccSub!='1')
                        strcpy(&String[CurrentPos],szOccSub);
                    CurrentPos=strlen(String);
                }
                else
                {
                    if ((bIsOccTagNT==false) && (bIsOccSubNS==true))
                        // OccSub est la valeur par d‰faut mais pas OccTag
                        // ==> Affichage de OccTag ( sauf si OccTag est € 1 ) et de OccSub avec '(ns)'
                    {
                        if (*szOccTag!='1')
                            strcpy(&String[CurrentPos],szOccTag);
                        CurrentPos=strlen(String);
                        sprintf(&String[CurrentPos],"%-3s",GetSubfield());
                        CurrentPos=strlen(String);
                        strcpy(&String[CurrentPos],"(ns)");
                        CurrentPos=strlen(String);
                    }
                    else
                        // OccTag et OccSub ne sont pas les valeurs par d‰faut
                        // ==> Affichage de OccTag et de OccSub
                    {
                        strcpy(&String[CurrentPos],szOccTag);
                        CurrentPos=strlen(String);
                        sprintf(&String[CurrentPos],"%-3s",GetSubfield());
                        CurrentPos=strlen(String);
                        strcpy(&String[CurrentPos],szOccSub);
                        CurrentPos=strlen(String);
                    }
                }
            }
        }
    }

    if (itsBegining)
    {
        if (itsBegining!=itsEnd)
            sprintf(&String[CurrentPos],"/%d-%d/",itsBegining,itsEnd);
        else
            sprintf(&String[CurrentPos],"/%d/",itsBegining);
    }

    return 0;
}

void TCD::ReplaceWildcards(const char *field, const char *subfield)
{
    for (int i = 0; i < 4; i++)
    {
        if (itsTag[i] == '?')
            itsTag[i] = field[i];
    }
    for (int i = 0; i < 2; i++)
    {
        if (itsSubfield[i] == '?')
            itsSubfield[i] = subfield[i];
    }
}
