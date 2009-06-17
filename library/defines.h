/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  defines.h
 *
 *
 
OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004
    Adapted by The National Library of Finland, 2004-2009

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland
*/

#ifndef _Defines_H
#define _Defines_H

#define USEMARCON_VERSION "3.13"
#define RULE_VERSION 313

#define INPUT 1
#define OUTPUT 2

#define FL_OK 1
#define FL_BAD 2
#define FL_CANCEL 3

#define START_OF_FIELD '\x1f'
#define START_OF_FIELD_STR "\x1f"
#define END_OF_FIELD '\x1e'
#define END_OF_RECORD '\x1d'

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

#if defined(_MSC_VER)
  #define l_strdup _strdup
  #define l_open _open
  #define l_close _close
  #define l_read _read
  #define l_write _write
  #define l_lseek _lseek
#else
  #define l_strdup strdup
  #define l_open open
  #define l_close close
  #define l_read read
  #define l_write write
  #define l_lseek lseek
#endif

#define MAXPATH 1024

#if !defined(TRUE)
  #define TRUE  1
#endif

#if !defined(FALSE)
  #define FALSE 0
#endif

typedef enum MARC_FILE_FORMAT
{
    MFF_NONSEGMENTED = 0,
    MFF_SEGMENTED = 1,
    MFF_MARCXML = 2,
    MFF_MARCXCHANGE = 3,
} MARC_FILE_FORMAT;

typedef enum DUPLICATE_PROCESSING_MODE
{
    DP_LEAVE = 0,
    DP_DELETE = 1,
    DP_DELETE_IGNORE_CASE = 2,
    DP_DELETE_SMART = 3,
} DUPLICATE_PROCESSING_MODE;

typedef enum IN_STR_POSITION
{
    SP_ANY = 0,
    SP_BEGINNING = 1,
    SP_END = 2,
    SP_BOTH = 3,
} IN_STR_POSITION;

#endif

