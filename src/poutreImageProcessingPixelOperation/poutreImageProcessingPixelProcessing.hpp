
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   poutreImageProcessing.hpp
 * @author Thomas Retornaz
 * @brief  Define import/export for shared libraries
 *
 *
 */

#include <poutreBase/poutreConfig.hpp>
#include <poutreBase/poutreTrace.hpp>

#ifdef POUTRE_DYNAMIC                                 // defined if POUTRE is compiled as a DLL
#  ifdef PoutreIPPixelProcessing_EXPORTS // defined if we are building the POUTRE DLL
                                                      // (instead of using it)
#    define IMP_API MODULE_EXPORT
#  else
#    define IMP_API MODULE_IMPORT
#  endif // POUTRE_DLL_EXPORTS
#  define IMP_LOCAL MODULE_LOCAL
#else // POUTRE_DLL is not defined: this means POUTRE is a static lib.
#  define IMP_API
#  define IMP_LOCAL
#endif // POUTRE_DLL

namespace poutre
{
  // TODO Doxydoc module
  IMP_API void dummy();

} // namespace poutre
