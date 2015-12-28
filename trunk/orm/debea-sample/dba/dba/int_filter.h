// File: int_filter.h
// Purpose: StoreableFilter for int
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAINT_FILTER_H
#define DBAINT_FILTER_H

#include "dba/storeablefilter.h"

namespace dba {

/**
StoreableFilter class for integer values
@ingroup filters
*/
class dbaDLLEXPORT Int : public StoreableFilter<int> {
  public:
    Int() {};
    /**
      Constructor.
      @param pData reference to class member or variable that filter will modify
    */
    Int(int& pData);
    /**
      Constructor.
      @param pData reference to class member or variable that filter will modify
    */
    Int(unsigned int& pData);
    /**
      Constructor for all types convertable to int
    */
    template <typename T> Int(T& pData);
    virtual std::string toString(const ConvSpec& pSpec) const throw (StoreableFilterException);
    virtual bool isNull() const;
    
    virtual void fromInt(const ConvSpec& pSpec, int pData) throw (StoreableFilterException);
    virtual void fromDouble(const ConvSpec& pSpec, double pData) throw (StoreableFilterException);
    virtual void fromString(const ConvSpec& pSpec, const std::string& pData) throw (StoreableFilterException);
    virtual void fromDate(const ConvSpec& pSpec, const tm& pDate) throw (StoreableFilterException);
    virtual void fromNull() throw (StoreableFilterException);
    virtual Database::StoreType getPrefferedStoreType() const { return Database::INTEGER; }
    ~Int();
};

template<typename T>
Int::Int(T& pData)
  : StoreableFilter<int>((int&)(pData))
{}

} //namespace

#endif
