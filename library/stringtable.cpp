/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2008
 *
 *  File:  stringtable.cpp
 *
 *  implements a class to manage string tables
 *
 */

#include "stringtable.h"
#include "tfile.h"

StringTableItem::StringTableItem() : m_next(NULL)
{
}

StringTable::StringTable(TStateManager* a_errorhandler) : m_first(NULL), m_last(NULL), m_next_table(NULL),
  m_errorhandler(a_errorhandler) 
{
}

StringTable::~StringTable()
{
    DeleteItems();
}

void StringTable::DeleteItems()
{
    StringTableItem* item = m_first;
    while (item)
    {
        StringTableItem* next = item->GetNext();
        delete item;
        item = next;
    }
    m_first = NULL;
    m_last = NULL;
}

bool StringTable::Load(const typestr a_filename)
{
    m_name = a_filename;

    DeleteItems();

    TFile file(a_filename, m_errorhandler);

    int res = file.Open();
    if (res != 0)
        return false;

    typestr line;
    typestr include_file;
    int lineno;
    while (!file.NextLine(&line, &include_file, &lineno))
    {
        char *p = strstr(line.str(), "//");
        if (p) 
            *p = '\0';

        typestr src;
        typestr dst;
        if (!parse_line(line.str(), src, dst))
            continue;

        StringTableItem* new_entry = AddItem();
        new_entry->m_src = src;
        new_entry->m_dst = dst;
    }
    return true;
}

StringTableItem* StringTable::AddItem()
{
    if (!m_first)
    {
        m_first = new StringTableItem();
        m_last = m_first;
        return m_first;
    }
    StringTableItem* item = new StringTableItem();
    m_last->SetNext(item);
    m_last = item;
    return item;
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

StringTableList::StringTableList(TStateManager *a_errorhandler) : m_first(NULL), m_last(NULL), m_errorhandler(a_errorhandler)
{
}

StringTableList::~StringTableList()
{
    DeleteTables();
}

void StringTableList::DeleteTables()
{
    StringTable* table = m_first;
    while (table)
    {
        StringTable* next = table->GetNextTable();
        delete table;
        table = next;
    }
    m_first = NULL;
    m_last = NULL;
}

StringTable* StringTableList::AddTable()
{
    if (!m_first)
    {
        m_first = new StringTable(m_errorhandler);
        m_last = m_first;
        return m_first;
    }
    StringTable* table = new StringTable(m_errorhandler);
    m_last->SetNextTable(table);
    m_last = table;
    return table;
}
