/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  defines.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004
    Adapted by The National Library of Finland, 2004-2006

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    Helsinki University Library, the National Library of Finland
*/

#ifndef _Defines_H
#define _Defines_H

#define USEMARCON_VERSION "2.00"

#define UNKNOWN 0
#define INPUT 1
#define OUTPUT 2

#define FL_OK 1
#define FL_BAD 2
#define FL_CANCEL 3

#define EMPTY_ITEM		"\"\""

#define TBLMAX 4096
#define BUFF_SIZE 1000
#define TBUF 100000

// SLASH is actually the directory separator
#if defined(_WIN32) && !defined(__CYGWIN__)
  #define SLASH '\\'
  #define SLASHSTR "\\"
#else
  #define SLASH '/'
  #define SLASHSTR "/"
#endif

#if defined(_MSC_VER)
  #define strcasecmp _stricmp
#endif

#define MAXPATH 1024

typedef enum MARC_FILE_FORMAT
{
    MFF_NONSEGMENTED = 0,
    MFF_SEGMENTED = 1,
    MFF_MARCXML = 2,
    MFF_MARCXCHANGE = 3,
} MARC_FILE_FORMAT;

#endif

