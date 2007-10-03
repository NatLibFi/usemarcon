/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  regexp.cpp
 *
 *  implements a class to manage input and output of MARC records
 *
 */

#include "regexp.h"

RegExp::RegExp() : m_pcre(NULL), m_match_count(0), m_error_code(0), m_error_pos(0) 
{
}

RegExp::RegExp(const char *a_regexp, bool a_utf8, bool a_multiline) : m_pcre(NULL), m_match_count(0), m_error_code(0), m_error_pos(0) 
{
    init(a_regexp, a_utf8, a_multiline);
}

RegExp::~RegExp()
{
    pcre_free(m_pcre);
}

bool RegExp::init(const char *a_regexp, bool a_utf8, bool a_multiline)
{
    pcre_free(m_pcre);

    int re_opts = PCRE_NEWLINE_ANYCRLF;
    if (a_utf8)
        re_opts |= PCRE_UTF8;
    if (a_multiline) 
        re_opts |= PCRE_MULTILINE;
    m_error_code = 0;
    m_error_pos = 0;
    const char *error_ptr = NULL;
    m_pcre = pcre_compile2(a_regexp, re_opts, &m_error_code, &error_ptr, &m_error_pos, NULL);
    if (error_ptr)
        m_error_pos_str = error_ptr;
    else
        m_error_pos_str = "";
    return m_pcre ? true : false;
}

int RegExp::exec(const char *a_str)
{
    m_str = a_str;
    m_match_count = pcre_exec(m_pcre, NULL, m_str.str(), strlen(m_str.str()), 0, 0, m_vector, m_vector_size);
    if (m_match_count == 0)
        m_match_count = m_vector_size / 3;
    return m_match_count;
}

int RegExp::match_start(int a_index)
{
    if (a_index < 0 || a_index >= m_vector_size / 3)
        return -1;
    return m_vector[2 * a_index];
}

int RegExp::match_end(int a_index)
{
    if (a_index < 0 || a_index >= m_vector_size / 3)
        return -1;
    return m_vector[2 * a_index + 1];
}

void RegExp::match(int a_index, typestr &a_str)
{
    if (a_index < 0 || a_index >= m_vector_size / 3)
        a_str.freestr();
    else
        a_str.str(m_str.str() + m_vector[2 * a_index], m_vector[2 * a_index + 1] - m_vector[2 * a_index]);
}
