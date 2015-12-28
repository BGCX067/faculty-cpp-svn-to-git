// File: storeablefilter.h
// Purpose: Base class for storeable filters
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTOREABLEFILTER_H
#define DBASTOREABLEFILTER_H

#include "dba/exception.h"
#include "dba/defs.h"
#include "dba/convspec.h"
#include "dba/database.h"

namespace dba {

/**
  Exception thrown by storeable filters
  @ingroup api filters
*/
class dbaDLLEXPORT StoreableFilterException : public DataException {
  public:
    /**
      Constructor.
      @param pCause error message
    */
    StoreableFilterException(const char* pCause) : DataException(pCause) {};
};

/**
Base class for routines that converts Storeable object members to arguents to Database queries and Database results to members. 
@warning You should not derive from this class - use StoreableFilter instead 
*/
class dbaDLLEXPORT StoreableFilterBase {
    //TODO add getPrefferedType for rest of filters in dba and wxdba
    friend class OStream;
  public:
    StoreableFilterBase() {};
    /**
      Convert type to string. Should throw dba::StoreableFilterException if conversion fails
      @param pSpec conversion parameters dependent of used database
      @return converted value.
    */
    virtual std::string toString(const ConvSpec& pSpec) const throw (StoreableFilterException) = 0;
    /**
      Check if C++ type should be stored as NULL value in database
      @return true if NULL should be stored, false otherwise
    */
    virtual bool isNull() const = 0;
    /**
      Convert to C++ type from int value received from database.
      Should throw dba::StoreableFilterException if conversion fails
      Must be implemented for BIND_INT usage
      @param pSpec conversion parameters dependent of used database
      @param pData source value for conversion
    */
    virtual void fromInt(const ConvSpec& pSpec, int pData) throw (StoreableFilterException);
    /**
      Convert to C++ type from double value received from database.
      Should throw dba::StoreableFilterException if conversion fails
      Must be implemented for BIND_FLT usage
      @param pSpec conversion parameters dependent of used database
      @param pData source value for conversion
    */
    virtual void fromDouble(const ConvSpec& pSpec, double pData) throw (StoreableFilterException);
    /**
      Convert to C++ type from std::string value received from database.
      Should throw dba::StoreableFilterException if conversion fails
      Must be implemented for BIND_STR usage
      @param pSpec conversion parameters dependent of used database
      @param pData source value for conversion
    */
    virtual void fromString(const ConvSpec& pSpec, const std::string& pData) throw (StoreableFilterException);
    /**
      Convert to C++ type from int value received from database.
      Should throw dba::StoreableFilterException if conversion fails
      Must be implemented for BIND_DAT usage
      @param pSpec conversion parameters dependent of used database
      @param pDate source value for conversion
    */
    virtual void fromDate(const ConvSpec& pSpec, const tm& pDate) throw (StoreableFilterException);
    /**
      Set C++ type from NULL value
    */
    virtual void fromNull() throw (StoreableFilterException) = 0;
    /**
      Update pointer to C++ member to new address. This method is used by dba
      just before filter usage.
      @param pNewMember address of new member
    */
    virtual void updateRef(void *pNewMember) = 0;
    /**
      Check if filter contains data to convert
    */
    virtual bool hasRef() const  = 0;
    /**
      Get pointer to C++ member.
      @throw APIException if updateRef was not called before
    */
    virtual void* getRef() const throw (APIException) = 0;
    /**
      Get preffered store type
    */
    virtual Database::StoreType getPrefferedStoreType() const { return Database::STRING; }
    virtual ~StoreableFilterBase() {};
};

/**
  Base class for storeable filters. Handles pointer update and construction of filter. You have to implement
  conversion rooutines to be able to create filters.
  @warning this filter will operate on different pointers to members of objects storead as mMember. There is \
  no guarantee that pointer that is set by updateRef will not change between calls to fromString or toXXX calls  
  @ingroup filters
*/
template <typename T>
class StoreableFilter : public StoreableFilterBase {
  public:
    StoreableFilter() 
      : mMember(NULL)
    {};
    /**
      Constructor.
      @param pMember object that this filter should modify
    */
    StoreableFilter(T& pMember) 
      : mMember(&pMember)
    {};
    virtual void updateRef(void *pNewMember) {
      mMember = (T*)pNewMember;
    };
    virtual bool hasRef() const {
      return mMember != NULL;
    }
    virtual void* getRef() const throw (APIException) {;
      if (mMember == NULL)
        throw APIException("Cannot get pointer to data. UpdateRef not called");
      return mMember;
    };
  protected:
    /**
      pointer to object that this filter should modify. Can be changed by updateRef.
    */
    T* mMember;
};

} //namespace

#endif
