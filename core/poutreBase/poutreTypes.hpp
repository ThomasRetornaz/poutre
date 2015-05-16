#ifndef POUTRETYPE_HPP__
#define POUTRETYPE_HPP__

#ifndef POUTREBASE_HPP__
#include <poutreBase/poutreBase.hpp>
#endif

#include <cstdint>
//#include <string>
#include <iosfwd>

namespace poutre
  {
  using  pbool = bool;
  using  pUINT8 = unsigned char;
  using  pINT8 = signed char;
  using  pUINT32 = std::uint32_t;
  using  pINT32 = std::int32_t;
  using  pFLOAT = float;
  using  pDOUBLE = long double;
  using  pINT64 = std::int64_t;
  using  pUINT64 = std::uint64_t;

  using  scoord = pINT32; //scalar coordinate
  using  rcoord = pFLOAT; //real coordinate

  using  offset = std::ptrdiff_t;

  using  str_type = std::string;
  using  prealtype = pFLOAT; 




  /*!
  * @brief Compare Operator
  */
  enum class CompOpType
    {
    CompOpUndef = 0,       //!< Undefined type 
    CompOpEqual = 1 << 0, //!p1==p2 
    CompOpDiff = 1 << 1,     //!p1!=p2 
    CompOpSup = 1 << 2,      //!p1>p2 
    CompOpSupEqual = 1 << 3, //!p1>=p2 
    CompOpInf = 1 << 4,      //!p1<p2
    CompOpInfEqual = 1 << 5,  //!p1<=p2
    _CompOp_Max = 1 << 5 //sync with the max value
    };

  //! operator<< for CompOpType
  BASE_API std::ostream& operator<<(std::ostream&, CompOpType);

  //! operator>> for CompOpType
  BASE_API std::istream& operator>>(std::istream&, CompOpType&);


  /*!
  * @brief Arithmetic Operator
  */
  enum class ArithOpType
    {
    ArithOpUndef = 0,           //!< Undefined type     
    ArithOpPlus = 1 << 1,       //!r=a0+a1 
    ArithOpMinus = 1 << 2,      //!r=a0-a1
    ArithOpMul = 1 << 3,        //!r=a0*a1
    ArithOpDiv = 1 << 4,        //!r=a0/a1
    ArithOpMod = 1 << 5,        //!r=a0%a1
    _ArithOp_Max = 1 << 5 //sync with the max value
    };

  //! operator<< for ArithOpType
  BASE_API std::ostream& operator<<(std::ostream&, ArithOpType);

  //! operator>> for ArithOpType
  BASE_API std::istream& operator>>(std::istream&, ArithOpType&);

  /*!
  * @brief Assignment Operator
  */
  enum class AssignOpType
    {
    AssignOpUndef = 0,       //!< Undefined type 
    AssignOp = 1 << 0,       //!r=a0
    AssignOpAdd = 1 << 1,    //!r+=a0
    AssignOpMinus = 1 << 2,  //!r-=a0
    AssignOpMul = 1 << 3,    //!r*=a0
    AssignOpDiv = 1 << 4,    //!r/=a0
    AssignOpMod = 1 << 5,    //!r/=a0
    AssignOpNegate = 1 << 6,    //!r/=-a0
    _AssignOpType_Max = 1 << 6 //sync with the max value
    };

  //! operator<< for AssignOpType
  BASE_API std::ostream& operator<<(std::ostream&, ArithOpType);

  //! operator>> for AssignOpType
  BASE_API std::istream& operator>>(std::istream&, ArithOpType&);


}
#endif