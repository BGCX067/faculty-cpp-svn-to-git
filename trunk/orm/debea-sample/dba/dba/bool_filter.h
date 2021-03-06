// File: bool_filter.h
// Purpose: Implementation of StoreableFilter for bool type
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBABOOL_FILTER_H
#define DBABOOL_FILTER_H

#include "dba/storeablefilter.h"

namespace dba {

/**
Class for convertion Database types to and from bool object member
@ingroup filters
*/
class dbaDLLEXPORT Bool : public StoreableFilter<bool> {
  public:
    Bool() {};
    /**
      Constructor.
      @param pData reference to class member or variable that filter will modify
    */
    Bool(bool& pData);
    virtual std::string toString(const ConvSpec& pSpec) const throw (StoreableFilterException);
    virtual bool isNull() const;
    
    virtual void fromInt(const ConvSpec& pSpec, int pData) throw (StoreableFilterException);
    virtual void fromDouble(const ConvSpec& pSpec, double pData) throw (StoreableFilterException);
    virtual void fromString(const ConvSpec& pSpec, const std::string& pData) throw (StoreableFilterException);
    virtual void fromNull() throw (StoreableFilterException);
    virtual Database::StoreType getPrefferedStoreType() const { return Database::INTEGER; }
    ~Bool();
};

} //namespace

#endif
