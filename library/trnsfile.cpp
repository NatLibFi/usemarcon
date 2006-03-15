/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2006
 *
 *  File:  trnsfile.cpp
 *
 *  implements a class to manage transcoding file
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include <malloc.h>
#include "trnsfile.h"
#include "error.h"
#include "tmarcrec.h"

///////////////////////////////////////////////////////////////////////////////
//
// TTransFile
//
///////////////////////////////////////////////////////////////////////////////
TTransFile::TTransFile( FILE_SPEC *FileSpec, TError *ErrorHandler)
: TFile( FileSpec, ErrorHandler), semi()
{
    itsFileInfo=new FILE_SPEC;
    if (itsFileInfo)
        memcpy(itsFileInfo,FileSpec,sizeof(FILE_SPEC));
    itsDefaultValueValid = false;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TTransFile
//
///////////////////////////////////////////////////////////////////////////////
TTransFile::~TTransFile()
{
    if (itsFileInfo) delete itsFileInfo;
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
///////////////////////////////////////////////////////////////////////////////
int TTransFile::Open()
{
    typestr         aLine;
    char*           InChar;
    char*           OutChar;
    typestr         InListe;
    typestr         OutListe;
    int             Line;
    FILE_SPEC       aSpec;

    // Ouverture du fichier
    if (TFile::Open())
        return 1;

    // Lecture de chaque ligne du fichier
    while( NextLine( &aLine, &aSpec, &Line )==0)
    {
        // Si la ligne n'est pas vide, la traiter
        if (*aLine.str())
        {
            // Decoupage sur le |
            InChar=OutChar=aLine.str();
            while(*OutChar && *OutChar!='|') ++OutChar;
            if (*OutChar) ++OutChar;

            // Test de la valeur par defaut
            if (memcmp(InChar,"#DEFAULT",8)==0)
            {
                GetValues(OutChar,&OutListe);
                itsDefaultValue = OutListe;
                itsDefaultValueValid = true;
            }
            else
            {
                // Affectation des valeurs
                GetValues(InChar,&InListe);
                GetValues(OutChar,&OutListe);

                // Creation de l'arbre lexicographique
                if (*InListe.str())
                    insere((unsigned char*)InListe.str(), (unsigned char*)strdup(OutListe.str()));

            }
        }
    }

    // On referme le fichier
    TFile::Close();
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
//
// Convert
//
// Cette methode convertit une notice en une autre conformement au fichier de
// transco charg‰
//
///////////////////////////////////////////////////////////////////////////////
int TTransFile::Convert( TMarcRecord* MarcIn, TMarcRecord* MarcOut )
{
    TMarcField* In;
    TMarcField* Out;

    // Recuperation de la cle de la notice dans itsErrorHandler->Temporary2
    strcpy((char *) itsErrorHandler->Temporary2,MarcIn->GetFirstField()->GetLib());

    // On commence par effectuer la recopie (sans transco) du label
    MarcOut->SetLabel(MarcIn->GetLabel());
    if (!MarcOut->GetLabel())
    {
        sprintf((char *) itsErrorHandler->Temporary,"Notice '%s' : label '%s'",(char *)itsErrorHandler->Temporary2,MarcIn->GetLabel());
        itsErrorHandler->SetErrorD( 3000, ERROR, (char *) itsErrorHandler->Temporary );
    }

    // On transcode ensuite chaque champ
    In=MarcIn->GetFirstField();
    Out=MarcOut->GetFirstField();
    while( In!=NULL )
    {
        // On cree un nouveau champ en sortie
        if (Out==NULL)
        {
            MarcOut->SetFirstField(new TMarcField);
            Out=MarcOut->GetFirstField();
        }
        else
        {
            Out->SetNextField(new TMarcField);
            Out=Out->GetNextField();
        }

        // On recopie le tag et les indicateurs du champ en entree
        Out->SetTag(In->GetTag());
        Out->SetIndicators(In->GetIndicators());

        // On transcode le libelle
        typestr result;
        Out->SetLib(Transcode(In->GetLib(), &result, In->GetLib(), In->GetTag()));
        if (!Out->GetLib())
        {
            sprintf((char *)itsErrorHandler->Temporary,"Notice '%s' : field '%s'",In->GetLib(),In->GetTag());
            itsErrorHandler->SetErrorD(5006, ERROR, result.str());
        }

        // On passe au champ suivant
        In=In->GetNextField();
    }

    return itsErrorHandler->GetErrorCode();
}


///////////////////////////////////////////////////////////////////////////////
//
// Transcode
//
// Cette methode transcode une chaine de caractere en une autre, conformement
// a la table chargee - inspire de ttrans.cc
//
///////////////////////////////////////////////////////////////////////////////
const char* TTransFile::Transcode( char* In, typestr *Out, char *Notice, char *Field )
{
    char *c = In;
    char *r = NULL;

    Out->str("");

    while(*c != '\0')
    {
        int iaux;
        r  = (char *)cherche((unsigned char*)c,&iaux);
        if((iaux == 0) || (r == NULL))
        {
            if (itsDefaultValueValid)
            {
                Out->append(itsDefaultValue.str());
                ++c;
                continue;
            }

            // aucune chaine de sustitution n'a ete trouvee, on conserve la valeur
            // en signalant l'erreur
            char tmp[2000];
            //DanX, Talis: copy of Notice reduced to 200 characters to prevent overrun of tmp with large data fields.
            char tempNotice[200];
            memcpy(tempNotice, Notice, 200);
            tempNotice[199] = '\0';

            if (Notice && Field)
                sprintf(tmp, "Notice '%s' : field '%s' ( Unknown character '%c' (\\%x) ) : table '%s'",tempNotice,Field,(unsigned char)*c,(unsigned char)*c,itsFileInfo->name);
            else
                sprintf(tmp, "( Unknown character '%c' (\\%x) ) : table '%s'",(unsigned char)*c,(unsigned char)*c,itsFileInfo->name);
            itsErrorHandler->SetErrorD( 3001, ERROR, tmp );
            Out->append_char(*c);
            c++;
        }
        else
        {
            /* on a trouve une correspondance */
            Out->append(r);
            c+=iaux;
        }
    }
    return(Out->str());
}

///////////////////////////////////////////////////////////////////////////////
//
// GetValues
//
///////////////////////////////////////////////////////////////////////////////
int TTransFile::GetValues(const char *src, typestr *dest)
{
    const char *c = src;
    int fin=0,iaux;
    dest->str("");

    while(!fin)
    {
        // On saute les caracteres non representatifs
        while((*c == ' ')||(*c == '\t')) c++;

        switch(*c)
        {
        case 0:
        case '\n':
        case '|':
            fin = 1;
            break;

        case '0':
            /* le prochain caractere est represente par un code hexa */
            sscanf(c,"%i",&iaux);
            dest->append_char((char) iaux);
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            sscanf(c,"%d",&iaux);
            dest->append_char((char) iaux);
            break;

        default:
            dest->append_char(*c);
            break;
        }
        /* on avance jusqu'au prochain espace */
        while((*c != '\0') && (*c != ' ') && (*c != '\t')) c++;
    }
    return 0;
}

