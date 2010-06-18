/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2010
 *
 *  File:  tmarcrec.cpp
 *
 *  implements a class to manage MARC records
 *
 */

#include <ctype.h>
#include "tmarcrec.h"
#include "statemanager.h"
#include "tmpplctn.h"
#include "tools.h"
#include "utf8proc.h"

///////////////////////////////////////////////////////////////////////////////
//
// TMarcRecord
//
///////////////////////////////////////////////////////////////////////////////
TMarcRecord::TMarcRecord(TUMApplication *Application)
{
    *itsLeader          = '\0';
    itsFirstField       = NULL;
    itsFirstInputTNI    = NULL;
    itsFirstOutputTNI   = NULL;
    itsInputIndSeparatorsID = 0;
    itsOutputIndSeparatorsID = 0;
    mStateManager     = Application->GetStateManager();
    *m_recordId = '\0';
}

///////////////////////////////////////////////////////////////////////////////
//
// Copy constructor
//
///////////////////////////////////////////////////////////////////////////////
TMarcRecord::TMarcRecord(const TMarcRecord &aRecord)
{
    strcpy(itsLeader, aRecord.GetLeader());

    strcpy(m_recordId, aRecord.m_recordId);

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
        Out->SetLib1(In->GetLib1());
        Out->SetLib2(In->GetLib2());
        Out->SetScript(In->GetScript());
        In = In->GetNextField();
    }


    itsFirstInputTNI = aRecord.GetFirstInputTNI();
    itsFirstOutputTNI = aRecord.GetFirstOutputTNI();
    itsInputIndSeparatorsID = aRecord.itsInputIndSeparatorsID;
    itsOutputIndSeparatorsID = aRecord.itsOutputIndSeparatorsID;
    mStateManager = aRecord.mStateManager;
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

    strcpy(itsLeader, aRecord.GetLeader());

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
        Out->SetLib1(In->GetLib1());
        Out->SetLib2(In->GetLib2());
        Out->SetScript(In->GetScript());
        In = In->GetNextField();
    }


    itsFirstInputTNI = aRecord.GetFirstInputTNI();
    itsFirstOutputTNI = aRecord.GetFirstOutputTNI();
    itsInputIndSeparatorsID = aRecord.itsInputIndSeparatorsID;
    itsOutputIndSeparatorsID = aRecord.itsOutputIndSeparatorsID;
    mStateManager = aRecord.mStateManager;
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

    // Delete existing tree
    DelTree();

    *m_recordId = '\0';

    // Leader
    memcpy(itsLeader, MarcString, 24);
    itsLeader[24]=0;

    // On isole l'adresse de d‰but des donn‰es
    memcpy(cdebutdata,&itsLeader[12],5);
    cdebutdata[5]=0;
    if (Val(cdebutdata,&debutdata))
        return mStateManager->SetErrorD(1007,FATAL,cdebutdata);
    pos=24;

    // On va maintenant renseigner les differents champs
    if ((itsFirstField = new TMarcField()) == NULL)
        return mStateManager->SetError(9041,FATAL);
    champ=itsFirstField;

    // Lecture du tag du champs
    memcpy(temp,&MarcString[pos],3);
    temp[3]=0;
    if (champ->SetTag(temp))
        return mStateManager->SetErrorD(1101,ERROR,temp);
    pos+=3;

    // Lecture de sa longueur
    memcpy(temp,&MarcString[pos],4);
    temp[4]=0;
    if (Val(temp,&lngchamp))
        return mStateManager->SetErrorD(1004,ERROR,temp);
    pos += 4;

    // Lecture de sa position
    memcpy(temp,&MarcString[pos],5);
    temp[5]=0;
    if (LongVal(temp,&posdata))
        return mStateManager->SetErrorD(1007,ERROR,temp);
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

    champ->SetLib1(&MarcString[dd],(unsigned int)lngchamp-1);

    if (strcmp(champ->GetTag(), "001") == 0)
    {
        strncpy(m_recordId, champ->GetLib1(), 50);
        m_recordId[50] = '\0';
    }

    while (MarcString[pos] != '\x1E')  // end of directory
    {
        champ->SetNextField(new TMarcField());
        if (champ->GetNextField()==NULL)
            return mStateManager->SetError(9041,ERROR);
        champ = champ->GetNextField();

        // Lecture du tag du champs
        memcpy(temp,&MarcString[pos],3);
        temp[3]=0;
        if (champ->SetTag(temp))
            return mStateManager->SetErrorD(1101,ERROR,temp);
        pos+=3;

        // Lecture de sa longueur
        memcpy(temp,&MarcString[pos],4);
        temp[4]=0;
        if (Val(temp,&lngchamp))
            return mStateManager->SetErrorD(1004,ERROR,temp);
        pos += 4;

        // Lecture de sa position
        memcpy(temp,&MarcString[pos],5);
        temp[5]=0;
        if (LongVal(temp,&posdata))
            return mStateManager->SetErrorD(1007,ERROR,temp);
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

        champ->SetLib1(&MarcString[dd],(unsigned int)lngchamp-1);

        if (strcmp(champ->GetTag(), "001") == 0)
        {
            strncpy(m_recordId, champ->GetLib1(), 50);
            m_recordId[50] = '\0';
        }
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
            return mStateManager->SetErrorD(1050, ERROR, xml.str());
        if (tag == "record")
        {
            xml = content;
            while (true)
            {
                if (!get_tag(xml, tag, attribs, content, remainder))
                {
                    if (!itsFirstField)
                        return mStateManager->SetErrorD(1050, ERROR, xml.str());
                    break;
                }

                xml = remainder;

                if (tag == "leader")
                {
                    content = unescape_xml(content);
                    size_t len = strlen(content.str());
                    for (size_t i = 1; i <= 24 - len; i++)
                        content.append_char(' ');
                    memcpy(itsLeader, content.str(), 24);
                }
                else if (tag == "controlfield" || tag == "datafield")
                {
                    if ((field = new TMarcField()) == NULL)
                        return mStateManager->SetError(9041,FATAL);
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
                                field_data += START_OF_FIELD;
                                field_data += get_attrib("code", attribs);
                                field_data += unescape_xml(subfield);
                            }
                        }
                        field->SetLib1(field_data.str());
                    }
                    else
                    {
                        field->SetLib1(unescape_xml(content).str());
                        if (strcmp(field->GetTag(), "001") == 0)
                        {
                            strncpy(m_recordId, field->GetLib1(), 50);
                            m_recordId[50] = '\0';
                        }
                    }

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
int TMarcRecord::ToString(typestr & a_marcstr)
{
    typestr directory;
    typestr data;

    size_t pos_data = 0;

    TMarcField *field = itsFirstField;
    while (field != NULL)
    {
        char *tag = field->GetTag();
        char *marcdata = field->GetLib1();

        char temps[20];
        size_t lng = strlen(marcdata);
        bool have_ind = IsFieldWithIndicators(OUTPUT, tag, marcdata, lng);
        if (!have_ind)
            sprintf(temps, "%s%04d%05d", tag, lng+1 < 9999 ? int(lng+1) : 9999, pos_data < 99999 ? int(pos_data) : 99999);
        else
            sprintf(temps, "%s%04d%05d", tag, lng+3 < 9999 ? int(lng+3) : 9999, pos_data < 99999 ? int(pos_data) : 99999);

        directory += temps;

        if (have_ind)
        {
            // There must be two indicators
            char *indicators = field->GetIndicators();
            if (*indicators && *(indicators + 1))
              data += indicators;
            else
              data += "  ";
            pos_data += 2;
        }
        data += marcdata;
        data += END_OF_FIELD;
        pos_data += lng + 1;

        field = field->GetNextField();
    }

    a_marcstr = "";
    a_marcstr.append(itsLeader, 24);
    while (strlen(a_marcstr.str()) < 24)
        a_marcstr += ' ';

    a_marcstr += directory + END_OF_FIELD;
    a_marcstr += data + END_OF_RECORD;

    // Set total length of the record to the leader
    char temps[20];
    sprintf(temps, "%05u", strlen(a_marcstr.str()));
    memcpy(a_marcstr.str(), temps, 5);

    // Set base address of data to the leader
    sprintf(temps, "%05u", 24 + strlen(directory.str()));
    memcpy(&a_marcstr.str()[12], temps, 5);

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
    if (!mStateManager->GetOutputXMLRecordFormat().is_empty())
    {
        a_xml.append(" format=\"");
        a_xml.append(mStateManager->GetOutputXMLRecordFormat());
        a_xml.append_char('\"');
    }
    if (!mStateManager->GetOutputXMLRecordType().is_empty())
    {
        a_xml.append(" type=\"");
        a_xml.append(mStateManager->GetOutputXMLRecordType());
        a_xml.append_char('\"');
    }
    a_xml.append(">\n");

    a_xml.append("  <leader>");
    a_xml.append(escape_xml(itsLeader));
    a_xml.append("</leader>\n");

    TMarcField *field = itsFirstField;
    while (field)
    {
        const char *tag = field->GetTag();
        const char *marcdata = field->GetLib1();
        size_t lng = strlen(marcdata);
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

            const char *p = strchr(marcdata, START_OF_FIELD);
            while (p)
            {
                char code = p[1];
                typestr subfield = p + 2;
                char *end = strchr(subfield.str(), START_OF_FIELD);
                if (end)
                    *end = '\0';

                a_xml.append("    <subfield code=\"");
                a_xml.append_char(code);
                a_xml.append("\">");
                a_xml.append(escape_xml(subfield));
                a_xml.append("</subfield>\n");

                p = strchr(p + 1, START_OF_FIELD);
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
    new_str.replace("'", "&apos;");
    new_str.replace("\"", "&quot;");

    return new_str;
}

typestr TMarcRecord::unescape_xml(const typestr & a_str)
{
    typestr new_str = a_str;

    // Convert numeric entities
    char* p = new_str.str();
    while (p = strchr(p, '&'))
    {
        char* p2 = p + 1;
        if (*p2 == '#')
        {
            ++p2;
            int value;
            const char* mask = "%u;";
            if (*p2 == 'x')
            {
                ++p2;
                mask = "%x;";
            }
            if (sscanf(p2, mask, &value) == 1)
            {
                p2 = p;
                typestr source;
                while (*p2 != ';')
                {
                    source.append_char(*p2);
                    ++p2;
                }
                source.append_char(';');

                char utf8[10];
                int chars = utf8proc_encode_char(value, (uint8_t *)utf8);
                utf8[chars] = '\0';

                new_str.replace(source.cstr(), utf8, p - new_str.str());
                p += chars;
                continue;
            }
        }
        ++p;
    }

    new_str.replace("&lt;", "<");
    new_str.replace("&gt;", ">");
    new_str.replace("&apos;", "'");
    new_str.replace("&quot;", "\"");
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
// SetLeader
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::SetLeader(const char *aLeader)
{
    memcpy(itsLeader, aLeader, 24);
    itsLeader[24]=0;
    if (strlen(aLeader) != 24)
        return 1;
    return 0;
};


///////////////////////////////////////////////////////////////////////////////
//
// Val
//
///////////////////////////////////////////////////////////////////////////////
int TMarcRecord::Val(char *buffer, unsigned long *valeur)
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
bool TMarcRecord::IsFieldWithIndicators(int IO, const char *tag, const char *field, size_t fieldlen)
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

int TMarcRecord::ProcessDuplicateFields(DUPLICATE_PROCESSING_MODE a_subfields, DUPLICATE_PROCESSING_MODE a_fields)
{
    if (a_subfields == DP_LEAVE && a_fields == DP_LEAVE)
        return 0;

    if (a_subfields != DP_LEAVE)
    {
        TMarcField *field = itsFirstField;
        while (field)
        {
            typestr marcdata = field->GetLib1();

            char *p = strchr(marcdata.str(), START_OF_FIELD);
            while (p)
            {
                // Extract a subfield
                char *end = strchr(p + 1, START_OF_FIELD);
                if (!end)
                    break; // Last subfield, there can be no more matches
                
                typestr subfield;
                subfield.str(p, end - p);

                // Iterate the rest of subfields
                char *comp_p = end;
                while (comp_p)
                {
                    char *comp_end = strchr(comp_p + 1, START_OF_FIELD);

                    if (comp_p[1] == subfield.str()[1])
                    {
                        typestr comp_subfield;
                        comp_subfield.str(comp_p, comp_end ? comp_end - comp_p : 0);

                        // compare the subfields according to the rules
                        int duplicate_subfield = 0;
                        if (a_subfields == DP_DELETE)
                        {
                            if (strcmp(subfield.str(), comp_subfield.str()) == 0) 
                                duplicate_subfield = 2;
                        }
                        else if (a_subfields == DP_DELETE_IGNORE_CASE || a_subfields == DP_DELETE_SMART)
                        {
                            if (utf8_stricmp(subfield.str() + 2, comp_subfield.str() + 2) == 0)
                            {
                                if (a_subfields == DP_DELETE_SMART)
                                {
                                    int caps1 = caps_count(subfield.str());
                                    int caps2 = caps_count(comp_subfield.str());
                                    if (caps1 <= caps2) 
                                        duplicate_subfield = 2;
                                    else
                                        duplicate_subfield = 1;
                                }
                                else
                                {
                                    duplicate_subfield = 2;
                                }
                            }
                        }
                        if (duplicate_subfield == 1)
                        {
                            strcpy(p, end);
                            field->SetLib1(marcdata.str());
                            end = p - 1;
                            break;
                        }
                        else if (duplicate_subfield == 2)
                        {
                            if (comp_end)
                                memmove(comp_p, comp_end, strlen(comp_end) + 1);
                            else
                                *comp_p = '\0';
                            field->SetLib1(marcdata.str());
                            comp_end = comp_p - 1;
                        }
                    }

                    if (!comp_end)
                        break;
                    comp_p = strchr(comp_end + 1, START_OF_FIELD);
                }
                
                p = strchr(end + 1, START_OF_FIELD);
            }

            field = field->GetNextField();
        }
    }

    if (a_fields == DP_LEAVE)
        return 0;

    TMarcField *field = itsFirstField;
    TMarcField *prev_field = NULL;
    while (field)
    {
        TMarcField *comp_prev_field = field;
        TMarcField *comp_field = field->GetNextField();
        while (comp_field && strcmp(comp_field->GetTag(), field->GetTag()) == 0)
        {
            if (strcmp(comp_field->GetIndicators(), field->GetIndicators()) == 0)
            {
                int duplicate_field = 0;
                if (a_fields == DP_DELETE)
                {
                    if (strcmp(field->GetLib1(), comp_field->GetLib1()) == 0) 
                        duplicate_field = 2;
                }
                else if (a_fields == DP_DELETE_IGNORE_CASE || a_fields == DP_DELETE_SMART)
                {
                    if (utf8_stricmp(field->GetLib1(), comp_field->GetLib1()) == 0)
                    {
                        if (a_fields == DP_DELETE_SMART)
                        {
                            int caps1 = caps_count(field->GetLib1());
                            int caps2 = caps_count(comp_field->GetLib1());
                            if (caps1 <= caps2) 
                                duplicate_field = 2;
                            else
                                duplicate_field = 1;
                        }
                        else
                        {
                            duplicate_field = 2;
                        }
                    }
                }
                if (duplicate_field == 1)
                {
                    if (prev_field)
                        prev_field->SetNextField(field->GetNextField());
                    else
                    {
                        itsFirstField = field->GetNextField();
                        prev_field = itsFirstField;
                    }
                    delete field;
                    field = NULL;
                    break;
                }
                else if (duplicate_field == 2)
                {
                    comp_prev_field->SetNextField(comp_field->GetNextField());
                    delete comp_field;
                    comp_field = comp_prev_field;
                }
            }

            comp_prev_field = comp_field;
            comp_field = comp_field->GetNextField();
        }

        if (field)
        {
            prev_field = field;
            field = field->GetNextField();
        }
        else
        {
            field = prev_field->GetNextField();
        }
    }

    return 0;
}
