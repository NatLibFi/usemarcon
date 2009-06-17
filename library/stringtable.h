/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  stringtable.h
 *
 *
 
CLASS
    StringTable

    implements a class to manage string tables

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2009

AUTHOR
    The National Library of Finland

*/

#ifndef _STRING_TABLE_H_
#define _STRING_TABLE_H_

#include "typedef.h"
#include "statemanager.h"

class StringTableItem
{
public:
    StringTableItem(); 

    StringTableItem *GetNext() { return m_next; }
    void SetNext(StringTableItem *a_next) { m_next = a_next; }

// Easy access before style, and these don't need internal management:
    typestr m_src;
    typestr m_dst;

private:
    StringTableItem* m_next;
};

class StringTable
{
public:
    StringTable(TStateManager *a_errorhandler); 
    ~StringTable();
    
    typestr GetName() { return m_name; }

    bool Load(const typestr a_filename);
    StringTableItem* AddItem();
    StringTableItem* GetFirstItem() { return m_first; }

    StringTable* GetNextTable() { return m_next_table; }
    void SetNextTable(StringTable *a_next) { m_next_table = a_next; }
  
    static bool parse_line(const char *a_line, typestr &a_src, typestr &a_dst);

private:
    StringTable(); 

    void DeleteItems();

    StringTableItem* m_first;
    StringTableItem* m_last;
    StringTable* m_next_table;
    typestr m_name;
    TStateManager *m_errorhandler;
};

class StringTableList
{
public:
    StringTableList(TStateManager *a_errorhandler); 
    ~StringTableList();
    
    StringTable* AddTable();
    StringTable* GetFirstTable() { return m_first; }

private:
    StringTableList();

    void DeleteTables();

    StringTable* m_first;
    StringTable* m_last;
    TStateManager *m_errorhandler;
};

#endif
