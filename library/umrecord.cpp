/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2008
 *
 *  File:  umrecord.cpp
 *
 *  implements a class to manage MARC records
 *
 */

#include "umrecord.h"
#include "error.h"
#include "rulefile.h"

#define NO_SUBFIELD         ""
#define LABEL               "**"
#define ALREADY_DONE        "--"
#define FIRST_INDICATOR     "I1"
#define SECOND_INDICATOR    "I2"

///////////////////////////////////////////////////////////////////////////////
//
// TUMRecord
//
///////////////////////////////////////////////////////////////////////////////
TUMRecord::TUMRecord(TUMApplication *Application) : TMarcRecord(Application)
{
    itsFirstCDLib   = NULL;
    itsLastCDLib    = NULL;

    itsSearchedField = NULL;
    itsSearchedFieldFound = false;
    memset(itsExistingFields, false, sizeof(itsExistingFields));
}

///////////////////////////////////////////////////////////////////////////////
//
// TUMRecord : Copie constructeur
//
///////////////////////////////////////////////////////////////////////////////
TUMRecord::TUMRecord(const TUMRecord &aRecord) : TMarcRecord((TMarcRecord) aRecord)
{
    if (aRecord.GetFirstCDLib())
    {
        itsFirstCDLib=itsLastCDLib=new TCDLib(aRecord.GetFirstCDLib());
        itsFirstCDLib->SetPrevious(NULL);
        while(itsLastCDLib->GetNext())
            itsLastCDLib=(TCDLib*)itsLastCDLib->GetNext();
    }
    else
        itsFirstCDLib = NULL;

    memcpy(itsExistingFields, aRecord.itsExistingFields, sizeof(itsExistingFields));
    itsSearchedField = NULL;
    itsSearchedFieldFound = false;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TUMRecord
//
///////////////////////////////////////////////////////////////////////////////
TUMRecord::~TUMRecord()
{
    DelTreeCDLib();
}

///////////////////////////////////////////////////////////////////////////////
//
// SortCD
//
///////////////////////////////////////////////////////////////////////////////
int TUMRecord::SortCD()
{
    return PartialSort(itsFirstCDLib);
}

///////////////////////////////////////////////////////////////////////////////
//
// SortCD
//
///////////////////////////////////////////////////////////////////////////////
int TUMRecord::SortCD(TCD* aCD, char* liste)
{
    TCDLib* Debut=itsFirstCDLib,*NewDebut=NULL,*New=NULL,*aCDLib;
    int i;
    char sub[5];

    if (!NextCD(&Debut,aCD)) return itsErrorHandler->GetErrorCode();

    for (i=0;liste[i];++i)
    {
        aCDLib=Debut;
        while( aCDLib && *aCDLib==*aCD )
        {
            strcpy(sub,aCDLib->GetSubfield());
            if (*sub=='$')
            {
                if (sub[1]==liste[i])
                {
                    TCDLib* Previous=(TCDLib*)aCDLib->GetPrevious();
                    if (aCDLib==Debut) Debut=(TCDLib*)aCDLib->GetNext();
                    if (Previous)
                    {
                        Previous->SetNext(aCDLib->GetNext());
                        if (Previous->GetNext())
                            Previous->GetNext()->SetPrevious(Previous);
                    }
                    else
                    {
                        itsFirstCDLib=(TCDLib*)aCDLib->GetNext();
                        itsFirstCDLib->SetPrevious(NULL);
                    }

                    if (New)
                    {
                        New->SetNext((TCD*)aCDLib);
                        aCDLib->SetPrevious((TCD*)New);
                        aCDLib->SetNext(NULL);
                        New=aCDLib;
                    }
                    else
                    {
                        New=NewDebut=aCDLib;
                        aCDLib->SetPrevious(NULL);
                        aCDLib->SetNext(NULL);
                    }
                    aCDLib=Debut;
                }
                else
                    aCDLib=(TCDLib*)aCDLib->GetNext();
            }
            else
                aCDLib=(TCDLib*)aCDLib->GetNext();
        }
    }

    if (NewDebut)
    {
        New->SetNext(Debut);
        if (Debut->GetPrevious())
        {
            Debut->GetPrevious()->SetNext(NewDebut);
            NewDebut->SetPrevious(Debut->GetPrevious());
        }
        else
        {
            itsFirstCDLib=NewDebut;
            NewDebut->SetPrevious(NULL);
        }
        Debut->SetPrevious(New);
    }

    return itsErrorHandler->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// PartialSort
//
///////////////////////////////////////////////////////////////////////////////
int TUMRecord::PartialSort(TCDLib *aFirst)
{
    TCDLib *currentCDLib = aFirst;

    while (currentCDLib)
    {
        TCDLib *Smallest = currentCDLib;
        TCDLib *aCDLib = (TCDLib *) Smallest->GetNext();

        // Find smallest CDLib
        while(aCDLib)
        {
            if (*aCDLib < *Smallest)
                Smallest = aCDLib;
            aCDLib = (TCDLib*)aCDLib->GetNext();
        }

        if (*Smallest < *currentCDLib)
        {
            // Move Smallest before currentCDLib
            // Change linkings:
            // 1. (current-1).next = Smallest
            // 2. current.previous = Smallest
            // 3. (smallest-1).next = (Smallest+1)
            // 4. (smallest+1).previous = (Smallest-1)
            // 5. itsLastCDLib = (Smallest-1) if Smallest was last
            // 6. Smallest.next = current
            // 7. Smallest.previous = (current-1)
            TCDLib *previous = (TCDLib *) currentCDLib->GetPrevious();
            if (previous)
                previous->SetNext(Smallest);
            else
                itsFirstCDLib = Smallest;
            currentCDLib->SetPrevious(Smallest);
            if (Smallest->GetPrevious())
                Smallest->GetPrevious()->SetNext(Smallest->GetNext());
            if (Smallest->GetNext())
                Smallest->GetNext()->SetPrevious(Smallest->GetPrevious());
            if (itsLastCDLib == Smallest)
                itsLastCDLib = (TCDLib *) Smallest->GetPrevious();
            Smallest->SetNext(currentCDLib);
            Smallest->SetPrevious(previous);
             
            currentCDLib = Smallest;
        }
        else
            currentCDLib = (TCDLib *) currentCDLib->GetNext();
    }

    return itsErrorHandler->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// FromCD
//
// This method creates the field list from the CDLib list
//
///////////////////////////////////////////////////////////////////////////////
int TUMRecord::FromCD(TRuleFile *RuleFile)
{
    TMarcField      *Field;
    TCDLib          *CDLib;
    TCD             CD(itsErrorHandler);

    // On supprime l'arbre des champs
    DelTree();
    SortCD();
    if (RuleFile)
        RuleFile->GetEvaluateRule()->SortRecord(this);

    // On recherche le CD de tag '000', pour remplir le label
    CD.SetTag("000");
    TCDLib* Search=(TCDLib*)itsFirstCDLib;
    if (!NextCD(&Search,&CD))
    {
        char tmp[50];
        sprintf(tmp, "(record %ld)", itsErrorHandler->GetRecordNumber());
        itsErrorHandler->SetErrorD(7109, ERROR, tmp);
    }
    else
        SetLeader(Search->GetContent().str());

    // Traitement de tous les autres champs
    CDLib=itsFirstCDLib;
    while (CDLib)
    {
        // Si le CDLib courant identifie le label on continue, il a deja ete traite
        if (*CDLib->GetTag() != '0' || strcmp(CDLib->GetTag(), "000") != 0)
        {
            // Le CDLib courant identifie le debut d'un champ, on cree donc un nouveau champ
            TMarcField* NewField=new TMarcField;

            // On remplit le Tag
            NewField->SetTag(CDLib->GetTag());

            // Recherche des indicateurs du champ courant
            CD.SetTag(CDLib->GetTag());
            CD.SetTagOccurrenceNumber(CDLib->GetTagOccurrenceNumber());
            CD.SetSubfield(FIRST_INDICATOR);

            // On recherche I1
            Search=CDLib;
            if (NextCD(&Search,&CD))
                NewField->SetI1(*Search->GetContent().str());
            else
                NewField->SetI1(0);

            // On recherche I2
            Search=CDLib;
            CD.SetSubfield(SECOND_INDICATOR);
            if (NextCD(&Search,&CD))
                NewField->SetI2(*Search->GetContent().str());
            else
                NewField->SetI2(0);

            // Si I1 existe et pas I2 ou inversement, il y a une erreur dans le fichier de
            // regle vraisemblablement. Dans ce cas l'indicateur manquant est mis a ' '
            if (NewField->GetI1() && !NewField->GetI2())
            {
                char tmp[80];
                NewField->SetI2(' ');
                sprintf(tmp,"(record %ld) I2 is missing in field %s", itsErrorHandler->GetRecordNumber(), NewField->GetTag());
                itsErrorHandler->SetErrorD( 5004, WARNING, tmp );
            }

            if (NewField->GetI2() && !NewField->GetI1())
            {
                char tmp[80];
                NewField->SetI1(' ');
                sprintf(tmp,"(record %ld) I1 is missing in field %s", itsErrorHandler->GetRecordNumber(), NewField->GetTag());
                itsErrorHandler->SetErrorD( 5004, WARNING, tmp );
            }

            // Remplissage du libelle : on ne specifie pas de sous-champ dans CD,
            // GetContent va donc reconstruire le contenu complet du champ

            CD.SetSubfield(NO_SUBFIELD);
            NewField->SetLib(CDLib->GetContent(&CD));

            // Si un lib existe bien, on insere ce nouveau champ dans
            // la notice de sortie. Si on n'a pas encore de champ dans la notice,
            // on initialise itsFirstField, sinon on concatene au champ courant

            if (NewField->GetLib1())
            {
                if (*(NewField->GetLib1()))
                {
                    if (itsFirstField==NULL)
                        itsFirstField=Field=NewField;
                    else
                    {
                        Field->SetNextField(NewField);
                        Field=NewField;
                    }
                }
            }
            else
            {
                delete NewField;
            }

            // Pour passer au champ suivant, il faut avancer jusqu'a ce qu'on soit sur
            // un nouveau debut de champ
            while( CDLib && *CDLib==CD )
                CDLib = (TCDLib *)CDLib->GetNext();
        }
        else
        {
            CDLib = (TCDLib *)CDLib->GetNext();
        }
    }

    if (itsErrorHandler->GetHandleLinkedFields())
        SeparateLinkedFields();

    return itsErrorHandler->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// ToCD
//
// This method creates the CDLib list from the field list
//
///////////////////////////////////////////////////////////////////////////////
int TUMRecord::ToCD(void)
{
    TMarcField      *Field=itsFirstField;
    TCDLib          CDLib(itsErrorHandler);

    if (itsErrorHandler->GetHandleLinkedFields())
        MergeLinkedFields();

    // On supprime l'ancien arbre de CDLib
    DelTreeCDLib();

    // On cree un premier CDLib qui contiendra le label.
    // Ce CDLib sera identifie par un numero de champ '000'
    // On insere ensuite ce CDLib dans la liste des CDLib.
    CDLib.SetTag("000");
    CDLib.SetContent(itsLeader, "", "");
    CDLib.SetTagOccurrenceNumber(1);
    InsertCDLib(&CDLib);

    // On parcourt ensuite chaque champ de la notice, et on
    // cree un CDLib correspondant au champ complet.
    // Ce CDLib sera decoupe en autant de CDLib unitaire qu'il y
    // a de sous-champ dans le champ, lors de son insertion par
    // la methode InsereCDLib.
    //            -----------
    while (Field)
    {
        // On remplit le Tag
        CDLib.SetOccurrenceNumber(0);
        CDLib.SetTagOccurrenceNumber(0);
        CDLib.SetSubOccurrenceNumber(0);
        CDLib.SetTag(Field->GetTag());

        // Si le champ possede des indicateurs, on en cree deux CDLib
        if (*(Field->GetIndicators()))
        {
            // tmp sera une chaine de caractere contenant le premier indicateur
            char tmp[2];
            tmp[1]=0;
            CDLib.SetSubfield(FIRST_INDICATOR);
            *tmp=Field->GetI1();

            // On affecte le contenu du CDLib avec l'indicateur, et on insere ce
            // nouveau CDLib dans la liste des CDLibs
            CDLib.SetContent(tmp, "", "");
            InsertCDLib(&CDLib);

            // On fait ensuite de meme pour le second indicateur
            CDLib.SetSubfield(SECOND_INDICATOR);
            *tmp=Field->GetI2();
            CDLib.SetContent(tmp, "", "");
            InsertCDLib(&CDLib);
        }

        // On remplit alors le CDLib avec le contenu complet du champ, en ne specifiant aucun
        // sous-champ. Lors de l'insertion par InsereCDLib, ce CDLib sera decoupe en
        // CDLib unitaires
        CDLib.SetSubfield(NO_SUBFIELD);
        CDLib.SetContent(Field->GetLib1(), Field->GetLib2(), Field->GetScript());
        InsertCDLib(&CDLib);

        // On passe ensuite au champ suivant
        Field = Field->GetNextField();
    }

    return itsErrorHandler->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// NextCd
//
///////////////////////////////////////////////////////////////////////////////
bool TUMRecord::NextCD(TCDLib **CDLib, TCD *CD)
{
    // Check if a field with this code exists at all, unless a wildcard is used
    if (!CD->TagContainsWildcard())
    {
        const char *cdtag = CD->GetTag();
        long tagnum = atol(cdtag);
        if (tagnum == 0 && (cdtag[0] != '0' || cdtag[1] != '0' || cdtag[2] != '0'))
            tagnum = -1;
        if (tagnum >= 0 && tagnum <= 999 && !itsExistingFields[tagnum])
            return false;
    }

    TCDLib* Search = *CDLib;
    while (Search)
    {
        if (*Search == *CD)
        {
            *CDLib = Search;
            return true;
        }
        Search = (TCDLib *)Search->GetNext();
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//
// PreviousCd
//
///////////////////////////////////////////////////////////////////////////////
bool TUMRecord::PreviousCD(TCDLib **CDLib, TCD *CD)
{
    TCDLib* Search = *CDLib;
    while (Search)
    {
        if (*Search == *CD)
        {
            *CDLib = Search;
            return true;
        }
        Search = (TCDLib *)Search->GetPrevious();
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//
// DelTreeCDLib
//
///////////////////////////////////////////////////////////////////////////////
int TUMRecord::DelTreeCDLib()
{
    TCDLib      *CurrentCDLib,
                *NextCDLib;

    CurrentCDLib = itsFirstCDLib;
    while (CurrentCDLib)
    {
        NextCDLib = (TCDLib *)CurrentCDLib->GetNext();
        delete CurrentCDLib;
        CurrentCDLib = NextCDLib;
    }
    itsFirstCDLib=itsLastCDLib=NULL;
    memset(itsExistingFields, false, sizeof(itsExistingFields));
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// InsereCDLib
//
// Cette methode insere un nouveau CDLib dans l'arbre des CDLibs :
// - Si le CDLib a inserer est unitaire (sont contenu ne possede pas de sous-champ),
//   il faut simplement le "mettre" dans l'arbre
// - S'il n'est pas unitaure, on le decoupe en sous CDLibs unitaires (sur les sous-champs)
//   et on reapplique cette methode a ces sous CDLib.
//
// "Mettre" un CDLib dans l'arbre signifie :
// - Si ce CDLib existe deja dans l'arbre, modifier sont contenu (CDIn peut contenir
//   une plage itsBegining -> itsEnd qui correspond a la plage a mettre a jour) par
//   la methode SetContent de TCDLib, qui utilise le CDIn pour savoir quelle partie du
//   contenu est a mettre a jour.
// - Si le CDLib n'existe pas dans l'arbre, on en ajoute un nouveau, en remplissant sont
//   contenu comme ci-dessus, en tenant compte du CDIn.
//
///////////////////////////////////////////////////////////////////////////////
TCDLib *TUMRecord::InsertCDLib(TCDLib* aCDLib, TCD* CDIn, bool aReplace)
{
    // Si le Tag number (NT) du CDLib a inserer n'est pas defini, on va le chercher :
    // On se positionne sur le dernier CDLib de l'arbre et on recherche le dernier
    // CDLib ayant le meme Tag

    if (aCDLib->GetTagOccurrenceNumber()<=0)
    {
        // On cree un CD dans lequel on va juste renseigne le Tag

        TCD aCD(itsErrorHandler);
        aCD.SetTag(aCDLib->GetTag());

        // On se positionne sur le dernier CDLib de la notice et on cherche le dernier
        // CDLib correspondant au CD precedant, c'est-a-dire ayant le meme Tag
        // Si on trouve un CDLib, notre nouveau CDLib aura son TagOccurenceNumber+1
        // sinon notre nouveau CDLib aura 1 comme TagOccurenceNumber

        TCDLib* Search=itsLastCDLib;
        if (PreviousCD(&Search,&aCD))
            aCDLib->SetTagOccurrenceNumber(Search->GetTagOccurrenceNumber()+1);
        else
            aCDLib->SetTagOccurrenceNumber(1);

    }

    // If we're replacing a complete field, delete the old
    // contents first, because we can't match the subfields one at a time.
    if (CDIn && !*CDIn->GetSubfield() && CDIn->GetBeginning() == 0 && CDIn->GetEnd() == 0)
    {
        TCDLib* Search = itsFirstCDLib;
        while (Search)
        {
            TCDLib* Next = (TCDLib *) Search->GetNext();

            if (strcmp(Search->GetTag(), CDIn->GetTag()) == 0 &&
              Search->GetTagOccurrenceNumber() == CDIn->GetTagOccurrenceNumber() &&
              *Search->GetSubfield() != 'I')
            {
                TCDLib *Previous = (TCDLib *) Search->GetPrevious();
                if (Previous)
                    Previous->SetNext(Next);
                else
                    itsFirstCDLib = Next;
                if (Next) 
                    Next->SetPrevious(Previous);
                else
                    itsLastCDLib = Previous;
                delete Search;
            }

            Search = Next;
        }
    }

    // Try to split the CDLib into sub-CDLib's (subfields).
    // If subfields are found, take one at a time and call this function recursively to insert it, then 
    // return.

    TCDLib* aSubCDLib;
    bool subfields_found = false;
    int pos=0, pos2=0;
    char sf[3];
    if (aCDLib->GetSubfield()) 
        strcpy(sf, aCDLib->GetSubfield());
    else 
        *sf = '\0';
    int sub_ns = CDIn ? CDIn->GetSubOccurrenceNumber() : 0;
    int sub_n = CDIn ? CDIn->GetOccurrenceNumber() : 0;
    TCDLib *insertedCDLib = NULL;
    while (aCDLib->NextSubCDLib(&aSubCDLib, &pos, &pos2, sf))
    {
        if (CDIn)
        {
            TCD CopyIn(CDIn);
            CopyIn.SetSubfield(aSubCDLib->GetSubfield());
            if (aReplace)
            {
                aSubCDLib->SetSubOccurrenceNumber(sub_ns);
                CopyIn.SetSubOccurrenceNumber(sub_ns);
                if (subfields_found)
                {
                    // Need to add other subfields after the first one replaced.
                    TCDLib *nextCDLib = (TCDLib *) insertedCDLib->GetNext();
                    TCDLib *newCDLib = new TCDLib(aCDLib, CDIn);
                    insertedCDLib->SetNext(newCDLib);
                    if (nextCDLib)
                    {
                        nextCDLib->SetPrevious(newCDLib);
                        newCDLib->SetNext(nextCDLib);
                    }
                    else
                    {
                        itsLastCDLib = newCDLib;
                    }
                    newCDLib->SetPrevious(insertedCDLib);

                    int occurrence = 0;
                    int sub_occurrence = 0;
                    GetOccurrenceNumbersForNew(aCDLib->GetTag(), aCDLib->GetTagOccurrenceNumber(), 
                        aSubCDLib->GetSubfield(), occurrence, sub_occurrence);

                    newCDLib->SetOccurrenceNumber(occurrence);
                    newCDLib->SetSubOccurrenceNumber(sub_occurrence);
                    newCDLib->SetSubfield(aSubCDLib->GetSubfield());
                    newCDLib->SetContent(aSubCDLib->GetContent());
                    
                    insertedCDLib = newCDLib;
                    delete aSubCDLib;
                    continue;
                }
            }
            else
            {
                aSubCDLib->SetSubOccurrenceNumber(0);
            }
            insertedCDLib = InsertCDLib(aSubCDLib,&CopyIn);
        }
        else
        {
            if (!aReplace)
                aSubCDLib->SetSubOccurrenceNumber(0);
            InsertCDLib(aSubCDLib);
        }

        subfields_found = true;
        delete aSubCDLib;
    }
    if (subfields_found) 
        return insertedCDLib;

    // The CDLib is a single subfield and will be inserted in the tree.
    // NT is specified (non-specified was processed at the beginning of this method).
    // Two possible cases remain:
    // - If NT is filled or subfield is not specified (in which case CDLib contains fixed fields),
    //   this CDLib is searched for in the record. If it's found, it is updated. If not, it is added.
    // - Otherwise a new CDLib is added.

    TCDLib* Search = itsFirstCDLib;
    bool subfieldFound = false;
    if (!*(aCDLib->GetSubfield()) || aCDLib->GetSubOccurrenceNumber() != 0)
    {
        while(Search)
        {
            subfieldFound = NextCD(&Search,(TCD*)aCDLib);
            if (!*(aCDLib->GetSubfield()) && *(Search->GetSubfield()))
            {
                Search = (TCDLib*)Search->GetNext();
                subfieldFound = false;
            }
            else
                break;
        }
    }
    if (subfieldFound)
    {
        // Corresponding CDLib already exists, update it
        Search->SetContent(aCDLib->GetContent(), CDIn);
        return Search;
    }
    else
    {
        // Corresponding CDLib doesn't exists, add as new

        const char *cdtag = aCDLib->GetTag();
        long tagnum = atol(cdtag);
        if (tagnum == 0 && (cdtag[0] != '0' || cdtag[1] != '0' || cdtag[2] != '0'))
            tagnum = -1;
        if (tagnum >= 0 && tagnum <= 999)
          itsExistingFields[tagnum] = true;

        if (itsFirstCDLib==NULL)
        {
            // Add the first CDLib
            itsFirstCDLib=itsLastCDLib=new TCDLib(aCDLib,CDIn);
            if (!itsFirstCDLib)
                itsErrorHandler->SetErrorD(5000,ERROR,"When creating the first new CDLib");
            itsFirstCDLib->SetPrevious(NULL);
            itsFirstCDLib->SetNext(NULL);
            itsLastCDLib->SetOccurrenceNumber(1);
            if (*(itsLastCDLib->GetSubfield()))
                itsLastCDLib->SetSubOccurrenceNumber(1);
            else
                itsLastCDLib->SetSubOccurrenceNumber(0);
        }
        else
        {
            // Add a new CDLib in the end of the list
            int occurrence = 0;
            int sub_occurrence = 0;
            GetOccurrenceNumbersForNew(aCDLib->GetTag(), aCDLib->GetTagOccurrenceNumber(), 
                aCDLib->GetSubfield(), occurrence, sub_occurrence);
            
            itsLastCDLib->SetNext((TCD*)(new TCDLib(aCDLib,CDIn)));
            if (!itsLastCDLib->GetNext())
                itsErrorHandler->SetErrorD(5000,ERROR,"When adding a new CDLib");
            itsLastCDLib->GetNext()->SetPrevious(itsLastCDLib);
            itsLastCDLib=(TCDLib*)itsLastCDLib->GetNext();

            itsLastCDLib->SetOccurrenceNumber(occurrence);
            if (*(itsLastCDLib->GetSubfield()))
                itsLastCDLib->SetSubOccurrenceNumber(sub_occurrence);
        }
        return itsLastCDLib;
    }
}

void TUMRecord::GetOccurrenceNumbersForNew(const char *a_tag, int a_tag_occurrence, const char *a_subfield, 
                                           int &a_occurrence, int &a_sub_occurrence)
{
    TCD aCD(itsErrorHandler);
    aCD.SetTag(a_tag);
    aCD.SetSubfield(a_subfield);

    // Find backwards from the end the previous subfield with same tag and subfield code 
    
    bool subfieldFound = false;
    TCDLib* Search = itsLastCDLib;
    while (Search)
    {
        subfieldFound = PreviousCD(&Search, &aCD);
        if (!*a_subfield && *(Search->GetSubfield()))
        {
            Search = (TCDLib*) Search->GetPrevious();
            subfieldFound = false;
        }
        else
            break;
    }
    if (subfieldFound)
    {
        a_occurrence = Search->GetOccurrenceNumber() + 1;
        if (Search->GetTagOccurrenceNumber() == a_tag_occurrence)
            a_sub_occurrence = Search->GetSubOccurrenceNumber() + 1;
        else
            a_sub_occurrence = 1;
    }
    else
    {
        a_occurrence = 1;
        a_sub_occurrence = 1;
    }
}

void TUMRecord::MergeLinkedFields()
{
    RegExp re(START_OF_FIELD_STR "6(\\d+)\\-(\\d+)([^" START_OF_FIELD_STR "]*)");

    // Iterate all 880 fields
    TMarcField* field = itsFirstField;
    TMarcField* prev_field = NULL;
    while (field)
    {
        if (strcmp(field->GetTag(), "880") != 0)
        {
            prev_field = field;
            field = field->GetNextField();
            continue;
        }
        const char* data = field->GetLib1();
        if (re.exec(data) <= 0)
        {
            typestr tmp = "Notice '";
            tmp += field->GetLib1();
            tmp += "' : field '";
            tmp += field->GetTag();
            tmp += "'";
            itsErrorHandler->SetErrorD(2601, WARNING, tmp.str());
        }
        else
        {
            typestr fieldcode;
            typestr occurrence;
            typestr script;
            re.match(1, fieldcode);
            re.match(2, occurrence);
            re.match(3, script);

            // Find linked field
            bool match_found = false;
            TMarcField* linked_field = itsFirstField;
            while (NextField(&linked_field, fieldcode.str()) == 0)
            {
                const char* linked_data = linked_field->GetLib1();

                if (re.exec(linked_data) > 0)
                {
                    typestr linked_occurrence;
                    re.match(2, linked_occurrence);
                    if (occurrence == linked_occurrence)
                    {
                        // We have a match
                        match_found = true;
                        linked_field->SetLib2(data);
                        linked_field->SetScript(script.str());
                        break;
                    }
                }
                linked_field = linked_field->GetNextField();
            }
            if (!match_found)
            {
                typestr tmp = "Notice '";
                tmp += field->GetLib1();
                tmp += "' : field '";
                tmp += field->GetTag();
                tmp += "'";
                itsErrorHandler->SetErrorD(2602, WARNING, tmp.str());
            }
        }
        // Delete the 880 field
        TMarcField *tmp_field = field;
        field = field->GetNextField();
        if (!prev_field)
            itsFirstField = field;
        else
            prev_field->SetNextField(field);
        delete tmp_field;
    }
}

void TUMRecord::SeparateLinkedFields()
{
    RegExp re_replace(START_OF_FIELD_STR "6.*?" START_OF_FIELD_STR);
    RegExp re_add_rep(START_OF_FIELD_STR);
    TMarcField* field = itsFirstField;
    TMarcField* prev_field880 = NULL;
    int field_num = 0;
    while (field)
    {
        if (field->GetLib2() && *field->GetLib2())
        {
            ++field_num;
            
            char link_normal[30], link_880[30];
            sprintf(link_normal, START_OF_FIELD_STR "6880-%02d" START_OF_FIELD_STR, field_num);
            sprintf(link_880, START_OF_FIELD_STR "6%03s-%02d%s" START_OF_FIELD_STR, field->GetTag(), field_num, field->GetScript());

            typestr fielddata = field->GetLib2();
            int res = re_replace.replace(fielddata, link_880, false);
            if (res < 0)
                itsErrorHandler->SetError(5601, WARNING);
            if (res == 0)
            {
                res = re_add_rep.replace(fielddata, link_880, false);
                if (res < 0)
                    itsErrorHandler->SetError(5601, WARNING);
            }

            TMarcField* field880 = new TMarcField();
            field880->SetTag("880");
            field880->SetIndicators(field->GetIndicators());
            field880->SetLib1(fielddata.str());
            field880->SetLib2(NULL);

            if (prev_field880)
            {
                field880->SetNextField(prev_field880->GetNextField());
                prev_field880->SetNextField(field880);
            }
            else
            {
                // Find correct position
                TMarcField* field_pos = itsFirstField;
                while (field_pos)
                {
                    if (strcmp(field_pos->GetTag(), "880") >= 0)
                        break;
                    TMarcField* next = field_pos->GetNextField();
                    if (next)
                        field_pos = next;
                    else
                        break;
                }
                if (field_pos)
                {
                    field880->SetNextField(field_pos->GetNextField());
                    field_pos->SetNextField(field880);
                }
            }
            prev_field880 = field880;

            fielddata = field->GetLib1();
            res = re_replace.replace(fielddata, link_normal, false);
            if (res < 0)
                itsErrorHandler->SetError(5601, WARNING);
            if (res == 0)
            {
                res = re_add_rep.replace(fielddata, link_normal, false);
                if (res < 0)
                    itsErrorHandler->SetError(5601, WARNING);
            }
            field->SetLib1(fielddata.str());
        }
        field->SetLib2(NULL);
        field = field->GetNextField();
    }
}
