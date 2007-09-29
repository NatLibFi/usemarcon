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

#include "typedef.h"
#include "FlexLexer.h"
#include "mempool.h"
#include "pcre.h"

// forward declarations
class SortElem;
class TRuleDoc;
class TUMRecord;
class TRule;
class TEvaluateRule;
class TCD;
class TCDLib;

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
        mRegExpResult(0)
    {
        debug_rule = 0;
        itsScanner.SetAllocator(&m_allocator);
    }

    virtual ~TEvaluateRule      ()
    {
    }

    int   Init_Evaluate_Rule(void *Doc,TRuleDoc *RDoc,TError *ErrorHandler,
            int dbg_rule, unsigned long ord, bool UTF8Mode);
    int   Evaluate_Rule(TUMRecord* In, TUMRecord* Out, TUMRecord* RealOut, TRule* Rule, TCDLib *ProcessCDL = NULL);
    int   End_Evaluate_Rule();

    int   SortRecord(TUMRecord* aRecord);

    virtual int yylex();
    virtual void yyerror(char *m);

private:
    TMarcScannerImpl itsScanner;

    MemoryPool m_allocator;

    TUMRecord* InputRecord, *OutputRecord, *RealOutputRecord;
    TCDLib*    InputCDL;
    TRule*     CurrentRule;
    TRuleDoc*  RuleDoc;
    SortElem*  ListSort;
    SortElem*  LastSort;
    TError*    itsErrorHandler;
    bool       itsUTF8Mode;

    typestr    mRegExpSearchString;
    int        mRegExpMatchVector[30];
    int        mRegExpResult;

    int Eval;
    int Error;
    char *RedoStr;
    char *MainInput;
    int AfterRedo;
    
    virtual int Precedes(TypeCD *, TypeCD *);
    virtual int Exists(TypeCD *);
    virtual int ExistsIn(TypeInst* a_str, TypeCD* a_cd);
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

    bool RegFindInternal(const char *a_str, const char *a_regexp);
    bool RegReplaceInternal(typestr &a_str, const char *a_regexp, const char *a_replacement, bool a_global);

    bool move_subfields(typestr &a_fielddata, TypeInst* a_source, TypeCD* a_new_pos, bool a_after, 
        TypeInst* a_target, TypeInst* a_prefix, TypeInst* a_suffix, TypeInst* a_preserved_punctuations,
        TypeInst* a_preserved_subfields);

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
    virtual TypeInst* NextSubIn(TypeInst* aStr, TypeCD* aFindCD, TypeInst* aOccurrence);

    /*
    PREVIOUSSUB
    */
    virtual TypeInst* PreviousSub(TypeCD* aFindCD, TypeInst *aOccurrence);
    virtual TypeInst* PreviousSubIn(TypeInst* aStr, TypeCD* aFindCD, TypeInst* aOccurrence);

    /*
        Instruction - Instruction
    */
    virtual TypeInst* Subtract( TypeInst* t1, TypeInst* t2 );

    /*
    Instruction * Instruction
    */
    virtual TypeInst* Multiply( TypeInst* t1, TypeInst* t2 );

    /*
    Instruction : Instruction
    */
    virtual TypeInst* Divide( TypeInst* t1, TypeInst* t2 );

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

    virtual void FreeTypeInst( TypeInst* t );

    virtual int CopyInst( TypeInst** In, TypeInst* From );

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
    virtual TypeInst* Add( TypeInst* t1, TypeInst* t2 );

    /*
        200(1)
    */
    virtual TypeInst* AddOcc( TypeInst* t1, TypeInst* t2 );

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
    virtual TypeInst* Replace( TypeInst* t1, TypeInst* t2, IN_STR_POSITION at, bool strict );

    /*
        REPLACEOCC( translation BY translation , OCCURENCE  [, STRICT] )
    */
    virtual TypeInst* ReplaceOcc( TypeInst* t1, TypeInst* t2, TypeInst* inCondOcc, bool strict );

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

    virtual TypeInst* RegFindNum( TypeInst* t1, TypeInst* t2 );
    virtual TypeInst* RegFindPos( TypeInst* t1, TypeInst* t2 );
    virtual TypeInst* RegFind( TypeInst* t1, TypeInst* t2 );

    /*
    RegMatch( translation )
    */
    virtual TypeInst* RegMatch( TypeInst* t1 );

    virtual TypeInst* RegReplace(TypeInst* a_regexp, TypeInst* a_replacement, TypeInst* a_options);

    virtual TypeInst* RegReplaceTable(TypeInst* a_table, TypeInst* a_options);

    virtual TypeInst* MoveBefore(TypeInst* a_source, TypeCD* a_before, TypeInst* a_target, 
        TypeInst* a_prefix, TypeInst* a_suffix, TypeInst* a_preserved_punctuations, TypeInst* a_preserved_subfields);

    virtual TypeInst* MoveAfter(TypeInst* a_source, TypeCD* a_after, TypeInst* a_target, 
        TypeInst* a_prefix, TypeInst* a_suffix, TypeInst* a_preserved_punctuations, TypeInst* a_preserved_subfields);

    /*
    InTable( translation, translation )
    */
    virtual int InTable( TypeInst* t1, TypeInst* t2 );

};
#endif

