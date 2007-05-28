/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2006
 *
 *  File:  rulefile.cpp
 *
 *  implements a class to manage the rule files
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include "rulefile.h"
#include "error.h"
#include "tools.h"

///////////////////////////////////////////////////////////////////////////////
//
// TRuleFile
//
///////////////////////////////////////////////////////////////////////////////
TRuleFile::TRuleFile(FILE_SPEC *FileSpec,TUMApplication *Application)
:TFile(FileSpec, Application->GetErrorHandler())
{
    itsFirstRule        = NULL;
    itsLastInputCD      = NULL;
    itsLastOutputCD     = NULL;
    itsFirstCodedData   = NULL;
    itsLastCodedData    = NULL;
    itsDocument         = NULL;
    itsApplication      = Application;
    itsErrorHandler     = Application->GetErrorHandler();
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TRuleFile
//
///////////////////////////////////////////////////////////////////////////////
TRuleFile::~TRuleFile()
{
    // John Hough - added test an delete
    if (itsDocument)
        delete itsDocument;

    // Unload the Rule Tree from memory
    DelTreeRule();
    DelTreeCodedData();
    if (itsLastInputCD) { delete itsLastInputCD;    itsLastInputCD  = NULL; }
    if (itsLastOutputCD)    { delete itsLastOutputCD;   itsLastOutputCD = NULL; }
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
    if (itsLastInputCD) { delete itsLastInputCD;    itsLastInputCD  = NULL; }
    if (itsLastOutputCD)    { delete itsLastOutputCD;   itsLastOutputCD = NULL; }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
///////////////////////////////////////////////////////////////////////////////
#define RULE_SIZE   BUFF_SIZE *4
int TRuleFile::OpenRuleFile()
{
    typestr     RuleLine;
    typestr     Illustration;
    TRule       *CurrentRule;
    FILE_SPEC   IncludedFileSpec;
    int     Result;
    int     Line;
    bool    IsRuleAnalysed;

    // Verify that the Rule File exists
    if (Exists()==false)
        return itsErrorHandler->SetErrorD(5001,WARNING,itsFileInfo->name);

    // Open the Rule file in Reading mode
    itsMode = FILE_READ;
    if (Open())
        return 1;

    itsDocument = new TRuleDoc(itsApplication);

    // Load the rule on into a Rule memory tree
    CurrentRule = new TRule(NULL, NULL, itsErrorHandler);
    if (!CurrentRule)
        return itsErrorHandler->SetError(5501,ERROR);
    CurrentRule->SetPreviousRule(NULL);
    itsFirstRule = CurrentRule;
    IsRuleAnalysed = false;
    while (!NextLine(&RuleLine,&IncludedFileSpec,&Line)) // Read a Line from the Rule File
    {
        Illustration.allocstr(strlen(IncludedFileSpec.name) + strlen(RuleLine.str()) + 100);
        sprintf(Illustration.str(),"in file '%s' at line %d :\n%s",IncludedFileSpec.name,Line-1,RuleLine.str());
        if (IsRuleAnalysed && strchr(RuleLine.str(),'|'))
        {
            // This is a new rule to process
            if (itsLastInputCD) { delete itsLastInputCD;    itsLastInputCD  = NULL; }
            if (CurrentRule->GetInputCD())
                itsLastInputCD = new TCD(CurrentRule->GetInputCD());
            if (itsLastOutputCD)    { delete itsLastOutputCD;   itsLastOutputCD = NULL; }
            if (CurrentRule->GetOutputCD())
                itsLastOutputCD = new TCD(CurrentRule->GetOutputCD());
            CurrentRule->SetNextRule(new TRule(itsLastInputCD,itsLastOutputCD,itsErrorHandler)); // Memory loading passed
            if (!CurrentRule->GetNextRule())
                return  itsErrorHandler->SetErrorD(5501,ERROR,Illustration.str());
            CurrentRule->GetNextRule()->SetPreviousRule(CurrentRule);
            CurrentRule = CurrentRule->GetNextRule();
        }

        if ((Result=CurrentRule->FromString(RuleLine.str()))<0) // Load Input and Output CDs in CDs
            // Error on rule
        {
            itsErrorHandler->SetErrorD(-Result,ERROR,Illustration.str());
            // Delete the erroneous rule
            if (CurrentRule->GetInputCD())  { delete(CurrentRule->GetInputCD());    CurrentRule->SetInputCD(NULL);  }
            if (CurrentRule->GetOutputCD()){ delete(CurrentRule->GetOutputCD());    CurrentRule->SetOutputCD(NULL); }
            if (CurrentRule->GetLib())      { delete(CurrentRule->GetLib());        CurrentRule->SetLib(NULL);  }
            IsRuleAnalysed = false;
        }
        else
            if (Result>0)
                IsRuleAnalysed = true;
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
    TRule*  aRule=itsFirstRule;

    itsErrorHandler->Reset();

    itsEvaluateRule.Init_Evaluate_Rule(itsDocument, itsApplication->GetRuleDoc(),
        itsApplication->GetErrorHandler(), itsApplication->GetDebugRule(),
        itsApplication->GetOrdinal(), itsApplication->GetUTF8Mode());

    // On parcourt chaque regle, et on en fait l'evaluation
    while( aRule )
    {
        TCD *OutputCD = aRule->GetOutputCD();
        if (aRule->GetInputCD()->TagContainsWildcard() || !OutputCD->TagIsWildcard())
        {
            if (OutputCD->GetIN())
            {
                itsEvaluateRule.Evaluate_Rule( In, In, aRule );
                In->SortCD();
            }
            else
            {
                itsEvaluateRule.Evaluate_Rule( In, Out, aRule );
            }
        }
        aRule=aRule->GetNextRule();
    }

    itsEvaluateRule.End_Evaluate_Rule();

    return itsErrorHandler->GetErrorCode();
}

int TRuleFile::ConvertInFieldOrder( TUMRecord* In, TUMRecord* Out )
{
    itsErrorHandler->Reset();

    itsEvaluateRule.Init_Evaluate_Rule(itsDocument, itsApplication->GetRuleDoc(),
        itsApplication->GetErrorHandler(), itsApplication->GetDebugRule(),
        itsApplication->GetOrdinal(), itsApplication->GetUTF8Mode());

    TCDLib* CDLIn=In->GetFirstCDLib();
    TCDLib* Last = In->GetLastCDLib();
    while (CDLIn)
    {
        bool can_match_tag_wildcard = true;
        bool can_match_subfield_wildcard = true;
        TRule* aRule=itsFirstRule;
        while(aRule)
        {
            TCD *RuleCD = aRule->GetInputCD();
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

                    // If the rule contains wildcard, it's only accepted if non-wildcard
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
                    if (RuleCD->TagContainsWildcard() || !aRule->GetOutputCD()->TagIsWildcard())
                    {
                        if (aRule->GetOutputCD()->GetIN())
                        {
                            itsEvaluateRule.Evaluate_Rule( In, In, aRule, CDLIn );
                            // Only added fields can be sorted here, otherwise our
                            // processing order would be broken
                            In->PartialSort((TCDLib *) Last->GetNext());
                        }
                        else
                        {
                            itsEvaluateRule.Evaluate_Rule( In, Out, aRule, CDLIn );
                        }
                    }
                } 
                while (false);
            }
            aRule=aRule->GetNextRule();
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
    FILE_SPEC       CodedDataSpec;
    FILE_SPEC       *testspec;
    TCodedData      *aCodedData,
                    *anOldCodedData;

    itsErrorHandler->Reset();
    testspec = ((TRuleDoc *) itsApplication->itsRuleDoc)->GetRuleSpec();
    memcpy(&CodedDataSpec,testspec,sizeof(FILE_SPEC));

    if (strchr(theName, SLASH))
        *CodedDataSpec.name = '\0';
    else
        copy_path_from_filename(CodedDataSpec.name, testspec->name);
    append_filename(CodedDataSpec.name, theName);

    if (!strchr(theName,'.'))
        append_filename(CodedDataSpec.name, ".tbl");

    anOldCodedData=itsFirstCodedData;
    while(anOldCodedData)
    {
        if (!memcmp(anOldCodedData->GetCodedDataName(),&CodedDataSpec,sizeof(FILE_SPEC)))
            // the coded data table was found
            return anOldCodedData;
        anOldCodedData = anOldCodedData->GetNextCodedData();
    }
    // this coded data table doesn't exist

    aCodedData=new TCodedData(&CodedDataSpec, itsErrorHandler);
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
