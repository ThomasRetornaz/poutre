// This is an open source non-commercial project. Dear PVS-Studio, please check
// it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"

#include <iostream>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainerCopieConvert.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterfaceCopieConvert.hpp>

TEST(poutreImageProcessingCopyConvert,Clone) {
  using ImageType = poutre::DenseImage<poutre::pUINT8>;
  using ImageTypeInterface = std::unique_ptr<poutre::IInterface>;
  ImageType img({3, 4}); //-V112
  EXPECT_EQ(img.GetPType(), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  ImageTypeInterface getSameii = poutre::Clone(img);

  ImageType *getSame = dynamic_cast<ImageType *>(getSameii.get());
  EXPECT_TRUE(getSame);
  //!= address
  EXPECT_NE(getSame, &(img));
  //!= data address
  EXPECT_NE(&(*((*getSame).data())), &(*(img.data()))); //-V522
  EXPECT_EQ((*getSame).GetCType(),
                    poutre::CompoundType::CompoundType_Scalar);
  EXPECT_EQ((*getSame).GetPType(), poutre::PType::PType_GrayUINT8);
  EXPECT_EQ((*getSame).GetImgType(), poutre::ImgType::ImgType_Dense);
  EXPECT_EQ((*getSame).GetXSize(), 4); //-V112
  EXPECT_EQ((*getSame).GetYSize(), 3);
  EXPECT_EQ((*getSame).GetNumDims(), 2);
  EXPECT_TRUE(!(*getSame).empty());
  auto getSamecoords = (*getSame).GetCoords();
  std::vector<std::size_t> getSameexpectedcoords = {3, 4}; //-V112
  EXPECT_TRUE(CheckEqualCollections(getSamecoords.begin(), getSamecoords.end(), getSameexpectedcoords.begin()));

  for (const auto &var : (*getSame)) {
    EXPECT_EQ(var, 10);
  }
}

TEST(poutreImageProcessingCopyConvert,getsamecoord) {
  using ImageTypeBase = poutre::DenseImage<poutre::pUINT8>;
  using ImageTypeInterface = std::unique_ptr<poutre::IInterface>;
  ImageTypeBase img({3, 4}); //-V112
  EXPECT_EQ(img.GetPType(), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  using ImageType = poutre::DenseImage<poutre::pFLOAT>;
  ImageTypeInterface getSameii =
      poutre::ConvertGeometry(img, poutre::PType::PType_F32);

  ImageType *getSame = dynamic_cast<ImageType *>(getSameii.get());
  EXPECT_TRUE(getSame);
  EXPECT_EQ((*getSame).GetCType(), //-V522
                    poutre::CompoundType::CompoundType_Scalar); //-V522
  EXPECT_EQ((*getSame).GetPType(), poutre::PType::PType_F32);
  EXPECT_EQ((*getSame).GetImgType(), poutre::ImgType::ImgType_Dense);
  EXPECT_EQ((*getSame).GetXSize(), 4); //-V112
  EXPECT_EQ((*getSame).GetYSize(), 3);
  EXPECT_EQ((*getSame).GetNumDims(), 2);
  auto getSamecoords = (*getSame).GetCoords();
  std::vector<std::size_t> getSameexpectedcoords = {3, 4}; //-V112
  EXPECT_TRUE(CheckEqualCollections(getSamecoords.begin(), getSamecoords.end(), getSameexpectedcoords.begin()));
}

TEST(poutreImageProcessingCopyConvert,copy_same_type) {
  { // integral types
    using ImageTypeBase = poutre::DenseImage<poutre::pUINT8>;

    ImageTypeBase img({3, 4}); //-V112
    img.assign(10);
    ImageTypeBase img2({3, 4}); //-V112
    img2.assign(4);             //-V112
    poutre::t_Copy(img, img2);
    EXPECT_TRUE(CheckEqualCollections(img.begin(), img.end(), img2.begin()));
  }
  { // compound
    using ImageTypeBase = poutre::DenseImage<poutre::c3pUINT8>;
    ImageTypeBase img({3, 4}); //-V112
    img.assign({1, 2, 3});
    ImageTypeBase img2({3, 4}); //-V112
    img2.assign({4, 5, 6});     //-V112
    poutre::t_Copy(img, img2);
    EXPECT_TRUE(CheckEqualCollections(img.begin(), img.end(), img2.begin()));
  }
}
