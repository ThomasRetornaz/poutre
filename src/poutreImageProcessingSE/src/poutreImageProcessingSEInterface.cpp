// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2018 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreImageProcessingSE/poutreImageProcessingSEInterface.hpp>

#include <string>

namespace poutre
{
  namespace se
  {
    std::ostream &operator<<(std::ostream &os, SEType seType)
    {
      switch( seType )
      {
      case SEType::SEType_NL: os << "SE_Neighborlist"; break;
      case SEType::SEType_Image: os << "SE_Image"; break;
      default: //  SEType::SEType_Undef;
        os << "Unknown SEType";
        break;
      }
      return os;
    }

    std::istream &operator>>(std::istream &is, SEType &seType)
    {
      seType = SEType::SEType_Undef;

      if( !is.good() )
        return is;

      std::string strType;
      is >> strType;
      if( is.bad() )
        return is;

      if( strType == "SE_Neighborlist" )
        seType = SEType::SEType_NL;
      else if( strType == "SE_Image" )
        seType = SEType::SEType_Image;
      else
      {
        POUTRE_RUNTIME_ERROR("Unable to read SEType from stream");
      }
      return is;
    }

    std::ostream &operator<<(std::ostream &os, const IStructuringElement &se) { return os; }

  } // namespace se
} // namespace poutre
