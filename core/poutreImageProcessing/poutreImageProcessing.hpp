#ifndef POUTREIMAGEPROCESSING_HPP__
#define POUTREIMAGEPROCESSING_HPP__

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef PoutreImageProcessing_EXPORTS // defined if we are building the POUTRE DLL (instead of using it)
#define BASE_API MODULE_EXPORT
#else
#define BASE_API MODULE_IMPORT
#endif // POUTRE_DLL_EXPORTS
#define BASE_LOCAL MODULE_LOCAL
#else // POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define BASE_API
#define BASE_LOCAL
#endif // POUTRE_DLL

namespace poutre
  {

   //TODO Doxydoc module

  }
#endif