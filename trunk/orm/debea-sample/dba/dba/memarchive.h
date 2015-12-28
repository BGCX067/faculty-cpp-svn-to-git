// File: memarchive.h
// Purpose: In-memory archive (not implemented yet)
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2005
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAMEMARCHIVE_H
#define DBAMEMARCHIVE_H

#include "dba/archive.h"
#include "dba/istream.h"
#include "dba/ostream.h"

namespace dba {

/**@internal
  Memory Input stream (unimplemented)
*/
class dbaDLLEXPORT MemIStream : public IStream {
  public:
    MemIStream() : IStream() {};
  private:
    virtual void open(dba::Storeable& pObj, const char* pTable = NULL) {};
    virtual bool getNext(Storeable* pObject) { return false; };
    virtual bool updateVars() { return false; };
    virtual void close() {};
    virtual void destroy() {};
    virtual const ConvSpec& getConversionSpecs() const { return mSpecs; };
    ConvSpec mSpecs;
};

class dbaDLLEXPORT MemOStream : public OStream {
  public:
    MemOStream() : OStream() {}
    virtual void open(const char* pTable = NULL) { mIsOpen = true; };
    virtual void begin() {};
    virtual void commit() {};
    virtual void rollback() {};
    virtual void close() {};
    virtual void destroy() {};
    virtual void assignId(Storeable* pObject) throw (Exception) {};
  private:
    virtual bool erase(Storeable* pObject) { Stream::makeOk(pObject); return true; };
    virtual bool update(Storeable* pObject) { Stream::makeOk(pObject); return true; };
    virtual bool store(Storeable* pObject) { Stream::makeOk(pObject); return true; };
    virtual const ConvSpec& getConversionSpecs() const { return mSpecs; };
    ConvSpec mSpecs;
};

/**
In-memory database (not implemented yet)
Can be used to create Storeable objects in OK state without using any database
*/
class dbaDLLEXPORT MemArchive : public Archive {
  public:
    MemArchive();
    virtual void open(const char* pOpenStr) {};
    /**
      Get stream for reading objects
    */
    MemIStream getIStream() { return MemIStream(); };
    /**
      Get stream for writing objects
    */
    MemOStream getOStream() { return MemOStream(); };
    virtual bool isOpen() const { return true; };
    ~MemArchive();
  private:
};


}

#endif
