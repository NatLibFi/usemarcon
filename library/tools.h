/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tools.h
 *
 *
 
OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2007

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _TOOLS_H
#define _TOOLS_H

#include <stdio.h>
#include <string.h>
#include "tfile.h"
#include "typedef.h"

#define APRES  0
#define DESSUS 1

#define AT_BEGINNING	0x01
#define INSIDE	    	0x02
#define AT_END		    0x04
#define EVERYWHERE	AT_BEGINNING | INSIDE | AT_END

int RemoveSpace		(char *String,int where = EVERYWHERE);
int InsertChar		(char *entree,int *lg_entree,int position,char *caracteres,int code);
int	ToUpperCase		(char *String);
int CodeHexaToChar	(char *String);

// trim whitespace from the beginning and the end of the string
void trim_string(char *str);

// trim single and double quotes from the beginning and the end of the string
void trim_string_quotes(char *str);

// check if a file can be opened for reading
bool file_available(const char* a_filename);

// read a string from the specified ini file
void get_ini_string(const char *section_name,
                    const char *key_name,
                    const char *default_value,
                    typestr & return_buffer,
                    const char *ini_name);

///////////////////////////////////////////////////////////////////////////////
//
// get_ini_filename()
//
// Calls get_ini_string() to fetch a filename from the .ini
// files. If the filename does not contain path (no slashes/backslashes), the path
// to the ini file itself (including slash/backslash) is added to the beginning
// of the path.
//
///////////////////////////////////////////////////////////////////////////////
void get_ini_filename(const char *section_name,
                      const char *key_name,
                      const char *default_value,
                      typestr & return_buffer,
                      const char *ini_name);

// write a string to the specified ini file (the file must exist already)
bool put_ini_string(const char *section_name,
                    const char *key_name,
                    const char *value,
                    const char *ini_name);

// copy the path of filename
void copy_path_from_filename(typestr & path, const char *filename);

// append a filename to the path
void append_filename(typestr & path, const char *filename);

// get a file attribute
bool file_exists(typestr & fspec);

// show a warning to the user
void show_warning(const char *message);

// show a message to the user
void show_message(const char *message);

// UTF-8 aware functions (utf8proc wrappers)
unsigned int utf8_strlen(const char *str);
unsigned int utf8_charindex(const char *str, unsigned long idx);
int utf8_stricmp(const char *str1, const char *str2);

// File handling
int readline(typestr &line, FILE *fh);

// String classification
int caps_count(const char *str);

// Trim string
void trim_string(typestr &a_dst, typestr &a_src);
void trim_string2(typestr2 &a_dst, typestr2 &a_src);

#endif // _TOOLS_H
