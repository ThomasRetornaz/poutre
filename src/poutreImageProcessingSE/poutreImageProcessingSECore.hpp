
//==============================================================================
//                  Copyright (c) 2018 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IPSECORE_HPP__
#define POUTRE_IPSECORE_HPP__

#include <poutreBase/poutreConfig.hpp>

/**
 * @file   poutreImageProcessingSECore.hpp
 * @author Thomas Retornaz
 * @brief  Structuring ELement Import/Export
 *
 *
 */

#ifdef POUTRE_DYNAMIC       // defined if POUTRE is compiled as a DLL
#  ifdef PoutreIPSE_EXPORTS // defined if we are building the POUTRE DLL (instead of using it)
#    define IPPSE_API MODULE_EXPORT
#  else
#    define IPPSE_API MODULE_IMPORT
#  endif // POUTRE_DLL_EXPORTS
#  define IPPSE_LOCAL MODULE_LOCAL
#else // POUTRE_DLL is not defined: this means POUTRE is a static lib.
#  define IPPSE_API
#  define IPPSE_LOCAL
#endif // POUTRE_DLL

namespace poutre
{
  // TODO Doxydoc group
}
#endif // POUTRE_IPSECORE_HPP__