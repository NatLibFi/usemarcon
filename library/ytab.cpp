#define YY_MarcParser_h_included

/*  A Bison++ parser, made from Analyse.y  */

 /* with Bison++ version bison++ Version 1.21-8, adapted from GNU bison by coetmeur@icdc.fr
  */

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* HEADER SECTION */
#if defined( _MSDOS ) || defined(MSDOS) || defined(__MSDOS__) 
#define __MSDOS_AND_ALIKE
#endif
#if defined(_WINDOWS) && defined(_MSC_VER)
#define __HAVE_NO_ALLOCA
#define __MSDOS_AND_ALIKE
#endif

#ifndef alloca
#if defined( __GNUC__)
#define alloca __builtin_alloca

#elif (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc)  || defined (__sgi)
#include <alloca.h>

#elif defined (__MSDOS_AND_ALIKE)
#include <malloc.h>
#ifndef __TURBOC__
/* MS C runtime lib */
#define alloca _alloca
#endif

#elif defined(_AIX)
#include <malloc.h>
#pragma alloca

#elif defined(__hpux)
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */

#endif /* not _AIX  not MSDOS, or __TURBOC__ or _AIX, not sparc.  */
#endif /* alloca not defined.  */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else
#ifndef __STDC__
#define const
#endif
#endif
#include <stdio.h>
#define YYBISON 1  
#define YY_MarcParser_LSP_NEEDED 
#define YY_MarcParser_ERROR_BODY  =0
#define YY_MarcParser_LEX_BODY  =0
#define YY_MarcParser_MEMBERS  \
  virtual const char* NextSubField(TypeCD*, TypeCD*) = 0; \
  virtual const char* LastSubField(TypeCD*,TypeCD*) = 0; \
  virtual int Precedes(TypeCD*, TypeCD*) = 0; \
  virtual int Exists(TypeCD*) = 0; \
  virtual const char* LireCD(TypeCD*) = 0; \
  virtual const char* NextBalise() = 0; \
  virtual const char* PreviousBalise() = 0; \
  virtual TypeCD* AllocCD() = 0; \
  virtual void FreeCD( TypeCD* CD ) = 0; \
  virtual TypeInst* Next_( TypeCD* cd1, TypeCD* cd2, int strict ) = 0; \
  virtual TypeInst* Last_( TypeCD* cd1, TypeCD* cd2, int strict ) = 0; \
        virtual TypeInst* NextBal() = 0; \
        virtual TypeInst* PreviousBal() = 0; \
  virtual TypeInst* Soust( TypeInst* t1, TypeInst* t2 ) = 0; \
  virtual TypeInst* Multi( TypeInst* t1, TypeInst* t2 ) = 0; \
  virtual TypeInst* Divis( TypeInst* t1, TypeInst* t2 ) = 0; \
  virtual TypeInst* Value( TypeInst* t ) = 0; \
  virtual int MemSto( TypeInst* n ) = 0; \
  virtual TypeInst* MemMem( TypeInst* n ) = 0; \
  virtual int MemClr( TypeInst* n  ) = 0; \
  virtual TypeInst* MemExc( TypeInst* n ) = 0; \
\
TypeInst* S; \
TypeInst* T; \
TypeInst* D; \
TypeCD* CDIn; \
\
TypeInst* N; \
TypeInst* NT; \
TypeInst* NS; \
TypeInst* NO; \
TypeInst* NSO; \
TypeInst* NTO; \
TypeInst* Memoire[100]; \
\
int debug_rule; \
char tempo[1000]; \
int  RedoFlag; \
unsigned long ordinal; \
\
virtual typestr Table( char*, char* ) = 0; \
virtual int   MustSort( char* ) = 0; \
\
virtual TypeInst* AllocTypeInst() = 0; \
virtual void FreeTypeInst( TypeInst* t ) = 0; \
virtual int Copie( TypeInst** In, TypeInst* From ) = 0; \
virtual int BoolEQ( TypeInst* t1, TypeInst* t2 ) = 0; \
virtual int BoolIn( TypeInst* t1, TypeInst* t2 ) = 0; \
virtual int BoolGT( TypeInst* t1, TypeInst* t2 ) = 0; \
virtual int BoolGE( TypeInst* t1, TypeInst* t2 ) = 0; \
virtual TypeInst* Ajout( TypeInst* t1, TypeInst* t2 ) = 0; \
virtual TypeInst* AjoutOcc( TypeInst* t1, TypeInst* t2 ) = 0; \
virtual char* ToString( TypeInst* t ) = 0; \
virtual TypeInst* String( TypeInst* t ) = 0; \
virtual TypeInst* Upper( TypeInst* t ) = 0; \
virtual TypeInst* Lower( TypeInst* t ) = 0; \
virtual TypeInst* Len( TypeInst* t ) = 0; \
virtual TypeInst* From( TypeInst* t, int strict ) = 0; \
virtual TypeInst* To( TypeInst* t, int strict ) = 0; \
virtual TypeInst* Between( TypeInst* t1, TypeInst* t2, int strict ) = 0; \
virtual TypeInst* Replace( TypeInst* t1, TypeInst* t2, int at, int strict ) = 0; \
virtual TypeInst* ReplaceOcc( TypeInst* t1, TypeInst* t2, TypeInst* inCondOcc, int strict ) = 0; \
virtual TypeInst* BFirst( TypeInst* t, int k ) = 0; \
virtual TypeInst* EFirst( TypeInst* t, int k ) = 0; \
virtual TypeInst* BLast( TypeInst* t, int k ) = 0; \
virtual TypeInst* ELast( TypeInst* t, int k ) = 0; \
virtual TypeInst* Table_( TypeInst* Nom ) = 0; \
virtual void PrintDebug(const char*) = 0; \
virtual TypeInst* RegFind( TypeInst* t1, TypeInst* t2 ) = 0; \
virtual TypeInst* RegMatch( TypeInst* t1 ) = 0; \
virtual TypeInst* RegReplace( TypeInst* t1, TypeInst* t2, TypeInst* t3 ) = 0; \

#define YY_MarcParser_CONSTRUCTOR_INIT  : S(NULL), T(NULL), D(NULL), CDIn(NULL), N(NULL), NT(NULL), NS(NULL), NO(NULL), NSO(NULL), NTO(NULL)

#pragma warning( disable : 4102 )

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "typedef.h"


typedef union
{
        int   code;
        TypeInst* inst;
        TypeCD*   tcd;
} yy_MarcParser_stype;
#define YY_MarcParser_STYPE yy_MarcParser_stype
/* %{ and %header{ and %union, during decl */
#define YY_MarcParser_BISON 1
#ifndef YY_MarcParser_COMPATIBILITY
#ifndef YY_USE_CLASS
#define  YY_MarcParser_COMPATIBILITY 1
#else
#define  YY_MarcParser_COMPATIBILITY 0
#endif
#endif

#if YY_MarcParser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YYLTYPE
#ifndef YY_MarcParser_LTYPE
#define YY_MarcParser_LTYPE YYLTYPE
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_MarcParser_STYPE 
#define YY_MarcParser_STYPE YYSTYPE
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_MarcParser_DEBUG
#define  YY_MarcParser_DEBUG YYDEBUG
#endif
#endif
#ifdef YY_MarcParser_STYPE
#ifndef yystype
#define yystype YY_MarcParser_STYPE
#endif
#endif
/* use goto to be compatible */
#ifndef YY_MarcParser_USE_GOTO
#define YY_MarcParser_USE_GOTO 1
#endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_MarcParser_USE_GOTO
#define YY_MarcParser_USE_GOTO 0
#endif

#ifndef YY_MarcParser_PURE
/*  YY_MarcParser_PURE */
#endif

/* section apres lecture def, avant lecture grammaire S2 */
/* prefix */
#ifndef YY_MarcParser_DEBUG
/* YY_MarcParser_DEBUG */
#endif


#ifndef YY_MarcParser_LSP_NEEDED
 /* YY_MarcParser_LSP_NEEDED*/
#endif



/* DEFAULT LTYPE*/
#ifdef YY_MarcParser_LSP_NEEDED
#ifndef YY_MarcParser_LTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YY_MarcParser_LTYPE yyltype
#endif
#endif
/* DEFAULT STYPE*/
      /* We used to use `unsigned long' as YY_MarcParser_STYPE on MSDOS,
	 but it seems better to be consistent.
	 Most programs should declare their own type anyway.  */

#ifndef YY_MarcParser_STYPE
#define YY_MarcParser_STYPE int
#endif
/* DEFAULT MISCELANEOUS */
#ifndef YY_MarcParser_PARSE
#define YY_MarcParser_PARSE yyparse
#endif
#ifndef YY_MarcParser_LEX
#define YY_MarcParser_LEX yylex
#endif
#ifndef YY_MarcParser_LVAL
#define YY_MarcParser_LVAL yylval
#endif
#ifndef YY_MarcParser_LLOC
#define YY_MarcParser_LLOC yylloc
#endif
#ifndef YY_MarcParser_CHAR
#define YY_MarcParser_CHAR yychar
#endif
#ifndef YY_MarcParser_NERRS
#define YY_MarcParser_NERRS yynerrs
#endif
#ifndef YY_MarcParser_DEBUG_FLAG
#define YY_MarcParser_DEBUG_FLAG yydebug
#endif
#ifndef YY_MarcParser_ERROR
#define YY_MarcParser_ERROR yyerror
#endif
#ifndef YY_MarcParser_PARSE_PARAM
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
#define YY_MarcParser_PARSE_PARAM
#ifndef YY_MarcParser_PARSE_PARAM_DEF
#define YY_MarcParser_PARSE_PARAM_DEF
#endif
#endif
#endif
#endif
#ifndef YY_MarcParser_PARSE_PARAM
#define YY_MarcParser_PARSE_PARAM void
#endif
#endif
#if YY_MarcParser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YY_MarcParser_LTYPE
#ifndef YYLTYPE
#define YYLTYPE YY_MarcParser_LTYPE
#else
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
#endif
#endif
#ifndef YYSTYPE
#define YYSTYPE YY_MarcParser_STYPE
#else
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
#endif
#ifdef YY_MarcParser_PURE
#ifndef YYPURE
#define YYPURE YY_MarcParser_PURE
#endif
#endif
#ifdef YY_MarcParser_DEBUG
#ifndef YYDEBUG
#define YYDEBUG YY_MarcParser_DEBUG 
#endif
#endif
#ifndef YY_MarcParser_ERROR_VERBOSE
#ifdef YYERROR_VERBOSE
#define YY_MarcParser_ERROR_VERBOSE YYERROR_VERBOSE
#endif
#endif
#ifndef YY_MarcParser_LSP_NEEDED
#ifdef YYLSP_NEEDED
#define YY_MarcParser_LSP_NEEDED YYLSP_NEEDED
#endif
#endif
#endif
#ifndef YY_USE_CLASS
/* TOKEN C */
#define	SEP	258
#define	FIN	259
#define	WNUMBER	260
#define	WSTRING	261
#define	PLUS	262
#define	MOINS	263
#define	MULTIPLIE	264
#define	DIVISE	265
#define	EQ	266
#define	NE	267
#define	_IN	268
#define	GT	269
#define	LT	270
#define	GE	271
#define	LE	272
#define	EXISTS	273
#define	PRECEDES	274
#define	FOLLOWS	275
#define	IF	276
#define	THEN	277
#define	ELSE	278
#define	AND	279
#define	OR	280
#define	NOT	281
#define	BY	282
#define	_STRICT	283
#define	AT	284
#define	BEGINING	285
#define	END	286
#define	BOTH	287
#define	VARS	288
#define	VARD	289
#define	STRING	290
#define	NUMERIC	291
#define	VAR_N	292
#define	VAR_NT	293
#define	VAR_NS	294
#define	VAR_NO	295
#define	VAR_NTO	296
#define	VAR_NSO	297
#define	TAG	298
#define	STAG	299
#define	FIX	300
#define	I1	301
#define	I2	302
#define	STR	303
#define	VAL	304
#define	LEN	305
#define	STO	306
#define	MEM	307
#define	EXC	308
#define	CLR	309
#define	LOWER	310
#define	UPPER	311
#define	FROM	312
#define	TO	313
#define	BETWEEN	314
#define	_DELETE	315
#define	REPLACE	316
#define	REPLACEOCC	317
#define	BFIRST	318
#define	EFIRST	319
#define	BLAST	320
#define	ELAST	321
#define	REDO	322
#define	SORT	323
#define	NEXT	324
#define	LAST	325
#define	TABLE	326
#define	ORDINAL	327
#define	YEAR	328
#define	MONTH	329
#define	DAY	330
#define	HOUR	331
#define	MINUTE	332
#define	SECOND	333
#define	NEXTBAL	334
#define	PREVIOUSBAL	335
#define	REGFIND	336
#define	REGMATCH	337
#define	REGREPLACE	338

 /* #defines tokens */
#else
/* CLASS */
#ifndef YY_MarcParser_CLASS
#define YY_MarcParser_CLASS MarcParser
#endif
#ifndef YY_MarcParser_INHERIT
#define YY_MarcParser_INHERIT
#endif
#ifndef YY_MarcParser_MEMBERS
#define YY_MarcParser_MEMBERS 
#endif
#ifndef YY_MarcParser_LEX_BODY
#define YY_MarcParser_LEX_BODY  
#endif
#ifndef YY_MarcParser_ERROR_BODY
#define YY_MarcParser_ERROR_BODY  
#endif
#ifndef YY_MarcParser_CONSTRUCTOR_PARAM
#define YY_MarcParser_CONSTRUCTOR_PARAM
#endif
#ifndef YY_MarcParser_CONSTRUCTOR_CODE
#define YY_MarcParser_CONSTRUCTOR_CODE
#endif
#ifndef YY_MarcParser_CONSTRUCTOR_INIT
#define YY_MarcParser_CONSTRUCTOR_INIT
#endif
/* choose between enum and const */
#ifndef YY_MarcParser_USE_CONST_TOKEN
#define YY_MarcParser_USE_CONST_TOKEN 0
/* yes enum is more compatible with flex,  */
/* so by default we use it */ 
#endif
#if YY_MarcParser_USE_CONST_TOKEN != 0
#ifndef YY_MarcParser_ENUM_TOKEN
#define YY_MarcParser_ENUM_TOKEN yy_MarcParser_enum_token
#endif
#endif

class YY_MarcParser_CLASS YY_MarcParser_INHERIT
{
public: 
#if YY_MarcParser_USE_CONST_TOKEN != 0
/* static const int token ... */
static const int SEP;
static const int FIN;
static const int WNUMBER;
static const int WSTRING;
static const int PLUS;
static const int MOINS;
static const int MULTIPLIE;
static const int DIVISE;
static const int EQ;
static const int NE;
static const int _IN;
static const int GT;
static const int LT;
static const int GE;
static const int LE;
static const int EXISTS;
static const int PRECEDES;
static const int FOLLOWS;
static const int IF;
static const int THEN;
static const int ELSE;
static const int AND;
static const int OR;
static const int NOT;
static const int BY;
static const int _STRICT;
static const int AT;
static const int BEGINING;
static const int END;
static const int BOTH;
static const int VARS;
static const int VARD;
static const int STRING;
static const int NUMERIC;
static const int VAR_N;
static const int VAR_NT;
static const int VAR_NS;
static const int VAR_NO;
static const int VAR_NTO;
static const int VAR_NSO;
static const int TAG;
static const int STAG;
static const int FIX;
static const int I1;
static const int I2;
static const int STR;
static const int VAL;
static const int LEN;
static const int STO;
static const int MEM;
static const int EXC;
static const int CLR;
static const int LOWER;
static const int UPPER;
static const int FROM;
static const int TO;
static const int BETWEEN;
static const int _DELETE;
static const int REPLACE;
static const int REPLACEOCC;
static const int BFIRST;
static const int EFIRST;
static const int BLAST;
static const int ELAST;
static const int REDO;
static const int SORT;
static const int NEXT;
static const int LAST;
static const int TABLE;
static const int ORDINAL;
static const int YEAR;
static const int MONTH;
static const int DAY;
static const int HOUR;
static const int MINUTE;
static const int SECOND;
static const int NEXTBAL;
static const int PREVIOUSBAL;
static const int REGFIND;
static const int REGMATCH;
static const int REGREPLACE;

 /* decl const */
#else
enum YY_MarcParser_ENUM_TOKEN { YY_MarcParser_NULL_TOKEN=0
	,SEP=258
	,FIN=259
	,WNUMBER=260
	,WSTRING=261
	,PLUS=262
	,MOINS=263
	,MULTIPLIE=264
	,DIVISE=265
	,EQ=266
	,NE=267
	,_IN=268
	,GT=269
	,LT=270
	,GE=271
	,LE=272
	,EXISTS=273
	,PRECEDES=274
	,FOLLOWS=275
	,IF=276
	,THEN=277
	,ELSE=278
	,AND=279
	,OR=280
	,NOT=281
	,BY=282
	,_STRICT=283
	,AT=284
	,BEGINING=285
	,END=286
	,BOTH=287
	,VARS=288
	,VARD=289
	,STRING=290
	,NUMERIC=291
	,VAR_N=292
	,VAR_NT=293
	,VAR_NS=294
	,VAR_NO=295
	,VAR_NTO=296
	,VAR_NSO=297
	,TAG=298
	,STAG=299
	,FIX=300
	,I1=301
	,I2=302
	,STR=303
	,VAL=304
	,LEN=305
	,STO=306
	,MEM=307
	,EXC=308
	,CLR=309
	,LOWER=310
	,UPPER=311
	,FROM=312
	,TO=313
	,BETWEEN=314
	,_DELETE=315
	,REPLACE=316
	,REPLACEOCC=317
	,BFIRST=318
	,EFIRST=319
	,BLAST=320
	,ELAST=321
	,REDO=322
	,SORT=323
	,NEXT=324
	,LAST=325
	,TABLE=326
	,ORDINAL=327
	,YEAR=328
	,MONTH=329
	,DAY=330
	,HOUR=331
	,MINUTE=332
	,SECOND=333
	,NEXTBAL=334
	,PREVIOUSBAL=335
	,REGFIND=336
	,REGMATCH=337
	,REGREPLACE=338

 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_MarcParser_PARSE (YY_MarcParser_PARSE_PARAM);
 virtual void YY_MarcParser_ERROR(char *msg) YY_MarcParser_ERROR_BODY;
#ifdef YY_MarcParser_PURE
#ifdef YY_MarcParser_LSP_NEEDED
 virtual int  YY_MarcParser_LEX (YY_MarcParser_STYPE *YY_MarcParser_LVAL,YY_MarcParser_LTYPE *YY_MarcParser_LLOC) YY_MarcParser_LEX_BODY;
#else
 virtual int  YY_MarcParser_LEX (YY_MarcParser_STYPE *YY_MarcParser_LVAL) YY_MarcParser_LEX_BODY;
#endif
#else
 virtual int YY_MarcParser_LEX() YY_MarcParser_LEX_BODY;
 YY_MarcParser_STYPE YY_MarcParser_LVAL;
#ifdef YY_MarcParser_LSP_NEEDED
 YY_MarcParser_LTYPE YY_MarcParser_LLOC;
#endif
 int   YY_MarcParser_NERRS;
 int    YY_MarcParser_CHAR;
#endif
#if YY_MarcParser_DEBUG != 0
 int YY_MarcParser_DEBUG_FLAG;   /*  nonzero means print parse trace     */
#endif
public:
 YY_MarcParser_CLASS(YY_MarcParser_CONSTRUCTOR_PARAM);
public:
 YY_MarcParser_MEMBERS 
};
/* other declare folow */
#if YY_MarcParser_USE_CONST_TOKEN != 0
const int YY_MarcParser_CLASS::SEP=258;
const int YY_MarcParser_CLASS::FIN=259;
const int YY_MarcParser_CLASS::WNUMBER=260;
const int YY_MarcParser_CLASS::WSTRING=261;
const int YY_MarcParser_CLASS::PLUS=262;
const int YY_MarcParser_CLASS::MOINS=263;
const int YY_MarcParser_CLASS::MULTIPLIE=264;
const int YY_MarcParser_CLASS::DIVISE=265;
const int YY_MarcParser_CLASS::EQ=266;
const int YY_MarcParser_CLASS::NE=267;
const int YY_MarcParser_CLASS::_IN=268;
const int YY_MarcParser_CLASS::GT=269;
const int YY_MarcParser_CLASS::LT=270;
const int YY_MarcParser_CLASS::GE=271;
const int YY_MarcParser_CLASS::LE=272;
const int YY_MarcParser_CLASS::EXISTS=273;
const int YY_MarcParser_CLASS::PRECEDES=274;
const int YY_MarcParser_CLASS::FOLLOWS=275;
const int YY_MarcParser_CLASS::IF=276;
const int YY_MarcParser_CLASS::THEN=277;
const int YY_MarcParser_CLASS::ELSE=278;
const int YY_MarcParser_CLASS::AND=279;
const int YY_MarcParser_CLASS::OR=280;
const int YY_MarcParser_CLASS::NOT=281;
const int YY_MarcParser_CLASS::BY=282;
const int YY_MarcParser_CLASS::_STRICT=283;
const int YY_MarcParser_CLASS::AT=284;
const int YY_MarcParser_CLASS::BEGINING=285;
const int YY_MarcParser_CLASS::END=286;
const int YY_MarcParser_CLASS::BOTH=287;
const int YY_MarcParser_CLASS::VARS=288;
const int YY_MarcParser_CLASS::VARD=289;
const int YY_MarcParser_CLASS::STRING=290;
const int YY_MarcParser_CLASS::NUMERIC=291;
const int YY_MarcParser_CLASS::VAR_N=292;
const int YY_MarcParser_CLASS::VAR_NT=293;
const int YY_MarcParser_CLASS::VAR_NS=294;
const int YY_MarcParser_CLASS::VAR_NO=295;
const int YY_MarcParser_CLASS::VAR_NTO=296;
const int YY_MarcParser_CLASS::VAR_NSO=297;
const int YY_MarcParser_CLASS::TAG=298;
const int YY_MarcParser_CLASS::STAG=299;
const int YY_MarcParser_CLASS::FIX=300;
const int YY_MarcParser_CLASS::I1=301;
const int YY_MarcParser_CLASS::I2=302;
const int YY_MarcParser_CLASS::STR=303;
const int YY_MarcParser_CLASS::VAL=304;
const int YY_MarcParser_CLASS::LEN=305;
const int YY_MarcParser_CLASS::STO=306;
const int YY_MarcParser_CLASS::MEM=307;
const int YY_MarcParser_CLASS::EXC=308;
const int YY_MarcParser_CLASS::CLR=309;
const int YY_MarcParser_CLASS::LOWER=310;
const int YY_MarcParser_CLASS::UPPER=311;
const int YY_MarcParser_CLASS::FROM=312;
const int YY_MarcParser_CLASS::TO=313;
const int YY_MarcParser_CLASS::BETWEEN=314;
const int YY_MarcParser_CLASS::_DELETE=315;
const int YY_MarcParser_CLASS::REPLACE=316;
const int YY_MarcParser_CLASS::REPLACEOCC=317;
const int YY_MarcParser_CLASS::BFIRST=318;
const int YY_MarcParser_CLASS::EFIRST=319;
const int YY_MarcParser_CLASS::BLAST=320;
const int YY_MarcParser_CLASS::ELAST=321;
const int YY_MarcParser_CLASS::REDO=322;
const int YY_MarcParser_CLASS::SORT=323;
const int YY_MarcParser_CLASS::NEXT=324;
const int YY_MarcParser_CLASS::LAST=325;
const int YY_MarcParser_CLASS::TABLE=326;
const int YY_MarcParser_CLASS::ORDINAL=327;
const int YY_MarcParser_CLASS::YEAR=328;
const int YY_MarcParser_CLASS::MONTH=329;
const int YY_MarcParser_CLASS::DAY=330;
const int YY_MarcParser_CLASS::HOUR=331;
const int YY_MarcParser_CLASS::MINUTE=332;
const int YY_MarcParser_CLASS::SECOND=333;
const int YY_MarcParser_CLASS::NEXTBAL=334;
const int YY_MarcParser_CLASS::PREVIOUSBAL=335;
const int YY_MarcParser_CLASS::REGFIND=336;
const int YY_MarcParser_CLASS::REGMATCH=337;
const int YY_MarcParser_CLASS::REGREPLACE=338;

 /* const YY_MarcParser_CLASS::token */
#endif
/*apres const  */
YY_MarcParser_CLASS::YY_MarcParser_CLASS(YY_MarcParser_CONSTRUCTOR_PARAM) YY_MarcParser_CONSTRUCTOR_INIT
{
#if YY_MarcParser_DEBUG != 0
YY_MarcParser_DEBUG_FLAG=0;
#endif
YY_MarcParser_CONSTRUCTOR_CODE;
};
#endif


#define	YYFINAL		312
#define	YYFLAG		32768
#define	YYNTBASE	87

#define YYTRANSLATE(x) ((unsigned)(x) <= 338 ? yytranslate[x] : 98)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    84,
    85,     2,     2,    86,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83
};

#if YY_MarcParser_DEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     7,    10,    14,    18,    20,    22,    24,
    26,    28,    31,    34,    38,    40,    42,    45,    47,    52,
    54,    59,    61,    63,    68,    75,    80,    87,    91,    95,
    99,   102,   106,   110,   114,   118,   122,   126,   130,   135,
   139,   143,   147,   149,   151,   153,   155,   157,   159,   161,
   163,   168,   170,   172,   174,   176,   178,   180,   182,   184,
   186,   188,   190,   194,   198,   202,   206,   211,   216,   221,
   226,   231,   236,   241,   246,   251,   256,   263,   268,   275,
   282,   291,   296,   304,   312,   320,   327,   337,   347,   357,
   364,   374,   384,   394,   403,   415,   427,   439,   448,   459,
   464,   469,   474,   479,   484,   489,   494,   499,   504,   509,
   514,   519,   524,   531,   538,   547,   552,   559,   566,   575,
   580,   585,   590,   597,   602,   609
};

static const short yyrhs[] = {    88,
     0,    89,     0,     7,    89,     0,    90,     4,     0,    90,
     3,     4,     0,    90,     3,    90,     0,    91,     0,    95,
     0,    97,     0,    67,     0,    93,     0,    93,    94,     0,
    93,    45,     0,    93,    94,    45,     0,    94,     0,    45,
     0,    94,    45,     0,    43,     0,    43,    84,    97,    85,
     0,    44,     0,    44,    84,    97,    85,     0,    46,     0,
    47,     0,    21,    96,    22,    97,     0,    21,    96,    22,
    97,    23,    97,     0,    21,    96,    22,    95,     0,    21,
    96,    22,    97,    23,    95,     0,    84,    96,    85,     0,
    96,    24,    96,     0,    96,    25,    96,     0,    26,    96,
     0,    97,    11,    97,     0,    97,    12,    97,     0,    97,
    13,    97,     0,    97,    14,    97,     0,    97,    15,    97,
     0,    97,    16,    97,     0,    97,    17,    97,     0,    18,
    84,    92,    85,     0,    92,    19,    92,     0,    92,    20,
    92,     0,    84,    97,    85,     0,    35,     0,    36,     0,
    73,     0,    74,     0,    75,     0,    76,     0,    77,     0,
    78,     0,    72,    84,    97,    85,     0,    79,     0,    80,
     0,    37,     0,    38,     0,    39,     0,    40,     0,    41,
     0,    42,     0,    33,     0,    34,     0,    92,     0,    97,
     7,    97,     0,    97,     8,    97,     0,    97,     9,    97,
     0,    97,    10,    97,     0,    48,    84,    97,    85,     0,
    49,    84,    97,    85,     0,    50,    84,    97,    85,     0,
    56,    84,    97,    85,     0,    55,    84,    97,    85,     0,
    51,    84,    97,    85,     0,    52,    84,    97,    85,     0,
    53,    84,    97,    85,     0,    54,    84,    97,    85,     0,
    57,    84,    97,    85,     0,    57,    84,    97,    86,    28,
    85,     0,    58,    84,    97,    85,     0,    58,    84,    97,
    86,    28,    85,     0,    59,    84,    97,    86,    97,    85,
     0,    59,    84,    97,    86,    97,    86,    28,    85,     0,
    60,    84,    97,    85,     0,    60,    84,    97,    86,    29,
    30,    85,     0,    60,    84,    97,    86,    29,    31,    85,
     0,    60,    84,    97,    86,    29,    32,    85,     0,    60,
    84,    97,    86,    28,    85,     0,    60,    84,    97,    86,
    29,    30,    86,    28,    85,     0,    60,    84,    97,    86,
    29,    31,    86,    28,    85,     0,    60,    84,    97,    86,
    29,    32,    86,    28,    85,     0,    61,    84,    97,    27,
    97,    85,     0,    61,    84,    97,    27,    97,    86,    29,
    30,    85,     0,    61,    84,    97,    27,    97,    86,    29,
    31,    85,     0,    61,    84,    97,    27,    97,    86,    29,
    32,    85,     0,    61,    84,    97,    27,    97,    86,    28,
    85,     0,    61,    84,    97,    27,    97,    86,    29,    30,
    86,    28,    85,     0,    61,    84,    97,    27,    97,    86,
    29,    31,    86,    28,    85,     0,    61,    84,    97,    27,
    97,    86,    29,    32,    86,    28,    85,     0,    62,    84,
    97,    27,    97,    86,    97,    85,     0,    62,    84,    97,
    27,    97,    86,    97,    86,    28,    85,     0,    63,    84,
    97,    85,     0,    63,    84,     5,    85,     0,    63,    84,
     6,    85,     0,    64,    84,    97,    85,     0,    64,    84,
     5,    85,     0,    64,    84,     6,    85,     0,    65,    84,
    97,    85,     0,    65,    84,     5,    85,     0,    65,    84,
     6,    85,     0,    66,    84,    97,    85,     0,    66,    84,
     5,    85,     0,    66,    84,     6,    85,     0,    69,    84,
    92,    85,     0,    69,    84,    92,    86,    92,    85,     0,
    69,    84,    92,    86,    28,    85,     0,    69,    84,    92,
    86,    92,    86,    28,    85,     0,    70,    84,    92,    85,
     0,    70,    84,    92,    86,    92,    85,     0,    70,    84,
    92,    86,    28,    85,     0,    70,    84,    92,    86,    92,
    86,    28,    85,     0,    68,    84,    35,    85,     0,    71,
    84,    35,    85,     0,    81,    84,    97,    85,     0,    81,
    84,    97,    86,    97,    85,     0,    82,    84,    97,    85,
     0,    83,    84,    97,    86,    97,    85,     0,    83,    84,
    97,    86,    97,    86,    97,    85,     0
};

#endif

#if YY_MarcParser_DEBUG != 0
static const short yyrline[] = { 0,
   129,   152,   158,   164,   166,   169,   171,   179,   181,   182,
   193,   200,   207,   214,   223,   228,   237,   246,   253,   263,
   270,   278,   283,   290,   302,   317,   328,   345,   347,   348,
   349,   350,   351,   352,   353,   354,   355,   356,   357,   361,
   366,   373,   375,   376,   377,   387,   397,   407,   417,   427,
   437,   450,   453,   456,   457,   458,   459,   460,   461,   462,
   463,   464,   472,   473,   474,   475,   476,   477,   478,   479,
   480,   481,   482,   483,   484,   485,   486,   488,   489,   491,
   493,   495,   496,   498,   500,   502,   503,   505,   507,   509,
   510,   512,   514,   516,   518,   520,   522,   524,   526,   528,
   529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
   539,   540,   541,   545,   549,   554,   558,   562,   566,   571,
   577,   578,   579,   580,   581,   583
};

static const char * const yytname[] = {   "$","error","$illegal.","SEP","FIN",
"WNUMBER","WSTRING","PLUS","MOINS","MULTIPLIE","DIVISE","EQ","NE","_IN","GT",
"LT","GE","LE","EXISTS","PRECEDES","FOLLOWS","IF","THEN","ELSE","AND","OR","NOT",
"BY","_STRICT","AT","BEGINING","END","BOTH","VARS","VARD","STRING","NUMERIC",
"VAR_N","VAR_NT","VAR_NS","VAR_NO","VAR_NTO","VAR_NSO","TAG","STAG","FIX","I1",
"I2","STR","VAL","LEN","STO","MEM","EXC","CLR","LOWER","UPPER","FROM","TO","BETWEEN",
"_DELETE","REPLACE","REPLACEOCC","BFIRST","EFIRST","BLAST","ELAST","REDO","SORT",
"NEXT","LAST","TABLE","ORDINAL","YEAR","MONTH","DAY","HOUR","MINUTE","SECOND",
"NEXTBAL","PREVIOUSBAL","REGFIND","REGMATCH","REGREPLACE","'('","')'","','",
"Program","Rules","Rule","SetOfInstr","Instruction","CD","TAGOCC","STAGOCC",
"Condition","Boolean","Translation",""
};
#endif

static const short yyr1[] = {     0,
    87,    88,    88,    89,    89,    90,    90,    91,    91,    91,
    92,    92,    92,    92,    92,    92,    92,    93,    93,    94,
    94,    94,    94,    95,    95,    95,    95,    96,    96,    96,
    96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
    96,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97
};

static const short yyr2[] = {     0,
     1,     1,     2,     2,     3,     3,     1,     1,     1,     1,
     1,     2,     2,     3,     1,     1,     2,     1,     4,     1,
     4,     1,     1,     4,     6,     4,     6,     3,     3,     3,
     2,     3,     3,     3,     3,     3,     3,     3,     4,     3,
     3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
     4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     3,     3,     3,     3,     4,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     6,     4,     6,     6,
     8,     4,     7,     7,     7,     6,     9,     9,     9,     6,
     9,     9,     9,     8,    11,    11,    11,     8,    10,     4,
     4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     6,     6,     8,     4,     6,     6,     8,     4,
     4,     4,     6,     4,     6,     8
};

static const short yydefact[] = {     0,
     0,     0,    60,    61,    43,    44,    54,    55,    56,    57,
    58,    59,    18,    20,    16,    22,    23,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    10,     0,     0,     0,
     0,     0,    45,    46,    47,    48,    49,    50,    52,    53,
     0,     0,     0,     0,     1,     2,     0,     7,    62,    11,
    15,     8,     9,     3,     0,     0,     0,    62,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     4,    13,    12,    17,     0,     0,     0,     0,     0,
    31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    42,     5,     6,    14,    63,    64,    65,    66,
     0,    28,    40,    41,    26,    24,    29,    30,    32,    33,
    34,    35,    36,    37,    38,    19,    21,    67,    68,    69,
    72,    73,    74,    75,    71,    70,    76,     0,    78,     0,
     0,    82,     0,     0,     0,   101,   102,   100,   104,   105,
   103,   107,   108,   106,   110,   111,   109,   120,   112,     0,
   116,     0,   121,    51,   122,     0,   124,     0,     0,    39,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    27,    25,    77,    79,    80,     0,
    86,     0,     0,     0,    90,     0,     0,   114,   113,     0,
   118,   117,     0,   123,   125,     0,     0,    83,     0,    84,
     0,    85,     0,     0,     0,     0,     0,     0,     0,    81,
     0,     0,     0,    94,     0,     0,     0,    98,     0,   115,
   119,   126,    87,    88,    89,    91,     0,    92,     0,    93,
     0,     0,     0,     0,     0,    99,    95,    96,    97,     0,
     0,     0
};

static const short yydefgoto[] = {   310,
    55,    56,    57,    58,    59,    60,    61,    62,    69,    63
};

static const short yypact[] = {   697,
   828,   764,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   -76,   -63,-32768,-32768,-32768,   -49,   -37,    44,
    61,    64,    67,   139,   141,   150,   177,   178,   179,   235,
   238,   239,   258,   259,   260,   295,-32768,   315,   316,   317,
   318,   321,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   322,   325,   326,   944,-32768,-32768,    89,-32768,-32768,   160,
    85,-32768,    35,-32768,   329,   764,   764,    83,   119,   651,
   944,   944,   944,   944,   944,   944,   944,   944,   944,   944,
   944,   944,   944,   944,   944,   944,   944,   312,   392,   472,
   552,   216,    -6,    -6,   222,   944,   944,   944,   944,   171,
   232,-32768,-32768,   275,-32768,   944,   944,   944,   944,    -6,
   109,   -22,   157,    -6,    -6,   892,   764,   764,   944,   944,
   944,   944,   944,   944,   944,   175,   236,   240,   319,   323,
   327,   331,   410,   414,   474,   478,     5,    15,    41,    23,
     9,    19,   330,   374,   482,   394,   395,   493,   408,   409,
   561,   411,   412,   565,   413,    91,   101,   419,   572,    46,
   630,   149,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   454,-32768,-32768,-32768,-32768,    -3,   109,   109,    35,    35,
    35,    35,    35,    35,    35,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   296,-32768,   375,
   944,-32768,   172,   944,   944,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   147,
-32768,   167,-32768,-32768,-32768,   944,-32768,   944,   828,-32768,
   892,   475,   476,    50,   477,    36,   103,   153,   479,   123,
   480,   132,   634,   130,-32768,    35,-32768,-32768,-32768,   379,
-32768,   134,   136,   142,-32768,   204,   944,-32768,-32768,   383,
-32768,-32768,   386,-32768,-32768,   944,   481,-32768,   548,-32768,
   549,-32768,   555,   499,   166,   145,   534,   560,   644,-32768,
   562,   563,   564,-32768,   152,   155,   173,-32768,   627,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   628,-32768,   641,-32768,
   642,   586,   587,   588,   589,-32768,-32768,-32768,-32768,   675,
   676,-32768
};

static const short yypgoto[] = {-32768,
-32768,   677,  -100,-32768,    32,-32768,   617,  -107,   -56,    -2
};


#define	YYLAST		1028


static const short yytable[] = {    70,
   165,   117,   118,   106,   107,   108,   109,    71,   175,   111,
   112,   106,   107,   108,   109,   106,   107,   108,   109,   231,
    72,   106,   107,   108,   109,   106,   107,   108,   109,   106,
   107,   108,   109,    68,    73,   204,    13,    14,    15,    16,
    17,   106,   107,   108,   109,   205,    74,   106,   107,   108,
   109,   100,   106,   107,   108,   109,   106,   107,   108,   109,
   177,   178,   172,    70,   113,   252,   253,   254,   126,   127,
   128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
   138,   139,   140,   141,   142,   145,   148,   151,   154,   197,
   198,   101,   102,   159,   160,   161,   162,    68,    68,   199,
   200,   114,   115,   167,   168,   169,   170,   202,   203,   106,
   107,   108,   109,   176,    70,    70,   179,   180,   181,   182,
   183,   184,   185,   245,   156,   157,   201,    75,   165,   105,
   225,   226,   117,   118,   249,   250,   106,   107,   108,   109,
   116,   171,   117,   118,    76,   173,   174,    77,    68,    68,
    78,   106,   107,   108,   109,   106,   107,   108,   109,   106,
   107,   108,   109,   106,   107,   108,   109,   119,   120,   121,
   122,   123,   124,   125,   239,   219,   220,   106,   107,   108,
   109,   106,   107,   108,   109,   221,   222,   255,   256,    13,
    14,    15,    16,    17,   241,   285,   286,   287,   234,   235,
   236,   237,   238,    14,   103,    16,    17,   259,   260,    13,
    14,    15,    16,    17,   265,   266,   262,   263,   268,   269,
   270,   271,    79,   243,    80,   244,   272,   273,   246,   288,
   289,   274,   275,    81,   228,   164,   296,   297,   257,   298,
   299,   163,   106,   107,   108,   109,   106,   107,   108,   109,
   155,   240,     2,   242,   276,   163,   158,   300,   301,   186,
    82,    83,    84,   279,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,   143,   144,    85,   166,
   187,    86,    87,   232,   188,   106,   107,   108,   109,   106,
   107,   108,   109,   106,   107,   108,   109,   106,   107,   108,
   109,    88,    89,    90,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    91,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,   146,   147,    92,    93,
    94,    95,   233,   189,    96,    97,   267,   190,    98,    99,
   277,   191,   110,   278,   206,   192,   106,   107,   108,   109,
   106,   107,   108,   109,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,   207,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,   149,   150,   209,   210,
   106,   107,   108,   109,   106,   107,   108,   109,   106,   107,
   108,   109,   212,   213,   193,   215,   216,   218,   194,   106,
   107,   108,   109,   223,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,   230,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,   152,   153,   195,   247,
   248,   251,   196,   258,   261,   280,   208,   106,   107,   108,
   109,   106,   107,   108,   109,   281,   282,   211,   106,   107,
   108,   109,   283,   284,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,   290,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,   106,   107,   108,   109,
   106,   107,   108,   109,   291,   214,   293,   294,   295,   217,
   106,   107,   108,   109,   302,   303,   224,   106,   107,   108,
   109,   119,   120,   121,   122,   123,   124,   125,   304,   305,
   306,   307,   308,   309,   311,   312,   104,    64,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     1,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   227,     0,     0,     2,   264,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   292,     3,
     4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    65,     0,     0,     0,     0,     0,     0,     0,    66,
     0,     0,     0,     0,     0,     0,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
     0,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    67,     2,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    54,     2,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,     0,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
     0,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54
};

static const short yycheck[] = {     2,
   101,    24,    25,     7,     8,     9,    10,    84,   116,    66,
    67,     7,     8,     9,    10,     7,     8,     9,    10,    23,
    84,     7,     8,     9,    10,     7,     8,     9,    10,     7,
     8,     9,    10,     2,    84,    27,    43,    44,    45,    46,
    47,     7,     8,     9,    10,    27,    84,     7,     8,     9,
    10,    54,     7,     8,     9,    10,     7,     8,     9,    10,
   117,   118,    85,    66,    67,    30,    31,    32,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    86,    87,    88,    89,    90,    91,    85,
    86,     3,     4,    96,    97,    98,    99,    66,    67,    85,
    86,    19,    20,   106,   107,   108,   109,    85,    86,     7,
     8,     9,    10,   116,   117,   118,   119,   120,   121,   122,
   123,   124,   125,   231,    93,    94,    86,    84,   229,    45,
    85,    86,    24,    25,    85,    86,     7,     8,     9,    10,
    22,   110,    24,    25,    84,   114,   115,    84,   117,   118,
    84,     7,     8,     9,    10,     7,     8,     9,    10,     7,
     8,     9,    10,     7,     8,     9,    10,    11,    12,    13,
    14,    15,    16,    17,    28,    85,    86,     7,     8,     9,
    10,     7,     8,     9,    10,    85,    86,    85,    86,    43,
    44,    45,    46,    47,    28,    30,    31,    32,   201,    28,
    29,   204,   205,    44,    45,    46,    47,    85,    86,    43,
    44,    45,    46,    47,    85,    86,    85,    86,    85,    86,
    85,    86,    84,   226,    84,   228,    85,    86,   231,    85,
    86,    28,    29,    84,    86,     4,    85,    86,    86,    85,
    86,    85,     7,     8,     9,    10,     7,     8,     9,    10,
    35,   220,    21,   222,   257,    85,    35,    85,    86,    85,
    84,    84,    84,   266,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,     5,     6,    84,    45,
    85,    84,    84,    28,    85,     7,     8,     9,    10,     7,
     8,     9,    10,     7,     8,     9,    10,     7,     8,     9,
    10,    84,    84,    84,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    84,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,     5,     6,    84,    84,
    84,    84,    28,    85,    84,    84,    28,    85,    84,    84,
    28,    85,    84,    28,    85,    85,     7,     8,     9,    10,
     7,     8,     9,    10,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    85,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,     5,     6,    85,    85,
     7,     8,     9,    10,     7,     8,     9,    10,     7,     8,
     9,    10,    85,    85,    85,    85,    85,    85,    85,     7,
     8,     9,    10,    85,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    85,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,     5,     6,    85,    85,
    85,    85,    85,    85,    85,    85,    85,     7,     8,     9,
    10,     7,     8,     9,    10,    28,    28,    85,     7,     8,
     9,    10,    28,    85,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    85,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,     7,     8,     9,    10,
     7,     8,     9,    10,    85,    85,    85,    85,    85,    85,
     7,     8,     9,    10,    28,    28,    85,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,    28,    28,
    85,    85,    85,    85,     0,     0,    60,     1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    85,    -1,    -1,    21,    85,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
    64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
    74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
    84,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    21,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    -1,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84
};
 /* fattrs + tables */

/* parser code folow  */


/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: dollar marks section change
   the next  is replaced by the list of actions, each action
   as one case of the switch.  */ 

#if YY_MarcParser_USE_GOTO != 0
/* 
 SUPRESSION OF GOTO : on some C++ compiler (sun c++)
  the goto is strictly forbidden if any constructor/destructor
  is used in the whole function (very stupid isn't it ?)
 so goto are to be replaced with a 'while/switch/case construct'
 here are the macro to keep some apparent compatibility
*/
#define YYGOTO(lb) {yy_gotostate=lb;continue;}
#define YYBEGINGOTO  enum yy_labels yy_gotostate=yygotostart; \
                     for(;;) switch(yy_gotostate) { case yygotostart: {
#define YYLABEL(lb) } case lb: {
#define YYENDGOTO } } 
#define YYBEGINDECLARELABEL enum yy_labels {yygotostart
#define YYDECLARELABEL(lb) ,lb
#define YYENDDECLARELABEL  };
#else
/* macro to keep goto */
#define YYGOTO(lb) goto lb
#define YYBEGINGOTO 
#define YYLABEL(lb) lb:
#define YYENDGOTO
#define YYBEGINDECLARELABEL 
#define YYDECLARELABEL(lb)
#define YYENDDECLARELABEL 
#endif
/* LABEL DECLARATION */
YYBEGINDECLARELABEL
  YYDECLARELABEL(yynewstate)
  YYDECLARELABEL(yybackup)
/* YYDECLARELABEL(yyresume) */
  YYDECLARELABEL(yydefault)
  YYDECLARELABEL(yyreduce)
  YYDECLARELABEL(yyerrlab)   /* here on detecting error */
  YYDECLARELABEL(yyerrlab1)   /* here on error raised explicitly by an action */
  YYDECLARELABEL(yyerrdefault)  /* current state does not do anything special for the error token. */
  YYDECLARELABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */
  YYDECLARELABEL(yyerrhandle)  
YYENDDECLARELABEL
/* ALLOCA SIMULATION */
/* __HAVE_NO_ALLOCA */
#ifdef __HAVE_NO_ALLOCA
int __alloca_free_ptr(char *ptr,char *ref)
{if(ptr!=ref) free(ptr);
 return 0;}

#define __ALLOCA_alloca(size) malloc(size)
#define __ALLOCA_free(ptr,ref) __alloca_free_ptr((char *)ptr,(char *)ref)

#ifdef YY_MarcParser_LSP_NEEDED
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		    __ALLOCA_free(yyls,yylsa)+\
		   (num))
#else
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		   (num))
#endif
#else
#define __ALLOCA_return(num) return(num)
#define __ALLOCA_alloca(size) alloca(size)
#define __ALLOCA_free(ptr,ref) 
#endif

/* ENDALLOCA SIMULATION */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (YY_MarcParser_CHAR = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        __ALLOCA_return(0)
#define YYABORT         __ALLOCA_return(1)
#define YYERROR         YYGOTO(yyerrlab1)
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          YYGOTO(yyerrlab)
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (YY_MarcParser_CHAR == YYEMPTY && yylen == 1)                               \
    { YY_MarcParser_CHAR = (token), YY_MarcParser_LVAL = (value);                 \
      yychar1 = YYTRANSLATE (YY_MarcParser_CHAR);                                \
      YYPOPSTACK;                                               \
      YYGOTO(yybackup);                                            \
    }                                                           \
  else                                                          \
    { YY_MarcParser_ERROR ("syntax error: cannot back up"); YYERROR; }   \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YY_MarcParser_PURE
/* UNPURE */
#define YYLEX           YY_MarcParser_LEX()
#ifndef YY_USE_CLASS
/* If nonreentrant, and not class , generate the variables here */
int     YY_MarcParser_CHAR;                      /*  the lookahead symbol        */
YY_MarcParser_STYPE      YY_MarcParser_LVAL;              /*  the semantic value of the */
				/*  lookahead symbol    */
int YY_MarcParser_NERRS;                 /*  number of parse errors so far */
#ifdef YY_MarcParser_LSP_NEEDED
YY_MarcParser_LTYPE YY_MarcParser_LLOC;   /*  location data for the lookahead     */
			/*  symbol                              */
#endif
#endif


#else
/* PURE */
#ifdef YY_MarcParser_LSP_NEEDED
#define YYLEX           YY_MarcParser_LEX(&YY_MarcParser_LVAL, &YY_MarcParser_LLOC)
#else
#define YYLEX           YY_MarcParser_LEX(&YY_MarcParser_LVAL)
#endif
#endif
#ifndef YY_USE_CLASS
#if YY_MarcParser_DEBUG != 0
int YY_MarcParser_DEBUG_FLAG;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif
#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif


#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)       __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */

#ifdef __cplusplus
static void __yy_bcopy (char *from, char *to, int count)
#else
#ifdef __STDC__
static void __yy_bcopy (char *from, char *to, int count)
#else
static void __yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
#endif
#endif
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}
#endif

int
#ifdef YY_USE_CLASS
 YY_MarcParser_CLASS::
#endif
     YY_MarcParser_PARSE(YY_MarcParser_PARSE_PARAM)
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
/* parameter definition without protypes */
YY_MarcParser_PARSE_PARAM_DEF
#endif
#endif
#endif
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YY_MarcParser_STYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1=0;          /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YY_MarcParser_STYPE yyvsa[YYINITDEPTH];        /*  the semantic value stack            */

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YY_MarcParser_STYPE *yyvs = yyvsa;     /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YY_MarcParser_LSP_NEEDED
  YY_MarcParser_LTYPE yylsa[YYINITDEPTH];        /*  the location stack                  */
  YY_MarcParser_LTYPE *yyls = yylsa;
  YY_MarcParser_LTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YY_MarcParser_PURE
  int YY_MarcParser_CHAR;
  YY_MarcParser_STYPE YY_MarcParser_LVAL;
  int YY_MarcParser_NERRS;
#ifdef YY_MarcParser_LSP_NEEDED
  YY_MarcParser_LTYPE YY_MarcParser_LLOC;
#endif
#endif

  YY_MarcParser_STYPE yyval;             /*  the variable used to return         */
				/*  semantic values from the action     */
				/*  routines                            */

  int yylen;
/* start loop, in which YYGOTO may be used. */
YYBEGINGOTO

#if YY_MarcParser_DEBUG != 0
  if (YY_MarcParser_DEBUG_FLAG)
    fprintf(stderr, "Starting parse\n");
#endif
  yystate = 0;
  yyerrstatus = 0;
  YY_MarcParser_NERRS = 0;
  YY_MarcParser_CHAR = YYEMPTY;          /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YY_MarcParser_LSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
YYLABEL(yynewstate)

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YY_MarcParser_STYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YY_MarcParser_LSP_NEEDED
      YY_MarcParser_LTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YY_MarcParser_LSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YY_MarcParser_LSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  YY_MarcParser_ERROR("parser stack overflow");
	  __ALLOCA_return(2);
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) __ALLOCA_alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      __ALLOCA_free(yyss1,yyssa);
      yyvs = (YY_MarcParser_STYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
      __ALLOCA_free(yyvs1,yyvsa);
#ifdef YY_MarcParser_LSP_NEEDED
      yyls = (YY_MarcParser_LTYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      __ALLOCA_free(yyls1,yylsa);
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YY_MarcParser_LSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YY_MarcParser_DEBUG != 0
      if (YY_MarcParser_DEBUG_FLAG)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YY_MarcParser_DEBUG != 0
  if (YY_MarcParser_DEBUG_FLAG)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  YYGOTO(yybackup);
YYLABEL(yybackup)

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* YYLABEL(yyresume) */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yydefault);

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (YY_MarcParser_CHAR == YYEMPTY)
    {
#if YY_MarcParser_DEBUG != 0
      if (YY_MarcParser_DEBUG_FLAG)
	fprintf(stderr, "Reading a token: ");
#endif
      YY_MarcParser_CHAR = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (YY_MarcParser_CHAR <= 0)           /* This means end of input. */
    {
      yychar1 = 0;
      YY_MarcParser_CHAR = YYEOF;                /* Don't call YYLEX any more */

#if YY_MarcParser_DEBUG != 0
      if (YY_MarcParser_DEBUG_FLAG)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(YY_MarcParser_CHAR);

#if YY_MarcParser_DEBUG != 0
      if (YY_MarcParser_DEBUG_FLAG)
	{
	  fprintf (stderr, "Next token is %d (%s", YY_MarcParser_CHAR, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, YY_MarcParser_CHAR, YY_MarcParser_LVAL);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    YYGOTO(yydefault);

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrlab);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrlab);

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YY_MarcParser_DEBUG != 0
  if (YY_MarcParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting token %d (%s), ", YY_MarcParser_CHAR, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (YY_MarcParser_CHAR != YYEOF)
    YY_MarcParser_CHAR = YYEMPTY;

  *++yyvsp = YY_MarcParser_LVAL;
#ifdef YY_MarcParser_LSP_NEEDED
  *++yylsp = YY_MarcParser_LLOC;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  YYGOTO(yynewstate);

/* Do the default action for the current state.  */
YYLABEL(yydefault)

  yyn = yydefact[yystate];
  if (yyn == 0)
    YYGOTO(yyerrlab);

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
YYLABEL(yyreduce)
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YY_MarcParser_DEBUG != 0
  if (YY_MarcParser_DEBUG_FLAG)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
{
                                  if (!RedoFlag)
                                  {
                                        FreeTypeInst(S);
                                        Copie(&S,yyval.inst);
                                        FreeTypeInst(yyval.inst);
                                        yyval.inst=NULL;
                                        return 1;
                                  }
                                  else
                                  {
                                        FreeTypeInst(S);
                                        S=T;
                                        Copie(&T,yyval.inst);
                                        FreeTypeInst(yyval.inst);
                                        yyval.inst=NULL;
                                        if (T->str.str()[0]==0) return 1;
                                        else              return 3;
                                  }
                                ;
    break;}
case 2:
{
                                  Copie(&yyval.inst,S);
                                  FreeTypeInst(S);
                                  S=NULL;
                                ;
    break;}
case 3:
{
                                  yyval.inst=Ajout(D,S);
                                  D=S=NULL;
                                ;
    break;}
case 7:
{
                                  FreeTypeInst(S);
                                  Copie(&S,yyvsp[0].inst);
                                  FreeTypeInst(yyvsp[0].inst);
                                  yyvsp[0].inst=NULL;
                                ;
    break;}
case 10:
{ PrintDebug("Redo");
                                  Copie(&yyval.inst,S);
                                  RedoFlag=1;
                                  Copie(&T,S);
/*                                ++NO->val;
                                  if (NTO->val>=0) ++NTO->val;
                                  else
                                  if (NSO->val>=0) ++NSO->val;*/
                                ;
    break;}
case 11:
{ PrintDebug("Tagocc");
                                  yyval.tcd=yyvsp[0].tcd;
                                  yyval.tcd->Fixed.freestr();
                                  yyval.tcd->SubField[0]=0;
                                  yyval.tcd->ns=0;
                                ;
    break;}
case 12:
{ PrintDebug("Tagocc Stagocc");
                                  yyval.tcd=yyvsp[-1].tcd;
                                  yyval.tcd->Fixed.freestr();
                                  strcpy(yyval.tcd->SubField,yyvsp[0].tcd->SubField);
                                  yyval.tcd->ns=yyvsp[0].tcd->ns;
                                  FreeCD(yyvsp[0].tcd);
                                  yyvsp[0].tcd=NULL;;
    break;}
case 13:
{ PrintDebug("Tagocc Fix");
                                  yyval.tcd=yyvsp[-1].tcd;
                                  yyval.tcd->SubField[0]=0;
                                  yyval.tcd->ns=0;
                                  yyval.tcd->Fixed.str(yyvsp[0].inst->str.str());
                                  FreeTypeInst(yyvsp[0].inst);
                                  yyvsp[0].inst=NULL;;
    break;}
case 14:
{ PrintDebug("Tagocc Stagocc Fix");
                                  yyval.tcd=yyvsp[-2].tcd;
                                  strcpy(yyval.tcd->SubField,yyvsp[-1].tcd->SubField);
                                  yyval.tcd->ns=yyvsp[-1].tcd->ns;
                                  yyval.tcd->Fixed.str(yyvsp[0].inst->str.str());
                                  FreeCD(yyvsp[-1].tcd);
                                  yyvsp[-1].tcd=NULL;
                                  FreeTypeInst(yyvsp[0].inst);
                                  yyvsp[0].inst=NULL;;
    break;}
case 15:
{ PrintDebug("Stagocc");
                                  yyval.tcd=yyvsp[0].tcd;
                                  yyval.tcd->Fixed.freestr();
                                  strcpy(yyval.tcd->Field,CDIn->Field);
                                  yyval.tcd->nt=0; ;
    break;}
case 16:
{ PrintDebug("Fix");
                                  yyval.tcd=AllocCD();
                                  strcpy(yyval.tcd->Field,CDIn->Field);
                                  yyval.tcd->nt=0;
                                  strcpy(yyval.tcd->SubField,CDIn->SubField);
                                  yyval.tcd->ns=0;
                                  yyval.tcd->Fixed.str(yyvsp[0].inst->str.str());
                                  FreeTypeInst(yyvsp[0].inst);
                                  yyvsp[0].inst=NULL;;
    break;}
case 17:
{ PrintDebug("Stagocc Fix");
                                  yyval.tcd=yyvsp[-1].tcd;
                                  yyval.tcd->Fixed.str(yyvsp[0].inst->str.str());
                                  strcpy(yyval.tcd->Field,CDIn->Field);
                                  yyval.tcd->nt=0;
                                  FreeTypeInst(yyvsp[0].inst);
                                  yyvsp[0].inst=NULL;;
    break;}
case 18:
{ PrintDebug("Tag");
                                          yyval.tcd=AllocCD();
                                          strcpy(yyval.tcd->Field,yyvsp[0].inst->str.str());
                                          yyval.tcd->nt=0;
                                          FreeTypeInst(yyvsp[0].inst);
                                          yyvsp[0].inst=NULL; ;
    break;}
case 19:
{ PrintDebug("Tag(...)");
                                          yyval.tcd=AllocCD();
                                          strcpy(yyval.tcd->Field,yyvsp[-3].inst->str.str());
                                          FreeTypeInst(yyvsp[-3].inst);
                                          yyvsp[-3].inst=NULL;
                                          yyval.tcd->nt=yyvsp[-1].inst->val;
                                          FreeTypeInst(yyvsp[-1].inst);
                                          yyvsp[-1].inst=NULL; ;
    break;}
case 20:
{ PrintDebug("Stag");
                                          yyval.tcd=AllocCD();
                                          strcpy(yyval.tcd->SubField,yyvsp[0].inst->str.str());
                                          yyval.tcd->ns=0;
                                          FreeTypeInst(yyvsp[0].inst);
                                          yyvsp[0].inst=NULL; ;
    break;}
case 21:
{ PrintDebug("Stag(...)");
                                          yyval.tcd=AllocCD();
                                          strcpy(yyval.tcd->SubField,yyvsp[-3].inst->str.str());
                                          FreeTypeInst(yyvsp[-3].inst);
                                          yyvsp[-3].inst=NULL;
                                          yyval.tcd->ns=yyvsp[-1].inst->val;
                                          FreeTypeInst(yyvsp[-1].inst);
                                          yyvsp[-1].inst=NULL; ;
    break;}
case 22:
{ PrintDebug("I1");
                                          yyval.tcd=AllocCD();
                                          strcpy(yyval.tcd->SubField,"I1");
                                          yyval.tcd->ns=0;
                                        ;
    break;}
case 23:
{ PrintDebug("I2");
                                          yyval.tcd=AllocCD();
                                          strcpy(yyval.tcd->SubField,"I2");
                                          yyval.tcd->ns=0;
                                        ;
    break;}
case 24:
{ PrintDebug("If...Then...");
                                          if (yyvsp[-2].code) yyval.inst=yyvsp[0].inst;
                                          else
                                           {
                                            FreeTypeInst(yyvsp[0].inst);
                                            yyvsp[0].inst=NULL;
                                            FreeTypeInst(S);
                                            S=NULL;
                                            return 2;
                                           }
                                        ;
    break;}
case 25:
{ PrintDebug("If...Then...Else...");
                                          if (yyvsp[-4].code)
                                          {
                                                yyval.inst=yyvsp[-2].inst;
                                                FreeTypeInst(yyvsp[0].inst);
                                                yyvsp[0].inst=NULL;
                                          }
                                          else
                                          {
                                                yyval.inst=yyvsp[0].inst;
                                                FreeTypeInst(yyvsp[-2].inst);
                                                yyvsp[-2].inst=NULL;
                                          }
                                        ;
    break;}
case 26:
{ PrintDebug("If...Then... (condition)");
                                          if (yyvsp[-2].code) yyval.inst=yyvsp[0].inst;
                                          else
                                           {
                                            FreeTypeInst(yyvsp[0].inst);
                                            yyvsp[0].inst=NULL;
                                            FreeTypeInst(S);
                                            S=NULL;
                                            return 2;
                                           }
                                        ;
    break;}
case 27:
{ PrintDebug("If...Then...Else... (condition)");
                                          if (yyvsp[-4].code)
                                          {
                                                yyval.inst=yyvsp[-2].inst;
                                                FreeTypeInst(yyvsp[0].inst);
                                                yyvsp[0].inst=NULL;
                                          }
                                          else
                                          {
                                                yyval.inst=yyvsp[0].inst;
                                                FreeTypeInst(yyvsp[-2].inst);
                                                yyvsp[-2].inst=NULL;
                                          }
                                        ;
    break;}
case 28:
{ PrintDebug("(B...)"); yyval.code=yyvsp[-1].code; ;
    break;}
case 29:
{ PrintDebug("B...and B..."); yyval.code=yyvsp[-2].code && yyvsp[0].code; ;
    break;}
case 30:
{ PrintDebug("B...or B...");yyval.code=yyvsp[-2].code || yyvsp[0].code; ;
    break;}
case 31:
{ PrintDebug("!B...");yyval.code=!yyvsp[0].code; ;
    break;}
case 32:
{ PrintDebug("...=...");yyval.code=BoolEQ(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 33:
{ PrintDebug("...!=...");yyval.code=!BoolEQ(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 34:
{ PrintDebug("..._In...");yyval.code=BoolIn(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 35:
{ PrintDebug("...>...");yyval.code=BoolGT(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 36:
{ PrintDebug("...<...");yyval.code=BoolGT(yyvsp[0].inst,yyvsp[-2].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 37:
{ PrintDebug("...>=...");yyval.code=BoolGE(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 38:
{ PrintDebug("...<=...");yyval.code=BoolGE(yyvsp[0].inst,yyvsp[-2].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 39:
{ PrintDebug("Exists(...)");
                                          yyval.code=Exists(yyvsp[-1].tcd);
                                          if (yyval.code==2) return 2;
                                          FreeCD(yyvsp[-1].tcd); yyvsp[-1].tcd=NULL; ;
    break;}
case 40:
{ PrintDebug("...Precedes...");
                                          yyval.code=Precedes(yyvsp[-2].tcd,yyvsp[0].tcd);
                                          if (yyval.code==2) return 2;
                                          FreeCD(yyvsp[-2].tcd); yyvsp[-2].tcd=NULL;
                                          FreeCD(yyvsp[0].tcd); yyvsp[0].tcd=NULL; ;
    break;}
case 41:
{ PrintDebug("...Follows...");
                                          yyval.code=Precedes(yyvsp[0].tcd,yyvsp[-2].tcd);
                                          if (yyval.code==2) return 2;
                                          FreeCD(yyvsp[0].tcd); yyvsp[0].tcd=NULL;
                                          FreeCD(yyvsp[-2].tcd); yyvsp[-2].tcd=NULL; ;
    break;}
case 42:
{ PrintDebug("(...)");yyval.inst=yyvsp[-1].inst; ;
    break;}
case 45:
{
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          yyval.inst=AllocTypeInst();
                                          sprintf(tempo,"%04d",1900+lt->tm_year);
                                          yyval.inst->str.str(tempo);
                                          yyval.inst->val=0;
                                        ;
    break;}
case 46:
{
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          yyval.inst=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_mon+1);
                                          yyval.inst->str.str(tempo);
                                          yyval.inst->val=0;
                                        ;
    break;}
case 47:
{
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          yyval.inst=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_mday);
                                          yyval.inst->str.str(tempo);
                                          yyval.inst->val=0;
                                        ;
    break;}
case 48:
{
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          yyval.inst=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_hour);
                                          yyval.inst->str.str(tempo);
                                          yyval.inst->val=0;
                                        ;
    break;}
case 49:
{
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          yyval.inst=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_min);
                                          yyval.inst->str.str(tempo);
                                          yyval.inst->val=0;
                                        ;
    break;}
case 50:
{
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          yyval.inst=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_sec);
                                          yyval.inst->str.str(tempo);
                                          yyval.inst->val=0;
                                        ;
    break;}
case 51:
{
                                          int i,j;
                                          char tmp1[20];
                                          yyval.inst=AllocTypeInst();
                                          sprintf(tmp1,"%d",ordinal);
                                          for (j=0,i=strlen(tmp1);i<yyvsp[-1].inst->val;++i,++j)
                                           tempo[j]='0';
                                          strcpy(&tempo[j],tmp1);
                                          yyval.inst->str.str(tempo);
                                          yyval.inst->val=0;
                                          FreeTypeInst(yyvsp[-1].inst);
                                          yyvsp[-1].inst=NULL;
                                        ;
    break;}
case 52:
{
                                                PrintDebug("NextSub");yyval.inst=NextBal();
                                        ;
    break;}
case 53:
{
                                                PrintDebug("PreviousSub");yyval.inst=PreviousBal();
                                        ;
    break;}
case 54:
{ PrintDebug("N");Copie(&yyval.inst,N); ;
    break;}
case 55:
{ PrintDebug("NT");Copie(&yyval.inst,NT); ;
    break;}
case 56:
{ PrintDebug("NS");Copie(&yyval.inst,NS); ;
    break;}
case 57:
{ PrintDebug("NO");Copie(&yyval.inst,NO); ;
    break;}
case 58:
{ PrintDebug("NTO"); Copie(&yyval.inst,NTO); ;
    break;}
case 59:
{ PrintDebug("NSO");Copie(&yyval.inst,NSO); ;
    break;}
case 60:
{ PrintDebug("S");Copie(&yyval.inst,S); ;
    break;}
case 61:
{ PrintDebug("S");Copie(&yyval.inst,D); ;
    break;}
case 62:
{ const char *ptr;
                                  PrintDebug("CD");
                                  yyval.inst=AllocTypeInst();
                                  ptr=LireCD(yyvsp[0].tcd);
                                  if (!ptr) return 2;
                                  yyval.inst->str.str(ptr);
                                  yyval.inst->val=0;
                                  FreeCD(yyvsp[0].tcd);;
    break;}
case 63:
{ PrintDebug("...+...");yyval.inst=Ajout(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 64:
{ PrintDebug("...-...");yyval.inst=Soust(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 65:
{ PrintDebug("...*...");yyval.inst=Multi(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 66:
{ PrintDebug("...:...");yyval.inst=Divis(yyvsp[-2].inst,yyvsp[0].inst); yyvsp[-2].inst=yyvsp[0].inst=NULL; ;
    break;}
case 67:
{ PrintDebug("Str(...)");yyval.inst=String(yyvsp[-1].inst); ;
    break;}
case 68:
{ PrintDebug("Val(...)");yyval.inst=Value(yyvsp[-1].inst); ;
    break;}
case 69:
{ PrintDebug("Len(...)");yyval.inst=Len(yyvsp[-1].inst); ;
    break;}
case 70:
{ PrintDebug("Upper(...)");yyval.inst=Upper(yyvsp[-1].inst); ;
    break;}
case 71:
{ PrintDebug("Lower(...)");yyval.inst=Lower(yyvsp[-1].inst); ;
    break;}
case 72:
{ PrintDebug("Sto(...)");Copie(&yyval.inst,S); MemSto(yyvsp[-1].inst); yyvsp[-1].inst=NULL; ;
    break;}
case 73:
{ PrintDebug("Mem(...)");yyval.inst=MemMem(yyvsp[-1].inst); yyvsp[-1].inst=NULL; ;
    break;}
case 74:
{ PrintDebug("Exc(...)");yyval.inst=MemExc(yyvsp[-1].inst); yyvsp[-1].inst=NULL; ;
    break;}
case 75:
{ PrintDebug("Clr(...)");Copie(&yyval.inst,S); MemClr(yyvsp[-1].inst); yyvsp[-1].inst=NULL; ;
    break;}
case 76:
{ PrintDebug("From(...)");yyval.inst=From(yyvsp[-1].inst,0); yyvsp[-1].inst=NULL; ;
    break;}
case 77:
{ PrintDebug("From(...,_STRICT)");yyval.inst=From(yyvsp[-3].inst,1); yyvsp[-3].inst=NULL; ;
    break;}
case 78:
{ PrintDebug("To(...)");yyval.inst=To(yyvsp[-1].inst,0); yyvsp[-1].inst=NULL; ;
    break;}
case 79:
{ PrintDebug("To(...,_STRICT)");yyval.inst=To(yyvsp[-3].inst,1); yyvsp[-3].inst=NULL; ;
    break;}
case 80:
{ PrintDebug("Between(...)");yyval.inst=Between(yyvsp[-3].inst,yyvsp[-1].inst,0); yyvsp[-3].inst=yyvsp[-1].inst=NULL; ;
    break;}
case 81:
{ PrintDebug("Between(...,_STRICT)");yyval.inst=Between(yyvsp[-5].inst,yyvsp[-3].inst,1); yyvsp[-5].inst=yyvsp[-3].inst=NULL; ;
    break;}
case 82:
{ PrintDebug("Delete(...)");yyval.inst=Replace(yyvsp[-1].inst,NULL,0,0); yyvsp[-1].inst=NULL; ;
    break;}
case 83:
{ PrintDebug("Delete(..., AT BEGINING)");yyval.inst=Replace(yyvsp[-4].inst,NULL,1,0); yyvsp[-4].inst=NULL; ;
    break;}
case 84:
{ PrintDebug("Delete(..., AT END)");yyval.inst=Replace(yyvsp[-4].inst,NULL,2,0); yyvsp[-4].inst=NULL; ;
    break;}
case 85:
{ PrintDebug("Delete(...,EVERYWHERE)");yyval.inst=Replace(yyvsp[-4].inst,NULL,3,0); yyvsp[-4].inst=NULL; ;
    break;}
case 86:
{ PrintDebug("Delete(...)");yyval.inst=Replace(yyvsp[-3].inst,NULL,0,1); yyvsp[-3].inst=NULL; ;
    break;}
case 87:
{ PrintDebug("Delete(..., AT BEGINING)");yyval.inst=Replace(yyvsp[-6].inst,NULL,1,1); yyvsp[-6].inst=NULL; ;
    break;}
case 88:
{ PrintDebug("Delete(..., AT END)");yyval.inst=Replace(yyvsp[-6].inst,NULL,2,1); yyvsp[-6].inst=NULL; ;
    break;}
case 89:
{ PrintDebug("replace(...,EVERYWHERE)");yyval.inst=Replace(yyvsp[-6].inst,NULL,3,1); yyvsp[-6].inst=NULL; ;
    break;}
case 90:
{ PrintDebug("replace(...)");yyval.inst=Replace(yyvsp[-3].inst,yyvsp[-1].inst,0,0); yyvsp[-3].inst=yyvsp[-1].inst=NULL; ;
    break;}
case 91:
{ PrintDebug("replace(..., AT BEGINING)");yyval.inst=Replace(yyvsp[-6].inst,yyvsp[-4].inst,1,0); yyvsp[-6].inst=yyvsp[-4].inst=NULL; ;
    break;}
case 92:
{ PrintDebug("replace(..., AT END)");yyval.inst=Replace(yyvsp[-6].inst,yyvsp[-4].inst,2,0); yyvsp[-6].inst=yyvsp[-4].inst=NULL; ;
    break;}
case 93:
{ PrintDebug("replace(...,EVERYWHERE)");yyval.inst=Replace(yyvsp[-6].inst,yyvsp[-4].inst,3,0); yyvsp[-6].inst=yyvsp[-4].inst=NULL; ;
    break;}
case 94:
{ PrintDebug("replace(...)");yyval.inst=Replace(yyvsp[-5].inst,yyvsp[-3].inst,0,1); yyvsp[-5].inst=yyvsp[-3].inst=NULL; ;
    break;}
case 95:
{ PrintDebug("replace(..., AT BEGINING)");yyval.inst=Replace(yyvsp[-8].inst,yyvsp[-6].inst,1,1); yyvsp[-8].inst=yyvsp[-6].inst=NULL; ;
    break;}
case 96:
{ PrintDebug("replace(..., AT END)");yyval.inst=Replace(yyvsp[-8].inst,yyvsp[-6].inst,2,1); yyvsp[-8].inst=yyvsp[-6].inst=NULL; ;
    break;}
case 97:
{ PrintDebug("replace(...,EVERYWHERE)");yyval.inst=Replace(yyvsp[-8].inst,yyvsp[-6].inst,3,1); yyvsp[-8].inst=yyvsp[-6].inst=NULL; ;
    break;}
case 98:
{ PrintDebug("replace(...,Occ)");yyval.inst=ReplaceOcc(yyvsp[-5].inst,yyvsp[-3].inst,yyvsp[-1].inst,0); yyvsp[-5].inst=yyvsp[-3].inst=yyvsp[-1].inst=NULL; ;
    break;}
case 99:
{ PrintDebug("replace(...,Occ)");yyval.inst=ReplaceOcc(yyvsp[-7].inst,yyvsp[-5].inst,yyvsp[-3].inst,1); yyvsp[-7].inst=yyvsp[-5].inst=yyvsp[-3].inst=NULL; ;
    break;}
case 100:
{ PrintDebug("BFirst(...)");yyval.inst=BFirst(yyvsp[-1].inst,0); yyvsp[-1].inst=NULL; ;
    break;}
case 101:
{ PrintDebug("BFirst(Number)");yyval.inst=BFirst(NULL,1); ;
    break;}
case 102:
{ PrintDebug("BFirst(String)");yyval.inst=BFirst(NULL,2); ;
    break;}
case 103:
{ PrintDebug("EFirst(...)");yyval.inst=EFirst(yyvsp[-1].inst,0); yyvsp[-1].inst=NULL; ;
    break;}
case 104:
{ PrintDebug("EFirst(Number)");yyval.inst=EFirst(NULL,1); ;
    break;}
case 105:
{ PrintDebug("EFirst(String)");yyval.inst=EFirst(NULL,2); ;
    break;}
case 106:
{ PrintDebug("BLast(...)");yyval.inst=BLast(yyvsp[-1].inst,0); yyvsp[-1].inst=NULL; ;
    break;}
case 107:
{ PrintDebug("BLast(Number)");yyval.inst=BLast(NULL,1); ;
    break;}
case 108:
{ PrintDebug("BLast(String)");yyval.inst=BLast(NULL,2); ;
    break;}
case 109:
{ PrintDebug("ELast(...)");yyval.inst=ELast(yyvsp[-1].inst,0); yyvsp[-1].inst=NULL; ;
    break;}
case 110:
{ PrintDebug("ELast(Number)");yyval.inst=ELast(NULL,1); ;
    break;}
case 111:
{ PrintDebug("ELast(String)");yyval.inst=ELast(NULL,2); ;
    break;}
case 112:
{ PrintDebug("Next(...)");yyval.inst=Next_(yyvsp[-1].tcd,NULL,0); yyvsp[-1].tcd=NULL; ;
    break;}
case 113:
{ PrintDebug("Next(...,...)");
                                          yyval.inst=Next_(yyvsp[-3].tcd,yyvsp[-1].tcd,0);
                                          if (!yyval.inst) return 2;
                                          yyvsp[-3].tcd=yyvsp[-1].tcd=NULL; ;
    break;}
case 114:
{ PrintDebug("Next(...,_STRICT)");
                                          yyval.inst=Next_(yyvsp[-3].tcd,NULL,1);
                                          if (!yyval.inst) return 2;
                                          yyvsp[-3].tcd=NULL; ;
    break;}
case 115:
{ PrintDebug("Next(...,...,_STRICT)");
                                          yyval.inst=Next_(yyvsp[-5].tcd,yyvsp[-3].tcd,1);
                                          if (!yyval.inst) return 2;
                                          yyvsp[-5].tcd=yyvsp[-3].tcd=NULL; ;
    break;}
case 116:
{ PrintDebug("Last(...)");
                                          yyval.inst=Last_(yyvsp[-1].tcd,NULL,0);
                                          if (!yyval.inst) return 2;
                                          yyvsp[-1].tcd=NULL; ;
    break;}
case 117:
{ PrintDebug("Last(...,...)");
                                          yyval.inst=Last_(yyvsp[-3].tcd,yyvsp[-1].tcd,0);
                                          if (!yyval.inst) return 2;
                                          yyvsp[-3].tcd=yyvsp[-1].tcd=NULL; ;
    break;}
case 118:
{ PrintDebug("Last(...,_STRICT)");
                                          yyval.inst=Last_(yyvsp[-3].tcd,NULL,1);
                                          if (!yyval.inst) return 2;
                                          yyvsp[-3].tcd=NULL; ;
    break;}
case 119:
{ PrintDebug("Last(...,...,_STRICT)");
                                          yyval.inst=Last_(yyvsp[-5].tcd,yyvsp[-3].tcd,1);
                                          if (!yyval.inst) return 2;
                                          yyvsp[-5].tcd=yyvsp[-3].tcd=NULL; ;
    break;}
case 120:
{ PrintDebug("Sort");
                                  MustSort(yyvsp[-1].inst->str.str());
                                  FreeTypeInst(yyvsp[-1].inst);
                                  yyvsp[-1].inst=NULL;
                                  return 2;
                                ;
    break;}
case 121:
{ PrintDebug("Table(...)");yyval.inst=Table_(yyvsp[-1].inst); yyvsp[-1].inst=NULL; ;
    break;}
case 122:
{ PrintDebug("RegFind(...)");yyval.inst=RegFind(yyvsp[-1].inst, NULL); yyvsp[-1].inst=NULL; ;
    break;}
case 123:
{ PrintDebug("RegFind(...,...)");yyval.inst=RegFind(yyvsp[-3].inst,yyvsp[-1].inst); yyvsp[-3].inst=yyvsp[-1].inst=NULL; ;
    break;}
case 124:
{ PrintDebug("RegMatch(...)");yyval.inst=RegMatch(yyvsp[-1].inst); yyvsp[-1].inst=NULL; ;
    break;}
case 125:
{ PrintDebug("RegReplace(...,...)");yyval.inst=RegReplace(yyvsp[-3].inst,yyvsp[-1].inst,NULL); yyvsp[-3].inst=yyvsp[-1].inst=NULL; ;
    break;}
case 126:
{ PrintDebug("RegReplace(...,...,...)");yyval.inst=RegReplace(yyvsp[-5].inst,yyvsp[-3].inst,yyvsp[-1].inst); yyvsp[-5].inst=yyvsp[-3].inst=yyvsp[-1].inst=NULL; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign  */
  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YY_MarcParser_LSP_NEEDED
  yylsp -= yylen;
#endif

#if YY_MarcParser_DEBUG != 0
  if (YY_MarcParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YY_MarcParser_LSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = YY_MarcParser_LLOC.first_line;
      yylsp->first_column = YY_MarcParser_LLOC.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  YYGOTO(yynewstate);

YYLABEL(yyerrlab)   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++YY_MarcParser_NERRS;

#ifdef YY_MarcParser_ERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      YY_MarcParser_ERROR(msg);
	      free(msg);
	    }
	  else
	    YY_MarcParser_ERROR ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YY_MarcParser_ERROR_VERBOSE */
	YY_MarcParser_ERROR("parse error");
    }

  YYGOTO(yyerrlab1);
YYLABEL(yyerrlab1)   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (YY_MarcParser_CHAR == YYEOF)
	YYABORT;

#if YY_MarcParser_DEBUG != 0
      if (YY_MarcParser_DEBUG_FLAG)
	fprintf(stderr, "Discarding token %d (%s).\n", YY_MarcParser_CHAR, yytname[yychar1]);
#endif

      YY_MarcParser_CHAR = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  YYGOTO(yyerrhandle);

YYLABEL(yyerrdefault)  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) YYGOTO(yydefault);
#endif

YYLABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YY_MarcParser_LSP_NEEDED
  yylsp--;
#endif

#if YY_MarcParser_DEBUG != 0
  if (YY_MarcParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

YYLABEL(yyerrhandle)

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yyerrdefault);

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    YYGOTO(yyerrdefault);

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrpop);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrpop);

  if (yyn == YYFINAL)
    YYACCEPT;

#if YY_MarcParser_DEBUG != 0
  if (YY_MarcParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = YY_MarcParser_LVAL;
#ifdef YY_MarcParser_LSP_NEEDED
  *++yylsp = YY_MarcParser_LLOC;
#endif

  yystate = yyn;
  YYGOTO(yynewstate);
/* end loop, in which YYGOTO may be used. */
  YYENDGOTO
}

/* END */





