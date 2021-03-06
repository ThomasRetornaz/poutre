// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreImageProcessingCore/include/poutreImageProcessingContainerCopieConvert.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterfaceCopieConvert.hpp>

namespace poutre
{
  std::unique_ptr<IInterface> Clone(const IInterface &i_img1)
  {
    POUTRE_ENTERING("Clone");
    return i_img1.Clone();
  }

  std::unique_ptr<IInterface> CloneGeometry(const IInterface &i_img1) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("CloneGeometry");
    auto imgType = i_img1.GetImgType();
    switch( imgType )
    {
    case ImgType::ImgType_Dense:
    {
      return CreateDense(i_img1.GetCoords(), i_img1.GetCType(), i_img1.GetPType());
    }
    break;
    default: POUTRE_RUNTIME_ERROR(poutre::format("CloneGeometry:: unsupported img type {}", imgType));
    }
  }

  std::unique_ptr<IInterface>
  ConvertGeometry(const IInterface &i_img1, CompoundType ctype, PType ptype) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("ConvertGeometry");
    auto imgType = i_img1.GetImgType();
    switch( imgType )
    {
    case ImgType::ImgType_Dense:
    {
      return CreateDense(i_img1.GetCoords(), ctype, ptype);
    }
    break;
    default: POUTRE_RUNTIME_ERROR(poutre::format("ConvertGeometry:: unsupported img type{}", imgType));
    }
  }

  std::unique_ptr<IInterface> ConvertGeometry(const IInterface &i_img1, PType ptype)
  {
    POUTRE_ENTERING("ConvertGeometry");
    return ConvertGeometry(i_img1, i_img1.GetCType(), ptype);
  }

  void Copy(const IInterface &i_img1, IInterface &i_img2) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("Copy");
    AssertSizesCompatible(i_img1, i_img2, "Copy images have not compatible sizes");
    AssertAsTypesCompatible(i_img1, i_img2, "Copy images have not compatible types may you have to use convert instead");
    auto dims = i_img1.GetNumDims();
    switch( dims )
    {
    case 1:
    {
    }
    break;
    case 2:
    {
    }
    break;
    default: POUTRE_RUNTIME_ERROR(poutre::format("Unsupported number of dims {:%D}", dims));
    }
  }
} // namespace poutre
