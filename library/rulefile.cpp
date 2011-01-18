/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2011
 *
 *  File:  rulefile.cpp
 *
 *  implements a class to manage the rule files
 *
 */

#include "trule.h"
#include "rulefile.h"
#include "statemanager.h"
#include "tools.h"

///////////////////////////////////////////////////////////////////////////////
//
// TRuleFile
//
///////////////////////////////////////////////////////////////////////////////
TRuleFile::TRuleFile(typestr & FileSpec, TUMApplication *Application)
: TFile(FileSpec, Application->GetStateManager()), mStringTables(Application->GetStateManager())
{
    itsFirstRule        = NULL;
    itsFirstCodedData   = NULL;
    itsLastCodedData    = NULL;
    itsMacros           = NULL;
    itsDocument         = NULL;
    itsApplication      = Application;
    mStateManager     = Application->GetStateManager();
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TRuleFile
//
///////////////////////////////////////////////////////////////////////////////
TRuleFile::~TRuleFile()
{
    if (itsDocument)
        delete itsDocument;

    DelTreeRule();
    DelTreeCodedData();
    if (itsMacros)
        delete itsMacros;
}

///////////////////////////////////////////////////////////////////////////////
//
// Close
//
///////////////////////////////////////////////////////////////////////////////
int TRuleFile::CloseRuleFile()
{
    // Unload the Rule tree from memory
    DelTreeRule();
    DelTreeCodedData();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
///////////////////////////////////////////////////////////////////////////////
int TRuleFile::OpenRuleFile()
{
    typestr     RuleLine;
    TRule       *CurrentRule;
    typestr     IncludedFileSpec;
    int     Result;
    int     Line;
    bool    IsRuleAnalysed;

    // Verify that the Rule File exists
    if (Exists()==false)
        return mStateManager->SetErrorD(5001, WARNING, itsFileInfo.str());

    if (itsMacros)
    {
        delete itsMacros;
        itsMacros = NULL;
    }

    // Open the Rule file in Reading mode
    itsMode = FILE_READ;
    if (Open())
        return 1;

    itsDocument = new TRuleDoc(itsApplication);

    // Load the rule on into a Rule memory tree
    CurrentRule = new TRule(mStateManager);
    if (!CurrentRule)
        return mStateManager->SetError(5501,ERROR);
    CurrentRule->SetPreviousRule(NULL);
    itsFirstRule = CurrentRule;
    IsRuleAnalysed = false;
    TCD* lastInputCD = NULL;
    TCD* lastOutputCD = NULL;
    typestr condition;
    int condition_count = 0;
    int condition_group = 0;
    int condition_line = 0;
    bool in_condition = false;
    while (!NextLine(&RuleLine, true, &IncludedFileSpec, &Line))
    {
        if (!RuleLine.is_empty())
        {
            // Apply macros
            StringTableItem* macro = itsMacros ? itsMacros->GetFirstItem() : NULL;
            while (macro)
            {
                RuleLine.replace(macro->m_src.str(), macro->m_dst.str());
                macro = macro->GetNext();
            }
        }

        char *p = RuleLine.str();
        while (*p == ' ' || *p == '\t')
            ++p;
        if (*p == '\0')
            continue;
        if (strncmp(p, "#define macro ", 14) == 0)
        {
            p += 14;
            while (*p == ' ' || *p == '\t') 
                ++p;
            typestr src;
            typestr dst;
            if (!StringTable::parse_line(p, src, dst))
            {
                typestr error;
                error.allocstr(strlen(IncludedFileSpec.str()) + strlen(RuleLine.str()) + 100);
                sprintf(error.str(), "in file '%s' at line %d:\n%s", IncludedFileSpec.str(), Line, RuleLine.str());
                return mStateManager->SetErrorD(5100, ERROR, error.str());
            }
            if (!itsMacros)
            {
                itsMacros = new StringTable(mStateManager);
            }
            StringTableItem* macro = itsMacros->AddItem();
            macro->m_src = src;
            macro->m_dst = dst;
            continue;
        }
        else if (strncmp(p, "#define version ", 16) == 0)
        {
            p += 16;
            while (*p == ' ' || *p == '\t') 
                ++p;
            int version = atoi(p);
            if (version == 0 || version > RULE_VERSION)
            {
                typestr error;
                char tmp[30];
                error = "File: ";
                error += IncludedFileSpec.str();
                error += ", line ";
                sprintf(tmp, "%d", Line);
                error += tmp;
                error += ". Supported rule version: ";
                sprintf(tmp, "%d", RULE_VERSION);
                error += tmp;
                if (version > 0)
                {
                    error += ", required rule version: ";
                    error += (p);
                }
                error += ". ";

                return mStateManager->SetErrorD(version == 0 ? 5050 : 5051, ERROR, error.str());
            }
            continue;
        }
        else if (strncmp(p, "#if ", 4) == 0)
        {
            if (in_condition)
            {
                typestr error;
                char tmp[30];
                error = "File: ";
                error += IncludedFileSpec.str();
                error += ", line ";
                sprintf(tmp, "%d", Line);
                error += tmp;
                return mStateManager->SetErrorD(5603, ERROR, error.str());
            }
            p += 4;
            while (*p == ' ' || *p == '\t') 
                ++p;
            condition = p;
            condition_group = ++condition_count;
            condition_line = Line;
            in_condition = true;
            continue;
        }
        else if (strncmp(p, "#endif", 6) == 0)
        {
            if (!in_condition)
            {
                typestr error;
                char tmp[30];
                error = "File: ";
                error += IncludedFileSpec.str();
                error += ", line ";
                sprintf(tmp, "%d", Line);
                error += tmp;
                return mStateManager->SetErrorD(5604, ERROR, error.str());
            }
            in_condition = false;
            continue;
        }

        if (IsRuleAnalysed && HasPipes(RuleLine.str()))
        {
            // This is a new rule to process
            /*if (reset_condition)
            {
                condition_group = 0;
                condition.freestr();
                reset_condition = false;
            }*/

            lastInputCD = CurrentRule->GetInputCD();
            lastOutputCD = CurrentRule->GetOutputCD();
            CurrentRule->SetNextRule(new TRule(mStateManager)); 
            if (!CurrentRule->GetNextRule())
            {
                typestr Illustration;
                Illustration.allocstr(strlen(IncludedFileSpec.str()) + strlen(RuleLine.str()) + 100);
                sprintf(Illustration.str(), "in file '%s' at line %d:\n%s", IncludedFileSpec.str(), Line, RuleLine.str());
                return  mStateManager->SetErrorD(5501,ERROR,Illustration.str());
            }
            CurrentRule->GetNextRule()->SetPreviousRule(CurrentRule);
            CurrentRule = CurrentRule->GetNextRule();
        }

        if ((Result=CurrentRule->FromString(RuleLine.str(), Line, lastInputCD, lastOutputCD))<0) // Load Input and Output CDs in CDs
            // Error on rule
        {
            typestr Illustration;
            Illustration.allocstr(strlen(IncludedFileSpec.str()) + strlen(RuleLine.str()) + 100);
            sprintf(Illustration.str(), "in file '%s' at line %d :\n%s", IncludedFileSpec.str(), Line, RuleLine.str());
            mStateManager->SetErrorD(-Result,ERROR,Illustration.str());
            // Delete the erroneous rule
            // No we don't. We should unwind the rule from the list too...
            //if (CurrentRule->GetInputCD())  { delete(CurrentRule->GetInputCD());    CurrentRule->SetInputCD(NULL);  }
            //if (CurrentRule->GetOutputCD()){ delete(CurrentRule->GetOutputCD());    CurrentRule->SetOutputCD(NULL); }
            //if (CurrentRule->GetLib())      { delete(CurrentRule->GetLib());        CurrentRule->SetLib(NULL);  }
            IsRuleAnalysed = false;
        }
        else if (Result > 0)
        {
            IsRuleAnalysed = true;
            if (in_condition)
                CurrentRule->SetCondition(condition.str(), condition_group);
        }
    }
    CurrentRule->SetNextRule(NULL);
    if (in_condition)
    {
        typestr error;
        char tmp[30];
        error = "File: ";
        error += IncludedFileSpec.str();
        error += ", line ";
        sprintf(tmp, "%d", condition_line);
        error += tmp;
        return mStateManager->SetErrorD(5605, ERROR, error.str());
    }


    // Close the Rule file
    Close();

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// Convert
//
// Cette methode convertit un UMRecord sous forme de CDLib en un autre,
// conformement au fichier de regles
//
///////////////////////////////////////////////////////////////////////////////

int TRuleFile::ConvertInRuleOrder(TUMRecord* In, TUMRecord* Out)
{
    mStateManager->ResetErrorCode();

    itsEvaluateRule.Init_Evaluate_Rule(itsDocument, itsApplication->GetRuleDoc(),
        itsApplication->GetStateManager(), itsApplication->GetDebugRule(),
        itsApplication->GetOrdinal(), itsApplication->GetUTF8Mode());

    // Evaluate all rules
    TRule* aRule = itsFirstRule;
    TCDLib* Last = In->GetLastCDLib();
    while (aRule)
    {
        const char* condition = aRule->GetCondition();
        if (condition && *condition)
        {
            mStateManager->SetErrorD(5602, ERROR, "#if condition");
            break;
        }
        TCD *OutputCD = aRule->GetOutputCD();
        if (aRule->GetInputCD()->TagContainsWildcard() || !OutputCD->TagIsWildcard())
        {
            if (OutputCD->GetIN())
            {
                itsEvaluateRule.Evaluate_Rule(In, In, Out, aRule);
                if (mStateManager->GetSortRecord())
                    In->SortCD();
                else
                    In->PartialSort((TCDLib *) Last->GetNext());
            }
            else
            {
                itsEvaluateRule.Evaluate_Rule(In, Out, Out, aRule);
            }
        }
        aRule = aRule->GetNextRule(); 
    }

    itsEvaluateRule.End_Evaluate_Rule();

    return mStateManager->GetErrorCode();
}

int TRuleFile::ConvertInFieldOrder(TUMRecord* In, TUMRecord* Out)
{
    mStateManager->ResetErrorCode();

    bool debug_rule = itsApplication->GetDebugRule();

    itsEvaluateRule.Init_Evaluate_Rule(itsDocument, itsApplication->GetRuleDoc(),
        itsApplication->GetStateManager(), debug_rule,
        itsApplication->GetOrdinal(), itsApplication->GetUTF8Mode());

    TCDLib* CDLIn=In->GetFirstCDLib();
    TCDLib* Last = In->GetLastCDLib();
    while (CDLIn)
    {
        bool can_match_tag_wildcard = true;
        bool can_match_subfield_wildcard = true;
        TRule* rule = itsFirstRule;
        while (rule)
        {
            TCD *RuleCD = rule->GetInputCD();
            if (*CDLIn == *RuleCD)
            {
                do // for easy exit only
                {
                    // If the rule doesn't specify a subfield, it's only accepted for the 
                    // first subfield of the field as it will handle the whole field
                    if (!*RuleCD->GetSubfield())
                    {
                        TCD *prevCD = CDLIn->GetPrevious();
                        if (prevCD && prevCD->GetTagOccurrenceNumber() == CDLIn->GetTagOccurrenceNumber() &&
                            strcmp(prevCD->GetTag(), CDLIn->GetTag()) == 0)
                            break;
                    }

                    // If the rule contains a wildcard, it's only accepted if non-wildcard
                    // rules for the field have not been processed already
                    if (RuleCD->TagContainsWildcard())
                    {
                        if (!can_match_tag_wildcard)
                            break;
                    }
                    else
                    {
                        can_match_tag_wildcard = false;
                    }
                    if (RuleCD->SubfieldContainsWildcard())
                    {
                        if (!can_match_subfield_wildcard)
                            break;
                    }
                    else
                    {
                        can_match_subfield_wildcard = false;
                    }
                    if (RuleCD->TagContainsWildcard() || !rule->GetOutputCD()->TagIsWildcard())
                    {
                        // Rule matches, check possible condition
                        const char* condition = rule->GetCondition();
                        if (condition && *condition)
                        {
                            bool passed = false;
                            if (itsEvaluateRule.CheckCondition(In, Out, CDLIn, rule, passed) || !passed)
                            {
                                if (debug_rule)
                                {
                                    typestr tmp;
                                    rule->ToString(tmp);
                                    printf("\nDebug: Condition %s not passed, ignoring rule: '%s'\n", condition, tmp.str());
                                }
                                // Find the last rule in this condition group and get out.
                                int group = rule->GetConditionGroup();
                                TRule* prev_rule = rule;
                                rule = rule->GetNextRule();
                                while (rule && group == rule->GetConditionGroup())
                                {
                                    if (debug_rule)
                                    {
                                        typestr tmp;
                                        rule->ToString(tmp);
                                        printf("\nDebug: Condition %s not passed, ignoring rule: '%s'\n", condition, tmp.str());
                                    }
                                    prev_rule = rule;
                                    rule = rule->GetNextRule();
                                }
                                rule = prev_rule;
                                break;
                            }
                        }

                        if (rule->GetOutputCD()->GetIN())
                        {
                            itsEvaluateRule.Evaluate_Rule(In, In, Out, rule, CDLIn);
                            // Only added fields can be sorted here, otherwise our
                            // processing order would be broken
                            In->PartialSort((TCDLib *) Last->GetNext());
                        }
                        else
                        {
                            itsEvaluateRule.Evaluate_Rule(In, Out, Out, rule, CDLIn);
                        }
                    }
                } 
                while (false);
            }
            rule = rule->GetNextRule(); 
        }

        CDLIn = (TCDLib *)CDLIn->GetNext();
    }

    itsEvaluateRule.End_Evaluate_Rule();

    return mStateManager->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// DelTreeRule
//
///////////////////////////////////////////////////////////////////////////////
void TRuleFile::DelTreeRule(void)
{
    TRule           *Courant,
        *Suivant;
    Courant=itsFirstRule;

    while(Courant)
    {
        Suivant=Courant->GetNextRule();
        delete Courant;
        Courant=Suivant;
    }
    itsFirstRule=NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// DelTreeCodedData
//
///////////////////////////////////////////////////////////////////////////////
void TRuleFile::DelTreeCodedData(void)
{
    TCodedData      *Courant,
        *Suivant;
    Courant=itsFirstCodedData;

    while(Courant)
    {
        Suivant=Courant->GetNextCodedData();
        delete Courant;
        Courant=Suivant;
    }
    itsFirstCodedData=itsLastCodedData=NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetCodedData
//
///////////////////////////////////////////////////////////////////////////////
TCodedData  *TRuleFile::GetCodedData(char *theName)
{
    typestr         CodedDataSpec;
    typestr         testspec;
    TCodedData      *aCodedData,
                    *anOldCodedData;

    testspec = ((TRuleDoc *) itsApplication->itsRuleDoc)->GetRuleSpec();

    if (strchr(theName, SLASH))
        CodedDataSpec.freestr();
    else
        copy_path_from_filename(CodedDataSpec, testspec.str());
    append_filename(CodedDataSpec, theName);

    if (!strchr(theName,'.'))
        append_filename(CodedDataSpec, ".tbl");

    anOldCodedData=itsFirstCodedData;
    while(anOldCodedData)
    {
        if (anOldCodedData->GetCodedDataName() == CodedDataSpec)
            // the coded data table was found
            return anOldCodedData;
        anOldCodedData = anOldCodedData->GetNextCodedData();
    }
    // this coded data table doesn't exist

    aCodedData = new TCodedData(CodedDataSpec, mStateManager);
    anOldCodedData = itsLastCodedData;

    if (aCodedData)
        // Succed in TCodedData creation
        if (!aCodedData->Open())
            // Succed in coded data table loading from file
        {
            if (!anOldCodedData)
                // there is no coded data table loaded ==> load this one to the head of the list
                itsFirstCodedData=itsLastCodedData=aCodedData;
            else
                // there is one table loaded at least ==> load this table to the end of the list
            {
                anOldCodedData->SetNextCodedData(aCodedData);
                itsLastCodedData=aCodedData;
            }
            return aCodedData;
        }

    // coded data table loading from file or TcodedData creation failure
    if (aCodedData)
        delete aCodedData;
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetStringTable
//
///////////////////////////////////////////////////////////////////////////////
StringTable* TRuleFile::GetStringTable(const char *a_tablename)
{
    typestr filename;
    if (!strchr(a_tablename, SLASH))
        copy_path_from_filename(filename, ((TRuleDoc *) itsApplication->itsRuleDoc)->GetRuleSpec().str());
    append_filename(filename, a_tablename);

    if (!strchr(filename.str(), '.'))
        append_filename(filename, ".tbl");

    // Check if the table has been loaded already
    StringTable* table = mStringTables.GetFirstTable();
    while (table)
    {
        if (table->GetName() == filename)
            return table;
        table = table->GetNextTable();
    }
    
    // Load the table
    table = mStringTables.AddTable();
    if (!table)
        return NULL;

    if (!table->Load(filename.str()))
    {
        return NULL;
    }
    return table;
}

bool TRuleFile::HasPipes(const char *a_str)
{
    const char* p = a_str;
    bool in_string = false;
    while (*p)
    {
        if (*p == '\'')
            in_string = !in_string;

        if (!in_string && *p == '|')
        {
            return true;
        }
        ++p;
    }
    return false;
}
