/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  trnsfile.cpp
 *
 *  implements a class to manage transcoding file
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
TTransFile::TTransFile(typestr & FileSpec, TError *ErrorHandler)
: TFile(FileSpec, ErrorHandler), semi()
{
    itsFileInfo = FileSpec;
    itsDefaultValueValid = false;
    itsDefaultCopy = false;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TTransFile
//
///////////////////////////////////////////////////////////////////////////////
TTransFile::~TTransFile()
{
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
    typestr         aSpec;

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
                if (strstr(OutChar, "COPY"))
                {
                    itsDefaultCopy = true;
                }
                else
                {
                    GetValues(OutChar,&OutListe);
                    itsDefaultValue = OutListe;
                    itsDefaultValueValid = true;
                }
            }
            else
            {
                // Affectation des valeurs
                GetValues(InChar,&InListe);
                GetValues(OutChar,&OutListe);

                // Creation de l'arbre lexicographique
                if (*InListe.str())
                    insere((unsigned char*)InListe.str(), (unsigned char*)l_strdup(OutListe.str()));

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

    // On commence par effectuer la recopie (sans transco) du label
    MarcOut->SetLeader(MarcIn->GetLeader());
    if (!MarcOut->GetLeader())
    {
        char rec[50];
        sprintf(rec, "(record %ld)", itsErrorHandler->GetRecordNumber());
        typestr tmp = typestr(rec) + " Leader '";
        tmp += MarcIn->GetLeader();
        tmp += '\'';
        itsErrorHandler->SetErrorD(3000, ERROR, tmp.str());
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
        Transcode(In->GetLib1(), result, In->GetLib1(), In->GetTag());
        Out->SetLib1(result.str());
        Transcode(In->GetLib2(), result, In->GetLib2(), In->GetTag());
        Out->SetLib2(result.str());
        if (!Out->GetLib1())
        {
            char rec[50];
            sprintf(rec, "(record %ld)", itsErrorHandler->GetRecordNumber());
            typestr tmp = typestr(rec) + " Field ";
            tmp += In->GetTag();
            tmp += ": '";
            tmp += In->GetLib1();
            tmp += '\'';
            itsErrorHandler->SetErrorD(5006, ERROR, tmp.str());
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
bool TTransFile::Transcode(char* In, typestr &Out, char *Notice, char *Field)
{
    char *c = In;
    char *r = NULL;

    Out.str("");

    if (!In)
        return true;

    bool res = true;
    while(*c != '\0')
    {
        int iaux;
        r  = (char *)cherche((unsigned char*)c,&iaux);
        if((iaux == 0) || (r == NULL))
        {
            if (itsDefaultCopy)
            {
                Out.append_char(*c);
                ++c;
                continue;
            }
            else if (itsDefaultValueValid)
            {
                Out.append(itsDefaultValue.str());
                ++c;
                continue;
            }

            // aucune chaine de sustitution n'a ete trouvee, on conserve la valeur
            // en signalant l'erreur
            typestr tmp;
            char c_hex[30];
            sprintf(c_hex, "%x", *c);
            if (Notice && Field)
            {
                tmp = "Notice '";
                tmp += Notice;
                tmp += "' : field '";
                tmp += Field;
                tmp += "' ( Unknown character '";
                tmp += *c;
                tmp += "' (\\";
                tmp += c_hex;
                tmp += ") ) : table '";
                tmp += itsFileInfo;
                tmp += '\'';
            }
            else
            {
                tmp = "( Unknown character '";
                tmp += *c;
                tmp += "' (\\";
                tmp += c_hex;
                tmp += ") ) : table '";
                tmp += itsFileInfo;
                tmp += '\'';
            }
            itsErrorHandler->SetErrorD(3001, ERROR, tmp.str());
            Out.append_char(*c);
            c++;
            res = false;
        }
        else
        {
            /* on a trouve une correspondance */
            Out.append(r);
            c+=iaux;
        }
    }
    return res;
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

bool TTransFile::Exists(char* In)
{
    char *c = In;

    if (!In)
        return true;

    while(*c != '\0')
    {
        int iaux;
        char *r  = (char *)cherche((unsigned char*)c,&iaux);
        if((iaux == 0) || (r == NULL))
        {
            return false;
        }
        else
        {
            /* on a trouve une correspondance */
            c+=iaux;
        }
    }
    return true;
}
