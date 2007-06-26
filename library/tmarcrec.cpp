/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  tmarcrec.cpp
 *
 *  implements a class to manage MARC records
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include <ctype.h>
#include "tmarcrec.h"
#include "error.h"
#include "objectlist.h"

///////////////////////////////////////////////////////////////////////////////
//
// TMarcRecord
//
///////////////////////////////////////////////////////////////////////////////
TMarcRecord::TMarcRecord(TUMApplication *Application)
{
    *itsLabel           = 0;
    itsFirstField       = NULL;
    itsFirstInputTNI    = NULL;
    itsFirstOutputTNI   = NULL;
    itsInputIndSeparatorsID = 0;
    itsOutputIndSeparatorsID = 0;
    itsErrorHandler     = Application->GetErrorHandler();
    itsDetails          = Application->GetDetails();
}

///////////////////////////////////////////////////////////////////////////////
//
// Copy constructor
//
///////////////////////////////////////////////////////////////////////////////
TMarcRecord::TMarcRecord(const TMarcRecord &aRecord)
{
    strcpy(itsLabel, aRecord.GetLabel());

    TMarcField* In = aRecord.GetFirstField();
    TMarcField* Out = NULL;
    while (In)
    {
        // On cree un nouveau champ en sortie
        if (!Out)
        {
            Out = new TMarcField;
            this->SetFirstField(Out);
        }
        else
        {
            Out->SetNextField(new TMarcField);
            Out = Out->GetNextField();
        }

        // On recopie le tag et les indicateurs du champ en entree
        Out->SetTag(In->GetTag());
        Out->SetIndicators(In->GetIndicators());
        Out->SetLib(In->GetLib());
        In=In->GetNextField();
    }


    itsFirstInputTNI = aRecord.GetFirstInputTNI();
    itsFirstOutputTNI = aRecord.GetFirstOutputTNI();
    itsInputIndSeparatorsID = aRecord.itsInputIndSeparatorsID;
    itsOutputIndSeparatorsID = aRecord.itsOutputIndSeparatorsID;
    itsErrorHandler = aRecord.itsErrorHandler;
    itsDetails = aRecord.itsDetails;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TMarcRecord
//
///////////////////////////////////////////////////////////////////////////////
TMarcRecord::~TMarcRecord()
{
    DelTree();
}

///////////////////////////////////////////////////////////////////////////////
//
// Assignment operator
//
///////////////////////////////////////////////////////////////////////////////
TMarcRecord & TMarcRecord::operator=(const TMarcRecord &aRecord)
{
    if (this == &aRecord)
    {
        return *this;
    }

    strcpy(itsLabel, aRecord.GetLabel());

    TMarcField* In = aRecord.GetFirstField();
    TMarcField* Out = NULL;
    while (In)
    {
        // On cree un nouveau champ en sortie
        if (!Out)
        {
            Out = new TMarcField;
            this->SetFirstField(Out);
        }
        else
        {
            Out->SetNextField(new TMarcField);
            Out = Out->GetNextField();
        }

        // On recopie le tag et les indicateurs du champ en entree
        Out->SetTag(In->GetTag());
        Out->SetIndicators(In->GetIndicators());
        Out->SetLib(In->GetLib());
        In=In->GetNextField();
    }


    itsFirstInputTNI = aRecord.GetFirstInputTNI();
    itsFirstOutputTNI = aRecord.GetFirstOutputTNI();
    itsInputIndSeparatorsID = aRecord.itsInputIndSeparatorsID;
    itsOutputIndSeparatorsID = aRecord.itsOutputIndSeparatorsID;
    itsErrorHandler = aRecord.itsErrorHandler;
    itsDetails = aRecord.itsDetails;

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
// NextField
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::NextField(TMarcField** Field,char* Tag)
{
    TMarcField* Courant=*Field;
    while(Courant!=NULL)
    {
        if (!strcmp(Courant->GetTag(),Tag))
        {
            *Field=Courant;
            return 0;
        }
        Courant=Courant->GetNextField();
    }
    *Field=NULL;
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
// FromString
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::FromString(char* MarcString)
{
    unsigned long       lngchamp,
                        debutdata,
                        pos,
                        dd,
                        posdata;
    char                cdebutdata[6],
                        temp[6];
    TMarcField          *champ;

    // On detruit dabord l'arbre s'il existe
    DelTree();

    // On renseigne le guide
    memcpy(itsLabel,MarcString,24);
    itsLabel[24]=0;

    // On isole l'adresse de d‰but des donn‰es
    memcpy(cdebutdata,&itsLabel[12],5);
    cdebutdata[5]=0;
    if (Val(cdebutdata,&debutdata))
        return itsErrorHandler->SetErrorD(1007,FATAL,cdebutdata);
    pos=24;

    // On va maintenant renseigner les differents champs
    if ((itsFirstField = new TMarcField()) == NULL)
        return itsErrorHandler->SetError(9041,FATAL);
    champ=itsFirstField;

    // Lecture du tag du champs
    memcpy(temp,&MarcString[pos],3);
    temp[3]=0;
    if (champ->SetTag(temp))
        return itsErrorHandler->SetErrorD(1101,ERROR,temp);
    pos+=3;

    // Lecture de sa longueur
    memcpy(temp,&MarcString[pos],4);
    temp[4]=0;
    if (Val(temp,&lngchamp))
        return itsErrorHandler->SetErrorD(1004,ERROR,temp);
    pos += 4;

    // Lecture de sa position
    memcpy(temp,&MarcString[pos],5);
    temp[5]=0;
    if (LongVal(temp,&posdata))
        return itsErrorHandler->SetErrorD(1007,ERROR,temp);
    pos += 5;

    // on remplit le premier champ
    // on alloue de la memoire pour le libelle
    dd=(long)debutdata+posdata;
    if (IsFieldWithIndicators(INPUT,champ->GetTag(),&MarcString[dd],lngchamp))
    {
        champ->SetI1(MarcString[dd++]);
        champ->SetI2(MarcString[dd++]);
        lngchamp-=2;
    }

    if (champ->SetLib(&MarcString[dd],(unsigned int)lngchamp-1))
        return itsErrorHandler->SetError(1009,ERROR);

    while (MarcString[pos] != '\x1E')  // end of directory
    {
        champ->SetNextField(new TMarcField());
        if (champ->GetNextField()==NULL)
            return itsErrorHandler->SetError(9041,ERROR);
        champ = champ->GetNextField();

        // Lecture du tag du champs
        memcpy(temp,&MarcString[pos],3);
        temp[3]=0;
        if (champ->SetTag(temp))
            return itsErrorHandler->SetErrorD(1101,ERROR,temp);
        pos+=3;

        // Lecture de sa longueur
        memcpy(temp,&MarcString[pos],4);
        temp[4]=0;
        if (Val(temp,&lngchamp))
            return itsErrorHandler->SetErrorD(1004,ERROR,temp);
        pos += 4;

        // Lecture de sa position
        memcpy(temp,&MarcString[pos],5);
        temp[5]=0;
        if (LongVal(temp,&posdata))
            return itsErrorHandler->SetErrorD(1007,ERROR,temp);
        pos += 5;

        // on remplit le champ
        // on alloue de la memoire pour le libelle
        dd=(long)debutdata+posdata;
        if (IsFieldWithIndicators(INPUT,champ->GetTag(),&MarcString[dd],lngchamp))
        {
            champ->SetI1(MarcString[dd++]);
            champ->SetI2(MarcString[dd++]);
            lngchamp-=2;
        }
        if (lngchamp < 1)
        {
            // A broken record
            lngchamp = 1;
        }

        if (champ->SetLib(&MarcString[dd],(unsigned int)lngchamp-1))
            return itsErrorHandler->SetError(1009,ERROR);
    }

    champ->SetNextField(NULL);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// FromXMLString
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::FromXMLString(typestr & a_xml)
{
    // Delete previous content
    DelTree();

    typestr xml = a_xml;

    TMarcField *field, *lastfield;

    while (true)
    {
        typestr tag, attribs, content, remainder;
        if (!get_tag(xml, tag, attribs, content, remainder))
            return itsErrorHandler->SetErrorD(1050, ERROR, xml.str());
        if (tag == "record")
        {
            xml = content;
            while (true)
            {
                if (!get_tag(xml, tag, attribs, content, remainder))
                {
                    if (!itsFirstField)
                        return itsErrorHandler->SetErrorD(1050, ERROR, xml.str());
                    break;
                }

                xml = remainder;

                if (tag == "leader")
                {
                    content = unescape_xml(content);
                    size_t len = strlen(content.str());
                    for (size_t i = 1; i <= 24 - len; i++)
                        content.append_char(' ');
                    memcpy(itsLabel, content.str(), 24);
                }
                else if (tag == "controlfield" || tag == "datafield")
                {
                    if ((field = new TMarcField()) == NULL)
                        return itsErrorHandler->SetError(9041,FATAL);
                    if (!itsFirstField)
                    {
                        itsFirstField = field;
                    }
                    else
                    {
                        lastfield->SetNextField(field);
                    }
                    lastfield = field;

                    field->SetTag(get_attrib("tag", attribs).str());

                    if (tag == "datafield")
                    {
                        field->SetI1(*get_attrib("ind1", attribs).str());
                        field->SetI2(*get_attrib("ind2", attribs).str());

                        typestr field_data;
                        typestr subfield;
                        while (get_tag(content, tag, attribs, subfield, remainder))
                        {
                            content = remainder;
                            if (tag == "subfield")
                            {
                                field_data.append_char('\x1F');
                                field_data.append(get_attrib("code", attribs));
                                field_data.append(unescape_xml(subfield));
                            }
                        }
                        field->SetLib(field_data.str());
                    }
                    else
                        field->SetLib(unescape_xml(content).str());

                }
            }
            break;
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// ToString
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::ToString(char* marc,long max_size)
{
    unsigned short  pos_dir;
                    
    unsigned long   debut_data,
                    pos_data,
                    lgtot,
                    lng;
    char            temp[20];
    TMarcField*     champ;

    // On compte les champs pour savoir ou commenceront les datas
    champ=itsFirstField;
    pos_data=25;
    while( champ!= NULL )
    {
        pos_data+=12;
        champ=champ->GetNextField();
    }
    debut_data=pos_data;

    // On recopie le guide au debut de la notice. Le guide sera complete par les longueurs plus tard
    memcpy(marc,itsLabel,24);
    pos_dir=24;
    champ = itsFirstField;
    while (champ != NULL)
    {
        char *tag = champ->GetTag();
        char *marcdata = champ->GetLib();
        lng = strlen(marcdata);
        bool have_ind = IsFieldWithIndicators(OUTPUT,tag,marcdata,lng);
        if (!have_ind)
            // pas d'indicateurs
            sprintf(temp,"%03s%04d%05d",tag,lng+1,pos_data-debut_data);
        else
            sprintf(temp,"%03s%04d%05d", tag,lng+3,pos_data-debut_data);

        // On construit la directorie, en verifiant que la taille du BUFFER alloue est suffisante
        if ((unsigned long)pos_dir+12>=(unsigned long)max_size)
            return itsErrorHandler->SetError(1009,ERROR);
        memcpy(&marc[pos_dir],temp,12);
        pos_dir += 12;

        // On construit la zone de donnees, en verifiant que le BUFFER alloue est suffisant
        if (!have_ind)
            // pas d'indicateur
        {
            if (pos_data+lng+1>=(unsigned long)max_size)
                return itsErrorHandler->SetError(1009,ERROR);
        }
        else
        {
            if (pos_data+lng+3>=(unsigned long)max_size)
                return itsErrorHandler->SetError(1009,ERROR);
            // There must be two indicators
            char *indicators = champ->GetIndicators();
            if (*indicators && *(indicators + 1))
              memcpy(&marc[pos_data],indicators,2);
            else
              memcpy(&marc[pos_data],"  ",2);
            pos_data += 2;
        }

        memcpy(&marc[pos_data],marcdata,lng);
        pos_data += lng;
        marc[pos_data++]=30;
        champ=champ->GetNextField();
    }

    // On renseigne le guide sur la position de depart des donnees
    sprintf(temp,"%05d",pos_dir+1);
    memcpy(&marc[12],temp,5);

    // On renseigne le guide sur la longueur de la notice
    lgtot = pos_data+1;
    sprintf(temp,"%05lu",lgtot);
    memcpy(marc,temp,5);

    marc[pos_dir]=30;
    marc[lgtot-1]=29;
    marc[lgtot]=0;

    DelTree();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// ToXMLString
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::ToXMLString(typestr &a_xml)
{
    a_xml = "<record";
    if (itsErrorHandler->GetOutputXMLRecordFormat().str())
    {
        a_xml.append(" format=\"");
        a_xml.append(itsErrorHandler->GetOutputXMLRecordFormat());
        a_xml.append_char('\"');
    }
    if (itsErrorHandler->GetOutputXMLRecordType().str())
    {
        a_xml.append(" type=\"");
        a_xml.append(itsErrorHandler->GetOutputXMLRecordType());
        a_xml.append_char('\"');
    }
    a_xml.append(">\n");

    a_xml.append("  <leader>");
    a_xml.append(escape_xml(itsLabel));
    a_xml.append("</leader>\n");

    TMarcField *field = itsFirstField;
    while (field)
    {
        const char *tag = field->GetTag();
        const char *marcdata = field->GetLib();
        unsigned long lng = strlen(marcdata);
        bool have_ind = IsFieldWithIndicators(OUTPUT, tag, marcdata, lng);
        if (!have_ind)
        {
            a_xml.append("  <controlfield tag=\"");
            a_xml.append(tag);
            a_xml.append("\">");
            a_xml.append(escape_xml(marcdata));
            a_xml.append("</controlfield>\n");
        }
        else
        {
            char *indicators = field->GetIndicators();
            a_xml.append("  <datafield tag=\"");
            a_xml.append(tag);
            a_xml.append("\" ind1=\"");
            a_xml.append_char(indicators[0] ? indicators[0] : ' ');
            a_xml.append("\" ind2=\"");
            a_xml.append_char(indicators[1] ? indicators[1] : ' ');
            a_xml.append("\">\n");

            char *p = strchr(marcdata, '\x1F');
            while (p)
            {
                char code = p[1];
                typestr subfield = p + 2;
                char *end = strchr(subfield.str(), '\x1F');
                if (end)
                    *end = '\0';

                a_xml.append("    <subfield code=\"");
                a_xml.append_char(code);
                a_xml.append("\">");
                a_xml.append(escape_xml(subfield));
                a_xml.append("</subfield>\n");

                p = strchr(p + 1, '\x1F');
            }

            a_xml.append("  </datafield>\n");
        }

        field = field->GetNextField();
    }
    a_xml.append("</record>\n");

    DelTree();

    return 0;
}

typestr TMarcRecord::escape_xml(const typestr & a_str)
{
    typestr new_str = a_str;

    new_str.replace("&", "&amp;");
    new_str.replace("<", "&lt;");
    new_str.replace(">", "&gt;");

    return new_str;
}

typestr TMarcRecord::unescape_xml(const typestr & a_str)
{
    typestr new_str = a_str;

    new_str.replace("&lt;", "<");
    new_str.replace("&gt;", ">");
    new_str.replace("&amp;", "&");

    return new_str;
}

bool TMarcRecord::get_tag(typestr & a_xml, typestr & a_tag, typestr & a_attribs, typestr & a_content, typestr & a_remainder)
{
    const char *xml = a_xml.str();
    const char *p = xml;

    while (*p && *p != '<')
        ++p;
    if (!*p)
        return false;
    ++p;

    // Tag
    a_tag.freestr();
    while (*p)
    {
        if (strchr(" \n\r\t>", *p))
            break;
        a_tag.append_char(*p);
        ++p;
    }
    if (!*p)
        return false;
    normalize_tag(a_tag);

/*    if (*a_tag.str() == '/')
    {
        a_attribs = "";
        a_content = "";
        a_remainder = p;
        return true;
    }
 */   
    // Possible attributes
    a_attribs.freestr();
    while (*p && *p != '>')
    {
        a_attribs.append_char(*p);
        ++p;
    }
    if (!*p)
        return false;
    ++p;

    // Content
    a_content.freestr();
    while (*p)
    {
        if (*p == '<')
        {
            // Check if we found the closing tag for the current tag.
            // This would break for nested tags, but that's not a problem
            // with the formats supported.
            ++p;
            if (*p != '/')
            {
                a_content.append_char('<');
                continue;
            }
            ++p;
            typestr tag2 = "";
            while (*p && *p != '>')
            {
                tag2.append_char(*p);
                ++p;
            }
            if (!*p)
                return false;
            ++p;
            normalize_tag(tag2);
            if (tag2 == a_tag)
                break;
            a_content.append_char('<');
            a_content.append_char('/');
            a_content.append(tag2);
            a_content.append_char('>');
            continue;
        }
        if (*p == '\n' || *p == '\r')
            a_content.append_char(' ');
        else
            a_content.append_char(*p);
        ++p;
    }
    
    a_remainder = p;
    return true;
}

typestr TMarcRecord::get_attrib(const char *a_name, typestr & a_attribs)
{
    const char *p = a_attribs.str();
    while (*p)
    {
        if (strchr(" \n\r\t", *p))
        {
            ++p;
            continue;
        }

        typestr name = "";
        while (*p != '=')
        {
            if (*p != ' ' && *p != '\t')
                name.append_char(*p);
            ++p;
        }
        
        while (*p == ' ' || *p == '=')
            ++p;

        typestr value = "";
        bool in_quotes = false;
        while (*p)
        {
            if (*p == '"')
            {
                in_quotes = !in_quotes;
                if (!in_quotes)
                    break;
                ++p;
                continue;
            }
            if (!in_quotes && strchr(" \t\n\r", *p))
                break;
            value.append_char(*p);
            ++p;
        }

        if (name == a_name)
            return value;

        ++p;
    }
    return "";
}

void TMarcRecord::normalize_tag(typestr & a_tag)
{
    typestr new_tag;

    // Remove namespace
    char *p = strchr(a_tag.str(), ':');
    if (p)
    {
        if (*a_tag.str() == '/')
            strcpy(&new_tag.str()[1], p + 1);
        else
            strcpy(new_tag.str(), p + 1);
    }
    else
        new_tag = a_tag;

    // Remove whitespace
    while (p = strpbrk(new_tag.str(), " \n\r\t"))
    {
        *p = '\0';
        strcat(new_tag.str(), p + 1);
    }
    a_tag = new_tag;
}

///////////////////////////////////////////////////////////////////////////////
//
// DelTree
//
///////////////////////////////////////////////////////////////////////////////
void TMarcRecord::DelTree()
{
    TMarcField      *Courant,
        *Suivant;
    Courant=itsFirstField;

    while(Courant)
    {
        Suivant=Courant->GetNextField();
        delete Courant;
        Courant=Suivant;
    }
    itsFirstField=NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetLabel
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::SetLabel(const char *aLabel)
{
    memcpy(itsLabel,aLabel,24);
    itsLabel[24]=0;
    if (strlen(aLabel)!=24)
        return 1;
    return 0;
};


///////////////////////////////////////////////////////////////////////////////
//
// Val
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::Val(char *buffer,unsigned long *valeur)
{
    unsigned short      i;

    *valeur=0;
    for (i=0; i<strlen(buffer); i++)
        if (!isdigit(buffer[i]))
            return 1;
    *valeur = atoi( buffer );
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// LongVal
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::LongVal(char* buffer,unsigned long *valeur)
{
    unsigned short      i;

    *valeur=0;
    for (i=0; i<strlen( buffer ); i++)
        if (!isdigit(buffer[i]))
            return 1;
    *valeur = atol( buffer );
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// IsFieldWithIndicators
//
///////////////////////////////////////////////////////////////////////////////
bool TMarcRecord::IsFieldWithIndicators(int IO, const char *tag, const char *field, unsigned long fieldlen)
{
    if ((IO == INPUT && itsFirstInputTNI) || (IO == OUTPUT && itsFirstOutputTNI))
        return IsItAFieldWithIndicators(tag, IO);
    else
        // Determination automatique des champs sans indicateurs
    {
        if (field[0] == 31 || field[1] == 31 || field[0] == '\0' || field[1] == '\0')
            return true;

        if (tag[0] == '0' && tag[1] == '0')
        {
            for(unsigned long i = 2; i < fieldlen; ++i)
                if (field[i] == 31)
                    return true;
            return false;
        }
        else
            return true;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// IsItAFieldWithIndicators
//
///////////////////////////////////////////////////////////////////////////////
bool TMarcRecord::IsItAFieldWithIndicators(const char *tag, int IO)
{
    TTagNoInd   *Current;

    switch(IO)
    {
    case INPUT  :   Current = itsFirstInputTNI; break;
    case OUTPUT :   Current = itsFirstOutputTNI;    break;
    default     :   Current = NULL;
    }
    while (Current)
    {
        char *TagFilter = Current->GetTag();
        int MatchingDigits = 0;
        for (int i = 0; i < 3; i++)
            if (TagFilter[i] == tag[i] || TagFilter[i] == '?')
                MatchingDigits++;
            else
                break;
        if (MatchingDigits == 3)
            return false;
        Current = Current->GetNext();
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetIndSeparatorsID
//
///////////////////////////////////////////////////////////////////////////////
long TMarcRecord::GetIndSeparatorsID(int IO) const
{
    switch (IO)
    {
    case INPUT  : return itsInputIndSeparatorsID;
    case OUTPUT : return itsOutputIndSeparatorsID;
    default     : return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// SetIndSeparatorsID
//
///////////////////////////////////////////////////////////////////////////////
void TMarcRecord::SetIndSeparatorsID(int IO, long ID)
{
    switch (IO)
    {
    case INPUT  : itsInputIndSeparatorsID=ID;   break;
    case OUTPUT : itsOutputIndSeparatorsID=ID;  break;
    default     : break;
    }
}

