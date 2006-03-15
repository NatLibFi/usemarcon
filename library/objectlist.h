/* 
 *  USEMARCON Software - Command Line version
 *  
 *  File:  objectlist.h
 *
 *
 *  NOTE:  IN NO WAY WHATSOEVER SHOULD THIS FILE BE USED IN THE EARLIER
 *         VERSIONS OF USEMARCON SOFTWARE.
 *

CLASS
    CDetails

    implements an interface to the conversion

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited

*/

#ifndef objectlist_h
#define objectlist_h

#include "defines.h"
#include "tmpplctn.h"
#include "error.h"


#ifndef OBJECT_DEFS
#define EXTERN extern
#else
#define EXTERN
#endif

#define ILLEGAL_LENGTH_RECORD "Illegal length marc record"

// An object used to pass data into the rest of the application
class CDetails 
{
private:
    bool m_interactive;
    char *m_pcRecord;
    int m_iLength;
    char *m_pcIniFileName;
    char m_lastErrorMessage[255];
    TUMApplication m_application;
    int m_argc;
    char **m_argv;
    bool m_initialized;
    char *m_inputMarcFileName;
    char *m_outputMarcFileName;
    bool m_force_verbose;
    bool m_disable_character_conversion;
    
public:
    CDetails()
    {
        m_pcRecord = NULL;
        m_iLength = 0;
        m_pcIniFileName = NULL;
        m_lastErrorMessage[0] = '\0';
        m_pcIniFileName = NULL;
        m_inputMarcFileName = NULL;
        m_outputMarcFileName = NULL;
        m_initialized = false;
        m_interactive = false;
        m_force_verbose = false;
        m_disable_character_conversion = false;
    };

    // Destructor called when the object goes out of scope
    ~CDetails()
    {
        DeleteData();

        if (m_pcIniFileName)
            free(m_pcIniFileName);

        if (m_inputMarcFileName)
            free(m_inputMarcFileName);
            
        if (m_outputMarcFileName)
            free(m_outputMarcFileName);
    }

    void DeleteData(void)
    {
        if (m_pcRecord)
        {
            free(m_pcRecord);
            m_pcRecord = NULL;
        }
    }

    void SetForceVerbose(bool fValue) { m_force_verbose = fValue; }
    bool GetForceVerbose(void) { return m_force_verbose; }

    void SetDisableCharacterConversion(bool fValue) { m_disable_character_conversion = fValue; }
    bool GetDisableCharacterConversion(void) { return m_disable_character_conversion; }

    void SetInteractive(bool fValue)
    {
        if (m_interactive == fValue)
            return;

        m_interactive = fValue;
        m_initialized = false;
    }

    bool GetInteractive(void) { return m_interactive; }

    void SetMarcRecord(const char *pcRecord, int iLength)
    {
        m_iLength = iLength;
        // Remove any old instance
        DeleteData();

        if (m_iLength <= TBUF)
        {
            m_pcRecord = reinterpret_cast<char *>(malloc(m_iLength + 1));
            memset(m_pcRecord, 0, m_iLength + 1);
            memcpy(m_pcRecord, pcRecord, m_iLength);
        }
        else
        {
            m_pcRecord = reinterpret_cast<char *>(malloc(strlen(ILLEGAL_LENGTH_RECORD) + 1));
            strcpy(m_pcRecord, ILLEGAL_LENGTH_RECORD);
            m_iLength = 0;
        }
    }

    void GetMarcRecord(char *&pcRecord, int &iLength)
    {
        pcRecord = strdup(m_pcRecord);
        iLength = m_iLength;
    }

    const char *GetIniFileName()
    {
        return m_pcIniFileName;
    }

    void SetIniFileName(const char *pcIniFile)
    {
        if (m_pcIniFileName && !strcmp(m_pcIniFileName, pcIniFile))
            return;
        if (m_pcIniFileName)
            free(m_pcIniFileName);
        m_pcIniFileName = strdup(pcIniFile);
        m_initialized = false;
    }

    const char *GetInputMarcFileName()
    {
        return m_inputMarcFileName;
    }

    void SetInputMarcFileName(const char *fname)
    {
        if (m_inputMarcFileName)
            free(m_inputMarcFileName);
        m_inputMarcFileName = strdup(fname);
    }

    const char *GetOutputMarcFileName()
    {
        return m_outputMarcFileName;
    }
    
    void SetOutputMarcFileName(const char *fname)
    {
        if (m_outputMarcFileName)
            free(m_outputMarcFileName);
        m_outputMarcFileName = strdup(fname);
    }
    
    char *GetLastErrorMessage()
    {
        return m_lastErrorMessage;
    }

    void SetLastErrorMessage(const char *msg)
    {
        strncpy(m_lastErrorMessage, msg, 255);
        m_lastErrorMessage[254] = '\0';
    }

    int Start(void)
    {
        int res = 0;
        if (!m_initialized)
        {
            res = m_application.StartUp(this);
            if (!res)
                m_initialized = true;
        }
        if (!res)
        {
            res = m_application.Convert();
            m_application.End();
        }

        if (m_application.GetErrorHandler())
            SetLastErrorMessage(m_application.GetErrorHandler()->GetLastErrorMessage());

        if (m_interactive)
        {
            printf("\n");
        }
            
        return res;
    }
};

#endif
