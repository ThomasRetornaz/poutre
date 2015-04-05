#ifndef POUTRETYPE_HPP__
#define POUTRETYPE_HPP__

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif


#include <cstdint>
#include <string>

namespace poutre
  {
  using  pbool = bool;
  using  pUINT8 = unsigned char;
  using  pINT8 = signed char;
  using  pUINT32 = std::uint32_t;
  using  pINT32 = std::int32_t;
  using  pFLOAT = float;
  //using  pDOUBLE = double;
  using  pINT64 = std::int64_t;
  using  pUINT64 = std::uint64_t;

  using  scoord = pINT32; //scalar coordinate
  using  rcoord = pFLOAT; //real coordinate

  using  offset = std::ptrdiff_t;

  using  str_type = std::string;
  using  prealtype = pFLOAT; 
}
#endif