
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#define POUTRE_IMAGEPROCESSINGCORE_HPP__

/**
 * @file   poutreImageProcessingCore.hpp
 * @author Thomas Retornaz
 * @brief  Define import/export for shared librairies
 *
 *
 */

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifdef POUTRE_DYNAMIC       // defined if POUTRE is compiled as a DLL
#ifdef PoutreIPCore_EXPORTS // defined if we are building the POUTRE DLL (instead of using it)
#define IMP_CORE_API MODULE_EXPORT
#else
#define IMP_CORE_API MODULE_IMPORT
#endif // POUTRE_DLL_EXPORTS
#define IMP_CORE_LOCAL MODULE_LOCAL
#else // POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define IMP_CORE_API
#define IMP_CORE_LOCAL
#endif // POUTRE_DLL

namespace poutre
{

    // TODO Doxydoc module

}
#endif // POUTRE_IMAGEPROCESSINGCORE_HPP__
