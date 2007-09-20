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
    Adapted by The National Library of Finland, 2004-2007

AUTHOR
    The National Library of Finland

*/

#ifndef _STRING_TABLE_H_
#define _STRING_TABLE_H_

#include "typedef.h"
#include "error.h"

class StringTable
{
public:
    StringTable(TError *a_errorhandler); 
    ~StringTable();
    
    StringTable *GetNext() { return m_next; }
    void SetNext(StringTable *a_next) { m_next = a_next; }

    typestr GetName() { return m_name; }

    bool Load(const typestr a_filename);

    static bool parse_line(const char *a_line, typestr &a_src, typestr &a_dst);

// Easy access before style, and these don't need internal management:
    typestr m_src;
    typestr m_dst;

private:
    StringTable(); 

    StringTable *m_next;
    typestr m_name;
    TError *m_errorhandler;
};

#endif