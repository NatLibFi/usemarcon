/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  typedef.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004
    Adapted by The National Library of Finland, 2004-2007


AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/
#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <string.h>

#if !defined(TRUE)
  #define TRUE  1
#endif

#if !defined(FALSE)
  #define FALSE 0
#endif

#include <malloc.h>

class typestr
{
private:
    static const unsigned int buffersize = 200;
    char m_buffer[buffersize];

    char *m_str;
    unsigned long m_size;

public:
    typestr() : m_str(NULL), m_size(0)
    {
    }

    typestr(const typestr & t)
    {
        m_str = NULL;
        m_size = 0;
        str(t.m_str);
    }

    typestr(const char *t)
    {
        m_str = NULL;
        m_size = 0;
        str(t);
    }

    ~typestr()
    {
        freestr();
    }

    typestr & operator=(const typestr & t)
    {
        str(t.m_str);
        return *this;
    }

    typestr & operator=(const char *t)
    {
        str(t);
        return *this;
    }

    bool operator == (const char *s)
    {
        return strcmp(m_str, s) == 0;
    }

    bool operator == (typestr & t)
    {
        return strcmp(m_str, t.str()) == 0;
    }

    void allocstr(unsigned long size)
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
    }

    void freestr()
    {
        if (m_str != m_buffer)
        {
            free(m_str);
        }
        *m_buffer = '\0';
        m_str = NULL;
        m_size = 0;
    }

    bool is_empty()
    {
        if (m_str && *m_str)
            return false;
        return true;
    }

    char* str(const char *str)
    {
        if (!str)
        {
            freestr();
            return NULL;
        }
        unsigned long needed = strlen(str) + 1;
        if (m_size < needed)
        {
            allocstr(needed);
        }
        strcpy(m_str, str);
        return m_str;
    }

    inline char *str()
    {
        return m_str;
    }

    char *append(const char *a_str, unsigned long a_len = 0)
    {
        if (!m_str) 
            return str(a_str);
        unsigned long existing_len = strlen(m_str);
        unsigned long append_len = a_len == 0 ? strlen(a_str) : a_len;

        unsigned long needed = existing_len + append_len + 1;
        if (m_size < needed)
        {
            needed += 100;
            typestr tmpstr = *this;
            allocstr(needed);
            strcpy(m_str, tmpstr.str());
        }
        strncat(m_str, a_str, append_len);
        return m_str;
    }

    typestr & append(typestr & a_str)
    {
        append(a_str.str());
        return *this;
    }

    // Optimized for repeating calls
    char *append_char(char c)
    {
        unsigned long existing_len = m_str ? strlen(m_str) : 0;
        unsigned long needed = existing_len + 1 + 1;
        if (m_size < needed)
        {
            needed += 100;
            typestr tmpstr = *this;
            allocstr(needed);
            strcpy(m_str, tmpstr.str() ? tmpstr.str() : "");
        }
        m_str[existing_len] = c;
        m_str[existing_len + 1] = '\0';
        return m_str;
    }

    void replace(const char *src, const char *dst)
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
            if (needed > 0 && m_size < existing_len + needed)
            {
                unsigned long p_pos = strlen(p);
                needed += 100;
                typestr tmpstr = *this;
                allocstr(needed);
                strcpy(m_str, tmpstr.str());
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

    // Lexer formatted string init
    void initstr(const char *s)
    {
        int i=1, j=0;
        allocstr(strlen(s) + 1);

        while(s[i])
        {
            switch(s[i])
            {
            case '$' :
                if (s[i+1]=='$')
                    m_str[j++]=s[++i];
                else
                    m_str[j++]=0x1F;
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

};

class TypeInst
{
public:
    int val;
    typestr str;
};


class TypeCD
{
public:
    TypeCD() : Output(false)
    {
        SubField[0] = '\0';
        SubField[1] = '\0';
    }

    char Field[4];
    int nt;
    char SubField[3];
    int ns;
    bool Output;
    typestr Fixed;
};

#endif
