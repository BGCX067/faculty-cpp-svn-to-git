// File: datetime_filter.h
// Purpose: StoreableFilter for struct tm
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBADATETIME_FILTER_H
#define DBADATETIME_FILTER_H

#include "dba/storeablefilter.h"

namespace dba {

/**
  Class for converting date and time. GMT offsets are calculated using mktime() call.
  This means that if you set ConvSpec::mGMTOffset to value different from 0
  you can use this filter for range 1970 +/- 68 years on 32-bit machines.
  @ingroup filters
*/
class dbaDLLEXPORT DateTime : public StoreableFilter<tm> {
  public:
    DateTime() {};
    /**
      Constructor
      @param pDate reference to object or class member that filer will modify
    */
    DateTime(tm& pDate);
    virtual std::string toString(const ConvSpec& pSpec) const throw (StoreableFilterException);
    virtual bool isNull() const;
    
    virtual void fromInt(const ConvSpec& pSpec, int pData) throw (StoreableFilterException);
    virtual void fromDouble(const ConvSpec& pSpec, double pData) throw (StoreableFilterException);
    virtual void fromString(const ConvSpec& pSpec, const std::string& pData) throw (StoreableFilterException);
    virtual void fromDate(const ConvSpec& pSpec, const tm& pDate) throw (StoreableFilterException);
    virtual void fromNull() throw (StoreableFilterException);
    virtual Database::StoreType getPrefferedStoreType() const { return Database::DATE; }
    virtual ~DateTime();
  private:
    void normalize();
};

}

#endif
