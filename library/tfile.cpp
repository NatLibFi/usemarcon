/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2006
 *
 *  File:  tfile.cpp
 *
 *  implements a base class to manage file input./output
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *
 */

#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include "tfile.h"
#include "tools.h"
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef _WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef O_BINARY
  #define O_BINARY 0
#endif

///////////////////////////////////////////////////////////////////////////////
//
// TFile
//
///////////////////////////////////////////////////////////////////////////////

TFile::TFile( FILE_SPEC *FileInfo, TError *ErrorHandler, char Mode, char Kind)
{
    itsFileInfo = FileInfo;
    itsMode     = Mode;
    itsKind     = Kind;
    itsLineNumber = 0;

    Included        = NULL;
    File            = NULL;
    iFile           = -1;

    itsErrorHandler = ErrorHandler;
}



///////////////////////////////////////////////////////////////////////////////
//
// TFile
//
///////////////////////////////////////////////////////////////////////////////

TFile::~TFile()
{
    // Make sure the file isn't left open
    Close();
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
///////////////////////////////////////////////////////////////////////////////
int TFile::Open()
{
    char    mode[2];

    if (itsKind==FILE_ASCII)
    {
        sprintf(mode,"%c",itsMode);
        if ((File=fopen(itsFileInfo->name, mode))==NULL)
        {
            return itsErrorHandler->SetErrorD(9501,WARNING,itsFileInfo->name);
        }
    }
    else
    {
        if (itsMode==FILE_READ)
            iFile=::open(itsFileInfo->name,O_RDONLY|O_BINARY,0640);
        else
            iFile=::open(itsFileInfo->name,O_CREAT|O_WRONLY|O_TRUNC|O_BINARY,0640);

        if (iFile == -1)
            return itsErrorHandler->SetErrorD(9502,WARNING,itsFileInfo->name);
    }

    Included=NULL;
    if (itsMode==FILE_READ && itsKind==FILE_ASCII)
        return SkipBeginning();
    else
        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Close
//
///////////////////////////////////////////////////////////////////////////////
int TFile::Close()
{
    if (itsKind==FILE_ASCII)
    {
        if (File)
            fclose(File);
    }
    else
    {
        if (iFile != -1)
            close(iFile);
    }
    File=NULL;
    iFile=-1;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Exists
//
///////////////////////////////////////////////////////////////////////////////
bool TFile::Exists()
{
    return file_exists(itsFileInfo);
}


///////////////////////////////////////////////////////////////////////////////
//
// NextLine
//
///////////////////////////////////////////////////////////////////////////////
int TFile::NextLine(typestr *aLine, FILE_SPEC* Spec, int *LineNumber)
{
    // NextLine() can't operate on binary files
    if (itsKind==FILE_BINARY)
        return itsErrorHandler->SetErrorD(9504,ERROR,itsFileInfo->name);

    if (Included!=NULL)
        // current line is an #include line
    {
        int rc;
        if ((rc = Included->NextLine(aLine,Spec,LineNumber)) <= 0)
            return rc;
        else
        {
            delete Included;
            Included=NULL;
        }
    }

    if (Spec)
        memcpy(Spec,itsFileInfo,sizeof(FILE_SPEC));

    typestr line;
    char buf[256];
    if (fgets(buf, 256, File)==NULL)
        return 1;
    line.str(buf);
    while (*buf && buf[strlen(buf)-1] != '\n')
    {
        if (fgets(buf, 256, File)==NULL)
            break;
        line.append(buf);
    }

    ++itsLineNumber;
    if (LineNumber) *LineNumber=itsLineNumber;

    // The current line begins with the '#include' word
    if (!memcmp(line.str(),"#include",8))
    {
        // looking for the name of the included file
        int i=9;
        while (line.str()[i] && line.str()[i]!='"')
            ++i;
        if (line.str()[i]!='"')
        {
            // '#include' directive not followed by a file name
            char Temporary[MAXPATH + 50];
            sprintf(Temporary,"%s : %d",Spec->name,itsLineNumber);
            itsErrorHandler->SetErrorD(9505,WARNING,Temporary);
        }
        else
        {
            int j=++i;
            while(line.str()[j])
            {
                if (line.str()[j]=='"') break;
                ++j;
            }

            line.str()[j] = '\0';

            FILE_SPEC IncludedFile;
            memcpy(&IncludedFile, itsFileInfo, sizeof(FILE_SPEC));
            if (strchr(&line.str()[i], SLASH))
                *IncludedFile.name = '\0';
            else
                copy_path_from_filename(IncludedFile.name, itsFileInfo->name);
            append_filename(IncludedFile.name, &line.str()[i]);
            Included=new TFile(&IncludedFile,itsErrorHandler,FILE_READ,FILE_ASCII);
            int rc=Included->Open();
            if (rc<0)
                return rc;
            rc=Included->NextLine(aLine,Spec,LineNumber);
            if (rc<=0)
                return rc;
            else
            {
                Included->Close();
                delete Included;
                Included=NULL;
            }
        }
    }
    if (line.str()[strlen(line.str())-1] == '\n')
        line.str()[strlen(line.str())-1] = '\0';

    char *comment_pos = strstr(line.str(), "//");
    if (comment_pos)
        *comment_pos = '\0';

    *aLine = line;

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// GetName
//
///////////////////////////////////////////////////////////////////////////////
FILE_SPEC *TFile::GetName(void)
{
    return itsFileInfo;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetName
//
///////////////////////////////////////////////////////////////////////////////
void TFile::SetName(FILE_SPEC *aName)
{
    itsFileInfo = aName;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetMode
//
///////////////////////////////////////////////////////////////////////////////
char TFile::GetMode(void)
{
    return itsMode;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetMode
//
///////////////////////////////////////////////////////////////////////////////
void TFile::SetMode(char aMode)
{
    itsMode = aMode;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetKind
//
///////////////////////////////////////////////////////////////////////////////
char TFile::GetKind(void)
{
    return itsKind;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetKind
//
///////////////////////////////////////////////////////////////////////////////
void TFile::SetKind(char aKind)
{
    itsKind = aKind;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetSize
//
///////////////////////////////////////////////////////////////////////////////
long TFile::GetSize(void)
{
    long    OldPosition,
        EndPosition;

    if (itsKind==FILE_BINARY)
    {
        OldPosition=lseek(iFile,0,SEEK_CUR);
        EndPosition=lseek(iFile,0,SEEK_END);
        lseek(iFile,OldPosition,SEEK_SET);
    }
    else
    {
        OldPosition=ftell(File);
        fseek(File,0,SEEK_END);
        EndPosition=ftell(File);
        fseek(File,OldPosition,SEEK_SET);
    }

    return EndPosition;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetPos
//
///////////////////////////////////////////////////////////////////////////////
long TFile::GetPos(void)
{
 if (itsKind==FILE_BINARY)
     return lseek(iFile,0,SEEK_CUR);
 else
     return ftell(File);
}

///////////////////////////////////////////////////////////////////////////////
//
// SkipBeginning
//
///////////////////////////////////////////////////////////////////////////////
int TFile::SkipBeginning()
{
    FILE_SPEC       aSpec;

    itsErrorHandler->Reset();

    typestr line;
    // Au debut de chaque fichier, il faut sauter deux lignes de commentaires
    if (NextLine(&line,&aSpec))
        itsErrorHandler->SetErrorD(9506,ERROR,aSpec.name);
    if(NextLine(&line,&aSpec))
        itsErrorHandler->SetErrorD(9506,ERROR,aSpec.name);
    return itsErrorHandler->GetErrorCode();
}

