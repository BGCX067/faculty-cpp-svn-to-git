// File: csv.h
// Purpose: Archive implementation for csv file format
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2004
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef CSVCSV_H
#define CSVCSV_H

#include <deque>
#include <list>
#include <string>
#include <fstream>
#include "dba/storeablefilter.h"
#include "dba/convspec.h"
#include "dba/archive.h"
#include "dba/istream.h"
#include "dba/ostream.h"
#include "dba/shared_ptr.h"

namespace dba {

class CSVIStream;
class CSVOStream;

/**
Exception thrown when there is problem reading or writing to csv file
@ingroup api
*/
class dbaDLLEXPORT CSVFileException : public DatabaseException {
  public:
    /**
      Constructor
      @param pError exception description
    */
    CSVFileException(const char* pError) : DatabaseException(pError) {};
};

/**
Exception thrown when there is problem parsing csv file contents.
@ingroup api
*/
class dbaDLLEXPORT CSVParseException : public DataException {
  public:
    /**
      Constructor
      @param pError exception description
    */
    CSVParseException(const char* pError) : DataException(pError) {};
};

#ifdef DBA_COMPAT_1_0
typedef CSVParseException CSVParserException;
#endif

/**
  CSV file parser
  @ingroup api
*/
class dbaDLLEXPORT CSVParser {
  public:
    /**
      Costructor.
      @param pStream file stream to read
    */
    CSVParser(std::ifstream& pStream);
    /**
      Parse one line from csv file.
      @return true if line was parsed, false if line was empty.
      @throw CSVParserException if non empty line parse failed.
    */
    bool parseLine() throw (CSVParseException);
    /**
      Get values readed by parseLine()
    */
    const std::list<std::string>& getValues() const;
    /**
      Set char that separates fields
      @param pSep field separator char
    */
    void setFieldSeparator(char pSep);
    /**
      Get char that separates fields
      @return field separator char
    */
    char getFieldSeparator() const;
    /**
      Check if all lines were readed
      @return true if there is no more data to read, false otherwise
    */
    bool eof();
    ~CSVParser();
  private:
    CSVParser(const CSVParser&);
    CSVParser& operator=(const CSVParser&);
    typedef enum {
      SCAN,
      FIELD,
      QUOTED_FIELD,
      QUOTE_TEST
    } state;
    
    std::ifstream& mStream;
    int mLine;
    int mChar;
    std::list<std::string> mValues;
    std::list<std::string>::iterator mCurrent;
    std::deque<state> mState;
    char mFieldSep;
        
    void onField(int pNum, std::string* pData);
    void onLine();
    void prepareList();
    void processSeparator(int& field, std::string& value, char c);
};

/**@internal 
  Describes mapping between store table member and csv column name or index
*/
class dbaDLLEXPORT mapping {
  public:
    mapping(int pCSVField, const char* pTable, const char* pField);
    mapping(const char* pCSVField, const char* pTable, const char* pField);
    mapping(const mapping& pMapping);
    mapping& operator=(const mapping& pMapping);
    ~mapping();
    bool operator<(const mapping& pMapping) const;
    int fnumber;
    char* fname;
    char* table;
    char* field;
  private:
};

/**
Implementation of Archive interface for csv file format

To fetch data or objects from csv files before opening archive you have to set mappings.
Mappings are used to make relation between object fields or binded vars and columns in CSV file.

Mappings can be done automatically or manually (see hasColumnNames()). Automatic mapping 
occurs always when:
 - archive has column names
 - addMapping was not called before open()
 
Automatic mapping is estabilished between store table entry or binded var and column that have 
the same name as field. It is not possibile to manually add mappings after automatic mapping was 
done 

Binded Vars.

When binding variables to output stream with automatic mapping order of binding is sigincfant. 
For example for this bindings:

@code
int a,b;
stream.bind(a,"test","a")
stream.bind(b,"test","b")
@endcode

first data for "a" column will be written and then data for "b".

You can add multiple bindings for the same column name - this will result in multiple column names 
in cvs file in order that variables were binded.

Mappings.

If there is no mapping for column that is not last on list (for example there are mappings for column
1,2,3 and 5 only) that column will be written as empty one if archive does not have setColnames() set to
true. In that case DataException will be thrown - driver don't know what column name to write because 
there is no mapping for it.

You should not create multiple streams from one CSVArchive at one time. Every call to a CSVOStream::open
will overwrite existing file. Reading data from a file using a CSVIStream and writing data to it using a CSVOStream
simultaneously is not a good idea and can give weird results

@ingroup api
*/
class dbaDLLEXPORT CSVArchive : public Archive, public ConvSpecContainer {
  public:
    CSVArchive();
    /**
      Add mapping using column number. This function will map pTable and pField
      to n-th column in cvs file (columns are numbered from 0). It can be called for archives
      with or without names. Don't use this function if you want to bind
      variables to fields - in this case only named mappings works.
      @param pCSVField column index from csv file
      @param pTable relation name from store table
      @param pField field name from store table
    */
    void addMapping(int pCSVField, const char* pTable, const char* pField);
    /**
      Add mapping using column name. This function will map pTable and pFiled from 
      object's store tables to particular column name.
      @param pCSVField column name from csv file
      @param pTable relation name from store table
      @param pField field name from store table
    */
    void addMapping(const char* pCSVField, const char* pTable, const char* pField);
    /**
      This flag tells internal CSVParser to ignore lines with no data.
      @param pFlag true if input stream should ignore empty lines, false otherwise
    */
    void ignoreEmptyLines(bool pFlag);
    /**
      This flag tells archive if file contains column names as first row or not.
      @param pFlag true if archive should read/create column names in first line of csv file
    */
    void hasColumnNames(bool pFlag);
    /**
      This flag tells archive if file contains column names as first row or not.
      @return true if hasColumnNames was set, false otherwise
    */
    bool hasColumnNames() const;
    /**
      Open archive.
      @param pOpenStr path to csv file.
    */
    virtual void open(const char* pOpenStr);
    /**
      Set field separator. Default is comma
      @param pSep new field separator char
    */
    void setFieldSeparator(char pSep);
#ifdef DBA_COMPAT_1_0    
    CSVIStream* getOIStream();
    CSVOStream* getOOStream();
    std::list<std::string> getColumns();
#endif    
    virtual IStream* getInputStream();
    virtual OStream* getOutputStream();
    /**
      create and return input stream
      @returns IStream derived class 
    */
    CSVIStream getIStream();
    /**
      create and return output stream
      @returns OStream derived class 
    */
    CSVOStream getOStream();
    /**
      Clear all mappings created by addMapping methods
    */
    void clearMappings();
    virtual bool isOpen() const;
    /**
      Destructor    
    */
    virtual ~CSVArchive();
  private:
    std::list<mapping> mMappings;
    bool mHasColNames;
    std::string mFilename;
    bool mIgnoreEmptyLines;
    char mFieldSeparator;
};

/** 
  IStream implementation for csv.
  This stream does not support transactions, calling begin() or rollback() 
  does nothing.
  @ingroup api
*/
class dbaDLLEXPORT CSVIStream : public dba::IStream, public ConvSpecContainer {
  public:
    /**
      Constructor used by CSVArchive.
      @param pFilename csv file name
      @param pMappings mappings set in archive
      @param pHasColNames if true first row contains names of colunms
      @param pIgnoreEmptyLines if true stream should skip empty lines
      @param pSpecs conversion specification from archive
    */
    CSVIStream(const char* pFilename, std::list<mapping>& pMappings, bool pHasColNames, bool pIgnoreEmptyLines, const ConvSpec& pSpecs);
    virtual void open(Storeable& pObject, const char* pRootTable = NULL);
    virtual bool getNext(Storeable* pObject);
    virtual bool updateVars();
    virtual void close();
    virtual void destroy();
    /**
      Return column names. List of column names contains strings without any conversion.
    */
    const std::list<std::string>& getColumns();
    /**
      Overrider field separator set from CSVArchive
      @param pSep new field separator char
    */
    void setFieldSeparator(char pSep);
    virtual ~CSVIStream();
  private:
    shared_ptr<std::ifstream> mStream;
    std::list<mapping> mMappings;
    std::list<std::string> mColNames;
    std::list<std::string> mValues;
    bool mHasColNames;
    bool mRowFetched;
    bool mIgnoreEmptyLines;
    std::string mFilename;
    shared_ptr<CSVParser> mParser;
    char mFieldSep;

    void createMappings(dba::Storeable& pObject);
    std::string getValueByIndex(const std::list<std::string>& pValue, int pPos);
    bool fetchRow();
    void fillBindedVars();    
    void openFile();
    void parseColumns();
};

/** 
  OStream implementation for csv
  @ingroup api
*/
class dbaDLLEXPORT CSVOStream : public dba::OStream, public ConvSpecContainer {
  public:
    /**
      Constructor used by CSVArchive.
      @param pFilename csv file name
      @param pMappings mappings set in archive
      @param pHasColNames if true first row contains names of colunms
      @param pSpecs conversion specification from archive
    */
    CSVOStream(const char* pFilename, std::list<mapping>& pMappings, bool pHasColNames, const dba::ConvSpec& pSpecs);
    virtual void open(const char* pRootTable = NULL);
    virtual void close();
    virtual void destroy();
    //csv format does not support transactions
    /**
      For CSV file format this method does nothing
    */
    virtual void begin() {};
    /**
      For CSV file format this method does nothing
    */
    virtual void commit() {};
    /**
      For CSV file format this method does nothing
    */
    virtual void rollback() {};
    /**
      Overrider field separator set from CSVArchive
      @param pSep new field separator cha
    */
    void setFieldSeparator(char pSep);
  private:
    std::list<mapping> mMappings;
    bool mHasColNames;
    bool mNoWriteYet;
    shared_ptr<std::ofstream> mStream;
    char mFieldSep;
    
    virtual bool erase(dba::Storeable* pObject);
    virtual bool update(dba::Storeable* pObject);
    virtual bool store(dba::Storeable* pObject);
    virtual void assignId(dba::Storeable* pObject) throw (dba::Exception) {};
    std::string escapeString(const std::string& pInput);
    void writeColumns();
    void createMappings();
    std::string createOutVal(const std::string& pData);
    std::string createOutVal(const tm& pData);
    std::string createOutVal(int pData);
    std::string createOutVal(double pData);
    std::string applyFilter(dba::StoreableFilterBase& pFilter, void* pMember, dba::Database::StoreType pType);
};

#ifdef DBA_COMPAT_1_0
typedef CSVIStream CSVOIStream;
typedef CSVOStream CSVOOStream;
#endif

};

#ifdef DBA_COMPAT_1_0
namespace csv {
  typedef dba::CSVParseException CSVParserException;
  typedef dba::CSVIStream CSVOIStream;
  typedef dba::CSVOStream CSVOOStream;
  typedef dba::CSVArchive CSVArchive;
};
#endif

#endif
