
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_TYPE_HPP__
#define POUTRE_TYPE_HPP__

/**
 * @file   poutreTypes.hpp
 * @author Thomas Retornaz
 * @brief  Common type alias and helpers
 * 
 * 
 */

#ifndef POUTRE_BASE_HPP__
#include <poutreBase/poutreBase.hpp>
#endif

#include <cstdint>
//#include <string>
#include <iosfwd>

namespace poutre
  {
  using  pbool = bool; //!boolean type
  using  pUINT8 = unsigned char; //!UINT8 type
  using  pINT8 = signed char; //!INT8 type
  using  pUINT32 = std::uint32_t; //!UINT32 type
  using  pINT32 = std::int32_t; //!INT32 type
  using  pFLOAT = float;        //!Float/Real type (default real type)
  using  pDOUBLE = long double; //!Float/Real type
  using  pINT64 = std::int64_t; //!INT64 type
  using  pUINT64 = std::uint64_t; //!UINT64 type

  using  scoord = pINT32; //!scalar coordinate
  using  rcoord = pFLOAT; //!real coordinate

  using  offset = std::ptrdiff_t; //!offset

  using  str_type = std::string;  //! string type
  using  prealtype = pFLOAT;  //! real type @see @c pFLOAT




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
  BASE_API std::ostream& operator<<(std::ostream&, AssignOpType);

  //! operator>> for AssignOpType
  BASE_API std::istream& operator>>(std::istream&, AssignOpType&);


}
#endif //POUTRE_TYPE_HPP__
