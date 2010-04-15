/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tfile.h
 *
 *
 
CLASS
    TFile

    implements a class to manage file input and output

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2010

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _TFILE_H_
#define _TFILE_H_

#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#ifdef _WIN32
#include <io.h>
#else
  #include <unistd.h>
#endif
#include "statemanager.h"
#include "typedef.h"

#define FILE_READ   'r'
#define FILE_WRITE  'w'
#define FILE_APPEND 'a'
#define FILE_ASCII  'A'
#define FILE_BINARY 'B'

class TFile
{
public:
    TFile(const typestr & FileInfo, TStateManager *StateManager, char Mode=FILE_READ, char Kind=FILE_ASCII);
    virtual ~TFile();
    virtual int          Open(void);
    virtual int          Close(void);

    bool         Exists(void);
    int          NextLine(typestr *aLine, bool aStringFile, typestr *Spec=NULL, int *LineNumber=NULL);
    typestr      GetName(void);
    void         SetName(typestr &aName);
    char         GetMode(void);
    void         SetMode(char aMode);
    char         GetKind(void);
    void         SetKind(char aKind);
    virtual long GetSize(void);
    virtual long GetPos(void);

protected:
    int          itsLineNumber;
    typestr      itsFileInfo;
    char         itsMode;
    char         itsKind;
    FILE         *File;
    int          iFile;
    TFile        *Included;
    TStateManager       *mStateManager;

    int          SkipBeginning();
};

#endif
