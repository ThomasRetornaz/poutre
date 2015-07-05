
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreBase/poutreTypes.hpp>
#include <sstream>

namespace poutre
  {
  //NOTE JUST FOR CONVENIENCE AT INTERFACE LAYER TO USE STRING INSTEAD OF VERBOSE ENUM

  std::ostream& operator<<(std::ostream& os, CompOpType arithOpType)
    {
    switch (arithOpType)
      {
        case CompOpType::CompOpDiff:
          os << "!=";
          break;
        case CompOpType::CompOpEqual:
          os << "==";
          break;
        case CompOpType::CompOpInf:
          os << "<";
          break;
        case CompOpType::CompOpInfEqual:
          os << "<=";
          break;
        case CompOpType::CompOpSup:
          os << ">";
          break;
        case CompOpType::CompOpSupEqual:
          os << ">=";
          break;
        default://  CompOpType::CompOpUndef; 
          os << "Unknown CompOpType";
          break;
      }
    return os;
    }


  std::istream& operator>>(std::istream& is, CompOpType& arithOpType)
    {
    arithOpType = CompOpType::CompOpUndef;

    if (!is.good( ))
      return is;

    std::string strType;
    is >> strType;
    if (is.bad( ))
      return is;

    if (strType == "!=")
      arithOpType = CompOpType::CompOpDiff;
    else if (strType == "==")
      arithOpType = CompOpType::CompOpEqual;
    else if (strType == "<")
      arithOpType = CompOpType::CompOpInf;
    else if (strType == ">")
      arithOpType = CompOpType::CompOpSup;
    else if (strType == "<=")
      arithOpType = CompOpType::CompOpInfEqual;
    else if (strType == ">=")
      arithOpType = CompOpType::CompOpSupEqual;
    else
      {
      POUTRE_RUNTIME_ERROR("Unable to read arithOpType from stream");
      }
    return is;
    }


 std::ostream& operator<<(std::ostream& os, ArithOpType arithOpType)
    {
    switch (arithOpType)
      {
        case ArithOpType::ArithOpPlus:
          os << "+";
          break;
        case ArithOpType::ArithOpMinus:
          os << "-";
          break;
        case ArithOpType::ArithOpMul:
          os << "*";
          break;
        case ArithOpType::ArithOpDiv:
          os << "/";
          break;
        case ArithOpType::ArithOpMod:
          os << "%";
          break;
        default://  ArithOpType::; 
          os << "Unknown ArithOpType";
          break;
      }
    return os;
    }


  std::istream& operator>>(std::istream& is, ArithOpType& arithOpType)
    {
    arithOpType = ArithOpType::ArithOpUndef;

    if (!is.good( ))
      return is;

    std::string strType;
    is >> strType;
    if (is.bad( ))
      return is;

    if (strType == "+")
      arithOpType = ArithOpType::ArithOpPlus;
    else if (strType == "-")
      arithOpType = ArithOpType::ArithOpMinus;
    else if (strType == "/")
      arithOpType = ArithOpType::ArithOpDiv;
    else if (strType == "%")
      arithOpType = ArithOpType::ArithOpMod;
    else if (strType == "*")
      arithOpType = ArithOpType::ArithOpMul;
    else
      {
      POUTRE_RUNTIME_ERROR("Unable to read ArithOpType from stream");
      }
    return is;
    }



  std::ostream& operator<<(std::ostream& os, AssignOpType assignOpType)
    {
    switch (assignOpType)
      {
        case AssignOpType::AssignOpAdd:
          os << "+=";
          break;
        case AssignOpType::AssignOpMinus:
          os << "-=";
          break;
        case AssignOpType::AssignOpMul:
          os << "*=";
          break;
        case AssignOpType::AssignOpDiv:
          os << "/=";
          break;
        case AssignOpType::AssignOpMod:
          os << "%=";
          break;
        case AssignOpType::AssignOp:
          os << "=";
          break;
        default://  AssignOpType::; 
          os << "Unknown AssignOpType";
          break;
      }
    return os;
    }


  std::istream& operator>>(std::istream& is, AssignOpType& assignOpType)
    {
    assignOpType = AssignOpType::AssignOpUndef;

    if (!is.good( ))
      return is;

    std::string strType;
    is >> strType;
    if (is.bad( ))
      return is;

    if (strType == "+=")
      assignOpType = AssignOpType::AssignOpAdd;
    else if (strType == "-=")
      assignOpType = AssignOpType::AssignOpMinus;
    else if (strType == "*=")
      assignOpType = AssignOpType::AssignOpMul;
    else if (strType == "/=")
      assignOpType = AssignOpType::AssignOpDiv;
    else if (strType == "%=")
      assignOpType = AssignOpType::AssignOpMod;
    else if (strType == "=")
      assignOpType = AssignOpType::AssignOp;
    else
      {
      POUTRE_RUNTIME_ERROR("Unable to read AssignOpType from stream");
      }
    return is;
    }

  
  

  }
