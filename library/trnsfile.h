/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  trnsfile.h
 *
 *
 
CLASS
    TTransFile

    implements a class to manage transcoding file

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
#ifndef _TTRANS_FILE_H_
#define _TTRANS_FILE_H_

#include "tfile.h"
#include "semi.h"

// forward declaration
class TMarcRecord;

class TTransFile : public TFile, public semi
{
public:
  TTransFile  (typestr & FileSpec, TStateManager *StateManager);
  ~TTransFile (void);

  int           Open        (void);
  int           Convert     (TMarcRecord* In, TMarcRecord* Out);
  bool          Transcode   (const char* In, typestr &Out, const char *Notice = NULL, const char *Field = NULL);
  bool          Exists(char* In);

protected:
  int           GetValues   (const char *src, typestr *dest);
  typestr       itsDefaultValue;
  bool          itsDefaultValueValid;
  bool          itsDefaultCopy;
};
#endif
