#ifndef YY_MarcParser_h_included
#define YY_MarcParser_h_included
/* before anything */
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
#endif
#include <stdio.h>
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

typedef union
{
        int   code;
        TypeInst* inst;
        TypeCD*   tcd;
} yy_MarcParser_stype;
#define YY_MarcParser_STYPE yy_MarcParser_stype
 /* %{ and %header{ and %union, during decl */
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
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_MarcParser_STYPE 
#define YY_MarcParser_STYPE YYSTYPE
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
/* use %define STYPE */
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_MarcParser_DEBUG
#define  YY_MarcParser_DEBUG YYDEBUG
/* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
/* use %define DEBUG */
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
/* YY_MarcParser_PURE */
#endif
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

/* TOKEN C */
#ifndef YY_USE_CLASS

#ifndef YY_MarcParser_PURE
extern YY_MarcParser_STYPE YY_MarcParser_LVAL;
#endif

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

 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
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
 int YY_MarcParser_PARSE(YY_MarcParser_PARSE_PARAM);
 virtual void YY_MarcParser_ERROR(char *msg) YY_MarcParser_ERROR_BODY;
#ifdef YY_MarcParser_PURE
#ifdef YY_MarcParser_LSP_NEEDED
 virtual int  YY_MarcParser_LEX(YY_MarcParser_STYPE *YY_MarcParser_LVAL,YY_MarcParser_LTYPE *YY_MarcParser_LLOC) YY_MarcParser_LEX_BODY;
#else
 virtual int  YY_MarcParser_LEX(YY_MarcParser_STYPE *YY_MarcParser_LVAL) YY_MarcParser_LEX_BODY;
#endif
#else
 virtual int YY_MarcParser_LEX() YY_MarcParser_LEX_BODY;
 YY_MarcParser_STYPE YY_MarcParser_LVAL;
#ifdef YY_MarcParser_LSP_NEEDED
 YY_MarcParser_LTYPE YY_MarcParser_LLOC;
#endif
 int YY_MarcParser_NERRS;
 int YY_MarcParser_CHAR;
#endif
#if YY_MarcParser_DEBUG != 0
public:
 int YY_MarcParser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
#endif
public:
 YY_MarcParser_CLASS(YY_MarcParser_CONSTRUCTOR_PARAM);
public:
 YY_MarcParser_MEMBERS 
};
/* other declare folow */
#endif


#if YY_MarcParser_COMPATIBILITY != 0
/* backward compatibility */
#ifndef YYSTYPE
#define YYSTYPE YY_MarcParser_STYPE
#endif

#ifndef YYLTYPE
#define YYLTYPE YY_MarcParser_LTYPE
#endif
#ifndef YYDEBUG
#ifdef YY_MarcParser_DEBUG 
#define YYDEBUG YY_MarcParser_DEBUG
#endif
#endif

#endif
/* END */
#endif
