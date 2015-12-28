//
// C++ Interface: globalfiltermapper
//
// Description: 
//
//
// Author: Lukasz Michalski <lm@zork.pl>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBAFILTERMAPPER_H
#define DBAFILTERMAPPER_H

#include "dba/exception.h"
#include <map>
#include <typeinfo>

namespace dba {

class StoreableFilterBase;

/**
Global static database of filter to type maps. Used by SQL class to get proper filter for C++ type when converting C++ data to %SQL representation. This database is initialized at program startup. Rading from this database should be thread safe.
*/
class FilterMapper {
  public:
    FilterMapper();
    ~FilterMapper();
    /**
      Add new mapping for C++ data type
    */
    void mapType(const std::type_info& pType, StoreableFilterBase* pFilter);
    /**
      Find filter for given type
    */
    dba::StoreableFilterBase& findFilter(const char* pTypeName) const throw (FilterNotFoundException);
    /**
      Add all mappings for default filters
    */
    void initDefaultMappings();
  private:
    typedef std::map<std::string, StoreableFilterBase*> FilterMap;
    FilterMap mMap;
};

} //namespace

#endif
