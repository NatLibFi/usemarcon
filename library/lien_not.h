/*
 *  USEMARCON Software - Library version
 *
 *  File:  lien_not.h
 *
 *
 
CLASS
    TMarcScannerImpl

    implements an interface to the lexical analysis

CLASS
    TEvaluateRule

    implements an class to evaluate rules

OVERVIEW TEXT
    USEMARCON Software - Library version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2007

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _TEVALUATERULE_FILE_H_
#define _TEVALUATERULE_FILE_H_

#include <ctype.h>
#include "typedef.h"
#include "FlexLexer.h"
#include "mempool.h"
//#include "tools.h"
#include "regexp.h"

// forward declarations
class SortElem;
class TRuleDoc;
class TUMRecord;
class TRule;
class TEvaluateRule;

class TMarcScannerImpl : public yyFlexLexer
{
private:
    long itsBufferLen;
    long itsBufferPos;
    char *itsBuffer;
    bool itsFirstTime;
    MemoryPool *m_allocator;
    int mLine;

protected:
    virtual int LexerInput(char *buf,int max_size);
public:
    TMarcScannerImpl() : itsBufferLen(0), itsBufferPos(0), itsBuffer(NULL), itsFirstTime(true), mLine(0)
    {
    }
    ~TMarcScannerImpl()
    {
        if (itsBuffer)
            free(itsBuffer);
    }

    void SetRule(TRule *Rule);
    void RewindBuffer();
    void SetAllocator(MemoryPool *mp) { m_allocator = mp; }
    const char *getCurrentLineContents() { return itsBuffer ? itsBuffer : ""; }
    int getCurrentLineNo() { return mLine; }
};

class TEvaluateRule : public MarcParser
{
public:
    TEvaluateRule       () : ListSort(NULL), RedoStr(NULL), MainInput(NULL), AfterRedo(0),
        Fini(0)
    {
        debug_rule = 0;
        itsScanner.SetAllocator(&m_allocator);
    }

    virtual ~TEvaluateRule      ()
    {
    }

    int   Init_Evaluate_Rule(void *Doc,TRuleDoc *RDoc,TError *ErrorHandler,
            int dbg_rule, unsigned long ord, bool UTF8Mode);
    int   Evaluate_Rule(TUMRecord* In,TUMRecord* Out,TRule* Rule, TCDLib *ProcessCDL = NULL);
    int   End_Evaluate_Rule();

    int   SortRecord(TUMRecord* aRecord);

    virtual int yylex();
    virtual void yyerror(char *m);

private:
    TMarcScannerImpl itsScanner;

    MemoryPool m_allocator;

    TUMRecord* InputRecord, *OutputRecord;
    TCDLib*    InputCDL;
    TCD*       theCDOut;
    TRule*     CurrentRule;
    TRuleDoc*  RuleDoc;
    SortElem*  ListSort;
    SortElem*  LastSort;
    TError*    itsErrorHandler;
    bool       itsUTF8Mode;

    CRegExp    itsRegExp;

    char TmpRule[80];

    int Eval;
    int Error;
    char *RedoStr;
    char *MainInput;
    int AfterRedo;
    int Fini;

    virtual int Precedes(TypeCD *, TypeCD *);
    virtual int Exists(TypeCD *);
    virtual typestr ReadCD(TypeCD *);
    virtual typestr Table(char *Nom, char *str);

    const char* NextBalise();
    const char* PreviousBalise();
    typestr NextSubField(TypeCD *, TypeCD *);
    typestr PreviousSubField(TypeCD *, TypeCD *);
    bool CompareOccurrence(TypeInst *aCondition, int aOccurrence);


    int   IsConcat(char *lib);
    int   Replace_N_NT_NS(int val,int N,int NT,int NS);
    void  ResetRedo();


    void  FinishCD(TypeCD* aCD);
    void  FinishTCD(TypeCD* aCD);
    void  PrCD(TypeCD* CD);
    void  ResetSort();
    virtual int   MustSort(char* n);

    virtual TypeCD* AllocCD();
    virtual void FreeCD( TypeCD* CD );
    virtual void PrintDebug(const char *s);

    /*
    Affichage d'une valeur d' Instruction
    */
    char* PrintT( TypeInst* t, char* buf );

    /*
    NEXT ( subfield [,subfield] [,STRICT] )
    */
    virtual TypeInst* Next_( TypeCD* cd1, TypeCD* cd2, int strict );

    /*
        LAST ( subfield [,subfield] [,STRICT] )
    */
    virtual TypeInst* Last_( TypeCD* cd1, TypeCD* cd2, int strict );

    /*
    NEXTSUB
    */
    virtual TypeInst* NextSub(TypeCD* aFindCD, TypeInst *aOccurrence);

    /*
    PREVIOUSSUB
    */
    virtual TypeInst* PreviousSub(TypeCD* aFindCD, TypeInst *aOccurrence);

    /*
        Instruction - Instruction
    */
    virtual TypeInst* Soust( TypeInst* t1, TypeInst* t2 );

    /*
    Instruction * Instruction
    */
    virtual TypeInst* Multi( TypeInst* t1, TypeInst* t2 );

    /*
    Instruction : Instruction
    */
    virtual TypeInst* Divis( TypeInst* t1, TypeInst* t2 );

    /*
    VAL( Instruction )
    */
    virtual TypeInst* Value( TypeInst* t );

    /*
        STO(i)
    */
    virtual int MemSto( TypeInst* n );

    /*
        MEM(i)
    */
    virtual TypeInst* MemMem( TypeInst* n );

    /*
        CLR(i)
    */
    virtual int MemClr( TypeInst* n  );

    /*
        EXC(i)
    */
    virtual TypeInst* MemExc( TypeInst* n );

    virtual TypeInst* AllocTypeInst();

    /*
    Liberation de la memoire associe a une Instruction
    */
    virtual void FreeTypeInst( TypeInst* t );

    /*
    Copie d'instruction
    */
    virtual int Copie( TypeInst** In, TypeInst* From );

    /*
        Comparaison de deux instructions
    */
    virtual int BoolEQ( TypeInst* t1, TypeInst* t2 );

    /*
        Comparaison de contenance de deux instructions
    */
    virtual int BoolIn( TypeInst* t1, TypeInst* t2 );

    /*
        Comparaison de deux instructions
    */
    virtual int BoolGT( TypeInst* t1, TypeInst* t2 );

    /*
        Comparaison de deux instructions
    */
    virtual int BoolGE( TypeInst* t1, TypeInst* t2 );

    /*
        Instruction + Instruction
    */
    virtual TypeInst* Ajout( TypeInst* t1, TypeInst* t2 );

    /*
        200(1)
    */
    virtual TypeInst* AjoutOcc( TypeInst* t1, TypeInst* t2 );

    /*
        Conversion de numerique en char* si necessaire pour une Instruction
    */
    virtual char* ToString( TypeInst* t );

    /*
        STR( Instruction )
    */
    virtual TypeInst* String( TypeInst* t );

    /*
        UPPER( Instruction )
    */
    virtual TypeInst* Upper( TypeInst* t );

    /*
        LOWER( Instruction )
    */
    virtual TypeInst* Lower( TypeInst* t );

    /*
        LEN( Instruction )
    */
    virtual TypeInst* Len( TypeInst* t );

    /*
        FROM( translation [, STRICT] )
    */
    virtual TypeInst* From( TypeInst* t, int strict );

    /*
        TO( translation [, STRICT] )
    */
    virtual TypeInst* To( TypeInst* t, int strict );

    /*
        BETWEEN( translation , translation [, STRICT] )
    */
    virtual TypeInst* Between( TypeInst* t1, TypeInst* t2, int strict );

    /*
        REPLACE( translation BY translation [, AT ...] [, STRICT] )
    */
    virtual TypeInst* Replace( TypeInst* t1, TypeInst* t2, int at, int strict );

    /*
        REPLACEOCC( translation BY translation , OCCURENCE  [, STRICT] )
    */
    virtual TypeInst* ReplaceOcc( TypeInst* t1, TypeInst* t2, TypeInst* inCondOcc, int strict );

    virtual int IsDigit( char c );

    /*
        BFIRST( translation | ... )
    */
    virtual TypeInst* BFirst( TypeInst* t, int k );

    /*
        EFIRST( translation | ... )
    */
    virtual TypeInst* EFirst( TypeInst* t, int k );

    /*
        BLAST( translation | ... )
    */
    virtual TypeInst* BLast( TypeInst* t, int k );

    /*
        ELAST( translation | ... )
    */
    virtual TypeInst* ELast( TypeInst* t, int k );

    /*
        TABLE ( subfield [,subfield] [,STRICT] )
    */
    virtual TypeInst* Table_( TypeInst* Nom );

    /*
    RegFind( translation, translation )
    */
    virtual TypeInst* RegFind( TypeInst* t1, TypeInst* t2 );

    /*
    RegMatch( translation )
    */
    virtual TypeInst* RegMatch( TypeInst* t1 );

    /*
    RegReplace( translation, translation, translation )
    */
    virtual TypeInst* RegReplace( TypeInst* t1, TypeInst* t2, TypeInst* t3 );

    /*
    InTable( translation, translation )
    */
    virtual int InTable( TypeInst* t1, TypeInst* t2 );

};
#endif

