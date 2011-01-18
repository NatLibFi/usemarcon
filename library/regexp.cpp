/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2011
 *
 *  File:  regexp.cpp
 *
 *  implements a class to manage input and output of MARC records
 *
 */

#include <ctype.h>
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
        re_opts |= PCRE_MULTILINE | PCRE_DOTALL;
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
    m_match_count = pcre_exec(m_pcre, NULL, m_str.str(), (int)strlen(m_str.str()), 0, 0, m_vector, m_vector_size);
    if (m_match_count == 0)
        m_match_count = m_vector_size / 3;
    return m_match_count;
}

int RegExp::replace(typestr &a_str, const char *a_replacement, bool a_global)
{
    int loop_count = 0;
    size_t start_pos = 0;
    int replaced = 0;
    while (loop_count++ < 10000)
    {
        int res = exec(a_str.str() + start_pos);
        if (res == PCRE_ERROR_NOMATCH)
            break;
        if (res < 0)
            return res;
        
        typestr replacement;
        const char *p = a_replacement;
        while (*p)
        {
            if (*p == '\\')
            {
                ++p;
                if (*p == '\\')
                    replacement.append_char('\\');
                else if (isdigit(*p))
                {
                    int index = *p - '0';
                    replacement.append(m_str.str() + m_vector[2 * index], m_vector[2 * index + 1] - m_vector[2 * index]);
                }
            }
            else
                replacement.append_char(*p);
            ++p;
        }
        size_t replace_start = m_vector[0];
        size_t replace_end = m_vector[1];
        typestr tmp;
        if (replace_start > 0)
            tmp.str(m_str.str(), replace_start);
        tmp.append(replacement);
        size_t prev_start_pos = start_pos;
        start_pos = strlen(tmp.str());
        tmp.append(m_str.str() + replace_end);
        *(a_str.str() + prev_start_pos) = '\0';
        a_str.append(tmp);

        ++replaced;

        if (!a_global)
            break;
    }
    return replaced;
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
    if (a_index < 0 || a_index >= m_vector_size / 3 || a_index > m_match_count)
        a_str.freestr();
    else
        a_str.str(m_str.str() + m_vector[2 * a_index], m_vector[2 * a_index + 1] - m_vector[2 * a_index]);
}
