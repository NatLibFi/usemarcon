/*  
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *  
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2003
 *  Adapted by The National Library of Finland, 2004-2007
 *
 *  File:  tmarcdoc.cpp
 *
 *  implements a pseudo-document class to manage MARC data
 *
 */

#include "error.h"
#include "defines.h"
#include "tools.h"
#include "tmarcdoc.h"
#include "rulefile.h"

///////////////////////////////////////////////////////////////////////////////
//
// Document class constructor
//
// Create and initialize the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TMarcDoc::TMarcDoc(TUMApplication *Application) 
{
  itsInputFile           = NULL;
  itsOutputFile          = NULL;
  itsMarcInputSpec       = NULL; 
  itsMarcOutputSpec      = NULL;
  itsInputRecord         = NULL;
  itsTransRecord         = NULL;
  itsOutputRecord        = NULL;
  itsConfInputSpec       = NULL;
  itsConfOutputSpec      = NULL;
  itsFirstInputTagNoInd  = NULL;
  itsFirstOutputTagNoInd = NULL; 
  itsMarcInputFile.Spec       = NULL;
  itsMarcInputFile.Conf       = NULL;
  itsMarcInputFile.Format    = MFF_NONSEGMENTED;
  itsMarcInputFile.BlockSize  = 2048;
  itsMarcInputFile.MinDataFree= 5;
  itsMarcInputFile.PaddingChar= 0x5E;
  itsMarcInputFile.LastBlock  = false;

  itsMarcOutputFile.Spec       = NULL;
  itsMarcOutputFile.Conf       = NULL;
  itsMarcOutputFile.Format     = MFF_NONSEGMENTED;
  itsMarcOutputFile.BlockSize  = 2048;
  itsMarcOutputFile.MinDataFree= 5;
  itsMarcOutputFile.PaddingChar= 0x5E;
  itsMarcOutputFile.LastBlock  = false;

  itsApplication = Application;
  itsErrorHandler = Application->GetErrorHandler();
}

///////////////////////////////////////////////////////////////////////////////
//
// Document class destructor
//
// Delete the internal data objects for this class
//
///////////////////////////////////////////////////////////////////////////////
TMarcDoc::~TMarcDoc( void )
{ 
  if (itsInputFile)              { delete itsInputFile;                  itsInputFile                    = NULL; }
  if (itsOutputFile)             { delete itsOutputFile;                 itsOutputFile                   = NULL; }
  if (itsInputRecord)            { delete itsInputRecord;                itsInputRecord                  = NULL; }  
  if (itsTransRecord)            { delete itsTransRecord;                itsTransRecord                  = NULL; }  
  if (itsOutputRecord)           { delete itsOutputRecord;               itsOutputRecord                 = NULL; }   
  DelTreeTagNoInd(itsFirstInputTagNoInd);
  DelTreeTagNoInd(itsFirstOutputTagNoInd); 
}

void TMarcDoc::Reset(void)
{
  DelTreeTagNoInd(itsFirstInputTagNoInd);
  itsFirstInputTagNoInd = NULL;
  DelTreeTagNoInd(itsFirstOutputTagNoInd); 
  itsFirstOutputTagNoInd = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// Open
//
// Called by DoOpen().  Do the actual work of restoring the object from
// a data source.
//
///////////////////////////////////////////////////////////////////////////////
bool TMarcDoc::Open(int IO)
{
  switch (IO)
  {
  case INPUT: 
    if (!itsInputFile)          // there is no Marc Input File
    {   
      if ((itsInputFile = new TMarcFile(itsMarcInputFile.Spec,itsApplication,FILE_READ,FILE_BINARY,
        GetMarcInputFileFormat(), GetMarcInputFileBlockSize(), 
        GetMarcInputFileMinDataFree(), GetMarcInputFilePaddingChar(), 
        GetMarcInputFileLastBlock()))==NULL)
      {
        itsErrorHandler->SetError(9021,ERROR);
        return false;       
      }
    }   
    if (itsInputFile->Open())
    {
      delete itsInputFile;
      itsInputFile = NULL;
      return false;
    }
    
    if (itsInputRecord)
      delete itsInputRecord;
    if ((itsInputRecord = new TUMRecord(itsApplication))==NULL)
    {
      itsErrorHandler->SetError(9031,ERROR);
      return false;
    }
    
    if (!itsMarcInputFile.Conf.is_empty())
    { 
      if (GetFirstTagNoInd(INPUT))
        UnloadTagNoInd(INPUT);
      if (!LoadTagNoInd(INPUT, itsMarcInputFile.Conf.str()))
      {
          itsConfInputSpec = itsMarcInputFile.Conf;
      }
      itsMarcInputFile.Conf.freestr();
    }
    else
    {
      if (GetFirstTagNoInd(INPUT))
        UnloadTagNoInd(INPUT);
      itsConfInputSpec.freestr();
    }  
    break;
    
  case OUTPUT: 
    if (!itsOutputFile)         // there is no Marc Output File
    { 
      if ((itsOutputFile = new TMarcFile(itsMarcOutputFile.Spec,
        itsApplication,FILE_WRITE,FILE_BINARY,
        GetMarcOutputFileFormat(), GetMarcOutputFileBlockSize(), 
        GetMarcOutputFileMinDataFree(), GetMarcOutputFilePaddingChar(), 
        GetMarcOutputFileLastBlock()))==NULL)
      {
        itsErrorHandler->SetError(9022,ERROR);
        return false;
      }
    }
    if (itsOutputFile->Open())
    {
      delete itsOutputFile;
      itsOutputFile = NULL;
      return false;
    }
    
    if (itsOutputRecord)
      delete itsOutputRecord;
    if ((itsOutputRecord = new TUMRecord(itsApplication))==NULL)
    {
      itsErrorHandler->SetError(9032,ERROR);
      return false;
    }
    itsMarcOutputSpec = itsMarcOutputFile.Spec; 
    
    if (!itsMarcOutputFile.Conf.is_empty())
    { 
      if (GetFirstTagNoInd(OUTPUT))
        UnloadTagNoInd(OUTPUT);
      if (!LoadTagNoInd(OUTPUT, itsMarcOutputFile.Conf.str()))
      {
        itsConfOutputSpec = itsMarcOutputFile.Conf;
      }

      itsMarcOutputFile.Conf.freestr();
    }
    else 
    {
      if (GetFirstTagNoInd(OUTPUT))
        UnloadTagNoInd(OUTPUT);
      itsConfOutputSpec.freestr();
    }        
    break;
    
  default:
    return false;
    break;
  }
  return true;
}  

bool TMarcDoc::Close(int IO)  
{ 
  switch(IO)
  {
  case INPUT:
    if (itsInputFile)                                           // a Marc Input File is already loaded
    { 
      // Current Marc Input File closing is confirmed
      if (itsInputFile->Close())                             // Close the current Marc Input File
        return false;                                         // Current Marc Input Closing failure
      delete itsInputFile; 
      itsInputFile = NULL;
      itsMarcInputSpec.freestr();
    } 
    break;
  case OUTPUT: 
    if (itsOutputFile)                                          // a Marc Output File is already loaded
    { 
      if (itsOutputFile->Close())             // Close the current Marc Output File
        return false;
      delete itsOutputFile;                           // Current Marc Output Closing failure
      itsOutputFile = NULL;
      itsMarcOutputSpec.freestr();
    }  
    break;
  default:
    return false;
    break;      
  }  
  return true;                                               
}  

///////////////////////////////////////////////////////////////////////////////
//
// Convert
//
///////////////////////////////////////////////////////////////////////////////
int TMarcDoc::Convert(TRuleDoc *RuleDoc)
{   
  // On reset le EH
  itsErrorHandler->Reset();
  
  // On commence par convertir itsTransRecord sous la forme utilisable par la 
  // conversion de regles, a savoir une suite de CDLib. 
  itsTransRecord->ToCD();
  
  // Si une notice de sortie existe, on la vide
  itsOutputRecord->DelTreeCDLib();
  itsOutputRecord->DelTree();
  
  // On convertit alors la notice itsTransRecord en itsOutputRecord conformement
  // au fichier de regles
  if (RuleDoc->GetFile())
  {
    if (itsApplication->GetConvertInFieldOrder())
        RuleDoc->GetFile()->ConvertInFieldOrder(itsTransRecord,itsOutputRecord);
    else
        RuleDoc->GetFile()->ConvertInRuleOrder(itsTransRecord,itsOutputRecord);
  }
  else
  { 
    if (itsOutputRecord) { delete itsOutputRecord; itsOutputRecord=NULL; }
    itsOutputRecord = new TUMRecord(*itsTransRecord);  
    if (!itsOutputRecord)
      itsErrorHandler->SetErrorD(3000,ERROR,"When copying Input record to Output record" );
  }
  
  // On reconstruit alors la notice de sortie sous la forme d'une liste de champs,
  // a partir de la liste des CDLib.
  itsOutputRecord->FromCD(RuleDoc->GetFile());
  
  return itsErrorHandler->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// Transcode
//
///////////////////////////////////////////////////////////////////////////////
int TMarcDoc::Transcode(TTransDoc *aTransDoc)
{
  TCheckDoc             *aCheckDoc;
  
  // On reset le EH
  itsErrorHandler->Reset();      
  
  // Check input
  aCheckDoc= itsApplication->GetCheckDoc();
  if (aCheckDoc)
  {
    if (aCheckDoc->GetInputFile())
      aCheckDoc->GetInputFile()->Verify(INPUT,itsInputRecord);
  }
    
  // On reset le EH
  itsErrorHandler->Reset();
  
  // Si itsTransRecord existe, on la supprime
  if (itsTransRecord) delete itsTransRecord;
  // Si aucun fichier de transco n'a ete charge, itsTransRecord est une simple copie
  // de la notice d'entree.
  // Si un fichier existe, alors on convertit itsInputRecord en itsTransRecord conformement
  // a la table de transco chargee
  
  if (itsApplication->GetDisableCharacterConversion() || !aTransDoc->NeedTranscoding())
  {
    itsTransRecord = new TUMRecord(*itsInputRecord);
    if (!itsTransRecord)
      itsErrorHandler->SetErrorD( 3000, ERROR, "When copying Input record to Trans record" );
  }
  else
  {
    itsTransRecord=new TUMRecord(itsApplication);
    if (!itsTransRecord)
      itsErrorHandler->SetErrorD( 3000, ERROR, "When creating a new Trans record" );
    aTransDoc->Convert(itsInputRecord, itsTransRecord);
  }
  
  return itsErrorHandler->GetErrorCode();
}

///////////////////////////////////////////////////////////////////////////////
//
// DelTreeTagNoInd
//
///////////////////////////////////////////////////////////////////////////////
void TMarcDoc::DelTreeTagNoInd(TTagNoInd *Start)
{
  TTagNoInd      *Courant,
    *Suivant;
  
  Courant=Start;
  while (Courant)
  { 
    Suivant = Courant->GetNext();
    delete Courant;
    Courant = Suivant;
  }
  Start = NULL;                          
}          

///////////////////////////////////////////////////////////////////////////////
//
// SetMarcInputFileSpec
//
///////////////////////////////////////////////////////////////////////////////
void TMarcDoc::SetMarcInputFileSpec(typestr & theSpec)
{ 
  itsMarcInputFile.Spec = theSpec;
} 

///////////////////////////////////////////////////////////////////////////////
//
// SetConfFileSpec
//
///////////////////////////////////////////////////////////////////////////////
void TMarcDoc::SetConfInputFileSpec(typestr & theSpec)
{
  itsMarcInputFile.Conf = theSpec; 
}

///////////////////////////////////////////////////////////////////////////////
//
// SetMarcOutputFileSpec
//
///////////////////////////////////////////////////////////////////////////////
void TMarcDoc::SetMarcOutputFileSpec(typestr & theSpec)
{ 
  itsMarcOutputFile.Spec = theSpec;
} 

///////////////////////////////////////////////////////////////////////////////
//
// SetConfOutputFileSpec
//
///////////////////////////////////////////////////////////////////////////////
void TMarcDoc::SetConfOutputFileSpec(typestr & theSpec)
{
  itsMarcOutputFile.Conf = theSpec;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetFirstTagNoInd
//
///////////////////////////////////////////////////////////////////////////////
void TMarcDoc::SetFirstTagNoInd(int IO,TTagNoInd *aTagNoInd)
{
  switch(IO)
  {
  case INPUT   : itsFirstInputTagNoInd = aTagNoInd;    break;
  case OUTPUT  : itsFirstOutputTagNoInd= aTagNoInd;    break;
  default      : break;
  } 
} 

///////////////////////////////////////////////////////////////////////////////
//
// GetFirstTagNoInd
//
///////////////////////////////////////////////////////////////////////////////
TTagNoInd *TMarcDoc::GetFirstTagNoInd(int IO)
{    
  switch(IO)
  {
  case INPUT   : return itsFirstInputTagNoInd;
  case OUTPUT  : return itsFirstOutputTagNoInd;
  default      : return NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// UnloadTagNoInd
//
///////////////////////////////////////////////////////////////////////////////
int TMarcDoc::UnloadTagNoInd(int IO)
{
  TTagNoInd      *aTagNoInd;
  
  switch (IO)
  {
  case INPUT   : 
    aTagNoInd = itsFirstInputTagNoInd;
    itsInputRecord->SetFirstInputTNI(NULL);
    break;  
    
  case OUTPUT  :
    aTagNoInd = itsFirstOutputTagNoInd;
    itsOutputRecord->SetFirstOutputTNI(NULL);
    break;
    
  default              : 
    aTagNoInd = NULL;
    break;
  }
  DelTreeTagNoInd(aTagNoInd);
  return 0;  
}   

///////////////////////////////////////////////////////////////////////////////
//
// LoadTagNoInd
//
///////////////////////////////////////////////////////////////////////////////
int TMarcDoc::LoadTagNoInd(int IO, char *Path)
{ 
  FILE           *Fic;
  TTagNoInd      *Current,
    *Previous;
    
  if (!*Path || (Fic=fopen(Path,"r"))==NULL)
    return 1;
  
  Current = new TTagNoInd;
  if (!Current)
  {
    fclose(Fic);
    return 1;
  }

  switch (IO)
  {
  case INPUT   : itsFirstInputTagNoInd = Current;      break;
  case OUTPUT  : itsFirstOutputTagNoInd= Current;      break;
  default      : fclose(Fic); return 1;
  }
  Previous = Current;

  typestr line;
  while (readline(line, Fic) > 0)
  {
    Current->SetTag(line.str());
    Current->SetNext(new TTagNoInd);
    if (!Current->GetNext())
    {
      fclose(Fic);
      return 1;
    }
    Previous = Current;
    Current = Current->GetNext(); 
  }
  if (Previous->GetNext())                              
    delete Previous->GetNext();
  Previous->SetNext(NULL);
  
  fclose(Fic);
  
  switch (IO)
  {
  case INPUT   : itsInputRecord->SetFirstInputTNI(itsFirstInputTagNoInd);      break;
  case OUTPUT  : itsOutputRecord->SetFirstOutputTNI(itsFirstOutputTagNoInd);   break;
  default      : return 1;
  } 
  return 0;
} 

///////////////////////////////////////////////////////////////////////////////
//
// SetIndSeparatorsID
//
///////////////////////////////////////////////////////////////////////////////
void TMarcDoc::SetIndSeparatorsID(int IO, long ID)
{ 
  switch (IO)
  {
  case INPUT   : itsInputRecord->SetIndSeparatorsID(IO,ID);    break;
  case OUTPUT  : itsOutputRecord->SetIndSeparatorsID(IO,ID);   break;
  default      : break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// GetIndSeparatorsID
//
///////////////////////////////////////////////////////////////////////////////
long TMarcDoc::GetIndSeparatorsID(int IO)
{ 
  switch (IO)
  {
  case INPUT   : return (itsInputRecord ? itsInputRecord->GetIndSeparatorsID(IO) : 0);
  case OUTPUT  : return (itsOutputRecord ? itsOutputRecord->GetIndSeparatorsID(IO) : 0);
  default      : return 0;
  }
} 

int TMarcDoc::Read(void)
{ 
  return itsInputFile->Read(itsInputRecord); 
}

int TMarcDoc::Write(void)          
{ 
  return itsOutputFile->Write(itsOutputRecord); 
}

int TMarcDoc::ProcessDuplicateFields()
{
  return itsOutputRecord->ProcessDuplicateFields(m_duplicateSubfields, m_duplicateFields);
}
