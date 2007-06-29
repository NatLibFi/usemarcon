/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tmarcdoc.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TMarcDoc

    implements a class to manage MARC files

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/

#ifndef TMarcDoc_H
#define TMarcDoc_H

#include "marcfile.h"
#include "umrecord.h"
#include "tagnoind.h"
#include "defines.h"
#include "ttrnsdoc.h"

#define STATUS_OK               1
#define LAST_PADDED             1

// forward declaration
class TRuleDoc;
class TTransDoc;
class TMarcFile;
class TUMRecord;

class TMarcDoc
{
public:
    TMarcDoc   (TUMApplication *Application);
    virtual ~TMarcDoc  ();

    virtual bool    Open                 (int IO);
    virtual bool    Close                (int IO);
    void            Reset                (void);

    virtual int     Convert              (TRuleDoc *aRuleDoc);
    virtual int     Transcode            (TTransDoc *aTransDoc);
    virtual int     LoadTagNoInd         (int IO,char *Path);
    virtual int     UnloadTagNoInd       (int IO);

    virtual void    DelTreeTagNoInd      (TTagNoInd *Start);

    virtual void    SetMarcInputFileSpec        (FILE_SPEC *theSpec );
    virtual void    SetMarcInputFileFormat      (MARC_FILE_FORMAT theFormat) { itsMarcInputFile.Format=theFormat; };
    virtual void    SetMarcInputFileBlockSize   (short theBlockSize) { itsMarcInputFile.BlockSize=theBlockSize; };
    virtual void    SetMarcInputFileMinDataFree (short theMinDataFree) { itsMarcInputFile.MinDataFree=theMinDataFree; };
    virtual void    SetMarcInputFilePaddingChar (char thePaddingChar) { itsMarcInputFile.PaddingChar=thePaddingChar; };
    virtual void    SetMarcInputFileLastBlock   (bool theLastBlock) { itsMarcInputFile.LastBlock=theLastBlock; };
    virtual void    SetConfInputFileSpec        (FILE_SPEC *theSpec);

    virtual void    SetMarcOutputFileSpec       (FILE_SPEC *theSpec );
    virtual void    SetMarcOutputFileFormat     (MARC_FILE_FORMAT theFormat) { itsMarcOutputFile.Format=theFormat; };
    virtual void    SetMarcOutputFileBlockSize  (short theBlockSize) { itsMarcOutputFile.BlockSize=theBlockSize; };
    virtual void    SetMarcOutputFileMinDataFree(short theMinDataFree) { itsMarcOutputFile.MinDataFree=theMinDataFree; };
    virtual void    SetMarcOutputFilePaddingChar(char thePaddingChar) { itsMarcOutputFile.PaddingChar=thePaddingChar; };
    virtual void    SetMarcOutputFileLastBlock  (bool theLastBlock) { itsMarcOutputFile.LastBlock=theLastBlock; };
    virtual void    SetConfOutputFileSpec       (FILE_SPEC *theSpec);

    virtual void    SetFirstTagNoInd     (int IO,TTagNoInd *aTagNoInd);
    virtual void    SetIndSeparatorsID              (int IO,long ID);

    virtual TMarcFile     *GetInputFile     (void) { return itsInputFile; };
    virtual TMarcFile     *GetOutputFile    (void) { return itsOutputFile; };
    virtual TUMRecord     *GetInputRecord   (void) { return itsInputRecord; };
    virtual TUMRecord     *GetOutputRecord  (void) { return itsOutputRecord; };
    virtual TUMRecord     *GetTransRecord   (void) { return itsTransRecord; };

    virtual long          GetIndSeparatorsID  (int IO);

    virtual FILE_SPEC     *GetMarcInputFileSpec        (void) { return itsMarcInputFile.Spec; };
    virtual MARC_FILE_FORMAT GetMarcInputFileFormat       (void) { return itsMarcInputFile.Format; };
    virtual short         GetMarcInputFileBlockSize    (void) { return itsMarcInputFile.BlockSize; };
    virtual short         GetMarcInputFileMinDataFree  (void) { return itsMarcInputFile.MinDataFree; };
    virtual char          GetMarcInputFilePaddingChar  (void) { return itsMarcInputFile.PaddingChar; };
    virtual bool          GetMarcInputFileLastBlock    (void) { return itsMarcInputFile.LastBlock; };
    virtual FILE_SPEC     *GetConfInputSpec            (void) { return itsConfInputSpec; }
    virtual FILE_SPEC     *GetMarcInputSpec            (void) { return itsMarcInputSpec; };

    virtual FILE_SPEC     *GetMarcOutputFileSpec       (void) { return itsMarcOutputFile.Spec; };
    virtual MARC_FILE_FORMAT GetMarcOutputFileFormat      (void) { return itsMarcOutputFile.Format; };
    virtual short         GetMarcOutputFileBlockSize   (void) { return itsMarcOutputFile.BlockSize; };
    virtual short         GetMarcOutputFileMinDataFree (void) { return itsMarcOutputFile.MinDataFree; };
    virtual char          GetMarcOutputFilePaddingChar (void) { return itsMarcOutputFile.PaddingChar; };
    virtual bool          GetMarcOutputFileLastBlock   (void) { return itsMarcOutputFile.LastBlock; };
    virtual FILE_SPEC     *GetConfOutputSpec           (void) { return itsConfOutputSpec; }
    virtual FILE_SPEC     *GetMarcOutputSpec           (void) { return itsMarcOutputSpec; };

    virtual TTagNoInd     *GetFirstTagNoInd       (int IO);

    virtual int           Read                    (void);
    virtual int           Write                   (void);

private:
    typedef struct
    {
        FILE_SPEC                   *Spec;
        FILE_SPEC                   *Conf;
        MARC_FILE_FORMAT            Format;
        short                       BlockSize;
        short                       MinDataFree;
        char                        PaddingChar;
        bool                        LastBlock;
    } MARCFILE;

    TUMRecord                     *itsInputRecord;
    TUMRecord                     *itsTransRecord;
    TUMRecord                     *itsOutputRecord;
    TMarcFile                     *itsInputFile;
    TMarcFile                     *itsOutputFile;
    FILE_SPEC                     *itsMarcInputSpec;
    FILE_SPEC                     *itsMarcOutputSpec;
    FILE_SPEC                     *itsConfInputSpec;
    FILE_SPEC                     *itsConfOutputSpec;
    FILE_SPEC                     *itsXVTFilePointer;
    TTagNoInd                     *itsFirstInputTagNoInd;
    TTagNoInd                     *itsFirstOutputTagNoInd;
    MARCFILE itsMarcInputFile;
    MARCFILE itsMarcOutputFile;

    TUMApplication                *itsApplication;
    TError                        *itsErrorHandler;
};

#endif
