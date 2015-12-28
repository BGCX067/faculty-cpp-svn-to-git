// File: convspec.h
// Purpose: Archive conversion specification
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBACONVSPEC_H
#define DBACONVSPEC_H

#include "dba/defs.h"

namespace dba {

/**
  Conversion specifiaction passed to StoreableFilter::toString by Archive
  @ingroup api
*/
class dbaDLLEXPORT ConvSpec {
  public:
    /**
      List of known encoding identifiers
    */
    typedef enum {
      ISO8859_1 = 1,   // West European (Latin1)
      ISO8859_2,       // Central and East European (Latin2)
      ISO8859_3,       // Esperanto (Latin3)
      ISO8859_4,       // Baltic (old) (Latin4)
      ISO8859_5,       // Cyrillic
      ISO8859_6,       // Arabic
      ISO8859_7,       // Greek
      ISO8859_8,       // Hebrew
      ISO8859_9,       // Turkish (Latin5)
      ISO8859_10,      // Variation of Latin4 (Latin6)
      ISO8859_11,      // Thai
      ISO8859_13,      // Baltic (Latin7)
      ISO8859_14,      // Latin8
      ISO8859_15,      // Latin9 (a.k.a. Latin0, includes euro)
  
      // Cyrillic charset soup (see http://czyborra.com/charsets/cyrillic.html)
      KOI8,            // KOI8 Russian
      KOI8_U,          // KOI8 Ukrainian
      BULGARIAN,       // used under Linux in Bulgaria
  
      // for DOS
      CP437,           // original MS-DOS codepage
      CP850,           // CP437 merged with Latin1
      CP852,           // CP437 merged with Latin2
      CP855,           // another cyrillic encoding
      CP866,           // and another one
      // and for Windows
      CP874,           // WinThai
      CP932,           // Japanese (shift-JIS)
      CP936,           // Chinese simplified (GB)
      CP949,           // Korean (Hangul charset)
      CP950,           // Chinese (traditional - Big5)
      CP1250,          // WinLatin2
      CP1251,          // WinCyrillic
      CP1252,          // WinLatin1
      CP1254,          // WinTurkish
      CP1255,          // WinHebrew
      CP1256,          // WinArabic
  
      UTF7,            // UTF-7 Unicode encoding
      UTF8,            // UTF-8 Unicode encoding
      EUC_JP          // Extended Unix Codepage for Japanese
    } charset;

    ConvSpec();
    /**
      Char used for decimal point for floating point to string conversions 
    */
    char mDecimalPoint;
    /**
      Database charset. Used by String filter to perform conversion from 
      applicatios charset to database charset.
      If set to NULL then no conversion is performed.
    */
    charset mDbCharset;
    /**
      GMT offset between application and database dates. If not zero then
      datetime conversion filters perform GMT offset conversion based
      on this value before storing or after loading values from database.
      
      Value is stored as number of seconds. Value is added to datetime
      before storing date, and substracted from loaded date.
    */
    int mGMTOffset;
    /**
      strftime compatibile format for datetime conversion to query value
    */
    const char* mTimestampFormat;
    /**
      strftime compatibile format for date conversion to query value
    */
    const char* mDateFormat;
#ifdef _WIN32
    /**
      Convert debea charset to MSW code page    
    */
    static int getMSWCodePage(charset pCharset);
#endif
    /**
      Convert debea charset to UNIX LC_TYPE locale
    */
    static const char* getUnixLCType(charset pCharset);
    /**
      Convert debea charset to xml charset name
    */
    static const char* getXmlEncodingName(charset pCharset);
};

class dbaDLLEXPORT ConvSpecContainer {
  public:
    ConvSpecContainer() {};
    ConvSpecContainer(const ConvSpec& pSpecs) : mConvSpecs(pSpecs) {};
    /**
      Set new convesion specifications for filters.
    */
    virtual void setConversionSpecs(const ConvSpec& pSpecs);
    
    /**
      Get current conversion specifications for filters
    */
    virtual const ConvSpec& getConversionSpecs() const;
    
    /**
      Get current conversion specification for in-place modifications
    */
    virtual ConvSpec& conversionSpecs();

    virtual ~ConvSpecContainer() {};
  protected:
    ConvSpec mConvSpecs;
};

} //namespace

#endif
