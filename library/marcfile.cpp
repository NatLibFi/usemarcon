/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  marcfile.cpp
 *
 *  implements a class to manage input and output of MARC records
 *
 */

#include <ctype.h>
#include "marcfile.h"
#include "error.h"
#include "objectlist.h"


///////////////////////////////////////////////////////////////////////////////
//
// TMarcFile
//
///////////////////////////////////////////////////////////////////////////////
TMarcFile::TMarcFile( FILE_SPEC *FileSpec, TUMApplication *Application, char Mode, char Kind,
    MARC_FILE_FORMAT Format, short BlockSize, short MinFree, char PaddingChar, bool LastBlock)
:TFile(FileSpec, Application->GetErrorHandler(), Mode, Kind)
{
    itsDocument= new TMarcDoc(Application);
    itsApplication = Application;
    itsErrorHandler = Application->GetErrorHandler();


    SetMarcInfoFormat(Format);
    SetMarcInfoBlockSize(BlockSize);
    SetMarcInfoMinDataFree(MinFree);
    SetMarcInfoPaddingChar(PaddingChar);
    SetMarcInfoLastBlock(LastBlock);
    itsCharsRead = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// ~TMarcFile
//
///////////////////////////////////////////////////////////////////////////////
TMarcFile::~TMarcFile()
{
    delete itsDocument;
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::Open()
{
    if (!itsApplication->GetDetails()->GetMarcRecordAvailable())
    {
        if (TFile::Open())          // Echec d'ouverture du fichier
            return 1;

        if (itsMode == FILE_WRITE && (GetMarcInfoFormat() == MFF_MARCXML || GetMarcInfoFormat() == MFF_MARCXCHANGE))
        {
            // Write XML header
            typestr xml = "<?xml version=\"1.0\"?>\n\n"
            "<collection>\n";

            unsigned long len = strlen(xml.str());
            if (write(iFile, xml.str(), len) != len)
                return 1;
        }
    }

    PosCour     = 0L;
    NumBloc     = 0L;
    TB          = GetMarcInfoBlockSize();
    EndOfFile       = false;
    itsEof          = false;
    BufPos          = 0;
    BufSize         = 0;
    m_filebuffer.freestr();
    memset(FinBande, GetMarcInfoPaddingChar(), 5);
    itsCharsRead = 0;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// val
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::val(const char *buffer, unsigned short *valeur)
{
    unsigned int    i;

    *valeur = 0;
    for (i=0; i<strlen(buffer); ++i)
        if (!isdigit(buffer[i]))
            return 1;
    *valeur = (unsigned short)atoi(buffer);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// longval
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::longval(const char *buffer, unsigned long *valeur)
{
    unsigned int    i;

    *valeur=0;
    for (i=0; i<strlen(buffer); ++i)
        if (!isdigit(buffer[i]))
            return 1;
    *valeur = atol(buffer);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// first_bloc
//
///////////////////////////////////////////////////////////////////////////////
bool TMarcFile::first_block()
{
    if (PosCour % GetMarcInfoBlockSize())
        return false;
    else
        return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// read_marc_scw
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::read_marc_scw(bool a_first)
{
    if (GetMarcInfoFormat() == MFF_NONSEGMENTED)
        return 0;

    char   scw[6];

    if (read_marc(5, scw))                    // Lecture du SCW
        if (!a_first)
            return itsErrorHandler->SetError(1003, DISPLAY);
        else
            return 1;

    PosCour += 5;
    NumBloc = PosCour / GetMarcInfoBlockSize();

    if (!memcmp(scw, FinBande, 5)) // fin de la bande
    {
        EndOfFile = true;
        return 1;
    }

    unsigned short vscw;
    if (val(scw, &vscw))             // Verification de sa valeur
        return itsErrorHandler->SetErrorD(1002, ERROR, (char *)scw);

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// read_marc
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::read_marc(unsigned long a_length, char * a_buffer)
{
    unsigned long pbuf = 0;
    unsigned long read_len = a_length;
    while (read_len > 0)
    {
        unsigned long buffered = BufSize - BufPos;
        if (buffered >= read_len)
        {
            if (a_buffer)
            {
                memcpy(&a_buffer[pbuf], &m_filebuffer.str()[BufPos], read_len);
                a_buffer[pbuf + read_len] = '\0';
            }
            BufPos = BufPos + read_len;
            read_len = 0;
        }
        else
        {
            if (itsEof)
            {
                EndOfFile = true;
                return 1;
            }
            if (a_buffer)
                memcpy(&a_buffer[pbuf], &m_filebuffer.str()[BufPos], buffered);
            pbuf = pbuf + buffered;
            read_len = read_len - buffered;

            long lng;

            // Use buffered data if available, otherwise use a file
            if (itsApplication->GetDetails()->GetMarcRecordAvailable())
            {
                char *pcRecord;
                int iLength = 0;
                itsApplication->GetDetails()->GetMarcRecord(pcRecord, iLength);

                iLength -= itsCharsRead;

                lng = iLength;
                m_filebuffer.allocstr(iLength + 1);
                memcpy(m_filebuffer.str(), &pcRecord[itsCharsRead], iLength);
                itsCharsRead += iLength;
                free(pcRecord);
            }
            else
            {
                m_filebuffer.allocstr(TB + 1);
                lng = ::read(iFile, m_filebuffer.str(), TB);
            }

            if (lng == -1)
                return itsErrorHandler->SetError(1502, ERROR);
            else if (lng < TB)
                itsEof = true;
            BufSize = lng;
            BufPos = 0;
        }
    }
    return 0;
}

bool TMarcFile::xml_read_tag(const char *a_tag, typestr & a_xml)
{
    char temp[2];

    while (read_marc(1, temp) == 0)
    {
        if (*temp == '<')
        {
            // Found beginning of a tag, find the end
            typestr tag = "<";
            while (true)
            {
                if (read_marc(1, temp))
                    return false;
                tag.append_char(*temp);
                if (*temp == '>')
                    break;
            }

            // Bypass possible namespace
            const char *p = strchr(tag.str(), ':');
            if (p && p < strchr(tag.str(), ' '))
                ++p;
            else
                p = &(tag.str()[1]);
            size_t tag_len = strlen(a_tag);
            if (strncmp(p, a_tag, tag_len) == 0 && strchr(" \t\n\r>", *(p + tag_len)))
            {
                // We have a match
                a_xml = tag;
                return true;
            }            
        }
    }
    return false;
}

bool TMarcFile::xml_read_until_end(const char *a_tag, typestr & a_xml)
{
    char temp[2];

    while (read_marc(1, temp) == 0)
    {
        if (*temp == '<')
        {
            // Found beginning of a tag, find the end
            typestr tag = "<";
            while (true)
            {
                if (read_marc(1, temp))
                    return false;
                tag.append_char(*temp);
                if (*temp == '>')
                    break;
            }

            a_xml.append(tag);

            // Check if this is an end tag
            if (tag.str()[1] != '/')
                continue;

            // Bypass possible namespace
            const char *p = strchr(tag.str(), ':');
            if (p && p < strchr(tag.str(), ' '))
                ++p;
            else
                p = &(tag.str()[2]);
            size_t tag_len = strlen(a_tag);
            if (strncmp(p, a_tag, tag_len) == 0 && strchr(" \t\n\r>", *(p + tag_len)))
            {
                // We have a match
                return true;
            }       
            continue;
        }
        a_xml.append_char(*temp);
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//
// Read
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::Read(TUMRecord *Record)
{
    if (GetMarcInfoFormat() == MFF_MARCXML || GetMarcInfoFormat() == MFF_MARCXCHANGE)
    {
        // This is somewhat crude, but we're trying to get away without actually 
        // parsing the xml here
        typestr xml;

        // Find beginning of a record
        if (!xml_read_tag("record", xml))
            if (EndOfFile)
                return 1;
            else
                return itsErrorHandler->SetError(1003,ERROR);

        // Read until end of the record
        if (!xml_read_until_end("record", xml))
            return itsErrorHandler->SetError(1003,ERROR);

        Record->FromXMLString(xml);
        return 0;
    }

    unsigned long block_end = (NumBloc + 1) * GetMarcInfoBlockSize();

    if (block_end - PosCour <= GetMarcInfoMinDataFree() && GetMarcInfoFormat() == MFF_SEGMENTED)
    {
        // S'il ne reste pas suffisament de caracteres disponibles dans le bloc, il est
        // impossible d'y lire un SCW, on saute donc au bloc suivant
        if (read_marc(block_end - PosCour, NULL))
            if (EndOfFile)
                return 1;
            else
                return itsErrorHandler->SetError(1003,ERROR);

        ++NumBloc;
        PosCour = NumBloc * GetMarcInfoBlockSize();
        block_end = PosCour + GetMarcInfoBlockSize();
    }

    if (read_marc_scw(1))
        return 1;
    block_end = (NumBloc + 1) * GetMarcInfoBlockSize();

    typestr Buffer;
    Buffer.allocstr(512);

    if (block_end - PosCour < GetMarcInfoMinDataFree())
        // On essai maintenant de lire les 5 premiers caracteres du guide, pour  avoir la
        // longueur de la notice. Mais ces 5 caracteres peuvent-etre a cheval sur deux blocs,
        // s'il reste moins de 5 caracteres dans le bloc. Un SCW sera donc present. Ce que
        // l'on traite ici
    {
        unsigned long read_len = block_end - PosCour;
        unsigned long read_remaining = 5 - read_len;
        
        Buffer.promise(read_len + read_remaining + 1);
        if (read_marc(read_len, Buffer.str()))
            return itsErrorHandler->SetError(1003,ERROR);

        ++NumBloc;
        PosCour = NumBloc * GetMarcInfoBlockSize();
        block_end = PosCour + GetMarcInfoBlockSize();

        if (read_marc_scw(0))
            return 1;

        // On se trouve en debut de bloc, il faut donc lire le SCW. Si on y arrive pas, il
        // y a forcement erreur, puisqu'on se trouve dans les 5 premiers caracteres du guide

        if (read_marc(read_remaining, &Buffer.str()[read_len]))
            return itsErrorHandler->SetError(1003,ERROR);

        PosCour += read_remaining;
        // Buffer contient maintenant les 5 premiers caracteres du guide
    }
    else
        // Les 5 premiers caracteres du guide, ici, sont tous dans le meme bloc ... on essai
        // donc de les lire
    {
        if (read_marc(5, Buffer.str()))
            return itsErrorHandler->SetError(1003,NONERROR);

        PosCour += 5;
    }

    // bypass a possible cr/lf
    while (*Buffer.str() == '\n' || *Buffer.str() == '\r')
    {
        memmove(Buffer.str(), &Buffer.str()[1], 5);
        if (read_marc(1, &Buffer.str()[4]))
            return itsErrorHandler->SetError(1003,ERROR);
    }

    unsigned long RecordLen;
    if (longval(Buffer.str(), &RecordLen))
        // On verifie que la longueur est numerique, et on affecte sa valeur a E_LNG_NOTICE
        return itsErrorHandler->SetErrorD(1004, ERROR, Buffer.str());

    unsigned long lbuf = 5;
    unsigned long read_remaining = RecordLen - 5;
    do
    // On lit maintenant le reste de la notice, que l'on concatene au BUFFER contenant
    // les 5 premiers caracteres du guide, pour avoir dans BUFFER la notice complete
    {
        if (first_block())
            // Si on est en debut de bloc, il faut passer par dessus le SCW de debut de bloc
            if (read_marc_scw(0))
                return 1;

        unsigned long read_len = read_remaining;
        // On lit une notice morceau par morceau, chaque morceau s'arretant soit a la fin
        // de la notice, soit a la fin du bloc courant.
        //     - lbuf est la longueur deja stockee dans Buffer
        //     - lreste est la longueur qui reste a lire pour avoir toute la notice
        //     - lalire est la longueur a lire dans le bloc courant

        if (read_len + PosCour > block_end)
            read_len = block_end - PosCour;

        Buffer.promise(read_len + lbuf + 1);
        if (read_marc(read_len, &Buffer.str()[lbuf]))
            return itsErrorHandler->SetError(1003,ERROR);

        lbuf += read_len;
        read_remaining -= read_len;
        PosCour += read_len;
        NumBloc = PosCour / GetMarcInfoBlockSize();
        block_end = (NumBloc + 1) * GetMarcInfoBlockSize();
    }
    while (read_remaining > 0);

    // La notice contenue dans le buffer est dispatch‰e en enregistrement Marc
    Record->FromString(Buffer.str());

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
//
// Write
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::Write(TUMRecord *Record)
{
    if (GetMarcInfoFormat() == MFF_MARCXML || GetMarcInfoFormat() == MFF_MARCXCHANGE)
    {
        typestr xml;
        int res = Record->ToXMLString(xml);
        if (res) 
            return res;

        if (itsApplication->GetDetails()->GetMarcRecordAvailable())
        {
            itsApplication->GetDetails()->SetMarcRecord(xml.str(), strlen(xml.str()));
            return 0;
        }

        if (write_marc(strlen(xml.str()), xml.str()))
            return itsErrorHandler->SetError(1005,ERROR);
        return 0;
    }

    typestr Buffer;
    // La notice contenue dans un enregistrement marc est regroup‰e dans une chaine
    Record->ToString(Buffer);

    unsigned long RecordLen;
    RecordLen=strlen(Buffer.str());

    if (itsApplication->GetDetails()->GetMarcRecordAvailable())
    {
        itsApplication->GetDetails()->SetMarcRecord(Buffer.str(), RecordLen);
        return 0;
    }

    if (RecordLen == 0)
        return 0;

    unsigned long block_end = (NumBloc + 1) * GetMarcInfoBlockSize();

    if (block_end - PosCour <= GetMarcInfoMinDataFree() && GetMarcInfoFormat() == MFF_SEGMENTED)
        // S'il ne reste pas assez de caracteres dans le bloc, on passe au bloc suivant
    {
        typestr temps;
        temps.allocstr(GetMarcInfoMinDataFree() + 1);
        memset(temps.str(), GetMarcInfoPaddingChar(), GetMarcInfoMinDataFree());
        temps.str()[GetMarcInfoMinDataFree()] = '\0';
        if (write_marc(block_end - PosCour, temps.str()))
            return itsErrorHandler->SetError(1005,ERROR);

        ++NumBloc;
        PosCour = NumBloc * GetMarcInfoBlockSize();
        block_end = PosCour + GetMarcInfoBlockSize();
    }

    unsigned long pos=0;
    unsigned long write_remaining = RecordLen;
    do
    {
        unsigned long write_len = write_remaining;
        if (GetMarcInfoFormat() == MFF_SEGMENTED)
        {
            if (write_len + PosCour + 5 > block_end)
                // La notice ne finira pas dans ce bloc
            {
                write_len = block_end - PosCour - 5;
                if (pos == 0)
                    // La notice commence dans ce bloc   --> SCW =1....
                {
                    if (write_marc_scw(1, write_len))
                        return 1;
                }
                // La notice ne commence pas dans ce bloc --> SCW =2....
                else
                    if (write_marc_scw(2, write_len))
                        return 1;
            }
            else
                // La notice finira dans ce bloc
            {
                if (pos == 0)
                    // La notice commence dans ce bloc   --> SCW =0....
                {
                    if (write_marc_scw(0, write_len))
                        return 1;
                }
                // La notice ne commence pas dans ce bloc --> SCW =3....
                else
                    if (write_marc_scw(3, write_len))
                        return 1;
            }
        }

        if (write_marc(write_len, &Buffer.str()[pos]))
            return itsErrorHandler->SetError(1005,ERROR);

        // On ecrit ce bout de notice dans le bloc
        PosCour += write_len;
        NumBloc = PosCour / GetMarcInfoBlockSize();
        block_end = (NumBloc + 1) * GetMarcInfoBlockSize();
        pos += write_len;
        write_remaining -= write_len;
    }
    while (write_remaining > 0);  // On a fini quand toute la notice est ecrite

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// write_marc_scw
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::write_marc_scw(short typ,unsigned long longueur)
{
    char scw[6];

    if (GetMarcInfoFormat() == MFF_NONSEGMENTED)
        return 0;

    sprintf(scw, "%1d%04d", typ, longueur + 5);
    scw[5] = '\0';
    if (write_marc(5, scw))
        return itsErrorHandler->SetError(1005, ERROR);

    PosCour += 5;
    NumBloc = PosCour / GetMarcInfoBlockSize();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// write_marc
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::write_marc(unsigned long a_length, char *a_buffer)
{
    unsigned long length_remaining = a_length;
    unsigned long pbuf = 0;
    while (length_remaining > 0)
    {
        unsigned long remaining = TB - BufPos;
        if (remaining >= length_remaining)
        {
            m_filebuffer.promise(BufPos + length_remaining + 1);
            memcpy(&m_filebuffer.str()[BufPos], &a_buffer[pbuf], length_remaining);
            BufPos = BufPos + length_remaining;
            length_remaining = 0;
        }
        else
        {
            m_filebuffer.promise(BufPos + remaining + 1);
            memcpy(&m_filebuffer.str()[BufPos], &a_buffer[pbuf], remaining);
            pbuf = pbuf + remaining;
            length_remaining -= remaining;
            if (write(iFile, m_filebuffer.str(), TB) != TB)
                return 1;
            BufPos = 0;
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Close
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::Close()
{
    if (itsMode == FILE_WRITE)
    {
        if (GetMarcInfoLastBlock())
        {
            m_filebuffer.promise(TB + 1);
            for (int i = BufPos; i < TB; i++)
                m_filebuffer.str()[i] = GetMarcInfoPaddingChar();
            m_filebuffer.str()[TB] = '\0';
            BufPos = TB;
        }
        if ((unsigned long)write(iFile, m_filebuffer.str(), BufPos) != BufPos)
            return itsErrorHandler->SetError(1006,ERROR);

        m_filebuffer = "";
        BufPos = 0;

        if (GetMarcInfoFormat() == MFF_MARCXML || GetMarcInfoFormat() == MFF_MARCXCHANGE)
        {
            // Write XML footer
            typestr xml = "</collection>\n";

            unsigned long len = strlen(xml.str());
            if (write(iFile, xml.str(), len) != len)
                return itsErrorHandler->SetError(1006,ERROR);
        }
    }

    TFile::Close();
    return 0;
}

long TMarcFile::GetSize(void)
{
    if (itsApplication->GetDetails()->GetMarcRecordAvailable())
    {
        return itsApplication->GetDetails()->GetMarcRecordLength();
    }
    else
    {
        return TFile::GetSize();
    }
}

long TMarcFile::GetPos(void)
{
    if (itsApplication->GetDetails()->GetMarcRecordAvailable())
    {
        return itsCharsRead;
    }
    else
    {
        return TFile::GetPos();
    }
}
