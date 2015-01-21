#ifndef POUTRETYPE_HPP__
#define POUTRETYPE_HPP__

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#include <boost/cstdint.hpp>
#include <string>

namespace poutre
  {
  typedef bool                    pbool;

  typedef unsigned char           pUINT8;
  typedef signed char             pINT8;
  typedef unsigned short int      pUINT16;
  typedef signed short int        pINT16;
  typedef uint32_t                pUINT32;
  typedef int32_t                 pINT32;
  typedef float                   pFLOAT;
  typedef double                  pDOUBLE;
#ifdef _WIN32
  typedef __int64                 pINT64;
  typedef unsigned __int64        pUINT64;
#else
  typedef long long int           pINT64;   
  typedef unsigned long long int  pUINT64;  
#endif

  typedef pINT32                   scoord;
  typedef pFLOAT                   rcoord;

#ifdef POUTRE_64BITS
  typedef pINT64                 offset;
#else
  typedef pINT32                 offset;
#endif

  typedef std::string             str_type;
  typedef std::wstring            wstr_type;

  typedef pFLOAT REAL_TYPE; //todo make this configurable trough cmake or add dispatch

  }
#endif