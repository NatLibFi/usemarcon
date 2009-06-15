/*
*  USEMARCON Software - Command Line version
*
*  File:  mempool.h
*

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2008

AUTHOR
    The National Library of Finland

*/

#include "typedef.h"

class MemoryPool
{
public:
    MemoryPool() : m_typeinst_pool_pos(-1), m_typecd_pool_pos(-1)
    {
    }

    ~MemoryPool()
    {
        for (int i = m_typeinst_pool_pos; i >= 0; i--)
        {
            delete m_typeinst_pool[i];
        }
        for (int i = m_typecd_pool_pos; i >= 0; i--)
        {
            delete m_typecd_pool[i];
        }
    }

    TypeInst* AllocTypeInst()
    {
        if (m_typeinst_pool_pos >= 0)
        {
            return m_typeinst_pool[m_typeinst_pool_pos--];
        }
        else
        {
            TypeInst *t = new TypeInst;
            ResetTypeInst(t);
            return t;
        }
    }

    void FreeTypeInst(TypeInst *t)
    {
        if (!t)
            return;
        if (m_typeinst_pool_pos < m_typeinst_pool_size - 1)
        {
            ResetTypeInst(t);
            m_typeinst_pool[++m_typeinst_pool_pos] = t;
        }
        else
        {
            delete t;
        }
    }
    TypeCD* AllocTypeCD()
    {
        if (m_typecd_pool_pos >= 0)
        {
            return m_typecd_pool[m_typecd_pool_pos--];
        }
        else
        {
            TypeCD *c = new TypeCD;
            ResetTypeCD(c);
            return c;
        }
    }

    void FreeTypeCD(TypeCD *c)
    {
        if (!c)
            return;
        ResetTypeCD(c);
        if (m_typecd_pool_pos < m_typecd_pool_size - 1)
        {
            m_typecd_pool[++m_typecd_pool_pos] = c;
        }
        else
        {
            delete c;
        }
    }

private:
    static const int m_typeinst_pool_size = 150;
    TypeInst* m_typeinst_pool[m_typeinst_pool_size];
    int m_typeinst_pool_pos;

    static const int m_typecd_pool_size = 120;
    TypeCD* m_typecd_pool[m_typecd_pool_size];
    int m_typecd_pool_pos;

    void ResetTypeInst(TypeInst *t)
    {
        t->str.freestr();
        t->str.s2.freestr();
        *t->str.script = '\0';
        t->val = 0;
    }

    void ResetTypeCD(TypeCD *c)
    {
        c->Fixed.freestr();
        *c->Field = '\0';
        c->ns = 0;
        c->nt = 0;
        *c->SubField = '\0';
        c->Output = false;
    }
};
