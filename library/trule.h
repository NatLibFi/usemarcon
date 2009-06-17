/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  trule.h
 *
 *
 
CLASS
    TRule

    implements a class to manage Rules

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
#ifndef _TRULE_H_
#define _TRULE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tcd.h"
#include "umrecord.h"

class TRule
{
public:
    TRule           (TStateManager *StateManager);
    TRule           (TRule *aRule);
    TRule           (TRule *aRule, const char* aRuleStr);
    virtual ~TRule  ();

    int   FromString      (char *aString, int aLine, const TCD* aDefaultInputCD, const TCD* aDefaultOutputCD);
    bool  ToString        (typestr & a_string);
    void  ResetLib        (void)              { mLib.freestr(); }

    inline TCD   *GetInputCD     (void)         { return mInputCD; }
    inline TCD   *GetOutputCD    (void)         { return mOutputCD; }
    inline char  *GetLib         (void)         { return mLib.str(); }
    inline TRule *GetNextRule    (void)         { return mNextRule; }
    inline TRule *GetPreviousRule(void)         { return mPreviousRule; }
    inline char  *GetCondition   (void)         { return mCondition.str(); }
    inline int   GetConditionGroup(void)        { return mConditionGroup; }
    inline bool  GetConcatenation()             { return mConcatenation; }

    void  SetInputCD      (TCD* aCD)            { mInputCD=aCD; }
    void  SetOutputCD     (TCD *aCD)            { mOutputCD=aCD; }
    int   SetLib          (const char *aLib);
    void  SetNextRule     (TRule *NextRule)     { mNextRule=NextRule; }
    void  SetPreviousRule (TRule *PreviousRule) { mPreviousRule = PreviousRule; }
    void  SetCondition    (const char *aCondition, int aGroup)  { mCondition = aCondition; mConditionGroup = aGroup; }

    int   GetLineNo() { return mLine; }

protected:
    TCD           *mInputCD;
    TCD           *mOutputCD;
    typestr       mLib;
    TRule         *mNextRule;
    TRule         *mPreviousRule;
    int           mLine;
    typestr       mCondition;
    int           mConditionGroup;
    bool          mConcatenation;
    
    TStateManager        *mStateManager;
};

#endif
