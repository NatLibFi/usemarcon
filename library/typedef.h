/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  typedef.h
 *
 *
 
OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004
    Adapted by The National Library of Finland, 2004-2009


AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/
#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <string.h>
#include <malloc.h>
#include "defines.h"

class typestr
{
protected:
    static const unsigned int buffersize = 200;
    char m_buffer[buffersize];

    char *m_str;
    unsigned long m_size;

public:
    typestr();
    typestr(const typestr & t);
    typestr(const char* t);

    ~typestr();

    void allocstr(unsigned long size);
    void freestr();
    bool is_empty();
    char* str(const char *str, unsigned long maxlen = 0);
    inline char *str() { return m_str; }
    inline const char *cstr() const { return m_str; }
    char *append(const char *a_str, unsigned long a_len = 0);
    // Optimized for repeating calls
    char *append_char(char c);
    void replace(const char *src, const char *dst);
    typestr & append(const typestr & a_str);
    // Promise space for given number of bytes without destroying original contents
    void promise(unsigned long size);

    typestr & operator=(const typestr & t)
    {
        allocstr(t.m_size);
        memcpy(m_str, t.m_str, t.m_size);
        return *this;
    }
    
    typestr & operator=(const char *t)
    {
        str(t);
        return *this;
    }

    bool operator == (const char *s)
    {
        return strcmp(m_str ? m_str : "", s ? s : "") == 0;
    }

    bool operator == (typestr & t)
    {
        return strcmp(m_str ? m_str : "", t.str() ? t.str() : "") == 0;
    }

    typestr & operator + (const typestr & t)
    {
        append(t.m_str);
        return *this;
    }

    typestr & operator + (const char *t)
    {
        append(t);
        return *this;
    }

    typestr & operator + (const char t)
    {
        append_char(t);
        return *this;
    }

    typestr & operator += (const typestr & t)
    {
        append(t.m_str);
        return *this;
    }

    typestr & operator += (const char *t)
    {
        append(t);
        return *this;
    }

    typestr & operator += (const char t)
    {
        append_char(t);
        return *this;
    }

    // Rule formatted string init
    void rulestr(const char *s);

};

class typestr2 : public typestr
{
public:
    typestr s2;
    char script[11];

    typestr2()
    {
        *script = '\0';
    }

    typestr2 & operator=(const typestr2 & t)
    {
        str(t.cstr());
        s2.str(t.s2.cstr());
        if (!*script)
            strcpy(script, t.script);
        return *this;
    }

    void set_script(const char *a_script)
    {
        if (!*script && a_script && *a_script)
        {
            strncpy(script, a_script, 10);
            script[10] = '\0';
        }
    }
};

class TypeInst
{
public:
    int val;
    typestr2 str;
};

class TypeCD
{
public:
    TypeCD() : Output(false), nt(0), ns(0)
    {
        Field[0] = '\0';
        Field[1] = '\0';
        Field[2] = '\0';
        Field[3] = '\0';
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
