
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_NEURALNETS_HPP__
#define POUTRE_NEURALNETS_HPP__

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifdef POUTRE_DYNAMIC           // defined if POUTRE is compiled as a DLL
#ifdef PoutreNeuralNets_EXPORTS // defined if we are building the POUTRE DLL (instead of using it)
#define NN_API MODULE_EXPORT
#else
#define NN_API MODULE_IMPORT
#endif // POUTRE_DLL_EXPORTS
#define NN_LOCAL MODULE_LOCAL
#else // POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define NN_API
#define NN_LOCAL
#endif // POUTRE_DLL

namespace poutre
{
    NN_API void dummy(void);

}
#endif // POUTRE_NEURALNETS_HPP__