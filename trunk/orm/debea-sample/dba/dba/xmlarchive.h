//
// C++ Interface: xmlarchive
//
// Description: 
//
//
// Author: Lukasz Michalski <lm@zork.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBAXMLARCHIVE_H
#define DBAXMLARCHIVE_H

#include "dba/archive.h"
#include "dba/xmlistream.h"
#include "dba/xmlostream.h"
#include "dba/xmlerrorhandler.h"
#include <libxml/tree.h>
#include <list>

namespace dba {

/**
  Archive for XML file format serialization using libxml2.

  This archive adds limited support for mapping xml document data to C++ objects using store tables.
  
Current limitations:
 - object identifiers are not supported.
 - class members specified in BIND_STR, BIND_INT, BIND_DAT and BIND_STB \
macros can be *all* stored as attributes or subelements, not both
 - class members specified in BIND_STR, BIND_INT, BIND_DAT and BIND_STB are loaded from \
subelements and attributes.
 - xml namespaces can be specified directly in storetable, but namespace declaration \
can be added only to root xml element.
 - xml elements are written with order specified in store table, but XMLArchive ignores \
order of readed elements - they are searched amon all nodes and attributes on current level.

*/
class dbaDLLEXPORT XMLArchive  : public Archive, public ConvSpecContainer, public XMLErrorHandler {
  public:
    XMLArchive();
    /**
      Set name of root node. If set to NULL then it is possible to IStream::get 
      only one object from archive file pointed by root element, 
      otherwise archive will try to get list of objects from subelement of root 
      element in xml file
      @param pName name of root object or NULL if elements from second level should be retrieved
    */
    void setRootNodeName(const char* pName);
    /**
      Add namespace to document. You have to call this method
      if there are namespace prefixed members in class store table
    */
    void addNamespace(const char* pName, const char* pPrefix);
    virtual void open(const char* pOpenStr);
    /**
      Open already created document in memory. Archive will not take 
      ownership on this document.
      @param pDocument libxml2 xmlDoc pointer to already created document
    */
    virtual void open(xmlDocPtr pDocument);
    /**
      Close archive and write xml document to file
    */
    void close();
    virtual bool isOpen() const { return mDocument != NULL; }
    void useElements(bool pFlag = true) { mUseElements = pFlag; }
    XMLOStream getOStream();
    XMLIStream getIStream();
    virtual IStream* getInputStream();
    virtual OStream* getOutputStream();
    virtual ~XMLArchive();
  private:
    xmlChar* mRootNodeName;
    /**
      pointer to xml document. NULL if archive is open in external document
    */ 
    xmlDocPtr mDocument;
    /**
      pointer to xml node that is root of archive. If archive is open using connect string
      then node points to root node of mDocument, otherwise node is set to 
      some node of external document
    */
    xmlNodePtr mRootNode;
    /**
      name of file where mDocument is serialized
      If empty then mDocument is supplied by user
    */
    std::string mFilename;
    /**
      should archive store/load members data from elements or attributes
    */
    bool mUseElements;
    
    /**
      Write contents of mDocument to file
    */
    void write();
    
    /**
      Convert ConvSpec::mDbEncoding constant to libxml2 encoding name
    */
    const char* getLibxmlEncoding();
    /**
      Convert libxml2 encoding name to ConvSpec constant
    */
    ConvSpec::charset getDebeaEncoding(const char* pLibxmlEncoding);
    /**
      update ConvSpec encoding
    */
    void updateEncoding();
};

} //namespace

#endif
