%option c++
%option never-interactive
%{

#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "typedef.h"
#include "mempool.h"
#include "ytab.h"

%}

blancs          [ \t\n]*
digits          [0-9]+
digit           [0-9]
maj             [A-Z]

tag             ({digit}|{maj}|\?)
indic1          ("I1"|"i1")
indic2          ("I2"|"i2")
subfield        "$"[0-9a-zA-Z]
fixedpos        "/"{digits}("-"{digits})?"/"

%%
\'[^\']+\'      {
                  yylval.inst=((MemoryPool *) allocator)->AllocTypeInst();
                  yylval.inst->str.rulestr(yytext);
                  return MarcParser::STRING;
                }
\'\'            {
                  yylval.inst=((MemoryPool *) allocator)->AllocTypeInst();
                  yylval.inst->str.str("");
                  return MarcParser::STRING;
                }
"%"?{tag}{tag}{tag}/("("|"$"|"/"|{indic1}|{indic2}) {
                  yylval.inst=((MemoryPool *) allocator)->AllocTypeInst();
                  yylval.inst->str.str(yytext);
                  return MarcParser::TAG;
                }
"@"{tag}{tag}{tag}      {
                  yylval.inst=((MemoryPool *) allocator)->AllocTypeInst();
                  yylval.inst->str.str(yytext+1);
                  return MarcParser::TAG;
                }
"%"{tag}{tag}{tag}      {
                  yylval.inst=((MemoryPool *) allocator)->AllocTypeInst();
                  yylval.inst->str.str(yytext);
                  return MarcParser::TAG;
                }
{indic1}        { return MarcParser::I1; }
{indic2}        { return MarcParser::I2; }
{subfield}      {
                  yylval.inst=((MemoryPool *) allocator)->AllocTypeInst();
                  yylval.inst->str.str(yytext);
                  return MarcParser::STAG;
                }
{fixedpos}      {
                  yylval.inst=((MemoryPool *) allocator)->AllocTypeInst();
                  yylval.inst->str.str(yytext);
                  return MarcParser::FIX;
                }
"#"             { return MarcParser::FIN; }
[,{}()]         return *yytext;
";"             { return MarcParser::SEP; }
{digits}        {
                  yylval.inst=((MemoryPool *) allocator)->AllocTypeInst();
                  yylval.inst->val=atoi(yytext);
                  return MarcParser::NUMERIC;
                }
"+"                     return MarcParser::PLUS;
"-"                     return MarcParser::MOINS;
"*"                     return MarcParser::MULTIPLIE;
":"                     return MarcParser::DIVISE;
S                       return MarcParser::VARS;
D                       return MarcParser::VARD;
[N|n][T|t][O|o]         return MarcParser::VAR_NTO;
[N|n][S|s][O|o]         return MarcParser::VAR_NSO;
[N|n][T|t]              return MarcParser::VAR_NT;
[N|n][S|s]              return MarcParser::VAR_NS;
[N|n][O|o]              return MarcParser::VAR_NO;
[N|n]                   return MarcParser::VAR_N;
[N|n][E|e][W|w]         return MarcParser::VAR_NEW;
[I|i]f                  return MarcParser::IF;
[T|t]hen                return MarcParser::THEN;
[E|e]lse                return MarcParser::ELSE;
[A|a]nd                 return MarcParser::AND;
[O|o]r                  return MarcParser::OR;
[N|n]ot                 return MarcParser::NOT;
"!="                    return MarcParser::NE;
"="                     return MarcParser::EQ;
[I|i][N|n]              return MarcParser::_IN;
">="                    return MarcParser::GE;
"<="                    return MarcParser::LE;
">"                     return MarcParser::GT;
"<"                     return MarcParser::LT;
[S|s]tr                 return MarcParser::STR;
[V|v]al                 return MarcParser::VAL;
[L|l]en                 return MarcParser::LEN;
[S|s]to                 return MarcParser::STO;
[M|m]em                 return MarcParser::MEM;
[E|e]xc                 return MarcParser::EXC;
[C|c]lr                 return MarcParser::CLR;
[F|f]rom                return MarcParser::FROM;
[T|t]o                  return MarcParser::TO;
[B|b]etween             return MarcParser::BETWEEN;
[S|s]trict              return MarcParser::_STRICT;
[D|d]elete              return MarcParser::_DELETE;
[R|r]eplace             return MarcParser::REPLACE;
[R|r]eplace[O|o]cc return MarcParser::REPLACEOCC;
[A|a]t                  return MarcParser::AT;
[B|b]y                  return MarcParser::BY;
[B|b]eginning           return MarcParser::BEGINNING;
[E|e]nding              return MarcParser::END;
[B|b]oundaries          return MarcParser::BOTH;
[B|b][F|f]irst          return MarcParser::BFIRST;
[E|e][F|f]irst          return MarcParser::EFIRST;
[B|b][L|l]ast           return MarcParser::BLAST;
[E|e][L|l]ast           return MarcParser::ELAST;
[N|n]umber              return MarcParser::WNUMBER;
[S|s]tring              return MarcParser::WSTRING;
[E|e]xists              return MarcParser::EXISTS;
[P|p]recedes            return MarcParser::PRECEDES;
[F|f]ollows             return MarcParser::FOLLOWS;
[R|r]edo                return MarcParser::REDO;
[S|s]ort                return MarcParser::SORT;
[N|n]ext                return MarcParser::NEXT;
[L|l]ast                return MarcParser::LAST;
[T|t]able               return MarcParser::TABLE;
[U|u]pper               return MarcParser::UPPER;
[L|l]ower               return MarcParser::LOWER;
[Y|y]ear                return MarcParser::YEAR;
[M|m]onth               return MarcParser::MONTH;
[D|d]ay                 return MarcParser::DAY;
[H|h]our                return MarcParser::HOUR;
[M|m]inute              return MarcParser::MINUTE;
[S|s]econd              return MarcParser::SECOND;
[O|o]rdinal             return MarcParser::ORDINAL;
[N|n]ext[S|s]ub         return MarcParser::NEXTSUB;
[P|p]revious[S|s]ub     return MarcParser::PREVIOUSSUB;
[R|r]eg[F|f]ind         return MarcParser::REGFIND;
[R|r]eg[M|m]atch        return MarcParser::REGMATCH;
[R|r]eg[R|r]eplace      return MarcParser::REGREPLACE;
[R|r]eg[R|r]eplace[T|t]able return MarcParser::REGREPLACETABLE;
[I|i]n[T|t]able         return MarcParser::INTABLE;
{blancs}                        ;
.                       lex_error = true;
%%

