// File: sqlutils.h
// Purpose: Various utilites for creation SQL queries
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2002
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)



#ifndef DBASQLUTILS_H
#define DBASQLUTILS_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>

#include "dba/conversion.h"
#include "dba/database.h"
#include "dba/sql.h"

namespace dba {

/**
  common %SQL utilities
  @ingroup api
*/

class dbaDLLEXPORT SQLUtils {
  public:
    /**
      Escape text string to conform %SQL standard
      @param pData string to modify
      @return escaped string  
    */
    static std::string escapeSQLData(const std::string& pData);
    /**
      Escape string used for LIKE clause to conform %SQL standard 
      @param pData string to modify
      @return escaped string  
    */
    static std::string escapeSQLLike(const std::string& pData);

#ifdef DBA_COMPAT_1_0    
    /**
      Create IN clause from objects that have toStr() method
      @return string with IN clause  
    */
    template<class T> static std::string createIN(const std::map<int,T>& pMap) {
      std::string in("(");
      for(typename std::map<int,T>::const_iterator it = pMap.begin(); it != pMap.end(); it++) {
        if (it != pMap.begin())
          in += ",";
        in += toStr(it -> first);
      };
      in += ")";
      return in;
    };
#endif
    template<class T> static SQL createIN(const T& pContainer, char pValType) {
      if (pContainer.empty())
        return SQL();
      std::string instr("(:");
      instr.append(1,pValType);
      for (unsigned i = 0; i < pContainer.size() - 1; ++i) {
        instr.append(", :");
        instr.append(1,pValType);
      }
      instr.append(")");
      SQL in(instr);
      for(typename T::const_iterator it = pContainer.begin(); it != pContainer.end(); it++)
        in << *it;
      return in;
    }
    /**
      Create IN clause from set of int values
      @param pSet list of values for in clause
      @return string with IN clause  
    */
    static SQL createIN(const std::set<int>& pSet) {
      return createIN(pSet, 'd');
    };

    /**
      Create part of %SQL SELECT for use with in clause
      @param pSet list of values for in clause
      @return string with IN clause  
    */
    static SQL createIN(const std::vector<id>& pSet) {
      return createIN(pSet, 'd');
    };

    /**
      Create part of %SQL SELECT for use with in clause
      @param pSet list of values for in clause
      @return string with IN clause  
    */
    static SQL createIN(const std::vector<int>& pSet) {
      return createIN(pSet, 'd');
    };
    /**
      Create IN clause from set of string values
      @param pSet list of values for in clause
      @return string with IN clause  
    */
    static SQL createIN(const std::set<std::string>& pSet) {
      return createIN(pSet, 's');
    };
      
    /**
      surround string by single quotes
      @param pVal string to quote
      @returns pVal with quotes
    */
    static std::string setSQLVal(const std::string& pVal) {
      std::string str(pVal);
      if (pVal.size() == 0) {
        str = "NULL";
      } else {
        str = "'" + escapeSQLData(str) + "'";
      };
      return str;
    };
    /**
      Get maximum value from one of columns
      @param pConn pointer to open database connection
      @param pTable name of table
      @param pIntCol name of column
      @return fetched value
    */
    static int getMax(DbConnection& pConn,const char* pTable, const char* pIntCol) {
      std::string query = "SELECT max(" + std::string(pIntCol) + ") FROM " + std::string(pTable);
      std::auto_ptr<DbResult> res(pConn.sendQuery(query));
      res->fetchRow();
      if (res->isNull(0))
        return 0;
      return res->getInt(0);
    };
};

};//namespace


#endif
