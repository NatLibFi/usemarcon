/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  marcfile.h
 *
 *
 
CLASS
    TMarcFile

    implements a class to manage MARC record files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2011

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _REGEXP_H_
#define _REGEXP_H_

#include "typedef.h"
#include "../pcre/pcre.h"

class RegExp
{
private:
    pcre* m_pcre;
    typestr m_str;
    static const int m_vector_size = 30;
    int m_vector[m_vector_size];
    int m_match_count;

    int m_error_code;
    int m_error_pos;
    typestr m_error_pos_str;

public:
    RegExp();
    RegExp(const char *a_regexp, bool a_utf8 = true, bool a_multiline = false);
    ~RegExp();

    bool init(const char *a_regexp, bool a_utf8 = true, bool a_multiline = false);
    int exec(const char *a_str);
    int replace(typestr &a_str, const char *a_replacement, bool a_global);

    int match_count() { return m_match_count; }
    int match_start(int a_index);
    int match_end(int a_index);
    void match(int a_index, typestr &a_str);

    int error_code() { return m_error_code; }
    int error_pos() { return m_error_pos; }
    typestr error_pos_str() { return m_error_pos_str; }
};

#endif
