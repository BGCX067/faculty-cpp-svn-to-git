//
// C++ Interface: xmlistream
//
// Description: 
//
//
// Author: Lukasz Michalski <lm@zork.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBAXMLISTREAM_H
#define DBAXMLISTREAM_H

#include "dba/istream.h"
#include "dba/xmlerrorhandler.h"
#include <libxml/tree.h>

namespace dba {

/**
Object input stream for XMLArchive
*/
class dbaDLLEXPORT XMLIStream : public IStream, public XMLErrorHandler, public ConvSpecContainer {
  public:
    XMLIStream(xmlDocPtr pDocument, xmlNodePtr pNode, const ConvSpec& pSpecs, bool pUseElements);
    /**
      If set to true, then stream will igore all nodes that are not mapped to class
      members or binded to variables. 
      @note Not implemented yet - needs store table reorganization
      @param pFlag true if stream should ignore not mapped elements, false (the default) otherwise
    */
    void ignoreNonMappedNodes(bool pFlag);
    /**
      If set to true, then stream will igore order of readed elements and try to 
      find element using its name among all elements on single level. Otherwise
      element order in xml file has to be the same as store table layout. Elements
      from parent store table should be placed before elements in child store table.
      @note Not implemented yet - needs store table reorganization
      @param pFlag true if stream should element order, false (the default) otherwise
    */
    void ignoreNodeOrder(bool pFlag);
    virtual void close();
    virtual void destroy();
    virtual bool getNext(Storeable* pObject);
    virtual bool updateVars() { return updateVars(mCurrentNode); }
    virtual bool isCollectionFilterSupported() const { return true; };
    virtual bool get(Storeable* pObject, const char* pRootTable = NULL) { return IStream::get(pObject, pRootTable); }
    virtual bool get(CollectionFilterBase* pFilter, const char* pRootTable = NULL);
    virtual ~XMLIStream();
  private:
    xmlDocPtr mDocument;
    xmlNodePtr mParentNode;
    xmlNodePtr mCurrentNode;
    bool mIgnoreNonMappedNodes;
    bool mIgnoreOrder;
    bool mUseElements;
    
    virtual void setIdsCondition(const char* pFKeyName, id pRelationId, const std::vector<id>& pIds);
    void applyFilters(Storeable* pObject, xmlNodePtr pNode);
    bool updateVars(xmlNodePtr pNode);
    xmlNodePtr setNextNode(xmlNodePtr pNode);
    xmlNodePtr findNonTextNode(xmlNodePtr pNode);
    void getChildren(Storeable* pParent, xmlNodePtr pNode);
    void updateCollection(Storeable* pParent, ColMemberEntry* pEntry, xmlNodePtr pNode);
    xmlAttrPtr findAttribute(xmlNodePtr pNode, const char* pMemberName);
    xmlNodePtr findNode(xmlNodePtr pNode, const char* pName);
    void moveCurrentNode(const char* pName);
    xmlNsPtr getXMLNamespace(const char* pName, xmlNodePtr pNode);
};

} //namespace

#endif
