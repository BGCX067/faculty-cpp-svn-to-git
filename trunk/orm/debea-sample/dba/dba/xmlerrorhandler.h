//
// C++ Interface: xmlerrorhandler
//
// Description: 
//
//
// Author: Lukasz Michalski <lm@zork.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBAXMLERRORHANDLER_H
#define DBAXMLERRORHANDLER_H

#include "dba/xmlexception.h"

extern "C" {
  #include <stdarg.h>
}

namespace dba {

/**
Handler for libxml2 errors
*/
class dbaDLLEXPORT XMLErrorHandler {
    friend class LibXML;
    friend void errorHandler_c(void* ctx, const char* msg, ...);
  public:
    /**
      Default contructor.
    */
    XMLErrorHandler();
    /**
      Default destructor.
    */
    virtual ~XMLErrorHandler();
  protected:
    /**
      For internal use only. Class that manages current error context. Upon construction it sets xml generic error
      function to ErrorContext::errorHandler_c passing 'this' of ErrorHandler that actually uses
      library.
    */
    class dbaDLLEXPORT ErrorContext {
      public:
        /**
          Constructor.
          @param pHandler ErrorHandler derived object that acutally is using library
        */
        ErrorContext(XMLErrorHandler* pHandler);
        /**
          Check if last operation caused error. This is recognized by checking size of mHandler->mLastError.
          @return true if there is error message or false otherwise
        */
        bool wasError() const;
        /**
          Create exception with last error data
        */
        XMLException createException() const;
        /**
          Destructor. It deregisters libxml2 generic error handler 
        */
        ~ErrorContext();
      private:
        ErrorContext(const ErrorContext&);
        ErrorContext& operator=(const ErrorContext&);
        XMLErrorHandler* mHandler;
    };
  private:
    std::string mLastError;

    virtual void onError(const char* msg, va_list pArgs);
    char* make_message(const char *fmt, va_list pArgs);

};

} //namespace

#endif
