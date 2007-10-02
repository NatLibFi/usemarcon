/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  trule.cpp
 *
 *  implements a class to manage rules
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
    mLine = 0;
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
    mLine = aRule->mLine;
}

TRule::TRule(TRule *aRule, const char* aRuleStr)
{
    itsInputCD          = new TCD(aRule->GetInputCD());
    itsOutputCD     = new TCD(aRule->GetOutputCD());
    SetLib(aRuleStr);
    itsNextRule     = aRule->GetNextRule();
    itsPreviousRule = aRule->GetPreviousRule();
    itsLastInputCD      = aRule->itsLastInputCD;
    itsLastOutputCD = aRule->itsLastOutputCD;
    itsErrorHandler = aRule->itsErrorHandler;
    mLine = aRule->mLine;
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

    if (!itsLib.is_empty())
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
int TRule::FromString(char *aString, int aLine)
{
    int ReturnCode;

    if (!*aString)
        // empty lines are ignored
        return 0;

    mLine = aLine;

    char* firstPipe = NULL;
    char* secondPipe = NULL;

    char* p = aString;
    bool in_string = false;
    while (*p)
    {
        if (*p == '\'')
            in_string = !in_string;

        if (!in_string && *p == '|')
        {
            if (!firstPipe) 
                firstPipe = p;
            else
            {
                secondPipe = p;
                break;
            }
        }
        ++p;
    }

    if (secondPipe)
    {
        // Looks like a full rule, check if CDIn is specified
        p = aString;
        typestr cdin;
        cdin.str(p, firstPipe - p - 1);
        cdin.replace(" ", "");
        if (!cdin.is_empty())
        {
            // CDIn is specified
            itsInputCD = new TCD(itsErrorHandler);
            if (!itsInputCD)
                return -5504;
            if ((ReturnCode = itsInputCD->FromString(cdin.str(), itsLastInputCD, INPUT)) != 0)
                return -ReturnCode;
        }
    }
    if (firstPipe)
    {
        if (!itsInputCD)
        {
            // No input CD
            itsInputCD = new TCD(itsErrorHandler);
            if (!itsInputCD)
                return -5504;
            if ((ReturnCode = itsInputCD->FromString("", itsLastInputCD, INPUT)) != 0)
                return -ReturnCode;
        }
        char* p_end;
        if (secondPipe)
        {
            p = firstPipe + 1;
            p_end = secondPipe - 1;
        }
        else
        {
            p = aString;
            p_end = firstPipe - 1;
        }
        typestr cdout;
        cdout.str(p, p_end - p);
        cdout.replace(" ", "");

        itsOutputCD = new TCD(itsErrorHandler);
        if (!itsOutputCD)
            return -5504;
        if ((ReturnCode = itsOutputCD->FromString(cdout.str(), itsLastOutputCD, OUTPUT)) != 0)
            return -ReturnCode;
    }
    
    if (secondPipe)
        p = secondPipe + 1;
    else if (firstPipe)
        p = firstPipe + 1;
    else 
        p = aString;
    
    typestr tmp = p;
    typestr rule;
    trim_string(rule, tmp);

    if (!rule.is_empty() && SetLib(rule.str()))
        return -5502;

    return secondPipe ? 2 : firstPipe ? 1 : 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// ToString
//
///////////////////////////////////////////////////////////////////////////////
bool TRule::ToString(typestr & a_string)
{
    a_string = "";

    if ((!itsInputCD) || (!itsOutputCD))
        return false;

    typestr tmp;
    if (!itsInputCD->ToString(tmp, INPUT))
        // Absence du Tag d'entrȨe
        return false;
    a_string += tmp;
    a_string += " | ";
    
    if (!itsOutputCD->ToString(tmp, OUTPUT))
        // Absence du Tag de sortie
        return false;
    a_string += tmp;
    while(strlen(a_string.str()) < 30)
        a_string.append_char(' ');

    a_string += " | ";

    if (itsLib.str())
    {
        a_string += itsLib;
        a_string.replace("\n", "                             ");
    }
    return true;
}

