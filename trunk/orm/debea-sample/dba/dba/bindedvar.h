// File: bindedvar.h
// Purpose: Base class for binded variables
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBABINDEDVAR_H
#define DBABINDEDVAR_H

#include <string>
#include "dba/defs.h"
#include "dba/shared_ptr.h"

namespace dba {

class Stream;

/**@internal
  Base class for binded variables
*/
class dbaDLLEXPORT BindedBase {
  protected:
    BindedBase(Stream* pOwner, void* pPtr);
    ~BindedBase();
  private:
    Stream* mOwner;
    void* mVarPtr;
};

/**@internal
  BinedInt is an int variable that is binded to some column in some table in database.
  When user gets next object from OStream all binded variables are updated.
  @deprecated use Stream::bind
  */
class dbaDLLEXPORT BindedInt : public BindedBase {
    friend class Stream;
  public:
    /**
      assign new value to object
    */
    int operator=(int i);
    /**
      convert object to int
    */
    int& operator*() const;
    /**
      cast object to id
    */
    operator int() const;
    /**
      @return int value
    */
    int getVal() const;
    /**
      Destructor
    */
    ~BindedInt();
  private:
    BindedInt(Stream* pOwner,int* pVar);
    shared_ptr<int> mValue;
};

/**@internal
  id that is binded to database column  
  @deprecated use Stream::bind
*/
class dbaDLLEXPORT BindedStid : public BindedBase {
    friend class Stream;
  public:
    /**
      assign new value to object
    */
    id operator=(id i);
    /**
      convert object to id
    */
    id& operator*() const;
    /**
      cast object to id
    */
    operator id() const;
    /**
      @return id value
    */
    id getVal() const;
    /**
      Destructor
    */
    ~BindedStid();
  private:
    BindedStid(Stream* pOwner, id* pVar);
    shared_ptr<id> mValue;
};


/**@internal
  String that is binded to database column  
  @deprecated use Stream::bind
*/
class dbaDLLEXPORT BindedString {
    friend class Stream;
  public:
    /**
      assign string to value
    */
    const std::string& operator=(const std::string& pStr);
    /**
      convert object to string
    */
    const std::string& operator*() const;
    /**
      cast object to string
    */
    operator std::string() const;
    /**
      Get string value
    */
    const std::string& getVal() const;
    /**
      Destructor
    */
    ~BindedString();
  private:
    BindedString(std::string* pVar);
    shared_ptr<std::string> mValue;
};

}

#endif
