/* 
 *  USEMARCON Software - Command Line version
 *  
 *  File:  usemarconlib.h
 *
 *
 
CLASS
    CUsemarcon

    implements an interface to the conversion

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by ATP Library Systems Ltd, Finland, 2002-2004
    Adapted by The National Library of Finland, 2004-2010

AUTHOR
    The National Library of Finland

*/

#ifndef usemarconlib_h
#define usemarconlib_h

class Usemarcon 
{
private:
    void *m_application;
    bool m_interactive;
    char *m_record;
    size_t m_length;
    char m_lastErrorMessage[255];
    char m_lastWarningMessage[255];
    bool m_initialized;
    bool m_force_verbose;
    bool m_disable_character_conversion;
    char *m_iniFileName;
    char *m_inputMarcFileName;
    char *m_outputMarcFileName;
    int m_errorCount;
    int m_warningCount;
    char *m_configOverrides;

    void SetLastErrorMessage(const char *a_msg);
    void SetLastWarningMessage(const char *a_msg);

public:
    Usemarcon(); 

    ~Usemarcon();

    void SetForceVerbose(bool a_value) { m_force_verbose = a_value; }
    bool GetForceVerbose() { return m_force_verbose; }

    void SetDisableCharacterConversion(bool a_value) { m_disable_character_conversion = a_value; }
    bool GetDisableCharacterConversion() { return m_disable_character_conversion; }

    void SetInteractive(bool a_value);
    bool GetInteractive() { return m_interactive; }

    bool GetMarcRecordAvailable() { return m_record ? true : false; }

    size_t GetMarcRecordLength() { return m_length; }

    void SetMarcRecord(const char *a_record, size_t a_length);
    void GetMarcRecord(char *&a_record, size_t &a_length);

    const char *GetIniFileName() { return m_iniFileName; }

    void SetIniFileName(const char *a_iniFile);

    void SetInputMarcFileName(const char *a_filename);
    const char *GetInputMarcFileName() { return m_inputMarcFileName; }

    void SetOutputMarcFileName(const char *a_filename);
    const char *GetOutputMarcFileName() { return m_outputMarcFileName; }
    
    const char *GetLastErrorMessage() const { return m_lastErrorMessage; }
    const char *GetLastWarningMessage() const { return m_lastWarningMessage; }

    int GetErrorCount() { return m_errorCount; }
    int GetWarningCount() { return m_warningCount; }

    void AddConfigOverride(const char *a_section, const char *a_setting, const char *a_value);
    void ClearConfigOverrides();

    int Convert();
};

#endif
