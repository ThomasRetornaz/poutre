#ifndef POUTRETYPE_HPP__
#define POUTRETYPE_HPP__

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#include <boost/cstdint.hpp>
#include <string>

namespace poutre
  {
  using  pbool = bool;
  using  pUINT8 = unsigned char;
  using  pINT8 = signed char;
  using  pUINT32 = uint32_t;
  using  pINT32 = int32_t;
  using  pFLOAT = float;
  //using  pDOUBLE = double;
  using  pINT64 = int64_t;
  using  pUINT64 = uint64_t;

  using  scoord = pINT32; //scalar coordinate
  using  rcoord = pFLOAT; //real coordinate

#ifdef POUTRE_64BITS
  using  offset = pINT64;
#else
  using  offset = pINT32;
#endif
  using  str_type = std::string;
  using  prealtype = pFLOAT; 
}
#endif