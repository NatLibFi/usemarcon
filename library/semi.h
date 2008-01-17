/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  semi.h
 *
 *
 
CLASS
    semi

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2008
*/

#ifndef SEMI
#define SEMI

class quadruplet
{
private:
   quadruplet();
public :
    unsigned char valeur; // value
    quadruplet *droite; // right
    quadruplet *bas; // bottom
    void *remplacant; // replacement ??
    quadruplet(unsigned char, quadruplet *, quadruplet *, void *);
    ~quadruplet();
};

class doublet
{
public:
    quadruplet *bas; // bottom
    void *remplacant; // replacement ??
    doublet();
    ~doublet();
};

class semi
{
public:
    doublet racine[256]; // root
    long taille; // size
    semi();
    ~semi();
    void *cherche(unsigned char *ch, int *); // search
    void insere(unsigned char *, void *); // insert
};

#endif
