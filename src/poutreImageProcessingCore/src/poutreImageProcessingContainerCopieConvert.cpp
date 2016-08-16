
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreImageProcessingCore/include/poutreImageProcessingContainerCopieConvert.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterfaceCopieConvert.hpp>
#include <boost/lexical_cast.hpp>

namespace poutre
  {

  std::unique_ptr<IInterface> Clone(const IInterface& i_img1)
    {
    return i_img1.Clone( );
    }


  std::unique_ptr<IInterface> CloneGeometry(const IInterface& i_img1, CompoundType ctype, PType ptype)
    {
    auto imgType=i_img1.GetImgType( );
    switch (imgType)
      {
        case ImgType::ImgType_Dense:
          {
          return CreateDense(i_img1.GetCoords( ), ctype, ptype);
          }break;
        default:
          POUTRE_RUNTIME_ERROR(("CloneGeometry:: unsupported img type" + boost::lexical_cast<std::string>(imgType)));
      }
    }

  std::unique_ptr<IInterface> CloneGeometry(const IInterface& i_img1, PType ptype)
    {
    return CloneGeometry(i_img1, i_img1.GetCType(), ptype);
    }

  }
