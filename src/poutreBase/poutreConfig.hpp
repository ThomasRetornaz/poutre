
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================
#ifndef POUTRE_CONFIG__HPP__
#define POUTRE_CONFIG__HPP__

#ifndef FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY
#endif
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/printf.h>

/**
 * @file   poutreConfig.hpp
 * @author Thomas Retornaz
 * @brief  Various macro and alias
 *
 *
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace poutre
{
#  define POUTRE_NOEXCEPT noexcept // NOLINT

#  define POUTRE_NOEXCEPT_IF(Predicate) noexcept((Predicate)) // NOLINT

#  define POUTRE_NOEXCEPT_EXPR(Predicate) noexcept((Predicate)) // NOLINT

#  ifndef NDEBUG
#    define POUTRE_NOEXCEPTONLYNDEBUG // NOLINT
#  else
#    define POUTRE_NOEXCEPTONLYNDEBUG POUTRE_NOEXCEPT // NOLINT
#  endif

#  define POUTRE_STATIC_CONSTEXPR static constexpr // NOLINT

#  define POUTRE_CONSTEXPR constexpr // NOLINT

#  define POUTRE_CONSTEXPR constexpr // NOLINT

// POUTRE_STRONG_INLINE is a stronger version of inline directive mostly to enforce inlining for
// MSVC compiler:using
// __forceinline on MSVC/INTEL,but still doesn't use GCC's/clang always_inline.
#  if( defined _MSC_VER ) || (defined(__ICC) || defined(__INTEL_COMPILER))
#    define POUTRE_STRONG_INLINE __forceinline // NOLINT
#  else
#    define POUTRE_STRONG_INLINE inline // NOLINT
#  endif

// POUTRE_ALWAYS_INLINE is the strongest version of inline directive, using __forceinline on
// MSVC/INTEL,and always inline for GCC/clang.
#  if defined(_MSC_VER)
#    define POUTRE_ALWAYS_INLINE __forceinline // NOLINT
#  elif defined(__GNUC__) && __GNUC__ > 3
  // Clang also defines __GNUC__ (as 4)
#    define POUTRE_ALWAYS_INLINE inline __attribute__((__always_inline__)) // NOLINT
#  else
#    define POUTRE_ALWAYS_INLINE inline // NOLINT
#  endif

#  if defined(_WIN32) || defined(_WIN64)
#    define poutreDeprecated       __declspec(deprecated) // NOLINT
#    define poutreDeprecatedMSG(x) __declspec(deprecated(x)) // NOLINT

#    if defined(_M_X64) || defined(_WIN64)
#      define POUTRE_64BITS
#    endif

#  else
#    define poutreDeprecated       __attribute__((deprecated))
#    define poutreDeprecatedMSG(x) poutreDeprecated

#    if defined(__LP64__)
#      define POUTRE_64BITS
#    endif

#  endif

  // Generic helper definitions for shared library support
#  if defined _WIN32 || defined __CYGWIN__
#    define MODULE_IMPORT __declspec(dllimport) // NOLINT
#    define MODULE_EXPORT __declspec(dllexport) // NOLINT
#    define MODULE_LOCAL
#  else
#    if __GNUC__ >= 4
#      define MODULE_IMPORT __attribute__((visibility("default")))
#      define MODULE_EXPORT __attribute__((visibility("default")))
#      define MODULE_LOCAL  __attribute__((visibility("hidden")))
#    else
#      define MODULE_IMPORT
#      define MODULE_EXPORT
#      define MODULE_LOCAL
#    endif
#  endif

#  define PROJECT_NAME__ Poutre // NOLINT

  // plateform specific section
#  if defined(_WIN32) || defined(_WIN64)

#    ifndef _CPPRTTI
#      error You should activate the RTTI for PROJECT_NAME__ to make it run correctly
#    endif

#    ifndef _CPPUNWIND
#      error You should activate the exception handling for PROJECT_NAME__ to make it run correcly
#    endif

#  endif

  template<typename... Args> inline std::string format(const char *fmt, const Args &...args)
  {
    return ::fmt::format(fmt, args...);
  }

// NOLINTNEXTLINE
#  define POUTRE_RUNTIME_ERROR(MSG)                                                                                         \
    throw std::runtime_error(fmt::format("Throw {%s} at {%s} {%S}", MSG, __FILE__, __LINE__)) //-V1003 //NOLINT
// NOLINTNEXTLINE
#  define POUTRE_NEVER_REACH_HERE                                                                                           \
    POUTRE_RUNTIME_ERROR("Code should never reach here, WTF Oo") // NOLINT
                                                                 // NOLINTNEXTLINE
#  define POUTRE_CHECK(CONDITION, MSG)                                                                                      \
    {                                                                                                                       \
      if( !(CONDITION) )                                                                                                    \
      {                                                                                                                     \
        POUTRE_RUNTIME_ERROR(MSG);                                                                                          \
      }                                                                                                                     \
    }

#  ifdef _DEBUG
#    if defined(_MSC_VER)
#      define POUTRE_ASSERTCHECK(CONDITION, MSG)                                                                            \
        {                                                                                                                   \
          if( !(CONDITION) )                                                                                                \
          {                                                                                                                 \
            __debugbreak();                                                                                                 \
          }                                                                                                                 \
        }
#    else
#      define POUTRE_ASSERTCHECK(CONDITION, MSG)                                                                            \
        {                                                                                                                   \
          if( !(CONDITION) )                                                                                                \
          {                                                                                                                 \
            __builtin_trap();                                                                                               \
          }                                                                                                                 \
        }
#    endif
#  else
#    define POUTRE_ASSERTCHECK(CONDITION, MSG)
#  endif

} // namespace poutre
#endif // DOXYGEN_SHOULD_SKIP_THIS
#endif
