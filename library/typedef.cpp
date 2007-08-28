/*  
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *  
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  typedef.cpp
 *
 *  implements some type classes
 *
 */

#include "typedef.h"

typestr::typestr() : m_str(NULL), m_size(0)
{
}

typestr::typestr(const typestr & t)
{
    m_str = NULL;
    m_size = 0;
    allocstr(t.m_size);
    memcpy(m_str, t.m_str, t.m_size);
}

typestr::typestr(const char *t)
{
    m_str = NULL;
    m_size = 0;
    str(t);
}

typestr::~typestr()
{
    freestr();
}

void typestr::allocstr(unsigned long size)
{
    freestr();
    if (size <= buffersize)
    {
        m_str = m_buffer;
        m_size = buffersize;
    }
    else
    {
        m_str = (char *) malloc(size);
        m_size = size;
    }
    *m_str = '\0';
}

void typestr::freestr()
{
    if (m_str != m_buffer)
    {
        free(m_str);
    }
    *m_buffer = '\0';
    m_str = NULL;
    m_size = 0;
}

bool typestr::is_empty()
{
    if (m_str && *m_str)
        return false;
    return true;
}

char* typestr::str(const char *str, unsigned long maxlen /*= 0*/)
{
    if (!str)
    {
        freestr();
        return NULL;
    }
    unsigned long needed = 1 + (maxlen > 0 ? maxlen : strlen(str));
    if (m_size < needed)
    {
        allocstr(needed);
    }
    if (maxlen > 0)
    {
        strncpy(m_str, str, maxlen);
        m_str[maxlen] = '\0';
    }
    else
        strcpy(m_str, str);
    return m_str;
}

char *typestr::append(const char *a_str, unsigned long a_len /*= 0*/)
{
    if (!m_str) 
        return str(a_str, a_len);
    unsigned long existing_len = strlen(m_str);
    unsigned long append_len = a_len == 0 ? strlen(a_str) : a_len;

    unsigned long needed = existing_len + append_len + 1;
    if (m_size < needed)
    {
        needed += 100;
        promise(needed);
    }
    strncat(m_str, a_str, append_len);
    return m_str;
}

// Optimized for repeating calls
char *typestr::append_char(char c)
{
    unsigned long existing_len = m_str ? strlen(m_str) : 0;
    unsigned long needed = existing_len + 1 + 1;
    if (m_size < needed)
    {
        needed += 100;
        promise(needed);
    }
    m_str[existing_len] = c;
    m_str[existing_len + 1] = '\0';
    return m_str;
}

void typestr::replace(const char *src, const char *dst)
{
    if (!m_str)
        return;

    unsigned long existing_len = strlen(m_str);
    unsigned long srclen = strlen(src);
    unsigned long dstlen = strlen(dst);
    long needed = dstlen - srclen;

    char *p = m_str;
    while (p = strstr(p, src))
    {
        if (needed > 0 && m_size <= existing_len + needed)
        {
            unsigned long p_pos = strlen(p);
            needed += 100;
            promise(existing_len + needed);
            p = strstr(m_str, src);
            while (strlen(p) > p_pos)
                p = strstr(p, src);
        }
        *p = '\0';
        typestr tmpstr = m_str;
        tmpstr.append(dst);
        tmpstr.append(p + srclen);
        strcpy(m_str, tmpstr.str());
        existing_len += needed;
        p += dstlen;
    }
}

typestr & typestr::append(const typestr & a_str)
{
    append(a_str.m_str);
    return *this;
}

void typestr::promise(unsigned long size)
{
    if (m_size >= size)
        return;
    
    typestr tmpstr = *this;
    allocstr(size);
    memcpy(m_str, tmpstr.str(), tmpstr.m_size);
}

// Lexer formatted string init
void typestr::initstr(const char *s)
{
    int i=1, j=0;
    allocstr(strlen(s) + 1);

    while(s[i])
    {
        switch(s[i])
        {
        case '$' :
            if (s[i+1]=='$')
                m_str[j++] = s[++i];
            else
                m_str[j++] = START_OF_FIELD;
            ++i;
            break;
        case '\\' : // hex value
            ++i;
            if (s[i]=='\\' || s[i]=='$') m_str[j++]=s[i++];
            else
            {
                int d=s[i++];
                int u=s[i++];
                if (d>='0' && d<='9') d-='0';
                else
                    if (d>='A' && d<='F') d-='A'-10;
                    else
                        if (d>='a' && d<='f') d-='a'-10;
                if (u>='0' && u<='9') u-='0';
                else
                    if (u>='A' && u<='F') u-='A'-10;
                    else
                        if (u>='a' && u<='f') u-='a'-10;
                m_str[j++]=16*d+u;
            }
            break;
        default : m_str[j++]=s[i++];
        }
    }
    m_str[j-1]=0;
}

