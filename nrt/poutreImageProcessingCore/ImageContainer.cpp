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
#include "poutreBase/poutreGeometry.hpp"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <poutreBase/poutreContainerView.hpp>
#include <poutreIO/poutreIOString.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <vector>


TEST(poutreImageProcessingContainer,ctor) {
  poutre::DenseImage<poutre::pUINT8> img({3, 4}); //-V112
  EXPECT_EQ(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  EXPECT_EQ(img.GetPType(), poutre::PType::PType_GrayUINT8);
  EXPECT_EQ(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  EXPECT_EQ(img.GetXSize(), 4); //-V112
  EXPECT_EQ(img.GetYSize(), 3);
  EXPECT_EQ(img.GetNumDims(), 2);
  EXPECT_TRUE(!img.empty());
  auto coords = img.GetCoords();
  auto expectedcoords = {3, 4}; //-V112
  EXPECT_TRUE(CheckEqualCollections(coords.begin(), coords.end(), expectedcoords.begin()));

  /*std::cout << "**************************************************" <<
  std::endl; std::cout << img << std::endl; std::cout <<
  "**************************************************" << std::endl;*/
}

TEST(poutreImageProcessingContainer,clone) {
  using ImageType = poutre::DenseImage<poutre::pUINT8>;
  using ImageTypeInterface = std::unique_ptr<poutre::IInterface>;
  ImageType img({3, 4}); //-V112
  EXPECT_EQ(img.GetPType(), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  ImageTypeInterface clonedinterface = img.Clone();

  ImageType *cloned = dynamic_cast<ImageType *>(clonedinterface.get());
  EXPECT_TRUE(cloned);
  //!= address
  EXPECT_NE(cloned, &(img));
  //!= data address
  EXPECT_NE(&(*((*cloned).data())), &(*(img.data()))); //-V522
  EXPECT_EQ((*cloned).GetPType(), poutre::PType::PType_GrayUINT8); //-V522
  EXPECT_EQ((*cloned).GetCType(),
                    poutre::CompoundType::CompoundType_Scalar);
  EXPECT_EQ((*cloned).GetImgType(), poutre::ImgType::ImgType_Dense);
  EXPECT_EQ((*cloned).GetXSize(), 4); //-V112
  EXPECT_EQ((*cloned).GetYSize(), 3);
  EXPECT_EQ((*cloned).GetNumDims(), 2);
  EXPECT_TRUE(!(*cloned).empty());
  auto coords = (*cloned).GetCoords();
  auto expectedcoords = {3, 4}; //-V112
  EXPECT_TRUE(CheckEqualCollections(coords.begin(), coords.end(), expectedcoords.begin()));
  for (const auto &var : (*cloned)) {
    EXPECT_EQ(var, 10);
  }

  //  std::cout << "**************************************************" <<
  //  std::endl; std::cout << (*cloned) << std::endl; std::cout <<
  //  "**************************************************" << std::endl;
}

TEST(poutreImageProcessingContainer,Factory) {
  {
    auto img =
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, //-V112
                            poutre::PType::PType_GrayUINT8); //-V112
      EXPECT_TRUE(img);
    EXPECT_EQ((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    EXPECT_EQ((*img).GetCType(),
                      poutre::CompoundType::CompoundType_Scalar);
    EXPECT_EQ((*img).GetPType(), poutre::PType::PType_GrayUINT8);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4}; //-V112
    EXPECT_TRUE(CheckEqualCollections(coords.begin(), coords.end(), expectedcoords.begin()));
    EXPECT_TRUE(dynamic_cast<poutre::DenseImage<poutre::pUINT8> *>(img.get()));
  }

  {
    auto img =
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, //-V112
                            poutre::PType::PType_GrayINT32); //-V112
      EXPECT_TRUE(img);
    EXPECT_EQ((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    EXPECT_EQ((*img).GetCType(),
                      poutre::CompoundType::CompoundType_Scalar);
    EXPECT_EQ((*img).GetPType(), poutre::PType::PType_GrayINT32);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4}; //-V112
    EXPECT_TRUE(CheckEqualCollections(coords.begin(), coords.end(), expectedcoords.begin()));
    EXPECT_TRUE(dynamic_cast<poutre::DenseImage<poutre::pINT32> *>(img.get()));
  }
  {
    auto img =
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, //-V112
                            poutre::PType::PType_F32); //-V112
      EXPECT_TRUE(img);
    EXPECT_EQ((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    EXPECT_EQ((*img).GetCType(),
                      poutre::CompoundType::CompoundType_Scalar);
    EXPECT_EQ((*img).GetPType(), poutre::PType::PType_F32);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4}; //-V112
    EXPECT_TRUE(CheckEqualCollections(coords.begin(), coords.end(), expectedcoords.begin()));
    EXPECT_TRUE(dynamic_cast<poutre::DenseImage<poutre::pFLOAT> *>(img.get()));
  }
  {
    auto img =
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, //-V112
                            poutre::PType::PType_GrayINT64); //-V112
      EXPECT_TRUE(img);
    EXPECT_EQ((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    EXPECT_EQ((*img).GetPType(), poutre::PType::PType_GrayINT64);
    EXPECT_EQ((*img).GetCType(),
                      poutre::CompoundType::CompoundType_Scalar);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4}; //-V112
    EXPECT_TRUE(CheckEqualCollections(coords.begin(), coords.end(), expectedcoords.begin()));
    EXPECT_TRUE(dynamic_cast<poutre::DenseImage<poutre::pINT64> *>(img.get()));
  }
  {
    EXPECT_THROW(
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, //-V112
                            poutre::PType::PType_Undef),
        std::runtime_error); //-V112
      EXPECT_THROW(
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Undef, //-V112
                            poutre::PType::PType_GrayINT64),
        std::runtime_error); //-V112
  }
}

TEST(poutreImageProcessingContainer,BasicIteration) {
  poutre::DenseImage<poutre::pUINT8> img({3, 4}); //-V112
  EXPECT_EQ(img.GetPType(), poutre::PType::PType_GrayUINT8);
  EXPECT_EQ(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  img.assign(10);

  for (const auto &var : img) {
    EXPECT_EQ(var, 10);
  }
  auto count = 0;
  for (auto it = img.begin(); it != img.end(); ++it, ++count) {
    EXPECT_EQ(*it, 10);
  }
  EXPECT_EQ(count, 12);
  count = 0;
  for (auto it = img.cbegin(); it != img.cend(); ++it, ++count) {
    EXPECT_EQ(*it, 10);
  }
  EXPECT_EQ(count, 12);

  // count = 0;
  // auto rit = img.rbegin();
  // auto rend = img.rend();
  // for (; rit != rend; ++rit, ++count) {
  //   EXPECT_EQ(*rit, 10);
  // }
  // EXPECT_EQ(count, 12);

  // count = 0;
  // auto crit = img.crbegin();
  // auto crend = img.crend();
  // for (; crit != crend; ++crit, ++count) {
  //   EXPECT_EQ(*crit, 10);
  // }
  // EXPECT_EQ(count, 12);

  // for (size_t i = 0u; i < img.size(); i++) {
  //   EXPECT_EQ(img[i], 10);
  //   EXPECT_EQ(img.at(i), 10);
  // }
  // EXPECT_EQ(img.front(), 10);
  // EXPECT_EQ(img.back(), 10);
}

TEST(poutreImageProcessingContainer,move) {
  // todo check assembly
  auto img(poutre::DenseImage<poutre::pUINT8>{3, 4}); //-V112
  EXPECT_EQ(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  EXPECT_EQ(img.GetPType(), poutre::PType::PType_GrayUINT8);
  EXPECT_EQ(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);

  auto coords = img.GetCoords();
  auto expectedcoords = {3, 4}; //-V112
  EXPECT_TRUE(CheckEqualCollections(coords.begin(), coords.end(), expectedcoords.begin()));

  poutre::DenseImage<poutre::pUINT8> img2({2, 3});
  EXPECT_EQ(img2.GetImgType(), poutre::ImgType::ImgType_Dense);
  EXPECT_EQ(img2.GetPType(), poutre::PType::PType_GrayUINT8);
  EXPECT_EQ(img2.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  auto coords2 = img2.GetCoords();
  auto expectedcoords2 = {2, 3};
  EXPECT_TRUE(CheckEqualCollections(coords2.begin(), coords2.end(), expectedcoords2.begin()));

  // move
  img2 = std::move(img);
  EXPECT_EQ(img2.GetImgType(), poutre::ImgType::ImgType_Dense);
  EXPECT_EQ(img2.GetPType(), poutre::PType::PType_GrayUINT8);
  EXPECT_EQ(img2.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  auto coordsmove = img2.GetCoords();
  auto expectedcoordsmove = {3, 4}; //-V112
  EXPECT_TRUE(CheckEqualCollections(coordsmove.begin(), coordsmove.end(), expectedcoordsmove.begin()));
}

TEST(poutreImageProcessingContainer,viewoverimage) {
  poutre::DenseImage<poutre::pUINT8> img1({3, 4}); //-V112
  // start view
  auto vimg1 = poutre::view(img1);
  EXPECT_EQ(vimg1.size(), 12);
  EXPECT_EQ(vimg1.bound(), (poutre::bd2d{3, 4})); //-V112
  EXPECT_EQ(vimg1.stride(), (poutre::idx2d{4, 1})); //-V112
}

TEST(poutreImageProcessingContainer,SetPixelGetPixel) {
  using ImageType = poutre::DenseImage<poutre::pINT32>;
  auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  auto img = dynamic_cast<ImageType *>(imgin.get());
  EXPECT_TRUE(img);
  (*img).SetPixel(2, 0, 10);  // x then y //-V522
  poutre::idx2d pix = {3, 0}; // y then x //-V522
  (*img).SetPixel(pix, 8);
  poutre::pt2D_scoord pix2 = {5, 3}; // x then y
  (*img).SetPixel(pix2, 9);

//  std::string expected = "Dense Scalar GINT32 2 5 6\
// 1 1 10 1 1 1\
// 1 1 1 1 1 1\
// 1 1 1 1 1 1\
// 8 1 1 5 5 9\
// 1 1 1 5 5 5\
//";
  auto val = (*img).GetPixel(2, 0); // x then y
  EXPECT_EQ(val, 10);
  poutre::idx2d p = {3, 0}; // y then x
  auto val2 = (*img).GetPixel(p);
  EXPECT_EQ(val2, 8);
  poutre::pt2D_scoord p2 = {5, 3}; // x then y
  auto val3 = (*img).GetPixel(p2);
  EXPECT_EQ(val3, 9);
}

TEST(poutreImageProcessingContainer,GetLineBuffer) {
  using ImageType = poutre::DenseImage<poutre::pINT32>;
  auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  auto img = dynamic_cast<ImageType *>(imgin.get());
  EXPECT_TRUE(img);
  auto line = (*img).GetLineBuffer(3); //-V522
  EXPECT_EQ(line[4], 5); //-V522

  EXPECT_EQ((*img).GetXSize(), 6);
  EXPECT_EQ((*img).GetYSize(), 5);
}
