#ifndef POUTRE_CONFIG__HPP__
#define POUTRE_CONFIG__HPP__

#include <boost/format.hpp>

/*!@file
 * Common configuration definitions. This file should not depend on any other file.
 */


namespace poutre
  {


#define POUTRE_NOEXCEPT BOOST_NOEXCEPT_OR_NOTHROW

#ifndef NDEBUG
#define POUTRE_NOEXCEPTONLYNDEBUG 
#else
#define POUTRE_NOEXCEPTONLYNDEBUG POUTRE_NOEXCEPT
#endif

#define POUTRE_STATIC_CONSTEXPR BOOST_STATIC_CONSTEXPR
#define POUTRE_CONSTEXPR BOOST_CONSTEXPR
#define POUTRE_CONSTEXPR_OR_CONST BOOST_CONSTEXPR_OR_CONST

  
// POUTRE_STRONG_INLINE is a stronger version of inline directive mostly to enforce inlining for MSVC compiler:using __forceinline on MSVC/INTEL,but still doesn't use GCC's/clang always_inline.
#if (defined _MSC_VER) || (defined(__ICC) || defined(__INTEL_COMPILER))
#define POUTRE_STRONG_INLINE __forceinline
#else
#define POUTRE_STRONG_INLINE inline
#endif

// POUTRE_ALWAYS_INLINE is the strongest version of inline directive, using __forceinline on MSVC/INTEL,and always inline for GCC/clang.
#define POUTRE_ALWAYS_INLINE BOOST_FORCEINLINE 

#if defined(_WIN32) || defined(_WIN64)
  #define poutreDeprecated __declspec(deprecated)
  #define poutreDeprecatedMSG(x) __declspec(deprecated(x))

#if defined(_M_X64) || defined(_WIN64)
#define POUTRE_64BITS
#endif

#else
  #define poutreDeprecated __attribute__((deprecated))
  #define poutreDeprecatedMSG(x) poutreDeprecated

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


#define POUTRE_RUNTIME_ERROR( MSG ) throw std::runtime_error((boost::format("Throw %s at %s(%d)") % MSG % __FILE__ % __LINE__).str())
#define POUTRE_NEVER_REACH_HERE POUTRE_RUNTIME_ERROR( "Code should never reach here, WTF Oo")
#define POUTRE_CHECK( CONDITION, MSG )           \
               {                                             \
      if ( !( CONDITION ) )                      \
                              {                                          \
         POUTRE_RUNTIME_ERROR( MSG );            \
                              }                                          \
               }

#ifdef _DEBUG
#if defined(_MSC_VER)
#define POUTRE_ASSERTCHECK( CONDITION, MSG ){    \
    if ( !( CONDITION ) )                        \
                        {                                          \
         __debugbreakpoint();                    \
                        }                                          \
               }
#else
#define POUTRE_ASSERTCHECK( CONDITION, MSG ){    \
    if ( !( CONDITION ) )                        \
                        {                                          \
         __builtin_trap();                       \
                        }                                          \
               }
#endif
#else
#define POUTRE_ASSERTCHECK( CONDITION, MSG )
#endif

} //namespace

#endif


