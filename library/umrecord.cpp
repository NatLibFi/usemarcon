/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2007
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

extern int debug_umrecord;

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
// Cette methode cree l'arbre des champs a partir de l'arbre des CDLibs
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

    if (debug_umrecord)
    {
        printf("Dans FromCD :\n");
        PrintCD();
    }

    // On recherche le CD de tag '000', pour remplir le label
    CD.SetTag("000");
    TCDLib* Search=(TCDLib*)itsFirstCDLib;
    if (!NextCD(&Search,&CD))
        itsErrorHandler->SetError(9202,ERROR);
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
                sprintf(tmp,"I2 is missing in field %s",NewField->GetTag());
                itsErrorHandler->SetErrorD( 5004, WARNING, tmp );
            }

            if (NewField->GetI2() && !NewField->GetI1())
            {
                char tmp[80];
                NewField->SetI1(' ');
                sprintf(tmp,"I1 is missing in field %s",NewField->GetTag());
                itsErrorHandler->SetErrorD( 5004, WARNING, tmp );
            }

            // Remplissage du libelle : on ne specifie pas de sous-champ dans CD,
            // GetContent va donc reconstruire le contenu complet du champ

            CD.SetSubfield(NO_SUBFIELD);
            NewField->SetLib(CDLib->GetContent(&CD).str());


            // Si un lib existe bien, on insere ce nouveau champ dans
            // la notice de sortie. Si on n'a pas encore de champ dans la notice,
            // on initialise itsFirstField, sinon on concatene au champ courant

            if (NewField->GetLib())
            {
                if (*(NewField->GetLib()))
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
    return itsErrorHandler->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// ToCD
//
// Cette methode cree l'arbre des CDLib a partir de l'arbre des champs (Field)
//
///////////////////////////////////////////////////////////////////////////////
int TUMRecord::ToCD(void)
{
    TMarcField      *Field=itsFirstField;
    TCDLib          CDLib(itsErrorHandler);

    // On supprime l'ancien arbre de CDLib
    DelTreeCDLib();

    // On cree un premier CDLib qui contiendra le label.
    // Ce CDLib sera identifie par un numero de champ '000'
    // On insere ensuite ce CDLib dans la liste des CDLib.
    CDLib.SetTag("000");
    CDLib.SetContent(itsLeader);
    CDLib.SetTagOccurrenceNumber(1);
    InsereCDLib(&CDLib);  // if != 0 ERREUR

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
            CDLib.SetContent(tmp);
            InsereCDLib(&CDLib);

            // On fait ensuite de meme pour le second indicateur
            CDLib.SetSubfield(SECOND_INDICATOR);
            *tmp=Field->GetI2();
            CDLib.SetContent(tmp);
            InsereCDLib(&CDLib);
        }

        // On remplit alors le CDLib avec le contenu complet du champ, en ne specifiant aucun
        // sous-champ. Lors de l'insertion par InsereCDLib, ce CDLib sera decoupe en
        // CDLib unitaires
        CDLib.SetSubfield(NO_SUBFIELD);
        CDLib.SetContent(Field->GetLib());
        InsereCDLib(&CDLib);

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
int TUMRecord::NextCD(TCDLib **CDLib, TCD *CD)
{
    // Check if a field with this code exists at all, unless a wildcard is used
    if (!CD->TagContainsWildcard())
    {
        char *cdtag = CD->GetTag();
        long tagnum = atol(cdtag);
        if (tagnum == 0 && (cdtag[0] != '0' || cdtag[1] != '0' || cdtag[2] != '0'))
            tagnum = -1;
        if (tagnum >= 0 && tagnum <= 999 && !itsExistingFields[tagnum])
            return 0;
    }

    TCDLib* Search=*CDLib;
    while (Search)
    {
        if (*Search==*CD)
        {
            *CDLib=Search;
            return 1;
        }
        Search = (TCDLib *)Search->GetNext();
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// PreviousCd
//
///////////////////////////////////////////////////////////////////////////////
int TUMRecord::PreviousCD(TCDLib **CDLib, TCD *CD)
{
    TCDLib* Search=*CDLib;
    while (Search)
    {
        if (*Search==*CD)
        {
            *CDLib=Search;
            return 1;
        }
        Search = (TCDLib *)Search->GetPrevious();
    }
    return 0;
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
int TUMRecord::InsereCDLib(TCDLib* aCDLib, TCD* CDIn, int Replace)
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

    // Try to split the CDLib into sub-CDLib's (subfields).
    // If subfields are found, take one at a time and call this function recursively to insert it, then 
    // return.

    TCDLib* aSubCDLib;
    bool subfields_found = false;
    int pos=0;
    char sf[3];
    if (aCDLib->GetSubfield()) strcpy(sf,aCDLib->GetSubfield());
    else *sf=0;
    while( aCDLib->NextSubCDLib(&aSubCDLib,&pos,sf) )
    {
        if (CDIn)
        {
            TCD CopyIn(CDIn);
            CopyIn.SetSubfield(aSubCDLib->GetSubfield());
            if (Replace)
            {
                CopyIn.SetSubOccurrenceNumber(aSubCDLib->GetSubOccurrenceNumber());
            }
            else
            {
                aSubCDLib->SetSubOccurrenceNumber(0);
            }
            InsereCDLib(aSubCDLib,&CopyIn);
        }
        else
        {
            if (!Replace)
                aSubCDLib->SetSubOccurrenceNumber(0);
            InsereCDLib(aSubCDLib);
        }

        subfields_found = true;
        delete aSubCDLib;
    }
    if (subfields_found) 
        return itsErrorHandler->GetErrorCode();

    // Si le CDLib est unitaire on l'insere dans l'arbre.
    // On sait que NT est rempli (on l'a fait au debut de cette methode s'il ne l'etait pas)
    // par contre, deux cas restent possibles pour NS
    // - S'il est rempli ou si il n'y a pas de Subfield de precise (auquel cas le CDLib
    //   correspond a un champ fixe), on recherche ce CDLib dans la notice. Si on en trouve
    //   un, on le met a jour (SetContent), sinon on l'ajoute.
    // - S'il n'est pas rempli alors qu'un subfield existe, on ajoute un nouveau CDLib.

    TCDLib* Search=itsFirstCDLib;
    int isDefined=1,found=0;
    if (*(aCDLib->GetSubfield()) && aCDLib->GetSubOccurrenceNumber()==0) isDefined=0;
    if (isDefined)
    {
        while(Search)
        {
            found=NextCD(&Search,(TCD*)aCDLib);
            if (!*(aCDLib->GetSubfield()) && *(Search->GetSubfield()))
            {
                Search=(TCDLib*)Search->GetNext();
                found=0;
            }
            else
                break;
        }
    }
    if (found)
    {
        // Un CDLib correspondant existe deja dans la notice, on le met a jour
        Search->SetContent(aCDLib->GetContent().str(), CDIn);
    }
    else
    {
        // Pas de CDLib existant, on l'ajoute dans l'arbre

        const char *cdtag = aCDLib->GetTag();
        long tagnum = atol(cdtag);
        if (tagnum == 0 && (cdtag[0] != '0' || cdtag[1] != '0' || cdtag[2] != '0'))
            tagnum = -1;
        if (tagnum >= 0 && tagnum <= 999)
          itsExistingFields[tagnum] = true;

        if (itsFirstCDLib==NULL)
        {
            // C'est le premier CDLib qu'on ajoute

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
            // L'arbre existe deja, on ajoute notre nouveau CDLib a la fin

            itsLastCDLib->SetNext((TCD*)(new TCDLib(aCDLib,CDIn)));
            if (!itsLastCDLib->GetNext())
                itsErrorHandler->SetErrorD(5000,ERROR,"When adding a new CDLib");
            itsLastCDLib->GetNext()->SetPrevious(itsLastCDLib);
            itsLastCDLib=(TCDLib*)itsLastCDLib->GetNext();


            // On va maintenant completer notre nouveau CDLib, en lui renseignant
            // ses Occurence Number et Subfield Occurence Number
            // Pour cela, on remplit un CD avec le Tag et le subfield, afin de
            // rechercher le dernier CDLib correspondant

            TCD aCD(itsErrorHandler);
            aCD.SetTag(itsLastCDLib->GetTag());
            aCD.SetSubfield(itsLastCDLib->GetSubfield());

            // On se positionne a la fin de la notice, et on recherche le dernier
            // CDlib ayant meme Tag et meme SubField

            found=0;
            TCDLib* Search=(TCDLib*)itsLastCDLib->GetPrevious();
            while(Search)
            {
                found=PreviousCD(&Search,&aCD);
                if (!*(aCD.GetSubfield()) && *(Search->GetSubfield()))
                {
                    Search=(TCDLib*)Search->GetPrevious();
                    found=0;
                }
                else
                    break;
            }
            if (found)
            {
                // Si on trouve au CDLib correspondant, l'occurence number de notre
                // nouveau CDLib est celle de celui trouve +1

                itsLastCDLib->SetOccurrenceNumber(Search->GetOccurrenceNumber()+1);


                // Si notre CDLib possede un Subfield, deux cas sont possibles :
                // - le dernier CDLib trouve est dans le meme Tag (TagOccurenceNumber egaux)
                //   auquel cas le subfield occurence number de notre nouveau CDLib est celui
                //   du CDLib trouve + 1
                // - le dernier CDLib trouve n'est pas dans le meme Tag, notre CDLib est donc le
                //   premier avec ce subfield dans le champ courant, son subfield occurence number
                //   doit donc valoir 1
                // Si notre CDLib n'a pas de SubField, son subfield occurence number est indefini (0)

                if (*(itsLastCDLib->GetSubfield()))
                {
                    if (Search->GetTagOccurrenceNumber()==itsLastCDLib->GetTagOccurrenceNumber())
                        itsLastCDLib->SetSubOccurrenceNumber(Search->GetSubOccurrenceNumber()+1);
                    else
                        itsLastCDLib->SetSubOccurrenceNumber(1);
                }
                else
                    itsLastCDLib->SetSubOccurrenceNumber(0);

            }
            else
            {
                // On n'a pas trouve de CDLib correspondant. Son occurence Number est donc 1,
                // de meme que son subfield occurence number, si le Subfield est renseigne. Si
                // le subfield n'est pas renseigne, son subfield occurence number est indefini (0)

                itsLastCDLib->SetOccurrenceNumber(1);
                if (*(itsLastCDLib->GetSubfield()))
                    itsLastCDLib->SetSubOccurrenceNumber(1);
                else
                    itsLastCDLib->SetSubOccurrenceNumber(0);
            }
        }
    }
    return itsErrorHandler->GetErrorCode();
}

void TUMRecord::PrintCD()
{
    TCDLib* Courant=itsFirstCDLib;
    while (Courant)
    {
        printf("%-3s(%d)%-2s[%d]\t{%d}\t<%s>\n",
            Courant->GetTag(), Courant->GetTagOccurrenceNumber(),
            Courant->GetSubfield(), Courant->GetSubOccurrenceNumber(),
            Courant->GetOccurrenceNumber(), Courant->GetContent().str());
        Courant=(TCDLib*)Courant->GetNext();
    }
}
