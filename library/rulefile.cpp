/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  rulefile.cpp
 *
 *  implements a class to manage the rule files
 *
 */

#include "trule.h"
#include "rulefile.h"
#include "error.h"
#include "tools.h"

///////////////////////////////////////////////////////////////////////////////
//
// TRuleFile
//
///////////////////////////////////////////////////////////////////////////////
TRuleFile::TRuleFile(typestr & FileSpec, TUMApplication *Application)
: TFile(FileSpec, Application->GetErrorHandler())
{
    itsFirstRule        = NULL;
    itsFirstCodedData   = NULL;
    itsLastCodedData    = NULL;
    itsFirstStringTable = NULL;
    itsLastStringTable  = NULL;
    itsMacros           = NULL;
    itsDocument         = NULL;
    itsApplication      = Application;
    itsErrorHandler     = Application->GetErrorHandler();
    mRulesDisabled      = false;
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
    DelTreeStringTable();
    DelTreeMacros();
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
    DelTreeStringTable();
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
        return itsErrorHandler->SetErrorD(5001, WARNING, itsFileInfo.str());

    DelTreeMacros();

    // Open the Rule file in Reading mode
    itsMode = FILE_READ;
    if (Open())
        return 1;

    itsDocument = new TRuleDoc(itsApplication);

    // Load the rule on into a Rule memory tree
    CurrentRule = new TRule(itsErrorHandler);
    if (!CurrentRule)
        return itsErrorHandler->SetError(5501,ERROR);
    CurrentRule->SetPreviousRule(NULL);
    itsFirstRule = CurrentRule;
    IsRuleAnalysed = false;
    StringTable *current_macro = NULL;
    TCD* lastInputCD = NULL;
    TCD* lastOutputCD = NULL;
    typestr block;
    while (!NextLine(&RuleLine,&IncludedFileSpec,&Line)) // Read a Line from the Rule File
    {
        char *p = RuleLine.str();
        while (*p == ' ' || *p == '\t')
            ++p;
        if (strncmp(p, "#define macro ", 14) == 0)
        {
            p += 14;
            typestr src;
            typestr dst;
            if (!StringTable::parse_line(p, src, dst))
            {
                typestr error;
                error.allocstr(strlen(IncludedFileSpec.str()) + strlen(RuleLine.str()) + 100);
                sprintf(error.str(), "in file '%s' at line %d:\n%s", IncludedFileSpec.str(), Line, RuleLine.str());
                return itsErrorHandler->SetErrorD(5100, ERROR, error.str());
            }
            if (!current_macro)
            {
                itsMacros = new StringTable(itsErrorHandler);
                current_macro = itsMacros;
            }
            else
            {
                current_macro->SetNext(new StringTable(itsErrorHandler));
                current_macro = current_macro->GetNext();
            }
            current_macro->m_src = src;
            current_macro->m_dst = dst;
            continue;
        }
        else if (strncmp(p, "#define version ", 16) == 0)
        {
            int version = atoi(p + 16);
            if (version == 0 || version > RULE_VERSION)
            {
                typestr error;
                char tmp[30];
                error = "File: ";
                error += IncludedFileSpec.str();
                error += ", line ";
                sprintf(tmp, "%ld", Line);
                error += tmp;
                error += ". Supported rule version: ";
                sprintf(tmp, "%ld", RULE_VERSION);
                error += tmp;
                if (version > 0)
                {
                    error += ", required rule version: ";
                    error += (p + 16);
                }
                error += ". ";

                return itsErrorHandler->SetErrorD(version == 0 ? 5050 : 5051, ERROR, error.str());
            }
            continue;
        }
        else if (strncmp(p, "#block ", 7) == 0)
        {
            p += 7;
            block = p;
            continue;
        }

        // Apply macros
        StringTable *macro = itsMacros;
        while (macro)
        {
            RuleLine.replace(macro->m_src.str(), macro->m_dst.str());
            macro = macro->GetNext();
        }

        if (IsRuleAnalysed && HasPipes(RuleLine.str()))
        {
            // This is a new rule to process
            lastInputCD = CurrentRule->GetInputCD();
            lastOutputCD = CurrentRule->GetOutputCD();
            CurrentRule->SetNextRule(new TRule(itsErrorHandler)); 
            if (!CurrentRule->GetNextRule())
            {
                typestr Illustration;
                Illustration.allocstr(strlen(IncludedFileSpec.str()) + strlen(RuleLine.str()) + 100);
                sprintf(Illustration.str(), "in file '%s' at line %d:\n%s", IncludedFileSpec.str(), Line, RuleLine.str());
                return  itsErrorHandler->SetErrorD(5501,ERROR,Illustration.str());
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
            itsErrorHandler->SetErrorD(-Result,ERROR,Illustration.str());
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
        }

        CurrentRule->SetBlockName(block.str());
    }
    CurrentRule->SetNextRule(NULL);

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

int TRuleFile::ConvertInRuleOrder( TUMRecord* In, TUMRecord* Out )
{
    itsErrorHandler->Reset();

    itsEvaluateRule.Init_Evaluate_Rule(itsDocument, itsApplication->GetRuleDoc(),
        itsApplication->GetErrorHandler(), itsApplication->GetDebugRule(),
        itsApplication->GetOrdinal(), itsApplication->GetUTF8Mode());

    // Evaluate all rules
    TRule* aRule = itsFirstRule;
    while (aRule)
    {
        TCD *OutputCD = aRule->GetOutputCD();
        bool change_block = false;
        if (aRule->GetInputCD()->TagContainsWildcard() || !OutputCD->TagIsWildcard())
        {
            if (OutputCD->GetIN())
            {
                itsEvaluateRule.Evaluate_Rule(In, In, Out, aRule, change_block);
                In->SortCD();
            }
            else
            {
                itsEvaluateRule.Evaluate_Rule(In, Out, Out, aRule, change_block);
            }
        }
        if (change_block)
        {
            typestr next_block = itsEvaluateRule.GetNextBlockName();
            typestr old_block = aRule->GetBlockName();
            aRule = aRule->GetNextRule();
            while (aRule)
            {
                typestr new_block = aRule->GetBlockName();
                if (next_block.is_empty())
                {
                    if (!(new_block == old_block))
                        break;
                }
                else
                {
                    if (new_block == next_block)
                        break;
                }
                aRule = aRule->GetNextRule();
            }
        }
        else
        {
            aRule = aRule->GetNextRule(); 
        }
    }

    itsEvaluateRule.End_Evaluate_Rule();

    return itsErrorHandler->GetErrorCode();
}

int TRuleFile::ConvertInFieldOrder(TUMRecord* In, TUMRecord* Out)
{
    itsErrorHandler->Reset();

    itsEvaluateRule.Init_Evaluate_Rule(itsDocument, itsApplication->GetRuleDoc(),
        itsApplication->GetErrorHandler(), itsApplication->GetDebugRule(),
        itsApplication->GetOrdinal(), itsApplication->GetUTF8Mode());


    if (mRulesDisabled)
    {
        // Reset state
        TRule* rule = itsFirstRule;
        while (rule)
        {
            rule->SetDisabled(false);
            rule = rule->GetNextRule();
        }
        mRulesDisabled = false;
    }

    TCDLib* CDLIn=In->GetFirstCDLib();
    TCDLib* Last = In->GetLastCDLib();
    while (CDLIn)
    {
        bool can_match_tag_wildcard = true;
        bool can_match_subfield_wildcard = true;
        TRule* rule = itsFirstRule;
        while (rule)
        {
            if (rule->GetDisabled())
            {
                rule = rule->GetNextRule();
                continue;
            }
            TCD *RuleCD = rule->GetInputCD();
            bool change_block = false;
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
                        if (rule->GetOutputCD()->GetIN())
                        {
                            itsEvaluateRule.Evaluate_Rule(In, In, Out, rule, change_block, CDLIn);
                            // Only added fields can be sorted here, otherwise our
                            // processing order would be broken
                            In->PartialSort((TCDLib *) Last->GetNext());
                        }
                        else
                        {
                            itsEvaluateRule.Evaluate_Rule(In, Out, Out, rule, change_block, CDLIn);
                        }
                    }
                } 
                while (false);
            }
            if (change_block)
            {
                mRulesDisabled = true;
                typestr next_block = itsEvaluateRule.GetNextBlockName();
                typestr old_block = rule->GetBlockName();
                rule->SetDisabled(true);
                rule = rule->GetNextRule();
                while (rule)
                {
                    typestr new_block = rule->GetBlockName();
                    if (next_block.is_empty())
                    {
                        if (!(new_block == old_block))
                            break;
                    }
                    else
                    {
                        if (new_block == next_block)
                            break;
                    }
                    rule->SetDisabled(true);
                    rule = rule->GetNextRule();
                }
            }
            else
            {
                rule = rule->GetNextRule(); 
            }
        }

        CDLIn = (TCDLib *)CDLIn->GetNext();
    }

    itsEvaluateRule.End_Evaluate_Rule();

    return itsErrorHandler->GetErrorCode();
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

    itsErrorHandler->Reset();
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

    aCodedData = new TCodedData(CodedDataSpec, itsErrorHandler);
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
// DelTreeStringTable
//
///////////////////////////////////////////////////////////////////////////////
void TRuleFile::DelTreeStringTable(void)
{
    StringTable *table = itsFirstStringTable;

    while (table)
    {
        StringTable *next = table->GetNext();
        delete table;
        table = next;
    }
    itsFirstStringTable = itsLastStringTable = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetStringTable
//
///////////////////////////////////////////////////////////////////////////////
StringTable *TRuleFile::GetStringTable(const char *a_tablename)
{
    itsErrorHandler->Reset();

    typestr filename;
    if (!strchr(a_tablename, SLASH))
        copy_path_from_filename(filename, ((TRuleDoc *) itsApplication->itsRuleDoc)->GetRuleSpec().str());
    append_filename(filename, a_tablename);

    if (!strchr(filename.str(), '.'))
        append_filename(filename, ".tbl");

    // Check if the table has been loaded already
    StringTable *table = itsFirstStringTable;
    while (table)
    {
        if (table->GetName() == filename)
            return table;
        table = table->GetNext();
    }
    
    // Load the table
    table = new StringTable(itsErrorHandler);
    if (!table)
        return NULL;

    if (!table->Load(filename.str()))
    {
        delete table;
        return NULL;
    }

    if (!itsLastStringTable)
    {
        itsFirstStringTable = itsLastStringTable = table;
    }
    else
    {
        itsLastStringTable->SetNext(table);
        itsLastStringTable = table;
    }
    return table;
}

void TRuleFile::DelTreeMacros()
{
    StringTable *macro = itsMacros;
    while(macro)
    {
        StringTable *next = macro->GetNext();
        delete macro;
        macro = next;
    }
    itsMacros = NULL;
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
