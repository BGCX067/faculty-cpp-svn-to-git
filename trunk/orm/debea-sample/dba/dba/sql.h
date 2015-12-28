//
// C++ Interface: sqlquery
//
// Description: 
//
//
// Author: Lukasz Michalski <lm@zork.pl>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBASQL_H
#define DBASQL_H

#include <typeinfo>
#include <vector>
#include <deque>
#include <string>
#include "dba/database.h"
#include "dba/storeablefilter.h"
#include "dba/shared_ptr.h"

namespace dba {

class FilterMapper;
class ConvSpec;

/**
  This class allows you to create and send parameterized %SQL queries to database and retrieve records 
  directly into C++ variables or classes.

  Parameters specification in %SQL query are:

- :s - variable should be formated as %SQL string, special chars will be escaped depending on if it is value to insert or data for %SQL LIKE keyword.
- :d - variable should be formatted as integer
- :f - variable should be formatted as floating point number

  You can escape ":" by putting double "::" in query.

  Conversion between database type and C++ type is done using StoreableFilter instances. dba::SQLArchive has 
  default mappings for all C++ types for which Debea provides filters. You can change those mappings using
  SQLArchive::mapType(). You can add new filters for other C++ types in the same way.

  Use operator<< to feed query with parameter data from C++ variables, and into() to specify C++ variables where
  data read from database should written.

  Some examples:
  @dontinclude queries.cpp
  @skip //insert some data into table
  @until );

  @skip //vars to put data in
  @until );

  see @ref dba_example_queries for complete example.
*/
class dbaDLLEXPORT SQL {
  private:
    class DataContainerBase {
      public:
        virtual void* getPtr() const = 0;
        virtual ~DataContainerBase(){};
      protected:
        DataContainerBase() {};
    };

    /**
      Owns variable of any type
    */
    template <typename T> class DataContainer : public DataContainerBase {
      public:
        DataContainer(T* pData) { mData = pData; }
        virtual void* getPtr() const { return mData; }
        virtual ~DataContainer() { delete mData; }
      private:
        T* mData;
    };
  public:
    /** Next position in sql SELECT*/
    static const int VAR_INDEX_UNDEFINED = -1;

    SQL();
    /**
      Create SQL from string containing %SQL query
      @param pData %SQL query data
    */
    SQL(const char* pData);
    /**
      Create SQL from string containing %SQL query
      @param pData %SQL query data
    */
    SQL(const std::string& pData);
    void setFilterDataForNextParam(DataContainerBase* pData, const char* pTypeName);
    void setFilterDataForNextParam(StoreableFilterBase* pFilter);
    void setFilterDataForNextVar(void* pData, const char* pTypeName, const char* pSQLName = NULL, int pSQLPos = VAR_INDEX_UNDEFINED);
    void setFilterDataForNextVar(StoreableFilterBase* pFilter, const char* pSQLName = NULL, int pSQLPos = VAR_INDEX_UNDEFINED);
    /**
      Create %SQL query string replacing parameter specifiers with data using pMapper as
      filter database and pSpecs for conversion specification
    */
    virtual std::string cstring(const FilterMapper& pMapper, const ConvSpec& pSpecs) const; 
    /**
      update variables defined by into() from DbResult.
    */
    virtual void updateVars(DbResult& pRes, const FilterMapper& pMapper, const ConvSpec& pSpecs) const;
    /**
      append query, data, filters and placeholders from other query.
    */
    void appendData(const SQL& pQuery);
    /**
      append filter data *only* from other query
    */
    void appendFilterData(const SQL& pQuery);

    /**
      Specify const char* data for query parameter
    */
    SQL& operator<< (const char* pData) {
      DataContainerBase* data = new DataContainer<std::string>(new std::string(pData));
      setFilterDataForNextParam(data,typeid(std::string).name());
      return *this;
    }

    /**
      Specify custom filter class for query parameter
      @param pData StoreableFilterBase derived object
    */
    template <typename T> SQL& operator<< (T* pData) {
      setFilterDataForNextParam(pData);
      return *this;
    };

    /**
      Specify C++ variable for query parameter. We copy passed data in this call.
      @param pData C++ variable with data for query parameter
    */
    template <typename T> SQL& operator<<(const T& pData) {
      DataContainerBase* data = new DataContainer<T>(new T(pData));
      setFilterDataForNextParam(data,typeid(pData).name());
      return *this;
    }

    /**
      Specify C++ variable where data readed from database should be written.
      @param pVar reference to output variable
    */
    template <typename T> SQL& into(T& pVar) {
      setFilterDataForNextVar(&pVar,typeid(pVar).name());
      return *this;
    }

    /**
      Specify custom conversion filter which will convert data readed from database into C++ variable.
      This filter should be constructed with reference to C++ variable where result of conversion should be written
      @param pFilter StoreableFilterBase derived object
    */
    SQL& into(StoreableFilterBase* pFilter) {
      setFilterDataForNextVar(pFilter);
      return *this;
    }

    /**
      Merge two instances of SQL into one.
    */
    dba::SQL& operator+ (const SQL& pFrom);
    dba::SQL& operator+= (const SQL& pFrom);

    virtual ~SQL();
  protected:
    void parseQuery();
  private:
    typedef enum {
      PARAM_UNKNOWN = 0,
      PARAM_ESCAPED,
      PARAM_STR_VALUE,
      PARAM_STR_LIKE,
      PARAM_INT,
      PARAM_DATE, //<! %s is used instead for now
      PARAM_FLOAT,
      PARAM_ESCAPE_CHAR
    } convert_type;

    /**
      Holds copies of variables passed with operator<<
    */
    class  ParamData {
      public:
        ParamData(DataContainerBase* pData, const char* pTypeName)
          : mData(pData),
            mTypeName(pTypeName),
            mFilter(NULL)
        {}
        ParamData(StoreableFilterBase* pFilter)
          : mData(NULL),
            mTypeName(NULL),
            mFilter(pFilter)
        {}
        ~ParamData() {}
        shared_ptr<DataContainerBase> mData;
        const char* mTypeName;
        shared_ptr<StoreableFilterBase> mFilter;
    };


    /**
      Holds references of variables specifed by into() variants
    */
    class VarData {
      public:
        VarData(void* pData, const char* pTypeName, const char* pSQLName, int pSQLIndex = VAR_INDEX_UNDEFINED)
          : mData(pData),
            mTypeName(pTypeName),
            mFilter(NULL),
            mSQLName(pSQLName),
            mSQLIndex(pSQLIndex),
            mStoreType(Database::UNKNOWN)
        {}
        VarData(StoreableFilterBase* pFilter, const char* pSQLName, int pSQLIndex = VAR_INDEX_UNDEFINED)
          : mData(NULL),
            mTypeName(NULL),
            mFilter(pFilter),
            mSQLName(pSQLName),
            mSQLIndex(pSQLIndex),
            mStoreType(Database::UNKNOWN)
        {}
        bool hasName() const { return mSQLName != NULL; }
        bool hasIndex() const { return mSQLIndex != VAR_INDEX_UNDEFINED; }
        void* mData;
        const char* mTypeName;
        const char* mSQLName;
        int mSQLIndex;
        Database::StoreType mStoreType;
        StoreableFilterBase* mFilter;
    };

    /**
      Describes single placeholder in %SQL statement string
    */
    class ParamPlaceholder {
      public:
        ParamPlaceholder(size_t pIndex, convert_type pConv, bool pShouldQuote = false)
          : mIndex(pIndex), mConvertType(pConv), mShouldQuote(pShouldQuote)
        {}
        bool operator==(const ParamPlaceholder& pArg) { 
          return (mIndex == pArg.mIndex && mConvertType == pArg.mConvertType && mShouldQuote == pArg.mShouldQuote);
        }
      size_t mIndex;
      convert_type mConvertType;
      bool mShouldQuote;
    };

    typedef std::vector<ParamData> Params;
    typedef std::vector<VarData> Vars;
    typedef std::vector<ParamPlaceholder> Placeholders;

    /**
      Primitive %SQL parser that creates ParamPlaceholder list
    */
    class dbaDLLEXPORT SQLParamParser {
      public:
        SQLParamParser(const std::string& pSQLQuery,SQL::Placeholders& pParams);
        /**
          Parse sql query and modify it in place by replacing placeholders with
          StoreableFilter generated output
        */
        void parse();
        ~SQLParamParser() {}
      private:
        typedef enum {
          SCAN,
          PARAM_TEST,
          QUOTED_FIELD,
          QUOTE_TEST,
          AFTER_LIKE,
          QUOTED_LIKE
        } state;
        std::string mSQLQuery;
        std::deque<state> mState;
        Placeholders& mPlaceholders;
        void setCurrentParamIndex(std::string::iterator& pPos);
        void checkSQLLike(const std::string::iterator& pPos);
};

    Params mParams;
    Vars mVars;
    Placeholders mPlaceholders;
    std::string mSQLQuery;

    std::string fillQueryParams(const FilterMapper& pMapper, const ConvSpec& pSpecs) const;
    std::string convertParam(const FilterMapper& pMapper, const ConvSpec& pSpecs, const ParamData& pData) const;
    int getCountWithoutEscapes(const Placeholders& pPh) const;
};

dba::SQL
operator+(const dba::SQL& pQuery, const SQL& pFrom);

} //namespace

#endif
