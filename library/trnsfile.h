/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  trnsfile.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    TTransFile

    implements a class to manage transcoding file

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2006

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    Helsinki University Library, the National Library of Finland

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
  TTransFile  (FILE_SPEC *FileSpec, TError *ErrorHandler);
  ~TTransFile (void);

  int           Open        (void);
  int           Convert     (TMarcRecord* In, TMarcRecord* Out);
  const char*   Transcode   ( char* In, typestr *Out, char *Notice=NULL, char *Field=NULL );

protected:
  int           GetValues   (const char *src, typestr *dest);
  typestr       itsDefaultValue;
  bool          itsDefaultValueValid;
  bool          itsDefaultCopy;
};
#endif
