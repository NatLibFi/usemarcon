
#ifndef MarcParser_h
#define MarcParser_h

#include "typedef.h"
#include "tcd.h"

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
#define YYFINAL  128
#define YYLAST   1410
#define YYNTOKENS  102
#define YYNNTS  12
#define YYNRULES  169
#define YYNSTATES  462
#define YYUNDEFTOK  2
#define YYMAXUTOK   353
#define YYTRANSLATE(YYX)    ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)


class MarcParser
{
protected:
  TypeInst* S;
  TypeInst* T;
  TypeInst* D;
  TypeCD* CDIn;
  
  TypeInst* V_TAG;
  TypeInst* V_SUB;
  TypeInst* N;
  TypeInst* NT;
  TypeInst* NS;
  TypeInst* NO;
  TypeInst* NSO;
  TypeInst* NTO;
  TypeInst* NEW;
  TypeInst* NEWEST;
  TypeInst* Memoire[100];

  TCD* mCDOut;

  bool m_debug_rule;
  bool mRedoFlag;
  unsigned long m_ordinal;

  virtual int Precedes(TypeCD*, TypeCD*) = 0;
  virtual int Exists(TypeCD*) = 0;
  virtual int ExistsIn(TypeInst* a_str, TypeCD* a_cd) = 0;
  virtual int InTable(TypeInst*, TypeInst*) = 0;
  virtual typestr2 ReadCD(TypeCD *) = 0;
  virtual TypeCD* AllocCD() = 0;
  virtual void FreeCD( TypeCD* CD ) = 0;
  virtual TypeInst* Next_( TypeCD* cd1, TypeCD* cd2, int strict ) = 0;
  virtual TypeInst* Last_( TypeCD* cd1, TypeCD* cd2, int strict ) = 0;
  virtual TypeInst* NextSub(TypeCD* aFindCD, TypeInst* aOccurrence) = 0;
  virtual TypeInst* NextSubIn(TypeInst* aStr, TypeCD* aFindCD, TypeInst* aOccurrence) = 0;
  virtual TypeInst* PreviousSub(TypeCD* aFindCD, TypeInst* aOccurrence) = 0;
  virtual TypeInst* PreviousSubIn(TypeInst* aStr, TypeCD* aFindCD, TypeInst* aOccurrence) = 0;
  virtual TypeInst* Subtract( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Multiply( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Divide( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Value( TypeInst* t ) = 0;
  virtual int MemSto( TypeInst* a_index, TypeInst* a_content = NULL ) = 0;
  virtual TypeInst* MemMem( TypeInst* n ) = 0;
  virtual int MemClr( TypeInst* n  ) = 0;
  virtual TypeInst* MemExc( TypeInst* n ) = 0;

  virtual typestr Table( char*, char* ) = 0;
  virtual int MustSort( char* ) = 0;

  virtual TypeInst* AllocTypeInst() = 0;
  virtual void FreeTypeInst( TypeInst* t ) = 0;
  virtual int CopyInst( TypeInst** In, TypeInst* From ) = 0;
  virtual int BoolEQ( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolIn( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolGT( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolGE( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Add( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual char* ToString( TypeInst* t ) = 0;
  virtual TypeInst* String( TypeInst* t ) = 0;
  virtual TypeInst* Upper( TypeInst* t ) = 0;
  virtual TypeInst* Lower( TypeInst* t ) = 0;
  virtual TypeInst* Len( TypeInst* t ) = 0;
  virtual TypeInst* From(TypeInst* t, bool a_strict) = 0;
  virtual TypeInst* To(TypeInst* t, bool a_strict) = 0;
  virtual TypeInst* Between(TypeInst* t1, TypeInst* t2, bool a_strict) = 0;
  virtual TypeInst* Replace( TypeInst* t1, TypeInst* t2, IN_STR_POSITION at, bool strict ) = 0;
  virtual TypeInst* ReplaceOcc( TypeInst* t1, TypeInst* t2, TypeInst* inCondOcc, bool strict ) = 0;
  virtual TypeInst* BFirst( TypeInst* t, int k ) = 0;
  virtual TypeInst* EFirst( TypeInst* t, int k ) = 0;
  virtual TypeInst* BLast( TypeInst* t, int k ) = 0;
  virtual TypeInst* ELast( TypeInst* t, int k ) = 0;
  virtual TypeInst* Table_( TypeInst* Nom ) = 0;
  virtual void PrintDebug(const char*) = 0;
  virtual TypeInst* RegFindNum( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* RegFindPos( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* RegFind( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* RegMatch( TypeInst* t1 ) = 0;
  virtual TypeInst* RegReplace(TypeInst* a_regexp, TypeInst* a_replacement, TypeInst* a_options) = 0;
  virtual TypeInst* RegReplaceTable( TypeInst* a_table, TypeInst* a_options) = 0;
  virtual TypeInst* MoveBefore(TypeInst* a_source, TypeCD* a_before, TypeInst* a_target, TypeInst* a_prefix, TypeInst* a_suffix, TypeInst* a_preserved_punctuations, TypeInst* a_preserved_subfields) = 0;
  virtual TypeInst* MoveAfter(TypeInst* a_source, TypeCD* a_after, TypeInst* a_target, TypeInst* a_prefix, TypeInst* a_suffix, TypeInst* a_preserved_punctuations, TypeInst* a_preserved_subfields) = 0;


public:

#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   enum yytokentype {
     SEP = 258,
     FIN = 259,
     WNUMBER = 260,
     WSTRING = 261,
     PLUS = 262,
     MINUS = 263,
     MULTIPLY = 264,
     DIVIDE = 265,
     EQ = 266,
     NE = 267,
     _IN = 268,
     GT = 269,
     LT = 270,
     GE = 271,
     LE = 272,
     EXISTS = 273,
     EXISTSIN = 274,
     PRECEDES = 275,
     FOLLOWS = 276,
     INTABLE = 277,
     CHECK = 278,
     AND = 279,
     OR = 280,
     NOT = 281,
     BY = 282,
     _STRICT = 283,
     AT = 284,
     BEGINING = 285,
     BEGINNING = 286,
     END = 287,
     BOTH = 288,
     VARS = 289,
     VARD = 290,
     STRING = 291,
     NUMERIC = 292,
     VAR_N = 293,
     VAR_NT = 294,
     VAR_NS = 295,
     VAR_NO = 296,
     VAR_NTO = 297,
     VAR_NSO = 298,
     VAR_NEW = 299,
     VAR_NEWEST = 300,
     VAR_TAG = 301,
     VAR_SUB = 302,
     TAG = 303,
     DTAG = 304,
     STAG = 305,
     FIX = 306,
     I1 = 307,
     I2 = 308,
     STR = 309,
     VAL = 310,
     LEN = 311,
     STO = 312,
     MEM = 313,
     EXC = 314,
     CLR = 315,
     LOWER = 316,
     UPPER = 317,
     FROM = 318,
     TO = 319,
     BETWEEN = 320,
     _DELETE = 321,
     REPLACE = 322,
     REPLACEOCC = 323,
     BFIRST = 324,
     EFIRST = 325,
     BLAST = 326,
     ELAST = 327,
     REDO = 328,
     SORT = 329,
     NEXT = 330,
     LAST = 331,
     TABLE = 332,
     ORDINAL = 333,
     YEAR = 334,
     MONTH = 335,
     DAY = 336,
     HOUR = 337,
     MINUTE = 338,
     SECOND = 339,
     NEXTSUB = 340,
     NEXTSUBIN = 341,
     PREVIOUSSUB = 342,
     PREVIOUSSUBIN = 343,
     REGFINDNUM = 344,
     REGFINDPOS = 345,
     REGFIND = 346,
     REGMATCH = 347,
     REGREPLACE = 348,
     REGREPLACETABLE = 349,
     MOVEBEFORE = 350,
     MOVEAFTER = 351,
     MOVEFIRST = 352,
     MOVELAST = 353
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
