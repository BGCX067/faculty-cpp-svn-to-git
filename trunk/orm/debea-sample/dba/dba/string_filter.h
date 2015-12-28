// File: string_filter.h
// Purpose: Storeable filter for std::string
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTRING_FILTER_H
#define DBASTRING_FILTER_H

#include "dba/storeablefilter.h"

namespace dba {

/**
dba filter for std::string class
@ingroup filters
*/
class dbaDLLEXPORT String : public StoreableFilter<std::string> {
  public:
    String() {};
    /**
      Constructor.
      @param pData reference to class member or variable that filter will modify
    */    
    String(std::string& pData);
    virtual std::string toString(const ConvSpec& pSpec) const throw (StoreableFilterException);
    virtual bool isNull() const;

    virtual void fromInt(const ConvSpec& pSpec, int pData) throw (StoreableFilterException);
    virtual void fromDouble(const ConvSpec& pSpec, double pData) throw (StoreableFilterException);
    virtual void fromString(const ConvSpec& pSpec, const std::string& pData) throw (StoreableFilterException);
    virtual void fromDate(const ConvSpec& pSpec, const tm& pDate) throw (StoreableFilterException);
    virtual void fromNull() throw (StoreableFilterException);
    ~String();
};

}

#endif
