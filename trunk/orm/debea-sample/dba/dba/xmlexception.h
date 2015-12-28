#ifndef DBAXMLEXCEPTION_H
#define DBAXMLEXCEPTION_H

#include "dba/exception.h"

namespace dba {

/**
  Exception for libxml2 related errors
*/
class dbaDLLEXPORT XMLException : public DatabaseException {
  public:
    /**
      Constructor
      @param pError exception description
    */
    XMLException(const char* pError) : DatabaseException(pError) {};
};

}

#endif
