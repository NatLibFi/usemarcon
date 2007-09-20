/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  stringtable.cpp
 *
 *  implements a class to manage string tables
 *
 */

#include "stringtable.h"
#include "tfile.h"

StringTable::StringTable(TError* a_errorhandler) : m_next(NULL), m_errorhandler(a_errorhandler) 
{
}

StringTable::~StringTable()
{
}

bool StringTable::Load(const typestr a_filename)
{
    m_name = a_filename;

    while (m_next)
    {
        StringTable* next = m_next->GetNext();
        delete m_next;
        m_next = next;
    }
    m_next = NULL;

    TFile file(a_filename, m_errorhandler);

    int res = file.Open();
    if (res != 0)
        return false;

    StringTable *entry = NULL;

    typestr line;
    typestr include_file;
    int lineno;
    while (!file.NextLine(&line, &include_file, &lineno))
    {
        char *p = strchr(line.str(), '//');
        if (p) 
            *p = '\0';

        typestr src;
        typestr dst;
        if (!parse_line(line.str(), src, dst))
            continue;

        if (!entry)
        {
            entry = this;
            m_src.rulestr(src.str());
            m_dst.rulestr(dst.str());
        }
        else
        {
            StringTable *new_entry = new StringTable(m_errorhandler);
            new_entry->m_src.rulestr(src.str());
            new_entry->m_dst.rulestr(dst.str());
            entry->SetNext(new_entry);
            entry = new_entry;
        }
    }
    return true;
}

bool StringTable::parse_line(const char *a_line, typestr &a_src, typestr &a_dst)
{
    const char* begin = a_line;
    while (*begin == ' ')
        ++begin;

    const char* pipe = strchr(begin, '|');
    if (!pipe)
        return false;
    const char *p = pipe;
    while (p > begin && *(p - 1) == ' ')
        --p;        
    a_src.str(begin, p - begin);

    begin = pipe + 1;
    while (*begin == ' ')
        ++begin;
    p = begin + strlen(begin);
    while (p > begin && *(p - 1) == ' ')
        --p;        

    a_dst.str(begin, p - begin);

    return true;
}
