%{
/*cppdef*/
#pragma warning( disable : 4102 )
#pragma warning( disable : 4065 )

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

/* Command to build:  ../bison+.pl --input analyse2.y --class MarcScanner --cpp ytab.cpp --hdr ytab.h --no-lines */

/*headerdef
#include "typedef.h"
#include "tcd.h"
*/

/*classdef
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
  TypeInst* NEWEST;
  TypeInst* Memoire[100];

  TCD* mCDOut;

  int debug_rule;
  char tempo[1000];
  bool RedoFlag;
  unsigned long ordinal;

  virtual int Precedes(TypeCD*, TypeCD*) = 0;
  virtual int Exists(TypeCD*) = 0;
  virtual int ExistsIn(TypeInst* a_str, TypeCD* a_cd) = 0;
  virtual int InTable(TypeInst*, TypeInst*) = 0;
  virtual typestr ReadCD(TypeCD *) = 0;
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
  virtual int   MustSort( char* ) = 0;

  virtual TypeInst* AllocTypeInst() = 0;
  virtual void FreeTypeInst( TypeInst* t ) = 0;
  virtual int CopyInst( TypeInst** In, TypeInst* From ) = 0;
  virtual int BoolEQ( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolIn( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolGT( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual int BoolGE( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* Add( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual TypeInst* AddOcc( TypeInst* t1, TypeInst* t2 ) = 0;
  virtual char* ToString( TypeInst* t ) = 0;
  virtual TypeInst* String( TypeInst* t ) = 0;
  virtual TypeInst* Upper( TypeInst* t ) = 0;
  virtual TypeInst* Lower( TypeInst* t ) = 0;
  virtual TypeInst* Len( TypeInst* t ) = 0;
  virtual TypeInst* From( TypeInst* t, int strict ) = 0;
  virtual TypeInst* To( TypeInst* t, int strict ) = 0;
  virtual TypeInst* Between( TypeInst* t1, TypeInst* t2, int strict ) = 0;
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

*/

/*constructor_init
S(NULL), T(NULL), D(NULL), CDIn(NULL), N(NULL), NT(NULL), NS(NULL), NO(NULL), NSO(NULL), NTO(NULL)
*/



%}

%union {
        int   code;
        TypeInst* inst;
        TypeCD*   tcd;
}


%token <code> SEP FIN WNUMBER WSTRING 
%token <code> PLUS MINUS MULTIPLY DIVIDE
%token <code> EQ NE _IN GT LT GE LE
%token <code> EXISTS EXISTSIN PRECEDES FOLLOWS INTABLE
%token <code> CHECK AND OR NOT
%token <code> BY _STRICT AT BEGINING BEGINNING END BOTH 

%token <inst> VARS VARD STRING NUMERIC
%token <inst> VAR_N VAR_NT VAR_NS VAR_NO VAR_NTO VAR_NSO VAR_NEW VAR_NEWEST
%token <inst> TAG DTAG STAG FIX I1 I2
%token <inst> STR VAL LEN STO MEM EXC CLR LOWER UPPER
%token <inst> FROM TO BETWEEN _DELETE REPLACE REPLACEOCC
%token <inst> BFIRST EFIRST BLAST ELAST
%token <inst> REDO SORT NEXT LAST TABLE ORDINAL
%token <inst> YEAR MONTH DAY HOUR MINUTE SECOND
%token <inst> NEXTSUB NEXTSUBIN PREVIOUSSUB PREVIOUSSUBIN 
%token <inst> REGFINDNUM REGFINDPOS REGFIND REGMATCH REGREPLACE REGREPLACETABLE
%token <inst> MOVEBEFORE MOVEAFTER

%left  SEP PLUS MINUS MULTIPLY DIVIDE

%type <inst> Program Rules Rule
%type <inst> SetOfInstr
%type <inst> Instruction Condition Translation
%type <tcd>  CD TAGOCC STAGOCC
%type <code> Boolean 

%%
Program :
        Rules                   {
                                  if (!RedoFlag)
                                  {
                                        FreeTypeInst(S);
                                        CopyInst(&S,$$);
                                        FreeTypeInst($$);
                                        $$=NULL;
                                        return 1;
                                  }
                                  else
                                  {
                                        FreeTypeInst(S);
                                        S=T;
                                        CopyInst(&T,$$);
                                        FreeTypeInst($$);
                                        $$=NULL;
                                        if (T->str.str()[0]==0) return 1;
                                        else              return 3;
                                  }
                                }
;

Rules :
        Rule                    {
                                  CopyInst(&$$,S);
                                  FreeTypeInst(S);
                                  S=NULL;
                                }
|       PLUS Rule               {
                                  $$=Add(D,S);
                                  D=S=NULL;
                                }
;

Rule :
        SetOfInstr FIN
|       SetOfInstr SEP FIN
;

SetOfInstr :
        SetOfInstr SEP SetOfInstr
|       Instruction             {
                                  FreeTypeInst(S);
                                  CopyInst(&S,$1);
                                  FreeTypeInst($1);
                                  $1=NULL;
                                }
;

Instruction :
        Condition
|       Translation
|       REDO                    { PrintDebug("Redo");
                                  CopyInst(&$$,S);
                                  RedoFlag=true;
                                  CopyInst(&T,S);
                                }
;

CD :
        TAGOCC                  { PrintDebug("Tagocc");
                                  $$=$1;
                                  $$->Fixed.freestr();
                                  $$->SubField[0]=0;
                                  $$->ns=0;
                                }
|       TAGOCC STAGOCC          { PrintDebug("Tagocc Stagocc");
                                  $$=$1;
                                  $$->Fixed.freestr();
                                  strcpy($$->SubField,$2->SubField);
                                  $$->ns=$2->ns;
                                  FreeCD($2);
                                  $2=NULL;}
|       TAGOCC FIX              { PrintDebug("Tagocc Fix");
                                  $$=$1;
                                  $$->SubField[0]=0;
                                  $$->ns=0;
                                  $$->Fixed.str($2->str.str());
                                  FreeTypeInst($2);
                                  $2=NULL;}
|       TAGOCC STAGOCC FIX      { PrintDebug("Tagocc Stagocc Fix");
                                  $$=$1;
                                  strcpy($$->SubField,$2->SubField);
                                  $$->ns=$2->ns;
                                  $$->Fixed.str($3->str.str());
                                  FreeCD($2);
                                  $2=NULL;
                                  FreeTypeInst($3);
                                  $3=NULL;}
|       STAGOCC                 { PrintDebug("Stagocc");
                                  $$=$1;
                                  $$->Fixed.freestr();
                                  if ($$->Output)
                                    strcpy($$->Field, mCDOut->GetTag());
                                  else
                                    strcpy($$->Field, CDIn->Field);
                                  $$->nt=0; }
|       FIX                     { PrintDebug("Fix");
                                  $$=AllocCD();
                                  strcpy($$->Field,CDIn->Field);
                                  $$->nt=0;
                                  strcpy($$->SubField,CDIn->SubField);
                                  $$->ns=0;
                                  $$->Fixed.str($1->str.str());
                                  FreeTypeInst($1);
                                  $1=NULL;}
|       STAGOCC FIX             { PrintDebug("Stagocc Fix");
                                  $$=$1;
                                  $$->Fixed.str($2->str.str());
                                  strcpy($$->Field,CDIn->Field);
                                  $$->nt=0;
                                  FreeTypeInst($2);
                                  $2=NULL;}
;

TAGOCC :
        TAG                             { PrintDebug("Tag");
                                          $$=AllocCD();
                                          if (*$1->str.str() == '%')
                                          {
                                            $$->Output = true;
                                            strcpy($$->Field,$1->str.str() + 1);
                                          }
                                          else
                                          {  
                                            strcpy($$->Field,$1->str.str());
                                          }
                                          $$->nt=0;
                                          FreeTypeInst($1);
                                          $1=NULL; }
|       TAG '(' Translation ')'         { PrintDebug("Tag(...)");
                                          $$=AllocCD();
                                          if (*$1->str.str() == '%')
                                          {
                                            $$->Output = true;
                                            strcpy($$->Field,$1->str.str() + 1);
                                          }
                                          else
                                          {  
                                            strcpy($$->Field,$1->str.str());
                                          }
                                          FreeTypeInst($1);
                                          $1=NULL;
                                          $$->nt=$3->val;
                                          FreeTypeInst($3);
                                          $3=NULL; }
;
STAGOCC :
        STAG                            { PrintDebug("Stag");
                                          $$=AllocCD();
                                          if (*$1->str.str() == '%')
                                          {
                                            $$->Output = true;
                                            strcpy($$->SubField,$1->str.str() + 1);
                                          }
                                          else
                                            strcpy($$->SubField,$1->str.str());
                                          $$->ns=0;
                                          FreeTypeInst($1);
                                          $1=NULL; }
|       STAG '(' Translation ')'        { PrintDebug("Stag(...)");
                                          $$=AllocCD();
                                          if (*$1->str.str() == '%')
                                          {
                                            $$->Output = true;
                                            strcpy($$->SubField,$1->str.str() + 1);
                                          }
                                          else
                                            strcpy($$->SubField,$1->str.str());
                                          FreeTypeInst($1);
                                          $1=NULL;
                                          $$->ns=$3->val;
                                          FreeTypeInst($3);
                                          $3=NULL; }
|       I1                              { PrintDebug("I1");
                                          $$=AllocCD();
                                          strcpy($$->SubField,"I1");
                                          $$->ns=0;
                                        }
|       I2                              { PrintDebug("I2");
                                          $$=AllocCD();
                                          strcpy($$->SubField,"I2");
                                          $$->ns=0;
                                        }
;

Condition :
        CHECK Boolean                   { PrintDebug("Check");
                                          if ($2)
                                            return 4;
                                          else
                                            return 2;
                                        }
;


Boolean :
        '(' Boolean ')'                 { PrintDebug("(B...)"); $$=$2; }
|       Boolean AND Boolean             { PrintDebug("B...and B..."); $$ = $1 && $3; }
|       Boolean OR Boolean              { PrintDebug("B...or B..."); $$ = $1 || $3; }
|       NOT Boolean                     { PrintDebug("!B..."); $$ = !$2; }
|       Translation EQ Translation      { PrintDebug("...=..."); $$ = BoolEQ($1,$3); $1=$3=NULL; }
|       Translation NE Translation      { PrintDebug("...!=..."); $$ = !BoolEQ($1,$3); $1=$3=NULL; }
|       Translation _IN Translation     { PrintDebug("..._In..."); $$ = BoolIn($1,$3); $1=$3=NULL; }
|       Translation GT Translation      { PrintDebug("...>..."); $$ = BoolGT($1,$3); $1=$3=NULL; }
|       Translation LT Translation      { PrintDebug("...<..."); $$ = BoolGT($3,$1); $1=$3=NULL; }
|       Translation GE Translation      { PrintDebug("...>=..."); $$ = BoolGE($1,$3); $1=$3=NULL; }
|       Translation LE Translation      { PrintDebug("...<=..."); $$ = BoolGE($3,$1); $1=$3=NULL; }
|       EXISTS '(' CD ')'               { PrintDebug("Exists(...)");
                                          $$=Exists($3);
                                          if ($$==2) return 2;
                                          FreeCD($3); $3=NULL; }
|       EXISTSIN '(' Translation ',' CD ')' { PrintDebug("ExistsIn(..., ...)");
                                          $$=ExistsIn($3, $5);
                                          FreeTypeInst($3); $3 = NULL;
                                          FreeCD($5); $5 = NULL;
                                        }
|       CD PRECEDES CD                  { PrintDebug("...Precedes...");
                                          $$=Precedes($1,$3);
                                          if ($$==2) return 2;
                                          FreeCD($1); $1=NULL;
                                          FreeCD($3); $3=NULL; }
|       CD FOLLOWS CD                   { PrintDebug("...Follows...");
                                          $$=Precedes($3,$1);
                                          if ($$==2) return 2;
                                          FreeCD($3); $3=NULL;
                                          FreeCD($1); $1=NULL; 
                                        }
|       INTABLE '(' Translation ',' Translation ')' { PrintDebug("InTable(..., ...)");
                                          $$=InTable($3, $5);
                                          $3 = NULL;
                                          $5 = NULL;
                                        }
;

Translation :
        '(' Translation ')'             { PrintDebug("(...)");$$=$2; }
|       STRING
|       NUMERIC
|       YEAR                            {
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          $$=AllocTypeInst();
                                          sprintf(tempo,"%04d",1900+lt->tm_year);
                                          $$->str.str(tempo);
                                          $$->val=0;
                                        }
|       MONTH                           {
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          $$=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_mon+1);
                                          $$->str.str(tempo);
                                          $$->val=0;
                                        }
|       DAY                             {
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          $$=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_mday);
                                          $$->str.str(tempo);
                                          $$->val=0;
                                        }
|       HOUR                            {
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          $$=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_hour);
                                          $$->str.str(tempo);
                                          $$->val=0;
                                        }
|       MINUTE                          {
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          $$=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_min);
                                          $$->str.str(tempo);
                                          $$->val=0;
                                        }
|       SECOND                          {
                                          time_t ns;
                                          struct tm *lt;
                                          time(&ns);
                                          lt=localtime(&ns);
                                          $$=AllocTypeInst();
                                          sprintf(tempo,"%02d",lt->tm_sec);
                                          $$->str.str(tempo);
                                          $$->val=0;
                                        }
|       ORDINAL '(' Translation ')'     {
                                          int i,j;
                                          char tmp1[20];
                                          $$=AllocTypeInst();
                                          sprintf(tmp1,"%d",ordinal);
                                          for (j=0,i=strlen(tmp1);i<$3->val;++i,++j)
                                           tempo[j]='0';
                                          strcpy(&tempo[j],tmp1);
                                          $$->str.str(tempo);
                                          $$->val=0;
                                          FreeTypeInst($3);
                                          $3=NULL;
                                        }
|       NEXTSUB                         {
                                          PrintDebug("NextSub"); $$=NextSub(NULL, NULL);
                                        }
|       NEXTSUB '(' CD ')'              { 
                                          PrintDebug("NextSub(...)");
                                          $$=NextSub($3, NULL);
                                        }
|       NEXTSUB '(' CD ',' Translation ')' { 
                                          PrintDebug("NextSub(..., ...)");
                                          $$=NextSub($3, $5);
                                        }
|       NEXTSUBIN '(' Translation ',' CD ')' { 
                                          PrintDebug("NextSubIn(..., ...)");
                                          $$=NextSubIn($3, $5, NULL);
                                        }
|       NEXTSUBIN '(' Translation ',' CD ',' Translation ')' { 
                                          PrintDebug("NextSubIn(..., ..., ...)");
                                          $$=NextSubIn($3, $5, $7);
                                        }
|       PREVIOUSSUB                     {
                                          PrintDebug("PreviousSub"); $$=PreviousSub(NULL, NULL);
                                        }
|       PREVIOUSSUB '(' CD ')'          { 
                                          PrintDebug("PreviousSub(...)");
                                          $$=PreviousSub($3, NULL);
                                        }
|       PREVIOUSSUB '(' CD ',' Translation ')' { 
                                          PrintDebug("PreviousSub(..., ...)");
                                          $$=PreviousSub($3, $5);
                                        }
|       PREVIOUSSUBIN '(' Translation ',' CD ')' { 
                                          PrintDebug("PreviousSubIn(..., ...)");
                                          $$=PreviousSubIn($3, $5, NULL);
                                        }
|       PREVIOUSSUBIN '(' Translation ',' CD ',' Translation ')' { 
                                          PrintDebug("PreviousSubIn(..., ..., ...)");
                                          $$=PreviousSubIn($3, $5, $7);
                                        }
|       VAR_N                           { PrintDebug("N");CopyInst(&$$,N); }
|       VAR_NT                          { PrintDebug("NT");CopyInst(&$$,NT); }
|       VAR_NS                          { PrintDebug("NS");CopyInst(&$$,NS); }
|       VAR_NO                          { PrintDebug("NO");CopyInst(&$$,NO); }
|       VAR_NTO                         { PrintDebug("NTO"); CopyInst(&$$,NTO); }
|       VAR_NSO                         { PrintDebug("NSO");CopyInst(&$$,NSO); }
|       VAR_NEW                         { PrintDebug("NEW");CopyInst(&$$,NEW); }
|       VAR_NEWEST                      { PrintDebug("LAST");CopyInst(&$$,NEWEST); }
|       VARS                            { PrintDebug("S");CopyInst(&$$,S); }
|       VARD                            { PrintDebug("S");CopyInst(&$$,D); }
|       CD                              { PrintDebug("CD");
                                          $$=AllocTypeInst();
                                          typestr ptr = ReadCD($1);
                                          if (!ptr.str()) return 2;
                                          $$->str = ptr;
                                          $$->val=0;
                                          FreeCD($1);
                                        }
|       Translation PLUS Translation    { PrintDebug("...+...");$$=Add($1,$3); $1=$3=NULL; }
|       Translation MINUS Translation   { PrintDebug("...-...");$$=Subtract($1,$3); $1=$3=NULL; }
|       Translation MULTIPLY Translation { PrintDebug("...*...");$$=Multiply($1,$3); $1=$3=NULL; }
|       Translation DIVIDE Translation  { PrintDebug("...:...");$$=Divide($1,$3); $1=$3=NULL; }
|       STR '(' Translation ')'         { PrintDebug("Str(...)");$$=String($3); }
|       VAL '(' Translation ')'         { PrintDebug("Val(...)");$$=Value($3); }
|       LEN '(' Translation ')'         { PrintDebug("Len(...)");$$=Len($3); }
|       UPPER '(' Translation ')'       { PrintDebug("Upper(...)");$$=Upper($3); }
|       LOWER '(' Translation ')'       { PrintDebug("Lower(...)");$$=Lower($3); }
|       STO '(' Translation ')'         { PrintDebug("Sto(...)");CopyInst(&$$,S); MemSto($3); $3=NULL; }
|       STO '(' Translation ',' Translation ')' { PrintDebug("Sto(...)"); MemSto($3, $5); $3=$5=NULL; }
|       MEM '(' Translation ')'         { PrintDebug("Mem(...)");$$=MemMem($3); $3=NULL; }
|       EXC '(' Translation ')'         { PrintDebug("Exc(...)");$$=MemExc($3); $3=NULL; }
|       CLR '(' Translation ')'         { PrintDebug("Clr(...)");CopyInst(&$$,S); MemClr($3); $3=NULL; }
|       FROM '(' Translation ')'        { PrintDebug("From(...)");$$=From($3,0); $3=NULL; }
|       FROM '(' Translation ',' _STRICT ')'
                                        { PrintDebug("From(...,_STRICT)");$$=From($3,1); $3=NULL; }
|       TO '(' Translation ')'          { PrintDebug("To(...)");$$=To($3,0); $3=NULL; }
|       TO '(' Translation ',' _STRICT ')'
                                        { PrintDebug("To(...,_STRICT)");$$=To($3,1); $3=NULL; }
|       BETWEEN '(' Translation ',' Translation ')'
                                        { PrintDebug("Between(...)");$$=Between($3,$5,0); $3=$5=NULL; }
|       BETWEEN '(' Translation ',' Translation ',' _STRICT ')'
                                        { PrintDebug("Between(...,_STRICT)");$$=Between($3,$5,1); $3=$5=NULL; }
|       _DELETE '(' Translation ')'     { PrintDebug("Delete(...)");$$=Replace($3, NULL, SP_ANY, 0); $3=NULL; }
|       _DELETE '(' Translation ',' AT BEGINING ')'
                                        { PrintDebug("Delete(..., AT BEGINING)");$$=Replace($3, NULL, SP_BEGINNING, false); $3=NULL; }
|       _DELETE '(' Translation ',' AT BEGINNING ')'
                                        { PrintDebug("Delete(..., AT BEGINNING)");$$=Replace($3, NULL, SP_BEGINNING, false); $3=NULL; }
|       _DELETE '(' Translation ',' AT END ')'
                                        { PrintDebug("Delete(..., AT END)");$$=Replace($3, NULL, SP_END, false); $3=NULL; }
|       _DELETE '(' Translation ',' AT BOTH ')'
                                        { PrintDebug("Delete(...,EVERYWHERE)");$$=Replace($3, NULL, SP_BOTH, false); $3=NULL; }
|       _DELETE '(' Translation ',' _STRICT ')' { PrintDebug("Delete(...)");$$=Replace($3, NULL, SP_ANY, true); $3=NULL; }
|       _DELETE '(' Translation ',' AT BEGINING ',' _STRICT ')'
                                        { PrintDebug("Delete(..., AT BEGINING)");$$=Replace($3, NULL, SP_BEGINNING, true); $3=NULL; }
|       _DELETE '(' Translation ',' AT BEGINNING ',' _STRICT ')'
                                        { PrintDebug("Delete(..., AT BEGINNING)");$$=Replace($3, NULL, SP_BEGINNING, true); $3=NULL; }
|       _DELETE '(' Translation ',' AT END ',' _STRICT ')'
                                        { PrintDebug("Delete(..., AT END)");$$=Replace($3, NULL, SP_END, true); $3=NULL; }
|       _DELETE '(' Translation ',' AT BOTH ',' _STRICT ')'
                                        { PrintDebug("replace(...,AT BOTH)");$$=Replace($3, NULL, SP_BOTH, true); $3=NULL; }
|       REPLACE '(' Translation BY Translation ')'      { PrintDebug("replace(...)");$$=Replace($3, $5, SP_ANY, false); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' AT BEGINING ')'
                                        { PrintDebug("replace(..., AT BEGINING)");$$=Replace($3, $5, SP_BEGINNING, false); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' AT BEGINNING ')'
                                        { PrintDebug("replace(..., AT BEGINNING)");$$=Replace($3, $5, SP_BEGINNING, false); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' AT END ')'
                                        { PrintDebug("replace(..., AT END)");$$=Replace($3, $5, SP_END, false); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' AT BOTH ')'
                                        { PrintDebug("replace(...,EVERYWHERE)");$$=Replace($3, $5, SP_BOTH, false); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' _STRICT ')'
                                        { PrintDebug("replace(...)");$$=Replace($3, $5, SP_ANY, true); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' AT BEGINING ',' _STRICT ')'
                                        { PrintDebug("replace(..., AT BEGINING)");$$=Replace($3, $5, SP_BEGINNING, true); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' AT BEGINNING ',' _STRICT ')'
                                        { PrintDebug("replace(..., AT BEGINNING)");$$=Replace($3, $5, SP_BEGINNING, true); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' AT END ',' _STRICT ')'
                                        { PrintDebug("replace(..., AT END)");$$=Replace($3, $5, SP_END, true); $3=$5=NULL; }
|       REPLACE '(' Translation BY Translation ',' AT BOTH ',' _STRICT ')'
                                        { PrintDebug("replace(...,EVERYWHERE)");$$=Replace($3, $5, SP_BOTH, true); $3=$5=NULL; }
|       REPLACEOCC '(' Translation BY Translation ',' Translation ')'
                                        { PrintDebug("replace(...,Occ)");$$=ReplaceOcc($3, $5, $7, false); $3=$5=$7=NULL; }
|       REPLACEOCC '(' Translation BY Translation ',' Translation ',' _STRICT ')'
                                        { PrintDebug("replace(...,Occ)");$$=ReplaceOcc($3, $5, $7, true); $3=$5=$7=NULL; }
|       BFIRST '(' Translation ')'      { PrintDebug("BFirst(...)");$$=BFirst($3,0); $3=NULL; }
|       BFIRST '(' WNUMBER ')'          { PrintDebug("BFirst(Number)");$$=BFirst(NULL,1); }
|       BFIRST '(' WSTRING ')'          { PrintDebug("BFirst(String)");$$=BFirst(NULL,2); }
|       EFIRST '(' Translation ')'      { PrintDebug("EFirst(...)");$$=EFirst($3,0); $3=NULL; }
|       EFIRST '(' WNUMBER ')'          { PrintDebug("EFirst(Number)");$$=EFirst(NULL,1); }
|       EFIRST '(' WSTRING ')'          { PrintDebug("EFirst(String)");$$=EFirst(NULL,2); }
|       BLAST '(' Translation ')'       { PrintDebug("BLast(...)");$$=BLast($3,0); $3=NULL; }
|       BLAST '(' WNUMBER ')'           { PrintDebug("BLast(Number)");$$=BLast(NULL,1); }
|       BLAST '(' WSTRING ')'           { PrintDebug("BLast(String)");$$=BLast(NULL,2); }
|       ELAST '(' Translation ')'       { PrintDebug("ELast(...)");$$=ELast($3,0); $3=NULL; }
|       ELAST '(' WNUMBER ')'           { PrintDebug("ELast(Number)");$$=ELast(NULL,1); }
|       ELAST '(' WSTRING ')'           { PrintDebug("ELast(String)");$$=ELast(NULL,2); }
|       NEXT '(' CD ')'                 { PrintDebug("Next(...)");$$=Next_($3,NULL,0); $3=NULL; }
|       NEXT '(' CD ',' CD ')'          { PrintDebug("Next(...,...)");
                                          $$=Next_($3,$5,0);
                                          if (!$$) return 2;
                                          $3=$5=NULL; }
|       NEXT '(' CD ',' _STRICT ')'     { PrintDebug("Next(...,_STRICT)");
                                          $$=Next_($3,NULL,1);
                                          if (!$$) return 2;
                                          $3=NULL; }
|       NEXT '(' CD ',' CD ',' _STRICT ')'
                                        { PrintDebug("Next(...,...,_STRICT)");
                                          $$=Next_($3,$5,1);
                                          if (!$$) return 2;
                                          $3=$5=NULL; }
|       LAST '(' CD ')'                 { PrintDebug("Last(...)");
                                          $$=Last_($3,NULL,0);
                                          if (!$$) return 2;
                                          $3=NULL; }
|       LAST '(' CD ',' CD ')'          { PrintDebug("Last(...,...)");
                                          $$=Last_($3,$5,0);
                                          if (!$$) return 2;
                                          $3=$5=NULL; }
|       LAST '(' CD ',' _STRICT ')'     { PrintDebug("Last(...,_STRICT)");
                                          $$=Last_($3,NULL,1);
                                          if (!$$) return 2;
                                          $3=NULL; }
|       LAST '(' CD ',' CD ',' _STRICT ')'
                                        { PrintDebug("Last(...,...,_STRICT)");
                                          $$=Last_($3,$5,1);
                                          if (!$$) return 2;
                                          $3=$5=NULL; }
|       SORT '(' STRING ')'     { PrintDebug("Sort");
                                  MustSort($3->str.str());
                                  FreeTypeInst($3);
                                  $3=NULL;
                                  return 2;
                                }
|       TABLE '(' STRING ')'            { PrintDebug("Table(...)");$$=Table_($3); $3=NULL; }
|       REGFINDNUM '(' Translation ')'  { PrintDebug("RegFindNum(...)");$$=RegFindNum($3, NULL); $3=NULL; }
|       REGFINDNUM '(' Translation ',' Translation ')'     { PrintDebug("RegFindNum(...,...)");$$=RegFindNum($3,$5); $3=$5=NULL; }
|       REGFINDPOS '(' Translation ')'  { PrintDebug("RegFindPos(...)");$$=RegFindPos($3, NULL); $3=NULL; }
|       REGFINDPOS '(' Translation ',' Translation ')'     { PrintDebug("RegFindPos(...,...)");$$=RegFindPos($3,$5); $3=$5=NULL; }
|       REGFIND '(' Translation ')'     { PrintDebug("RegFind(...)");$$=RegFind($3, NULL); $3=NULL; }
|       REGFIND '(' Translation ',' Translation ')'     { PrintDebug("RegFind(...,...)");$$=RegFind($3,$5); $3=$5=NULL; }
|       REGMATCH '(' Translation ')'    { PrintDebug("RegMatch(...)");$$=RegMatch($3); $3=NULL; }
|       REGREPLACE '(' Translation ',' Translation ')'
                                        { PrintDebug("RegReplace(...,...)");$$=RegReplace($3,$5,NULL); $3=$5=NULL; }
|       REGREPLACE '(' Translation ',' Translation ',' Translation ')'
                                        { PrintDebug("RegReplace(...,...,...)");$$=RegReplace($3,$5,$7); $3=$5=$7=NULL; }
|       REGREPLACETABLE '(' Translation ')'
                                        { PrintDebug("RegReplaceTable(...,...)");$$=RegReplaceTable($3,NULL); $3=NULL; }
|       REGREPLACETABLE '(' Translation ',' Translation ')'
                                        { PrintDebug("RegReplaceTable(...,...)");$$=RegReplaceTable($3,$5); $3=$5=NULL; }
|       MOVEBEFORE '(' Translation ',' CD ')' { PrintDebug("MoveBefore(..., ...)");
                                          $$=MoveBefore($3, $5, NULL, NULL, NULL, NULL, NULL); 
                                          $3=NULL; 
                                          $5=NULL;
                                        }
|       MOVEBEFORE '(' Translation ',' CD ',' Translation ')' { PrintDebug("MoveBefore(..., ..., ...)");
                                          $$=MoveBefore($3, $5, $7, NULL, NULL, NULL, NULL); 
                                          $3=$7=NULL; 
                                          $5=NULL;
                                        }
|       MOVEBEFORE '(' Translation ',' CD ',' Translation ',' Translation ',' Translation ')' { PrintDebug("MoveBefore(..., ..., ..., ..., ...)");
                                          $$=MoveBefore($3, $5, $7, $9, $11, NULL, NULL); 
                                          $3=$7=$9=$11=NULL; 
                                          $5=NULL;
                                        }
|       MOVEBEFORE '(' Translation ',' CD ',' Translation ',' Translation ',' 
            Translation ',' Translation ')' { PrintDebug("MoveBefore(..., ..., ..., ..., ..., ...)");
                                          $$=MoveBefore($3, $5, $7, $9, $11, $13, NULL); 
                                          $3=$7=$9=$11=$13=NULL; 
                                          $5=NULL;
                                        }
|       MOVEBEFORE '(' Translation ',' CD ',' Translation ',' Translation ',' 
            Translation ',' Translation ',' Translation ')' { PrintDebug("MoveBefore(..., ..., ..., ..., ..., ..., ...)");
                                          $$=MoveBefore($3, $5, $7, $9, $11, $13, $15); 
                                          $3=$7=$9=$11=$13=$15=NULL; 
                                          $5=NULL;
                                        }
|       MOVEAFTER '(' Translation ',' CD ',' Translation ')' { PrintDebug("MoveAfter(..., ..., ...)");
                                          $$=MoveAfter($3, $5, $7, NULL, NULL, NULL, NULL); 
                                          $3=$7=NULL; 
                                          $5=NULL;
                                        }
|       MOVEAFTER '(' Translation ',' CD ')' { PrintDebug("MoveAfter(..., ...)");
                                          $$=MoveAfter($3, $5, NULL, NULL, NULL, NULL, NULL); 
                                          $3=NULL; 
                                          $5=NULL;
                                        }
|       MOVEAFTER '(' Translation ',' CD ',' Translation ',' Translation ',' Translation ')' { PrintDebug("MoveAfter(..., ..., ..., ..., ...)");
                                          $$=MoveAfter($3, $5, $7, $9, $11, NULL, NULL); 
                                          $3=$7=$9=$11=NULL; 
                                          $5=NULL;
                                        }
|       MOVEAFTER '(' Translation ',' CD ',' Translation ',' Translation ',' 
            Translation ',' Translation ')' { PrintDebug("MoveAfter(..., ..., ..., ..., ..., ...)");
                                          $$=MoveAfter($3, $5, $7, $9, $11, $13, NULL); 
                                          $3=$7=$9=$11=$13=NULL; 
                                          $5=NULL;
                                        }
|       MOVEAFTER '(' Translation ',' CD ',' Translation ',' Translation ',' 
            Translation ',' Translation ',' Translation ')' { PrintDebug("MoveAfter(..., ..., ..., ..., ..., ..., ...)");
                                          $$=MoveAfter($3, $5, $7, $9, $11, $13, $15); 
                                          $3=$7=$9=$11=$13=$15=NULL; 
                                          $5=NULL;
                                        }
;

%%
