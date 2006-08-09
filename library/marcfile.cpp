/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2006
 *
 *  File:  marcfile.cpp
 *
 *  implements a class to manage input and output of MARC records
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include <ctype.h>
#include "marcfile.h"
#include "error.h"

// John Hough
#include "objectlist.h"


///////////////////////////////////////////////////////////////////////////////
//
// TMarcFile
//
///////////////////////////////////////////////////////////////////////////////
TMarcFile::TMarcFile( FILE_SPEC *FileSpec, TUMApplication *Application, char Mode, char Kind,
    bool Format, short BlockSize, short MinFree, char PaddingChar, bool LastBlock)
:TFile(FileSpec, Application->GetErrorHandler(), Mode, Kind)
{

    itsDocument= new TMarcDoc(Application);
    itsApplication = Application;
    itsErrorHandler = Application->GetErrorHandler();


    Buffer      = NULL;
    Buf         = new unsigned char[TBLMAX];
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
    if (Buffer)
    {
        free(Buffer);
        Buffer  = NULL;
    }
    if (Buf)
    {
        delete [] Buf;
        Buf     = NULL;
    }

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
    }

    NumNotice       = 0L;
    PosCour     = 0L;
    NumBloc     = 0L;
    TB              = GetMarcInfoBlockSize();
    if (!Buffer)
        if ((Buffer=(unsigned char *)malloc(TBUF))==NULL)
            return itsErrorHandler->SetError(1501,ERROR);
    EndOfFile       = false;
    itsEof          = false;
    PBuf            = 0;
    TBuf            = 0;
    *Buf            = 0;
    memset(FinBande,GetMarcInfoPaddingChar(),5);
    itsCharsRead = 0;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// val
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::val(unsigned char* buffer, unsigned short *valeur)
{
    unsigned int    i;

    *valeur = 0;
    for (i=0; i<strlen((const char *)buffer); ++i)
        if (!isdigit(buffer[i]))
            return 1;
        *valeur = (unsigned short)atoi((const char *)buffer);
        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// longval
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::longval(unsigned char* buffer, unsigned long *valeur)
{
    unsigned int    i;

    *valeur=0;
    for (i=0; i<strlen((const char *)buffer); ++i)
        if (!isdigit(buffer[i]))
            return 1;
        *valeur = atol((const char *)buffer);
        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// debut_bloc
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::debut_bloc()
{
    if (PosCour%GetMarcInfoBlockSize())
        return 0;
    else
        return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
// lire_scw
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::lire_scw(char debut)
{
    unsigned char   scw[6];
    unsigned short  vscw;

    if (GetMarcInfoFormat()!=SEGMENTED)
        return 0;

    if (lire(5,scw))                    // Lecture du SCW
        if (!debut)
            return itsErrorHandler->SetError(1003,DISPLAY);
        else
            return 1;

    PosCour += 5;
    NumBloc = PosCour/GetMarcInfoBlockSize();
    FinBloc = (NumBloc+1)*GetMarcInfoBlockSize();

    if (!memcmp(scw,FinBande,5  )) // fin de la bande
    {
        EndOfFile   = true;
        return 1;
    }

    if (val(scw,&vscw))             // Verification de sa valeur
        return itsErrorHandler->SetErrorD(1002,ERROR,(char *)scw);

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// lire
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::lire(unsigned short taille, unsigned char * buffer)
{
    unsigned short  pbuf,
                    lreste;
    short           lng;

    pbuf=0;
    lreste=taille;
    while (lreste>0)
    {
        Reste=(unsigned short)(TBuf-PBuf);
        if (Reste>=lreste)
        {
            memcpy(&buffer[pbuf],&Buf[PBuf],lreste);
            buffer[pbuf+lreste]=0;
            PBuf = (unsigned short)(PBuf + lreste);
            lreste=0;
        }
        else
        {
            if (itsEof)
            {
                EndOfFile=true;
                return 1;
            }
            memcpy(&buffer[pbuf],&Buf[PBuf],Reste);
            pbuf = (unsigned short)(pbuf + Reste);
            lreste = (unsigned short)(lreste - Reste);

            // Use buffered data if available, otherwise use a file
            if (itsApplication->GetDetails()->GetMarcRecordAvailable())
            {
                char *pcRecord;
                int iLength = 0;
                itsApplication->GetDetails()->GetMarcRecord(pcRecord, iLength);

                iLength -= itsCharsRead;
                if (iLength > TB)
                    iLength = TB;

                lng = iLength;
                memcpy(Buf, &pcRecord[itsCharsRead], iLength);
                itsCharsRead += iLength;
                free(pcRecord);
            }
            else
                lng = (short)::read(iFile,Buf,TB);

            if (lng==-1)
                return itsErrorHandler->SetError(1502,ERROR);
            else
                if (lng<TB)
                    itsEof=true;
                TBuf=lng;
                PBuf=0;
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Read
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::Read(TUMRecord *Record)
{
    unsigned long   lalire,
        lbuf,
        lreste;
    unsigned char   temp[TBLMAX];

    FinBloc=GetMarcInfoBlockSize()*(NumBloc+1);

    if ((FinBloc-PosCour<=(unsigned long)(GetMarcInfoMinDataFree())) &&
        (GetMarcInfoFormat()==SEGMENTED))
    {
        // S'il ne reste pas suffisament de caracteres disponibles dans le bloc, il est
        // impossible d'y lire un SCW, on saute donc au bloc suivant
        if (lire((unsigned short)(FinBloc-PosCour),temp))
            if (EndOfFile)
                return 1;
            else
                return itsErrorHandler->SetError(1003,ERROR);

        ++NumBloc;
        PosCour=NumBloc*GetMarcInfoBlockSize();
        FinBloc=PosCour+GetMarcInfoBlockSize();
    }

    if (lire_scw(1))
        return 1;

    if (FinBloc-PosCour < (unsigned long)(GetMarcInfoMinDataFree()))
        // On essai maintenant de lire les 5 premiers caracteres du guide, pour  avoir la
        // longueur de la notice. Mais ces 5 caracteres peuvent-etre a cheval sur deux blocs,
        // s'il reste moins de 5 caracteres dans le bloc. Un SCW sera donc present. Ce que
        // l'on traite ici
    {
        lalire = FinBloc-PosCour;
        lreste = 5-lalire;
        if (lire((short)lalire,Buffer))
            return itsErrorHandler->SetError(1003,ERROR);

        ++NumBloc;
        PosCour=NumBloc*GetMarcInfoBlockSize();
        FinBloc=PosCour+GetMarcInfoBlockSize();

        if (lire_scw(0))
            return 1;

        // On se trouve en debut de bloc, il faut donc lire le SCW. Si on y arrive pas, il
        // y a forcement erreur, puisqu'on se trouve dans les 5 premiers caracteres du guide

        if (lire((unsigned short)lreste,&Buffer[lalire]))
            return itsErrorHandler->SetError(1003,ERROR);

        PosCour+=lreste;
        // Buffer contient maintenant les 5 premiers caracteres du guide
    }
    else
        // Les 5 premiers caracteres du guide, ici, sont tous dans le meme bloc ... on essai
        // donc de les lire
    {
        if (lire(5,Buffer))
            return itsErrorHandler->SetError(1003,NONERROR);

        PosCour+=5;
    }

    // bypass a possible cr/lf
    while (*Buffer == '\n' || *Buffer == '\r')
    {
        memmove(Buffer, &Buffer[1], 5);
        if (lire(1,&Buffer[4]))
            return itsErrorHandler->SetError(1003,ERROR);
    }

    if (longval(Buffer,&LngNotice))
        // On verifie que la longueur est numerique, et on affecte sa valeur a E_LNG_NOTICE
        return itsErrorHandler->SetErrorD(1004,ERROR,(char *)Buffer);

    lbuf = 5;
    lreste = LngNotice-5;
    do
    // On lit maintenant le reste de la notice, que l'on concatene au BUFFER contenant
    // les 5 premiers caracteres du guide, pour avoir dans BUFFER la notice complete
    {
        if (debut_bloc())
            // Si on est en debut de bloc, il faut passer par dessus le SCW de debut de bloc
            if (lire_scw(0))
                return 1;

        lalire = lreste;
        // On lit une notice morceau par morceau, chaque morceau s'arretant soit a la fin
        // de la notice, soit a la fin du bloc courant.
        //     - lbuf est la longueur deja stockee dans Buffer
        //     - lreste est la longueur qui reste a lire pour avoir toute la notice
        //     - lalire est la longueur a lire dans le bloc courant

        if (lalire+PosCour>FinBloc)
            lalire=FinBloc-PosCour;

        if (lire((short)lalire,&Buffer[lbuf]))
            return itsErrorHandler->SetError(1003,ERROR);

        lbuf += lalire;
        lreste -= lalire;
        PosCour += lalire;
        NumBloc=PosCour/GetMarcInfoBlockSize();
        FinBloc=(NumBloc+1)*GetMarcInfoBlockSize();
    }
    while (lreste>0);

    // La notice contenue dans le buffer est dispatch‰e en enregistrement Marc
    Record->FromString((char *)(Buffer));
    ++NumNotice;

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
//
// Write
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::Write(TUMRecord *Record)
{
    unsigned long   pos,
        laecrire,
        lreste;
    unsigned char   temp[TBLMAX];

    // La notice contenue dans un enregistrement marc est regroup‰e dans une chaine
    Record->ToString((char *)(Buffer),TBUF);

    lreste=LngNotice=strlen((const char *)Buffer);

    if (itsApplication->GetDetails()->GetMarcRecordAvailable())
    {
        itsApplication->GetDetails()->SetMarcRecord((char *)Buffer, lreste);
        return 0;
    }

    if (lreste==0)
        return 0;

    ++NumNotice;
    FinBloc=(NumBloc+1)*GetMarcInfoBlockSize();

    if ((FinBloc-PosCour<=(unsigned long)(GetMarcInfoMinDataFree())) &&
        (GetMarcInfoFormat()==SEGMENTED))
        // S'il ne reste pas assez de caracteres dans le bloc, on passe au bloc suivant
    {
        memset(temp,GetMarcInfoPaddingChar(),GetMarcInfoMinDataFree());
        temp[GetMarcInfoMinDataFree()]=0;
        if (ecrire((unsigned short)(FinBloc-PosCour),temp))
            return itsErrorHandler->SetError(1005,ERROR);

        ++NumBloc;
        PosCour=NumBloc*GetMarcInfoBlockSize();
        FinBloc=PosCour+GetMarcInfoBlockSize();
    }

    pos=0;
    do
    {
        laecrire=lreste;
        if (GetMarcInfoFormat()==SEGMENTED)
        {
            if ( laecrire+PosCour+5 > FinBloc )
                // La notice ne finira pas dans ce bloc
            {
                laecrire=FinBloc-PosCour-5;
                if (pos==0)
                    // La notice commence dans ce bloc   --> SCW =1....
                {
                    if (ecrire_scw(1,(short)laecrire))
                        return 1;
                }
                // La notice ne commence pas dans ce bloc --> SCW =2....
                else
                    if (ecrire_scw(2,(short)laecrire))
                        return 1;
            }
            else
                // La notice finira dans ce bloc
            {
                if (pos==0)
                    // La notice commence dans ce bloc   --> SCW =0....
                {
                    if (ecrire_scw(0,(short)laecrire))
                        return 1;
                }
                // La notice ne commence pas dans ce bloc --> SCW =3....
                else
                    if (ecrire_scw(3,(short)laecrire))
                        return 1;
            }
        }

        if (ecrire( (short)laecrire, &Buffer[pos] ))
            return itsErrorHandler->SetError(1005,ERROR);

        // On ecrit ce bout de notice dans le bloc
        PosCour+= laecrire;
        NumBloc=PosCour/(GetMarcInfoBlockSize());
        FinBloc=(NumBloc+1)*(GetMarcInfoBlockSize());
        pos += laecrire;
        lreste -= laecrire;
    }
    while (lreste>0);  // On a fini quand toute la notice est ecrite
    return 0;
}




///////////////////////////////////////////////////////////////////////////////
//
// ecrire_scw
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::ecrire_scw(short typ,unsigned short longueur)
{
    unsigned char   scw[6];

    if (GetMarcInfoFormat()!=SEGMENTED)
        return 0;

    sprintf((char *)scw,"%1d%04d",typ,longueur+5);
    scw[5]=0;
    if (ecrire(5,scw))
        return itsErrorHandler->SetError(1005,ERROR);

    PosCour += 5;
    NumBloc=PosCour/GetMarcInfoBlockSize();
    FinBloc=(NumBloc+1)*GetMarcInfoBlockSize();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// ecrire
//
///////////////////////////////////////////////////////////////////////////////
int TMarcFile::ecrire( unsigned short taille, unsigned char* buffer )
{
    unsigned short  pbuf,
        lreste;

    lreste=taille;
    pbuf=0;
    while (lreste>0)
    {
        Reste=(unsigned short)(TB-PBuf);
        if (Reste>=lreste)
        {
            memcpy(&Buf[PBuf],&buffer[pbuf],lreste);
            PBuf =(unsigned short)(PBuf + lreste);
            lreste=0;
        }
        else
        {
            memcpy(&Buf[PBuf],&buffer[pbuf],Reste);
            pbuf = (unsigned short)(pbuf + Reste);
            lreste = (unsigned short)(lreste - Reste);
            if (write(iFile,Buf,TB)!=TB)
                return 1;
            PBuf=0;
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
    int     i;

    if (itsMode==FILE_WRITE)
    {
        if (GetMarcInfoLastBlock())
        {
            for (i=PBuf;i<TB;i++)
                Buf[i] = GetMarcInfoPaddingChar();
            Buf[TB] = 0;
            PBuf=(unsigned short)TB;
        }
        if ((unsigned long)write(iFile,Buf,PBuf)!=PBuf)
            return itsErrorHandler->SetError(1006,ERROR);

        *Buf=0;
        PBuf=0;
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
