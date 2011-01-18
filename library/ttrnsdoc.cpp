/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2011
 *
 *  File:  ttrnsdoc.cpp
 *
 *  implements a class to manage file transcoding files
 *
 */

#include "statemanager.h"
#include "defines.h"
#include "ttrnsdoc.h"
#include "tmpplctn.h"
#include "marc8.h"

///////////////////////////////////////////////////////////////////////////////
//
// Document class constructor
//
// Create and initialize the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TTransDoc::TTransDoc(TStateManager *StateManager)
{
    itsFile         = NULL;
    itsTransSpec    = NULL;
    mStateManager = StateManager;
    itsCharset = CHARSET_DEFAULT;
}

///////////////////////////////////////////////////////////////////////////////
//
// Document class destructor
//
// Delete the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TTransDoc::~TTransDoc( void )
{
    if (itsFile)        
    { 
        delete itsFile;       
        itsFile = NULL; 
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
// Called by DoOpen().  Do the actual work of restoring the object from
// a data source.
//
///////////////////////////////////////////////////////////////////////////////
bool TTransDoc::Open(void)
{
    // Add code here to set internal data members of the class based
    // on information read from some data source
    if ((itsFile=new TTransFile(itsFilePointer, mStateManager))==NULL)
    {
        mStateManager->SetError(9013,ERROR);
        return false;
    }
    if (itsFile->Open())
    {
        delete itsFile;
        itsFile = NULL;
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// OpenTransFile
//
///////////////////////////////////////////////////////////////////////////////
bool TTransDoc::OpenTransFile(char *DefaultFile)
{
    if (DefaultFile)
    {
        itsFilePointer = DefaultFile;
    }
    
    if (Open()) // Open the selected Rule file
    {
        itsTransSpec = itsFilePointer;
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//
// SeInputFileCharacterSet
//
///////////////////////////////////////////////////////////////////////////////
bool TTransDoc::SetInputFileCharacterSet(const char *charset)
{
    if (*charset == '\0' || strcasecmp(charset, "UTF-8") == 0 || strcasecmp(charset, "UTF8") == 0 || strcasecmp(charset, "DEFAULT") == 0)
    {
        itsCharset = CHARSET_DEFAULT;
    }
    else if (strcasecmp(charset, "MARC-8") == 0 || strcasecmp(charset, "MARC8") == 0)
    {
        itsCharset = CHARSET_MARC8;
    }
    else
    {
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Convert
//
///////////////////////////////////////////////////////////////////////////////
int TTransDoc::Convert(TMarcRecord* In, TMarcRecord* Out)
{
    if (itsCharset != CHARSET_DEFAULT && itsFile)
    {
        TMarcRecord *record = new TUMRecord(mStateManager->GetApplication());
        if (!record)
        {
            mStateManager->SetErrorD( 3000, ERROR, "When creating a new Trans record" );
        }
        ConvertToUTF8(In, record);
        return itsFile->Convert(record, Out);
    }
    else if (itsCharset != CHARSET_DEFAULT)
    {
        return ConvertToUTF8(In, Out);
    }
    else if (itsFile)
    {
        return itsFile->Convert(In, Out);
    }
    else
    {
        *Out = *In;
        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// ConvertToUTF8
//
///////////////////////////////////////////////////////////////////////////////
int TTransDoc::ConvertToUTF8(TMarcRecord* MarcIn, TMarcRecord* MarcOut)
{
    TMarcField* In;
    TMarcField* Out;

    // On commence par effectuer la recopie (sans transco) du label
    MarcOut->SetLeader(MarcIn->GetLeader());
    if (!MarcOut->GetLeader())
    {
        typestr tmp = "Notice '";
        tmp += MarcIn->GetFirstField()->GetLib1();
        tmp += "' : leader '";
        tmp += MarcIn->GetLeader();
        tmp += '\'';
        mStateManager->SetErrorD(3000, ERROR, tmp.str());
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
        Out->SetLib1(Transcode(In->GetLib1(), &result, In->GetLib1(), In->GetTag()));
        Out->SetLib2(Transcode(In->GetLib2(), &result, In->GetLib2(), In->GetTag()));
        Out->SetScript(In->GetScript());
        if (!Out->GetLib1())
        {
            typestr tmp = "Notice '";
            tmp += In->GetLib1();
            tmp += "' : field '";
            tmp += In->GetTag();
            tmp += '\'';
            mStateManager->SetErrorD(5006, ERROR, tmp.str());
        }

        // On passe au champ suivant
        In=In->GetNextField();
    }

    return mStateManager->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// Transcode
//
// Converts a string to UTF-8
//
///////////////////////////////////////////////////////////////////////////////
const char* TTransDoc::Transcode(char* In, typestr *Out, char *Notice, char *Field)
{
    char *c = In;
    char *r = NULL;

    Out->str("");

    typestr Combining;

    if (!In)
        return (Out->str());

    if (itsCharset == CHARSET_MARC8)
    {
        char g0 = MARC8_SET_BASIC_LATIN;
        bool g0_multibyte = false;
        char g1 = MARC8_SET_EXTENDED_LATIN;
        bool g1_multibyte = false;
        while (*c != '\0')
        {
            // Space as is
            if (*c == ' ')
            {
                Out->append_char(' ');
                c++;
                continue;
            }

            // Escape sequence
            if (*c == '\x1B')
            {
                char esc1 = *(c+1);
                if (esc1 != '\0')
                {
                    // Check for single byte set switches
                    switch (esc1)
                    {
                    case MARC8_SET_DEFAULT:
                    case MARC8_SET_GREEK_SYMBOL:
                    case MARC8_SET_SUBSCRIPT:
                    case MARC8_SET_SUPERSCRIPT:
                        g0 = esc1; 
                        c += 2; 
                        continue;
                    }
                    char esc2 = *(c+2);
                    if (esc2 != '\0')
                    {
                        // Check for multibyte byte set switches
                        switch (esc1)
                        {
                        case MARC8_SET_G0_SINGLEBYTE_A:
                        case MARC8_SET_G0_SINGLEBYTE_B:
                            g0 = esc2;
                            c += 3;
                            continue;
                        case MARC8_SET_G1_SINGLEBYTE_A:
                        case MARC8_SET_G1_SINGLEBYTE_B:
                            g1 = esc2;
                            c += 3;
                            continue;
                        case MARC8_SET_MULTIBYTE:
                            {
                                char esc3 = *(c+3);
                                if (esc3 == MARC8_SET_EXTENDED_LATIN_PRE)
                                {
                                    c++;
                                    esc3 = *(c+3);
                                }

                                if (esc2 == MARC8_SET_G1_MULTIBYTE_A || esc2 == MARC8_SET_G1_MULTIBYTE_B)
                                {
                                    g1 = esc3;
                                    c += 4;
                                    continue;
                                }
                                if (esc2 == MARC8_SET_G0_MULTIBYTE_B)
                                {
                                    g0 = esc3;
                                    c += 4;
                                    continue;
                                }
                                // G0 is default if no other designation is given
                                g0 = esc2;
                                c += 3;
                                continue;
                            }
                        default:
                            c += 2;
                            continue;
                        }
                    }
                }
                // Shouldn't get here with a valid MARC-8 escape sequence
                c++;
                continue;
            }
            // Shortcut for BASIC latin...
            if (g0 == MARC8_SET_BASIC_LATIN && *c >= '\x1B' && *c <= '\x7E')
            {
                Out->append_char(*c);
                if (!Combining.is_empty())
                {
                    Out->append(Combining.str());
                    Combining.freestr();
                }
                c++;
                continue;
            }
            // Check for char in G0 set
            char res[10];
            bool combining;
            if (marc8_find_char(c, g0, res, &combining))
            {
                if (combining)
                {
                    Combining.append(res);
                }
                else
                {
                    Out->append(res);
                    if (!Combining.is_empty())
                    {
                        Out->append(Combining.str());
                        Combining.freestr();
                    }
                }
                c += (g0 == MARC8_SET_EACC) ? 3 : 1;
                continue;
            }
            // Check for char in G1 set
            if (marc8_find_char(c, g1, res, &combining))
            if (*res)
            {
                if (combining)
                {
                    Combining.append(res);
                }
                else
                {
                    Out->append(res);
                    if (!Combining.is_empty())
                    {
                        Out->append(Combining.str());
                        Combining.freestr();
                    }
                }
                c += (g1 == MARC8_SET_EACC) ? 3 : 1;
                continue;
            }
            // Shouldn't get here with a valid MARC-8 sequence
            c++;
        }
    }
    return(Out->str());
}
