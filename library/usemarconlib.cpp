/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2009
 *
 *  File:  usemarconlib.cpp
 *
 *  implements an interface to the conversion
 *
 */


#include <stdlib.h>
#include "tmpplctn.h"
#include "usemarconlib.h"

Usemarcon::Usemarcon() : m_record(NULL), m_length(0), m_iniFileName(NULL),
    m_inputMarcFileName(NULL), m_outputMarcFileName(NULL),
    m_initialized(false), m_interactive(false), m_force_verbose(false), 
    m_disable_character_conversion(false)
{
    m_lastErrorMessage[0] = '\0';

    m_application = new TUMApplication();
}

Usemarcon::~Usemarcon()
{
    free(m_record);

    if (m_iniFileName)
        free(m_iniFileName);

    if (m_inputMarcFileName)
        free(m_inputMarcFileName);
        
    if (m_outputMarcFileName)
        free(m_outputMarcFileName);

    delete (TUMApplication*) m_application;
}

void Usemarcon::SetInteractive(bool a_value)
{
    if (m_interactive == a_value)
        return;

    m_interactive = a_value;
    m_initialized = false;
}

void Usemarcon::SetMarcRecord(const char *a_record, int a_length)
{
    free(m_record);

    m_length = a_length;
    m_record = reinterpret_cast<char *>(malloc(m_length + 1));
    memset(m_record, 0, m_length + 1);
    memcpy(m_record, a_record, m_length);
}

void Usemarcon::GetMarcRecord(char *&a_record, int &a_length)
{
    a_record = m_record ? l_strdup(m_record) : NULL;
    a_length = m_length;
}

void Usemarcon::SetIniFileName(const char *a_iniFile)
{
    if (m_iniFileName && !strcmp(m_iniFileName, a_iniFile))
        return;
    if (m_iniFileName)
        free(m_iniFileName);
    m_iniFileName = l_strdup(a_iniFile);
    m_initialized = false;
}

void Usemarcon::SetInputMarcFileName(const char *a_filename)
{
    if (m_inputMarcFileName)
        free(m_inputMarcFileName);
    m_inputMarcFileName = l_strdup(a_filename);
}

void Usemarcon::SetOutputMarcFileName(const char *a_filename)
{
    if (m_outputMarcFileName)
        free(m_outputMarcFileName);
    m_outputMarcFileName = l_strdup(a_filename);
}

void Usemarcon::SetLastErrorMessage(const char *a_msg)
{
    strncpy(m_lastErrorMessage, a_msg, 255);
    m_lastErrorMessage[254] = '\0';
}

int Usemarcon::Convert()
{
    int res = 0;

    TUMApplication *application = reinterpret_cast<TUMApplication*>(m_application);
    if (!m_initialized)
    {
        res = application->StartUp(m_iniFileName, m_interactive,
            m_record, m_length, m_force_verbose, m_inputMarcFileName,
            m_outputMarcFileName, m_disable_character_conversion);
        if (!res)
            m_initialized = true;
    }
    else
    {
        application->SetMarcRecord(m_record, m_length);
    }
    if (!res)
    {
        res = application->Convert();
        if (m_length)
        {
            free(m_record);
            application->GetMarcRecord(m_record, m_length);
        }
        application->End();
    }

    if (application->GetStateManager())
        SetLastErrorMessage(application->GetStateManager()->GetLastErrorMessage());

    if (m_interactive)
    {
        printf("\n");
    }
        
    return res;
}
