
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_BASE_HPP__
#define POUTRE_BASE_HPP__

/**
 * @file   poutreBase.hpp
 * @author Thomas Retornaz
 * @brief  Define import/export for shared libraries
 *
 *
 */

#include <poutreBase/poutreConfig.hpp>

#ifdef POUTRE_DYNAMIC       // defined if POUTRE is compiled as a DLL
#  ifdef PoutreBase_EXPORTS // defined if we are building the POUTRE DLL (instead of using it)
#    define BASE_API MODULE_EXPORT
#  else
#    define BASE_API MODULE_IMPORT
#  endif // POUTRE_DLL_EXPORTS
#  define BASE_LOCAL MODULE_LOCAL
#else // POUTRE_DLL is not defined: this means POUTRE is a static lib.
#  define BASE_API
#  define BASE_LOCAL
#endif // POUTRE_DLL

namespace poutre
{
  /**
   * @addtogroup poutre_base_group Base/Common objects and types
   *@{
   */
  //! @} doxygroup: poutre_base_group
} // namespace poutre
#endif // POUTRE_BASE_HPP__
