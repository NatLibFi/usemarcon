/*
 *  USEMARCON Software - Command Line version
 *
 *  File:  tmarcrec.h
 *
 *
 
CLASS
    TMarcRecord

    implements a class to manage MARC record

OVERVIEW TEXT
    USEMARCON Software - Command Line version
    Copyright The British Library, The USEMarcon Consortium, 1995-2000
    Adapted by Crossnet Systems Limited - British Library Contract No. BSDS 851
    Adapted by ATP Library Systems Ltd, Finland, 2002-2003
    Adapted by The National Library of Finland, 2004-2011

AUTHOR
    Crossnet Systems Limited
    ATP Library Systems Limited
    The National Library of Finland

*/

#ifndef _TMARCREC_H_
#define _TMARCREC_H_

#include "mrcfield.h"
#include "tagnoind.h"
#include "statemanager.h"

// forward declaration
class CDetails;

class TMarcRecord
{
public:
    TMarcRecord         (TUMApplication *Application);
    TMarcRecord         (const TMarcRecord &aRecord);
    virtual ~TMarcRecord        ();

    TMarcRecord & operator=(const TMarcRecord &rec);

    int           NextField           (TMarcField** Field, char *Tag);
    int           FromString          (char *MarcString);
    int           FromXMLString       (typestr & a_xml);
    int           ToString            (typestr & a_marcstr);
    int           ToXMLString         (typestr & a_xml);
    void          DelTree             (void);
    const char    *GetLeader          (void) const        { return itsLeader; };
    int           SetLeader           (const char *aLeader);
    TMarcField    *GetFirstField      (void) const        { return itsFirstField; };
    void          SetFirstField       (TMarcField *Field) { itsFirstField=Field; };
    TTagNoInd     *GetFirstInputTNI   (void) const        { return itsFirstInputTNI; };
    TTagNoInd     *GetFirstOutputTNI  (void) const        { return itsFirstOutputTNI; };
    void          SetFirstInputTNI    (TTagNoInd *aTNI)   { itsFirstInputTNI=aTNI; };
    void          SetFirstOutputTNI   (TTagNoInd *aTNI)   { itsFirstOutputTNI=aTNI; };
    void          SetIndSeparatorsID  (int IO,long ID);
    long          GetIndSeparatorsID  (int IO) const;
    const char*   GetRecordId() const { return m_recordId; }
    int           ProcessDuplicateFields(DUPLICATE_PROCESSING_MODE a_subfields, DUPLICATE_PROCESSING_MODE a_fields);

private:
    int                   Val                 (char *buffer, unsigned long *valeur);
    int                   LongVal             (char *buffer, unsigned long *valeur);
    bool                  IsFieldWithIndicators(int IO, const char *tag, const char *field, size_t fieldlen);
    bool                  IsItAFieldWithIndicators(const char *tag, int IO);
    bool                  get_tag(typestr & a_xml, typestr & a_tag, typestr & a_attribs, typestr & a_content, typestr & a_remainder);
    typestr               get_attrib(const char *a_name, typestr & a_attribs);
    void                  normalize_tag(typestr & a_tag);
    typestr               escape_xml(const typestr & a_str);
    typestr               unescape_xml(const typestr & a_str);

protected:
    char                  itsLeader[25];
    TMarcField            *itsFirstField;
    TTagNoInd             *itsFirstInputTNI;
    TTagNoInd             *itsFirstOutputTNI;
    long                  itsInputIndSeparatorsID;
    long                  itsOutputIndSeparatorsID;
    char                  m_recordId[50];

    TStateManager                *mStateManager;

    CDetails              *itsDetails;
};

#endif
