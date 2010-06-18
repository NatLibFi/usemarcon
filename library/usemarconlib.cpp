/*
 *  USEMARCON Software - Command Line version
 *  Copyright The British Library, The USEMarcon Consortium, 1995-2000
 *  Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
 *
 *  Adapted by ATP Library Systems Ltd, Finland, 2002-2004
 *  Adapted by The National Library of Finland, 2004-2010
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
    m_disable_character_conversion(false), m_errorCount(0), m_warningCount(0),
    m_configOverrides(NULL)
{
    m_lastErrorMessage[0] = '\0';
    m_lastWarningMessage[0] = '\0';

    m_application = new TUMApplication();
}

Usemarcon::~Usemarcon()
{
    free(m_configOverrides);
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

void Usemarcon::SetMarcRecord(const char *a_record, size_t a_length)
{
    free(m_record);

    m_length = a_length;
    m_record = reinterpret_cast<char *>(malloc(m_length + 1));
    memset(m_record, 0, m_length + 1);
    memcpy(m_record, a_record, m_length);
}

void Usemarcon::GetMarcRecord(char *&a_record, size_t &a_length)
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

void Usemarcon::SetLastWarningMessage(const char *a_msg)
{
    strncpy(m_lastWarningMessage, a_msg, 255);
    m_lastWarningMessage[254] = '\0';
}

int Usemarcon::Convert()
{
    int res = 0;

    TUMApplication *application = reinterpret_cast<TUMApplication*>(m_application);
    if (!m_initialized)
    {
        res = application->Initialize(m_iniFileName, m_interactive,
            m_record, m_length, m_force_verbose, m_inputMarcFileName,
            m_outputMarcFileName, m_disable_character_conversion, m_configOverrides);
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
    {
        SetLastErrorMessage(application->GetStateManager()->GetLastErrorMessage());
        SetLastWarningMessage(application->GetStateManager()->GetLastWarningMessage());
        m_errorCount = application->GetStateManager()->GetErrorCount();
        m_warningCount = application->GetStateManager()->GetWarningCount();
    }
    else
    {
        SetLastErrorMessage("");
        SetLastWarningMessage("");
        m_errorCount = 0;
        m_warningCount = 0;
    }

    if (m_interactive)
    {
        printf("\n");
    }
        
    return res;
}

void Usemarcon::AddConfigOverride(const char *a_section, const char *a_setting, const char *a_value)
{
    size_t needed = (m_configOverrides ? strlen(m_configOverrides) : 0) + 
        strlen(a_section) + strlen(a_setting) + strlen(a_value) + 6;

    char* newstr = (char*) malloc(needed);
    if (m_configOverrides)
    {
        strcpy(newstr, m_configOverrides);
        free(m_configOverrides);
        strcat(newstr, "\t\t");
    }
    else
        *newstr = '\0';
    strcat(newstr, a_section);
    strcat(newstr, "\t");
    strcat(newstr, a_setting);
    strcat(newstr, "\t");
    strcat(newstr, a_value);
    m_configOverrides = newstr;
    m_initialized = false;
}

void Usemarcon::ClearConfigOverrides()
{
    free(m_configOverrides);
    m_configOverrides = NULL;
    m_initialized = false;
}
