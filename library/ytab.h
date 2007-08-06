
#ifndef MarcParser_h
#define MarcParser_h

#include "typedef.h"

#ifdef short
# undef short
#endif
#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif
#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif
#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif
#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif
#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif
#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)
#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif
#define YYFINAL  106
#define YYLAST   1128
#define YYNTOKENS  91
#define YYNNTS  12
#define YYNRULES  139
#define YYNSTATES  344
#define YYUNDEFTOK  2
#define YYMAXUTOK   342
#define YYTRANSLATE(YYX)    ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)


class MarcParser
{
protected:
  TypeInst* S;
  TypeInst* T;
  TypeInst* D;
  TypeCD* CDIn;

  TypeInst* N;
  TypeInst* NT;
  TypeInst* NS;
  TypeInst* NO;
  TypeInst* NSO;
  TypeInst* NTO;
  TypeInst* NEW;
  TypeInst* Memoire[100];

  int debug_rule;
  char tempo[1000];
  bool RedoFlag;
  unsigned long ordinal;

  virtual int Precedes(TypeCD*, TypeCD*) = 0;
  virtual int Exists(TypeCD*) = 0;
  virtual int InTable(TypeInst*, TypeInst*) = 0;
  virtual typestr ReadCD(TypeCD *) = 0;
  virtual TypeCD* AllocCD() = 0;
  virtual void FreeCD( TypeCD* CD ) = 0;
  virtual TypeInst* Next_( TypeCD* cd1, TypeCD* cd2, int strict ) = 0;
  virtual TypeInst* Last_( TypeCD* cd1, TypeCD* cd2, int strict ) = 0;
  virtual TypeInst* NextSub(TypeCD* aFindCD, TypeInst* aOccurrence) = 0;
  virtual TypeInst* PreviousSub(TypeCD* aFindCD, TypeInst* aOccurrence) = 0;
  virtual TypeInst* Soust( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Multi( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Divis( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Value( TypeInst* t ) = 0;
  virtual int MemSto( TypeInst* n ) = 0;
  virtual TypeInst* MemMem( TypeInst* n ) = 0;
  virtual int MemClr( TypeInst* n  ) = 0;
  virtual TypeInst* MemExc( TypeInst* n ) = 0;

  virtual typestr Table( char*, char* ) = 0;
  virtual int   MustSort( char* ) = 0;

  virtual TypeInst* AllocTypeInst() = 0;
  virtual void FreeTypeInst( TypeInst* t ) = 0;
  virtual int Copie( TypeInst** In, TypeInst* From ) = 0;
  virtual int BoolEQ( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolIn( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolGT( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolGE( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Ajout( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* AjoutOcc( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual char* ToString( TypeInst* t ) = 0;
  virtual TypeInst* String( TypeInst* t ) = 0;
  virtual TypeInst* Upper( TypeInst* t ) = 0;
  virtual TypeInst* Lower( TypeInst* t ) = 0;
  virtual TypeInst* Len( TypeInst* t ) = 0;
  virtual TypeInst* From( TypeInst* t, int strict ) = 0;
  virtual TypeInst* To( TypeInst* t, int strict ) = 0;
  virtual TypeInst* Between( TypeInst* t1, TypeInst* t2, int strict ) = 0;
  virtual TypeInst* Replace( TypeInst* t1, TypeInst* t2, int at, int strict ) = 0;
  virtual TypeInst* ReplaceOcc( TypeInst* t1, TypeInst* t2, TypeInst* inCondOcc, int strict ) = 0;
  virtual TypeInst* BFirst( TypeInst* t, int k ) = 0;
  virtual TypeInst* EFirst( TypeInst* t, int k ) = 0;
  virtual TypeInst* BLast( TypeInst* t, int k ) = 0;
  virtual TypeInst* ELast( TypeInst* t, int k ) = 0;
  virtual TypeInst* Table_( TypeInst* Nom ) = 0;
  virtual void PrintDebug(const char*) = 0;
  virtual TypeInst* RegFind( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* RegMatch( TypeInst* t1 ) = 0;
  virtual TypeInst* RegReplace( TypeInst* t1, TypeInst* t2, TypeInst* t3 ) = 0;


public:

#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   enum yytokentype {
     SEP = 258,
     FIN = 259,
     WNUMBER = 260,
     WSTRING = 261,
     PLUS = 262,
     MOINS = 263,
     MULTIPLIE = 264,
     DIVISE = 265,
     EQ = 266,
     NE = 267,
     _IN = 268,
     GT = 269,
     LT = 270,
     GE = 271,
     LE = 272,
     EXISTS = 273,
     PRECEDES = 274,
     FOLLOWS = 275,
     INTABLE = 276,
     IF = 277,
     THEN = 278,
     ELSE = 279,
     AND = 280,
     OR = 281,
     NOT = 282,
     BY = 283,
     _STRICT = 284,
     AT = 285,
     BEGINING = 286,
     BEGINNING = 287,
     END = 288,
     BOTH = 289,
     VARS = 290,
     VARD = 291,
     STRING = 292,
     NUMERIC = 293,
     VAR_N = 294,
     VAR_NT = 295,
     VAR_NS = 296,
     VAR_NO = 297,
     VAR_NTO = 298,
     VAR_NSO = 299,
     VAR_NEW = 300,
     TAG = 301,
     DTAG = 302,
     STAG = 303,
     FIX = 304,
     I1 = 305,
     I2 = 306,
     STR = 307,
     VAL = 308,
     LEN = 309,
     STO = 310,
     MEM = 311,
     EXC = 312,
     CLR = 313,
     LOWER = 314,
     UPPER = 315,
     FROM = 316,
     TO = 317,
     BETWEEN = 318,
     _DELETE = 319,
     REPLACE = 320,
     REPLACEOCC = 321,
     BFIRST = 322,
     EFIRST = 323,
     BLAST = 324,
     ELAST = 325,
     REDO = 326,
     SORT = 327,
     NEXT = 328,
     LAST = 329,
     TABLE = 330,
     ORDINAL = 331,
     YEAR = 332,
     MONTH = 333,
     DAY = 334,
     HOUR = 335,
     MINUTE = 336,
     SECOND = 337,
     NEXTSUB = 338,
     PREVIOUSSUB = 339,
     REGFIND = 340,
     REGMATCH = 341,
     REGREPLACE = 342
   };
#endif
  #if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
  typedef union YYSTYPE
  
  {
          int   code;
          TypeInst* inst;
          TypeCD*   tcd;
  }
  
  	YYSTYPE;
  # define yystype YYSTYPE
  # define YYSTYPE_IS_DECLARED 1
  # define YYSTYPE_IS_TRIVIAL 1
  #endif
  virtual int yylex() = 0;
  virtual void yyerror(char *) = 0;

protected:

  
  #if ! defined yyoverflow || YYERROR_VERBOSE
  
  # ifdef YYSTACK_USE_ALLOCA
  #  if YYSTACK_USE_ALLOCA
  #   ifdef __GNUC__
  #    define YYSTACK_ALLOC __builtin_alloca
  #   elif defined __BUILTIN_VA_ARG_INCR
  #    include <alloca.h>
  #   elif defined _AIX
  #    define YYSTACK_ALLOC __alloca
  #   elif defined _MSC_VER
  #    include <malloc.h>
  #    define alloca _alloca
  #   else
  #    define YYSTACK_ALLOC alloca
  #    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  #     include <stdlib.h>
  #     ifndef _STDLIB_H
  #      define _STDLIB_H 1
  #     endif
  #    endif
  #   endif
  #  endif
  # endif
  
  # ifdef YYSTACK_ALLOC
  #  define YYSTACK_FREE(Ptr) do {; } while (YYID (0))
  #  ifndef YYSTACK_ALLOC_MAXIMUM
  #   define YYSTACK_ALLOC_MAXIMUM 4032
  #  endif
  # else
  #  define YYSTACK_ALLOC YYMALLOC
  #  define YYSTACK_FREE YYFREE
  #  ifndef YYSTACK_ALLOC_MAXIMUM
  #   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
  #  endif
  #  if (defined __cplusplus && ! defined _STDLIB_H         && ! ((defined YYMALLOC || defined malloc)  	     && (defined YYFREE || defined free)))
  #   include <stdlib.h>
  #   ifndef _STDLIB_H
  #    define _STDLIB_H 1
  #   endif
  #  endif
  #  ifndef YYMALLOC
  #   define YYMALLOC malloc
  #   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  void *malloc (YYSIZE_T);
  #   endif
  #  endif
  #  ifndef YYFREE
  #   define YYFREE free
  #   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  void free (void *);
  #   endif
  #  endif
  # endif
  #endif
  
  
  #if (! defined yyoverflow       && (! defined __cplusplus  	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))
  union yyalloc
  {
    yytype_int16 yyss;
    YYSTYPE yyvs;
    };
  # define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)
  # define YYSTACK_BYTES(N)       ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE))        + YYSTACK_GAP_MAXIMUM)
  # ifndef YYCOPY
  #  if defined __GNUC__ && 1 < __GNUC__
  #   define YYCOPY(To, From, Count)        __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
  #  else
  #   define YYCOPY(To, From, Count)        do  	{  	  YYSIZE_T yyi;  	  for (yyi = 0; yyi < (Count); yyi++)  	    (To)[yyi] = (From)[yyi];  	}        while (YYID (0))
  #  endif
  # endif
  # define YYSTACK_RELOCATE(Stack)      do        {  	YYSIZE_T yynewbytes;  	YYCOPY (&yyptr->Stack, Stack, yysize);  	Stack = &yyptr->Stack;  	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM;  	yyptr += yynewbytes / sizeof (*yyptr);        }      while (YYID (0))
  
  #endif
#if (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  void yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
;
#else
  void yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
;
#endif
#if (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  void yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
;
#else
  void yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
;
#endif
#if (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  void yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
;
#else
  void yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
;
#endif
#if (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  void yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
;
#else
  void yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
;
#endif
  int yyi;
  int yydebug;
  int yyx;
#if (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  void yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
;
#else
  void yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
;
#endif
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  int yyparse(void *YYPARSE_PARAM);
#else
  int yyparse(YYPARSE_PARAM);
#endif
#else
#if (defined __STDC__ || defined __C99__FUNC__       || defined __cplusplus || defined _MSC_VER)
  int yyparse(void);
#else
  int yyparse();
#endif
#endif
  int yystate;
  int yyn;
  int yyresult;
  int yyerrstatus;
  YYSTYPE yyval;

public:
  MarcParser() : S(NULL), T(NULL), D(NULL), CDIn(NULL), N(NULL), NT(NULL), NS(NULL), NO(NULL), NSO(NULL), NTO(NULL) {}

};
#endif //MarcParser_h
