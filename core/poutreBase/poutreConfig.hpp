#ifndef POUTRE_CONFIG__HPP__
#define POUTRE_CONFIG__HPP__


/*!@file
 * Common configuration definitions. This file should not depend on any other file.
 */

namespace poutre
  {

#if defined(_WIN32) || defined(_WIN64)
  #define poutreDeprecated __declspec(deprecated)
  #define poutreDeprecatedMSG(x) __declspec(deprecated(x))

#if defined(_M_X64) || defined(_WIN64)
#define POUTRE_64BITS
#endif

#else
  #define poutreDeprecated __attribute__((deprecated))
  #define poutreDeprecatedMSG(x) yayiDeprecated

  #if defined(__LP64__)
    #define POUTRE_64BITS
  #endif

#endif


  // Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define MODULE_IMPORT __declspec(dllimport)
  #define MODULE_EXPORT __declspec(dllexport)
  #define MODULE_LOCAL
#else
  #if __GNUC__ >= 4
  #define MODULE_IMPORT __attribute__ ((visibility ("default")))
  #define MODULE_EXPORT __attribute__ ((visibility ("default")))
  #define MODULE_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
  #define MODULE_IMPORT
  #define MODULE_EXPORT
  #define MODULE_LOCAL
  #endif
#endif

#define PROJECT_NAME__ Poutre

  // plateform specific section
#if defined(_WIN32) || defined (_WIN64)

#	ifndef _CPPRTTI
#		error You should activate the RTTI for PROJECT_NAME__ to make it run correctly
#	endif

#	ifndef _CPPUNWIND
#		error You should activate the exception handling for PROJECT_NAME__ to make it run correcly
#	endif

#endif

//to be moved
#include "boost/format.hpp"
#define POUTRE_RUNTIME_ERROR( MSG ) throw std::runtime_error(boost::format("Throw %s at %s(%lu)") % MSG % __FILE__ % __LINE__);
#define POUTRE_CHECK( CONDITION, MSG )           \
   {                                                        \
      if ( !( CONDITION ) )                                 \
      {                                                     \
         POUTRE_RUNTIME_ERROR( MSG );               \
      }                                                     \
   }
#ifdef _DEBUG   
#define POUTRE_ASSERTCHECK( CONDITION, MSG ){                                                        \
    if ( !( CONDITION ) )                                 \
      {                                                     \
#if defined(_MSC_VER)
         __debugbreakpoint();               \
#else		 
#endif
      }                                                     \
   }
#else
#define POUTRE_ASSERTCHECK( CONDITION, MSG )
#endif

} //namespace

#endif


