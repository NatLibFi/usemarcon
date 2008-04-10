/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2008
 *
 *  File:  tfile.cpp
 *
 *  implements a base class to manage file input./output
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

TFile::TFile(const typestr & FileInfo, TStateManager *StateManager, char Mode, char Kind)
{
    itsFileInfo = FileInfo;
    itsMode     = Mode;
    itsKind     = Kind;
    itsLineNumber = 0;

    Included        = NULL;
    File            = NULL;
    iFile           = -1;

    mStateManager = StateManager;
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
        if ((File=fopen(itsFileInfo.str(), mode))==NULL)
        {
            return mStateManager->SetErrorD(9501, WARNING, itsFileInfo.str());
        }
    }
    else
    {
        if (itsMode==FILE_READ)
            iFile=::l_open(itsFileInfo.str(),O_RDONLY|O_BINARY,0640);
        else
            iFile=::l_open(itsFileInfo.str(),O_CREAT|O_WRONLY|O_TRUNC|O_BINARY,0640);

        if (iFile == -1)
            return mStateManager->SetErrorD(9502, WARNING, itsFileInfo.str());
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
            l_close(iFile);
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
int TFile::NextLine(typestr *aLine, typestr *Spec, int *LineNumber)
{
    // NextLine() can't operate on binary files
    if (itsKind == FILE_BINARY)
        return mStateManager->SetErrorD(9504, ERROR, itsFileInfo.str());

    if (Included)
        // current line is an #include line
    {
        int rc;
        if ((rc = Included->NextLine(aLine, Spec, LineNumber)) <= 0)
            return rc;
        else
        {
            delete Included;
            Included=NULL;
        }
    }

    if (Spec)
        *Spec = itsFileInfo;

    typestr line;
    int lines_read;
    if (!(lines_read = readline(line, File)))
        return 1;

    itsLineNumber += lines_read;
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
            typestr tmp; 
            tmp.allocstr(strlen(Spec->str()) + 50);
            sprintf(tmp.str(), "%s : %d", Spec, itsLineNumber);
            mStateManager->SetErrorD(9505, WARNING, tmp.str());
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

            typestr IncludedFile = itsFileInfo;
            if (strchr(&line.str()[i], SLASH))
                *IncludedFile.str() = '\0';
            else
                copy_path_from_filename(IncludedFile, itsFileInfo.str());
            append_filename(IncludedFile, &line.str()[i]);
            Included=new TFile(IncludedFile, mStateManager, FILE_READ,FILE_ASCII);
            int rc=Included->Open();
            if (rc<0)
                return rc;
            rc=Included->NextLine(aLine, Spec, LineNumber);
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
typestr TFile::GetName(void)
{
    return itsFileInfo;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetName
//
///////////////////////////////////////////////////////////////////////////////
void TFile::SetName(typestr & aName)
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
        OldPosition=l_lseek(iFile,0,SEEK_CUR);
        EndPosition=l_lseek(iFile,0,SEEK_END);
        l_lseek(iFile,OldPosition,SEEK_SET);
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
     return l_lseek(iFile,0,SEEK_CUR);
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
    mStateManager->Reset();

    typestr line;
    typestr aSpec;
    // Au debut de chaque fichier, il faut sauter deux lignes de commentaires
    if (NextLine(&line, &aSpec))
        mStateManager->SetErrorD(9506, ERROR, aSpec.str());
    if(NextLine(&line, &aSpec))
        mStateManager->SetErrorD(9506, ERROR, aSpec.str());
    return mStateManager->GetErrorCode();
}

