/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2008
 *
 *  File:  tools.cpp
 *
 *  implements a set of utility functions
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include "tools.h"
#include "defines.h"
#include "utf8proc.h"

///////////////////////////////////////////////////////////////////////////////
//
// RemoveSpace
//
///////////////////////////////////////////////////////////////////////////////
int RemoveSpace(char *String,int where)
{
    int     CurrentPos=0,
        LastPos,
        MaxPos=strlen(String);
    char * tmp_string = NULL;

    if (!MaxPos) return 0;

    LastPos=MaxPos-1;

    if (where&AT_END)
        // Remove all spaces at end
    {
        for (LastPos; LastPos>=0 && (isspace(String[LastPos]) || String[LastPos]=='\t'); --LastPos)
        ;
        if (LastPos<0)
        {
            *String=0;
            return 0;
        }

        String[LastPos+1]=0;
        MaxPos=LastPos+1;
    }

    if (where&AT_BEGINNING)
        // Remove all spaces at begining
    {
        for (CurrentPos; CurrentPos<MaxPos && (isspace(String[CurrentPos]) || String[CurrentPos]=='\t'); CurrentPos++ )
            ;
        if (CurrentPos > 0)
        {
            if (MaxPos)  MaxPos-=CurrentPos;
            if (LastPos) LastPos-=CurrentPos;
            memmove(String, String+CurrentPos, MaxPos+1);
        }
    }

    if (where&INSIDE)
        // Remove all spaces inside the string
    {
        for (CurrentPos; CurrentPos && (isspace(String[CurrentPos]) || String[CurrentPos]=='\t'); CurrentPos++)
            ;
        for (CurrentPos; CurrentPos<LastPos; CurrentPos++)
            if (isspace(String[CurrentPos]) || String[CurrentPos]=='\t')
            {
                if (LastPos) LastPos--;
                if (MaxPos)  MaxPos--;
                memmove(String+CurrentPos, String+CurrentPos+1, MaxPos+1-CurrentPos);
                CurrentPos--;
            }
    }

    return MaxPos;
}

///////////////////////////////////////////////////////////////////////////////
//
// InsertChar
//
///////////////////////////////////////////////////////////////////////////////
int InsertChar(char *entree,int *lg_entree,int position,char *caracteres,int code)
{
    int i,j,decal,decalage;

    decal=strlen(caracteres);
    decalage=decal-code;
    for (j=*lg_entree;j>=position+code;j--)
        entree[j+decalage]=entree[j];
    for (i=0; i<decal; i++)
        entree[position++]=caracteres[i];
    (*lg_entree)+=decal;
    return(decal);
}

///////////////////////////////////////////////////////////////////////////////
//
// ToUpperCase
//
///////////////////////////////////////////////////////////////////////////////
int ToUpperCase(char *String)
{
    int     CurrentPos;
    int     MaxPos=strlen(String);

    for (CurrentPos=0; CurrentPos<MaxPos; CurrentPos++)
        String[CurrentPos]=(char)toupper((int)String[CurrentPos]);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// CodeHexaToChar
//
///////////////////////////////////////////////////////////////////////////////
int CodeHexaToChar(char *String)
{
    int iLongueur;
    int iPosEntree;
    int iPosSortie=0;
    char    sSortie[100];
    char    sCodeHexa[5];
    long    lChar;

    sSortie[iPosSortie]=0;
    iLongueur=strlen(String);
    // Recherche d'un code Hexa ( introduit par '0x' )
    for (iPosEntree=0; iPosEntree<iLongueur; iPosEntree++)
    {
        if (!memcmp(&String[iPosEntree],"0x",2))
        {
            // chaine '0x' trouvee ==> il s'agit d'un code Hexa uniquement si
            // deux caractères au moins suivent, sinon il s'agit simplement de
            // '0' et de 'x' en tant que caracteres propres.
            memcpy(sCodeHexa,&String[iPosEntree],4);
            sCodeHexa[4]=0;
            if (strlen(sCodeHexa)==4)
                // '0x' introduit bien un code Hexa ==> conversion en caractere.
            {
                sscanf(&sCodeHexa[2],"%x",&lChar);
                sprintf(&sSortie[iPosSortie++],"%c",lChar);
                iPosEntree+=3;
            }
            else
                // '0x' n'est a considerer que comme une chaine composee des deux
                // carateres '0' et 'x' ==> rien a faire.
                sSortie[iPosSortie++]=String[iPosEntree];
        }
        else
            // carateres normal ==> rien a faire.
            sSortie[iPosSortie++]=String[iPosEntree];
    }
    sSortie[iPosSortie]=0;
    strcpy(String,sSortie);
    return 0;
}

void trim_string(char *str)
{
    size_t len = strlen(str);
    while (len && isspace(str[len - 1]))
    {
        str[len - 1] = '\0';
        --len;
    }

    char *p = str;
    while (isspace(*p))
        ++p;
    if (p > str)
        memmove(str, p, len - 1);
}

void trim_string_quotes(char *str)
{
    size_t len = strlen(str);
    while (len && (str[len - 1] == '\'' || str[len - 1] == '"'))
    {
        str[len - 1] = '\0';
        --len;
    }

    char *p = str;
    while (*p == '\'' || *p == '"')
        ++p;
    if (p > str)
        memmove(str, p, len - 1);
}

bool file_available(const char* a_filename)
{
    FILE *f;
    if ((f = fopen(a_filename, "r")))
    {
        fclose(f);
        return true;
    }
    return false;
}

void get_ini_string(const char *section_name,
                    const char *key_name,
                    const char *default_value,
                    typestr &return_buffer,
                    const char *ini_name)
{
    FILE *ini_file;
    bool found = false;

    return_buffer = "";
    if ((ini_file = fopen(ini_name, "r")))
    {
        char line[2048];
        char line_key[2048];
        bool section_ok = !*section_name;

        while (!found && fgets(line, sizeof(line), ini_file))
        {
            trim_string(line);

            switch (*line)
            {
                case '\0':
                case ';':
                case '#':
                    continue;

                case '[': // section
                    line[strlen(line) - 1] = '\0';
                    section_ok = !*section_name || strcasecmp(line + 1, section_name) == 0;
                    continue;

                default: // assume it's a value
                    if (!section_ok)
                        continue;
                    char *eq_pos = strchr(line, '=');
                    if (!eq_pos)
                        continue;
                    *eq_pos = '\0';
                    strcpy(line_key, line);
                    trim_string(line_key);
                    if (strcasecmp(line_key, key_name) == 0)
                    {
                        // found a match
                        return_buffer = eq_pos + 1;
                        found = true;
                        break;
                    }
                    break;
            }
        }
        fclose(ini_file);
    }

    if (!found)
        return_buffer = default_value;

    trim_string(return_buffer.str());
    trim_string_quotes(return_buffer.str());
}

void get_ini_filename(const char *section_name,
                      const char *key_name,
                      const char *default_value,
                      typestr & return_buffer,
                      const char *ini_name)
{
    get_ini_string(section_name, key_name, default_value, return_buffer, ini_name);
    if (*return_buffer.str() && !strchr(return_buffer.str(), SLASH))
    {
        typestr full_path;
        copy_path_from_filename(full_path, ini_name);
        append_filename(full_path, return_buffer.str());
        return_buffer = full_path;
    }
}

bool put_ini_string(const char *section_name,
                    const char *key_name,
                    const char *value,
                    const char *ini_name)
{
    FILE *ini_file;
    FILE *tmp_file;
    char tmp_name[MAXPATH];
    strcpy(tmp_name, ini_name);
    if (strlen(tmp_name) > MAXPATH - 6)
        tmp_name[MAXPATH - 5] = '\0';
    strcat(tmp_name, ".tmp");

    if (!(ini_file = fopen(ini_name, "r")))
        return false;

    if (!(tmp_file = fopen(tmp_name, "w")))
    {
        fclose(ini_file);
        return false;
    }

    char full_line[2048];
    char line[2048];
    char line_key[2048];
    bool section_ok = !*section_name;
    bool value_written = false;
    long empty_lines = 0;

    // if we encounter errors, we'll finally discard the resulting file
    long errors = 0;

    while (fgets(full_line, sizeof(full_line), ini_file))
    {
        strcpy(line, full_line);
        trim_string(line);

        switch (*line)
        {
        case '\0':
            empty_lines++;
            break;
        case ';':
        case '#':
            while (empty_lines)
            {
                if (fputs("\n", tmp_file) == EOF)
                    errors++;
                empty_lines--;
            }
            if (fputs(full_line, tmp_file) == EOF)
                errors++;
            continue;

        case '[': // section
            if (!value_written && section_ok)
            {
                // we're moving to another section but the value wasn't found,
                // write a new line
                if (fputs(key_name, tmp_file) == EOF)
                    errors++;
                if (fputs("=", tmp_file) == EOF)
                    errors++;
                if (fputs(value, tmp_file) == EOF)
                    errors++;
                if (fputs("\n", tmp_file) == EOF)
                    errors++;
                value_written = true;
            }
            line[strlen(line) - 1] = '\0';
            section_ok = !*section_name || strcasecmp(line + 1, section_name) == 0;
            while (empty_lines)
            {
                if (fputs("\n", tmp_file) == EOF)
                    errors++;
                empty_lines--;
            }
            if (fputs(full_line, tmp_file) == EOF)
                errors++;
            continue;

        default: // assume it's a value
            bool write_line = true;
            if (section_ok)
            {
                char *eq_pos = strchr(line, '=');
                if (eq_pos)
                {
                    *eq_pos = '\0';
                    strcpy(line_key, line);
                    trim_string(line_key);
                    if (strcasecmp(line_key, key_name) == 0)
                    {
                        // found a match, replace it
                        write_line = false;
                        if (fputs(key_name, tmp_file) == EOF)
                            errors++;
                        if (fputs("=", tmp_file) == EOF)
                            errors++;
                        if (fputs(value, tmp_file) == EOF)
                            errors++;
                        if (fputs("\n", tmp_file) == EOF)
                            errors++;
                        value_written = true;
                    }
                }
            }
            while (empty_lines)
            {
                if (fputs("\n", tmp_file) == EOF)
                    errors++;
                empty_lines--;
            }
            if (write_line)
                if (fputs(full_line, tmp_file) == EOF)
                    errors++;

            break;
        }
    }
    if (!value_written)
    {
        // need to create a new section and write the value
        if (fputs("\n", tmp_file) == EOF)
            errors++;
        if (fputs("[", tmp_file) == EOF)
            errors++;
        if (fputs(section_name, tmp_file) == EOF)
            errors++;
        if (fputs("]\n", tmp_file) == EOF)
            errors++;
        if (fputs(key_name, tmp_file) == EOF)
            errors++;
        if (fputs("=", tmp_file) == EOF)
            errors++;
        if (fputs(value, tmp_file) == EOF)
            errors++;
        if (fputs("\n", tmp_file) == EOF)
            errors++;
    }
    fclose(ini_file);
    fclose(tmp_file);
    if (errors)
        return false;

    char tmp_name2[MAXPATH];
    strcpy(tmp_name2, ini_name);
    if (strlen(tmp_name2) > MAXPATH - 7)
        tmp_name[MAXPATH - 6] = '\0';
    strcat(tmp_name2, ".tmp2");

    // rename the original file
    if (rename(ini_name, tmp_name2))
        return false;
    // rename the new file
    if (rename(tmp_name, ini_name))
    {
        // oh no, try to back out
        rename(tmp_name2, ini_name);
        return false;
    }
    // remove the original file
    remove(tmp_name2);

    return true;
}

void copy_path_from_filename(typestr & path, const char *filename)
{
    path = "";
    const char *slashp = strrchr(filename, SLASH);
    if (slashp)
    {
        int i = 1 + slashp - filename;
        path.append(filename, i);
    }
}

void append_filename(typestr & path, const char *filename)
{
    path += filename;
}

bool file_exists(typestr & fspec)
{
    FILE *fp;

    fp = fopen(fspec.str(), "r");
    if (fp == NULL)
        return false;
    fclose(fp);
    return true;
}

void show_warning(const char *message)
{
    printf("%s\n", message);
}

void show_message(const char *message)
{
    printf("Message: %s\n", message);
}

unsigned int utf8_glypheme_length(const char *p)
{
    //unsigned int len = utf8_charlen(*p);

    int32_t cp;
    unsigned int len = utf8proc_iterate((uint8_t *) p, -1, &cp);

    // Check for combining characters
    const char *p2 = p + len;
    while (*p2)
    {
        unsigned int comb_len = utf8proc_iterate((uint8_t *) p2, -1, &cp);
        const utf8proc_property_t* prop = utf8proc_get_property(cp);
        if (!prop->combining_class)
            break;
        len += comb_len;
        p2 += comb_len;
    }
    return len;
}

unsigned int utf8_strlen(const char *str)
{
    const char *p = str;
    unsigned int i = 0;
    while (*p)
    {
        ++i;
        unsigned int clen = utf8_glypheme_length(p);
        while (*p && clen > 0)
        {
            ++p;
            --clen;
        }
    }
    return i;
}

unsigned int utf8_charindex(const char *str, unsigned long idx)
{
    unsigned int utf8_idx = 0;
    const char *p = str;
    for (unsigned int i = 0; i < idx; i++)
    {
        if (!*p)
        {
            ++utf8_idx;
            continue;
        }
        unsigned int clen = utf8_glypheme_length(p);
        if (clen < 0) 
            break;
        utf8_idx += clen;
        while (*p && clen > 0)
        {
            ++p;
            --clen;
        }
    }
    return utf8_idx;
}

int utf8_stricmp(const char *str1, const char *str2)
{
    int result = 0;

    char *str1_utf8 = NULL;
    if (utf8proc_map((unsigned char *) str1, 0, (unsigned char **) &str1_utf8, UTF8PROC_NULLTERM | UTF8PROC_CASEFOLD) > 0)
    {
        char *str2_utf8 = NULL;
        if (utf8proc_map((unsigned char *) str2, 0, (unsigned char **) &str2_utf8, UTF8PROC_NULLTERM | UTF8PROC_CASEFOLD) > 0)
        {
            result = strcmp(str1_utf8, str2_utf8);
            free(str2_utf8);
        }
        free(str1_utf8);
    }
    return result;
}

int readline(typestr &a_line, FILE *a_fh)
{
    a_line = "";
    char buf[1024];
    int lines = 1;
    bool eol = false;
    bool first = true;
    while (!eol)
    {
        if (fgets(buf, 1024, a_fh) == NULL)
        {
            if (first)
                return 0;
            break;
        }
        first = false;

        int len = strlen(buf);
        if (buf[len-1] == '\n')
        {
            if (len >= 2 && buf[len-2] == '\r')
            {
                --len;
                buf[len-1] = '\n';
            }
            if ((len == 2 && buf[len-2] == '\\') || (len > 2 && buf[len-2] == '\\' && buf[len-3] != '\\'))
            {
                ++lines;
                buf[len-2] = ' ';
            }
            else
            {
                eol = true;
            }
        }
        a_line.append(buf);
    }
    return lines;
}

int caps_count(const char *str)
{
    int result = 0;
    
    const char *p = str;
    while (*p)
    {
        if (*p >= 'A' && *p <= 'Z')
            ++result;
        ++p;
    }
    return result;
}

void trim_string(typestr &a_dst, typestr &a_src)
{
    char* p = a_src.str();
    while (*p == ' ')
        ++p;
    char* p_end = a_src.str() + strlen(a_src.str()) - 1;
    while(p_end > p && *p_end == ' ')
        --p_end;
    a_dst.str(p, p_end - p + 1);
}

void trim_string2(typestr2 &a_dst, typestr2 &a_src)
{
    trim_string(a_dst, a_src);
    trim_string(a_dst.s2, a_src.s2);
    a_dst.set_script(a_src.script);
}
