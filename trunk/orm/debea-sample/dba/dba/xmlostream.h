//
// C++ Interface: xmlostream
//
// Description: 
//
//
// Author: Lukasz Michalski <lm@zork.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBAXMLOSTREAM_H
#define DBAXMLOSTREAM_H

#include "dba/ostream.h"
#include "dba/xmlerrorhandler.h"
#include <libxml/tree.h>

namespace dba {

/**
Object output stream for XMLArchive
*/
class dbaDLLEXPORT XMLOStream  : public OStream, public XMLErrorHandler, public ConvSpecContainer {
  public:
    XMLOStream(xmlDocPtr pDocument, xmlNodePtr pNode, bool pReplaceParentNode, const ConvSpec& pSpecs, bool pUseElements);
    virtual void close();
    virtual void destroy();
    virtual void begin() { throw APIException("begin() not supported for XML format"); }
    virtual void commit() { throw APIException("commit() not supported for XML format"); }
    virtual void rollback() { throw APIException("rollback() not supported for XML format"); }
    virtual ~XMLOStream();
  protected:
    virtual void assignId(Storeable* pObject) throw (Exception);
    virtual bool erase(Storeable* pObject);
    virtual bool update(Storeable* pObject);
    virtual bool store(Storeable* pObject);
    virtual bool isCollectionFilterSupported() const { return true; };
    virtual bool putMemberChildren(Storeable* pObject, ColMemberEntry& pMember, CollectionFilterBase& pFilter, const char* pTableName);
    virtual std::vector<id> loadRefData(const char* pTable, const char* pFkName, id pCollId, id pId);
    virtual bool deleteRefData(const std::vector<id>& pIds, const char* pTableName);
    virtual void initMemberChildrenStore(const Storeable& pObject, const ColMemberEntry& pMember);
    virtual void endMemberChildrenStore(const Storeable& pObject, const ColMemberEntry& pMember);
  private:
    xmlDocPtr mDocument;
    xmlNodePtr mParentNode;
    xmlNodePtr mLastAddedNode;
    bool mReplaceParentNode;
    bool mUseElements;
    
    void updateNodeFromVars(xmlNodePtr pNode, mt_class* pTable);
    void updateNodeFromObject(xmlNodePtr pNode, const Storeable& pObject, mt_class* pTable);
    void setAttribute(xmlNodePtr pNode, const char* pName, const xmlChar* pData);
    xmlNodePtr createNode(const char* pName);
    void updateNodeData(xmlNodePtr pNode, const char* pName, const char* pContent);
};

} //namespace

#endif
