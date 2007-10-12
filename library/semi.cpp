/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  semi.cpp
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semi.h"
#define MAXTAILLE 256
#define MAXID 80

quadruplet::quadruplet(unsigned char c,quadruplet *d,quadruplet *b, void *r)
{
    valeur = c;
    droite = d;
    bas = b;
    remplacant = r;
}

quadruplet::~quadruplet()
{
    if (droite)
        delete droite;
    if (bas)
        delete bas;

    if (remplacant)
        free(remplacant);
}

doublet::doublet()
{
    bas = NULL;
    remplacant = NULL;
}

doublet::~doublet()
{
    if (bas)
        delete bas;

    if (remplacant)
        free(remplacant);
}

semi::semi()
{
    taille =0;
}

semi::~semi()
{
}

void *semi::cherche(unsigned char *ch, int *i)
{
    // recherche dans l'arborescence la chaine ch, retourne le nombre de caracteres
    // qui a pu etre trouve a la sortie, remp contient l'adresse du pointeur sur la
    // chaine qu'il faut associer a ch
    *i=0;
    int ii =1;
    void *remp = NULL;
    if(*ch == '\0')return(NULL);
    // A special optimization treatment for the first character. 
    // It's quickly found in a table.
    doublet *d = racine + *ch;
    if(d->remplacant != NULL)
    {
        *i = ii;
        remp = d->remplacant;
    }
    quadruplet *r = d->bas;
    unsigned char *c = ch+1;
    while(*c != '\0'){
        while((r != NULL) && (r->valeur < *c))
            r = r->droite;
        if((r == NULL)||(r->valeur > *c))return(remp);
        ii++;
        if(r->remplacant!= NULL)
        {
            *i = ii;
            remp = r->remplacant;
        }
        c++;
        r= r->bas;
    }
    //  toute la chaine a pu etre trouvee
    return(remp);
}

void semi::insere(unsigned char *ch, void *remp)
{
    // Inserting a new entry into the tree. ch indicates the string to
    // replace and remp is the replacement

    quadruplet *dn;

    if(*ch != '\0')
    {
        // The first character is put directly on the table
        doublet *d = racine + *ch;
        void **pavant = &(d->remplacant);
        quadruplet **pprec = &(d->bas);
        unsigned char *c = ch+1;
        quadruplet *r = d->bas;
        while(*c != '\0')
        {
            while((r != NULL) && (r->valeur < *c))
            {
                pprec = &(r->droite);
                pavant = &(r->remplacant);
                r = r->droite;
            }
            if((r == NULL)||(r->valeur >*c))
            {
                // Need to add a quadruplet into the list
                dn = new quadruplet(*c,r,NULL,NULL);
                c++;
                *pprec = dn;
                pavant = &(dn->remplacant);
                pprec = &(dn->bas);
                r = dn ->bas;
            }
            else
            {
                pprec = &(r->bas);
                pavant = &(r->remplacant);
                r= r->bas;
                c++;
            }
        }
        // In all cases replacement can be assigned to pavant->remplacant
        if(*pavant == NULL)
            *pavant = remp;
        else
            free((char *)remp);
    }
    else
        free((char *)remp);
}

