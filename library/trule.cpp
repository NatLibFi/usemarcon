/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2008
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
TRule::TRule(TError *ErrorHandler)
{
    mInputCD      = NULL;
    mOutputCD     = NULL;
    mNextRule     = NULL;
    mPreviousRule = NULL;
    mErrorHandler = ErrorHandler;
    mLine = 0;
    mConditionGroup = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// TRule copy constructor
//
///////////////////////////////////////////////////////////////////////////////
TRule::TRule(TRule *aRule)
{
    mInputCD          = new TCD(aRule->GetInputCD());
    mOutputCD     = new TCD(aRule->GetOutputCD());
    SetLib(aRule->GetLib());
    mNextRule     = aRule->GetNextRule();
    mPreviousRule = aRule->GetPreviousRule();
    mErrorHandler = aRule->mErrorHandler;
    mLine = aRule->mLine;
    mCondition = aRule->mCondition;
    mConditionGroup = aRule->mConditionGroup;
}

TRule::TRule(TRule *aRule, const char* aRuleStr)
{
    mInputCD          = new TCD(aRule->GetInputCD());
    mOutputCD     = new TCD(aRule->GetOutputCD());
    SetLib(aRuleStr);
    mNextRule     = aRule->GetNextRule();
    mPreviousRule = aRule->GetPreviousRule();
    mErrorHandler = aRule->mErrorHandler;
    mLine = aRule->mLine;
    mCondition = aRule->mCondition;
    mConditionGroup = aRule->mConditionGroup;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TRule
//
///////////////////////////////////////////////////////////////////////////////
TRule::~TRule()
{
    if (mInputCD) { delete mInputCD; mInputCD  = NULL; }
    if (mOutputCD) { delete mOutputCD; mOutputCD = NULL; }
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

    if (!mLib.is_empty())
    {
        mLib.append_char('\n');
        mLib.append(lib.str());
    }
    else
    {
        mLib = lib;
    }
    if (!mLib.str())
        return mErrorHandler->SetError(5502,ERROR);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// FromString
//
///////////////////////////////////////////////////////////////////////////////
int TRule::FromString(char *aString, int aLine, const TCD* aDefaultInputCD, const TCD* aDefaultOutputCD)
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
            mInputCD = new TCD(mErrorHandler);
            if (!mInputCD)
                return -5504;
            if ((ReturnCode = mInputCD->FromString(cdin.str(), aDefaultInputCD, INPUT)) != 0)
                return -ReturnCode;
        }
    }
    if (firstPipe)
    {
        if (!mInputCD)
        {
            // No input CD
            mInputCD = new TCD(mErrorHandler);
            if (!mInputCD)
                return -5504;
            if ((ReturnCode = mInputCD->FromString("", aDefaultInputCD, INPUT)) != 0)
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

        mOutputCD = new TCD(mErrorHandler);
        if (!mOutputCD)
            return -5504;
        if ((ReturnCode = mOutputCD->FromString(cdout.str(), aDefaultOutputCD, OUTPUT)) != 0)
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

    if ((!mInputCD) || (!mOutputCD))
        return false;

    typestr tmp;
    if (!mInputCD->ToString(tmp, INPUT))
        // Absence du Tag d'entrȨe
        return false;
    a_string += tmp;
    a_string += " | ";
    
    if (!mOutputCD->ToString(tmp, OUTPUT))
        // Absence du Tag de sortie
        return false;
    a_string += tmp;
    while(strlen(a_string.str()) < 30)
        a_string.append_char(' ');

    a_string += " | ";

    if (mLib.str())
    {
        a_string += mLib;
        a_string.replace("\n", "                             ");
    }
    return true;
}

