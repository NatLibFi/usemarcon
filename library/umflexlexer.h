/*
*  USEMARCON Software - Command Line version
*
*  File:  umflexlexer.h
*

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2011

AUTHOR
    The National Library of Finland

*/

#ifndef __UM_FLEX_LEXER_H
#define __UM_FLEX_LEXER_H

#include "ytab.h"
#include <FlexLexer.h>

class umFlexLexer : public yyFlexLexer 
{
public:
    int yylex(MarcParser::YYSTYPE &yylval, void *allocator);

};

#endif
