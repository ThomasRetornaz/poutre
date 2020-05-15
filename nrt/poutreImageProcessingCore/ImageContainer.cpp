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

BOOST_AUTO_TEST_SUITE(poutreImageProcessingContainer)
BOOST_AUTO_TEST_CASE(ctor) {
  poutre::DenseImage<poutre::pUINT8> img({3, 4}); //-V112
  BOOST_CHECK_EQUAL(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL(img.GetXSize(), 4); //-V112
  BOOST_CHECK_EQUAL(img.GetYSize(), 3);
  BOOST_CHECK_EQUAL(img.GetNumDims(), 2);
  BOOST_CHECK(!img.empty());
  auto coords = img.GetCoords();
  auto expectedcoords = {3, 4}; //-V112
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(),
                                expectedcoords.begin(), expectedcoords.end());

  /*std::cout << "**************************************************" <<
  std::endl; std::cout << img << std::endl; std::cout <<
  "**************************************************" << std::endl;*/
}

BOOST_AUTO_TEST_CASE(clone) {
  using ImageType = poutre::DenseImage<poutre::pUINT8>;
  using ImageTypeInterface = std::unique_ptr<poutre::IInterface>;
  ImageType img({3, 4}); //-V112
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  ImageTypeInterface clonedinterface = img.Clone();

  ImageType *cloned = dynamic_cast<ImageType *>(clonedinterface.get());
  BOOST_REQUIRE(cloned);
  //!= address
  BOOST_CHECK_NE(cloned, &(img));
  //!= data address
  BOOST_CHECK_NE(&(*((*cloned).data())), &(*(img.data()))); //-V522
  BOOST_CHECK_EQUAL((*cloned).GetPType(), poutre::PType::PType_GrayUINT8); //-V522
  BOOST_CHECK_EQUAL((*cloned).GetCType(),
                    poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL((*cloned).GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL((*cloned).GetXSize(), 4);
  BOOST_CHECK_EQUAL((*cloned).GetYSize(), 3);
  BOOST_CHECK_EQUAL((*cloned).GetNumDims(), 2);
  BOOST_CHECK(!(*cloned).empty());
  auto coords = (*cloned).GetCoords();
  auto expectedcoords = {3, 4}; //-V112
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(),
                                expectedcoords.begin(), expectedcoords.end());

  for (const auto &var : (*cloned)) {
    BOOST_CHECK_EQUAL(var, 10);
  }

  //  std::cout << "**************************************************" <<
  //  std::endl; std::cout << (*cloned) << std::endl; std::cout <<
  //  "**************************************************" << std::endl;
}

BOOST_AUTO_TEST_CASE(Factory) {
  {
    auto img =
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar,
                            poutre::PType::PType_GrayUINT8); //-V112
    BOOST_REQUIRE(img);
    BOOST_CHECK_EQUAL((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    BOOST_CHECK_EQUAL((*img).GetCType(),
                      poutre::CompoundType::CompoundType_Scalar);
    BOOST_CHECK_EQUAL((*img).GetPType(), poutre::PType::PType_GrayUINT8);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4}; //-V112
    BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(),
                                  expectedcoords.begin(), expectedcoords.end());
    BOOST_CHECK(dynamic_cast<poutre::DenseImage<poutre::pUINT8> *>(img.get()));
  }

  {
    auto img =
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar,
                            poutre::PType::PType_GrayINT32); //-V112
    BOOST_REQUIRE(img);
    BOOST_CHECK_EQUAL((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    BOOST_CHECK_EQUAL((*img).GetCType(),
                      poutre::CompoundType::CompoundType_Scalar);
    BOOST_CHECK_EQUAL((*img).GetPType(), poutre::PType::PType_GrayINT32);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4}; //-V112
    BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(),
                                  expectedcoords.begin(), expectedcoords.end());
    BOOST_CHECK(dynamic_cast<poutre::DenseImage<poutre::pINT32> *>(img.get()));
  }
  {
    auto img =
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar,
                            poutre::PType::PType_F32); //-V112
    BOOST_REQUIRE(img);
    BOOST_CHECK_EQUAL((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    BOOST_CHECK_EQUAL((*img).GetCType(),
                      poutre::CompoundType::CompoundType_Scalar);
    BOOST_CHECK_EQUAL((*img).GetPType(), poutre::PType::PType_F32);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4}; //-V112
    BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(),
                                  expectedcoords.begin(), expectedcoords.end());
    BOOST_CHECK(dynamic_cast<poutre::DenseImage<poutre::pFLOAT> *>(img.get()));
  }
  {
    auto img =
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar,
                            poutre::PType::PType_GrayINT64); //-V112
    BOOST_REQUIRE(img);
    BOOST_CHECK_EQUAL((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    BOOST_CHECK_EQUAL((*img).GetPType(), poutre::PType::PType_GrayINT64);
    BOOST_CHECK_EQUAL((*img).GetCType(),
                      poutre::CompoundType::CompoundType_Scalar);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4}; //-V112
    BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(),
                                  expectedcoords.begin(), expectedcoords.end());
    BOOST_CHECK(dynamic_cast<poutre::DenseImage<poutre::pINT64> *>(img.get()));
  }
  {
    BOOST_CHECK_THROW(
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, //-V112
                            poutre::PType::PType_Undef),
        std::runtime_error); //-V112
    BOOST_CHECK_THROW(
        poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Undef, //-V112
                            poutre::PType::PType_GrayINT64),
        std::runtime_error); //-V112
  }
}

BOOST_AUTO_TEST_CASE(BasicIteration) {
  poutre::DenseImage<poutre::pUINT8> img({3, 4}); //-V112
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  img.assign(10);

  for (const auto &var : img) {
    BOOST_CHECK_EQUAL(var, 10);
  }
  auto count = 0;
  for (auto it = img.begin(); it != img.end(); ++it, ++count) {
    BOOST_CHECK_EQUAL(*it, 10);
  }
  BOOST_CHECK_EQUAL(count, 12);
  count = 0;
  for (auto it = img.cbegin(); it != img.cend(); ++it, ++count) {
    BOOST_CHECK_EQUAL(*it, 10);
  }
  BOOST_CHECK_EQUAL(count, 12);

  // count = 0;
  // auto rit = img.rbegin();
  // auto rend = img.rend();
  // for (; rit != rend; ++rit, ++count) {
  //   BOOST_CHECK_EQUAL(*rit, 10);
  // }
  // BOOST_CHECK_EQUAL(count, 12);

  // count = 0;
  // auto crit = img.crbegin();
  // auto crend = img.crend();
  // for (; crit != crend; ++crit, ++count) {
  //   BOOST_CHECK_EQUAL(*crit, 10);
  // }
  // BOOST_CHECK_EQUAL(count, 12);

  // for (size_t i = 0u; i < img.size(); i++) {
  //   BOOST_CHECK_EQUAL(img[i], 10);
  //   BOOST_CHECK_EQUAL(img.at(i), 10);
  // }
  // BOOST_CHECK_EQUAL(img.front(), 10);
  // BOOST_CHECK_EQUAL(img.back(), 10);
}

BOOST_AUTO_TEST_CASE(move) {
  // todo check assembly
  auto img(poutre::DenseImage<poutre::pUINT8>{3, 4}); //-V112
  BOOST_CHECK_EQUAL(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);

  auto coords = img.GetCoords();
  auto expectedcoords = {3, 4}; //-V112
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(),
                                expectedcoords.begin(), expectedcoords.end());

  poutre::DenseImage<poutre::pUINT8> img2({2, 3});
  BOOST_CHECK_EQUAL(img2.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img2.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img2.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  auto coords2 = img2.GetCoords();
  auto expectedcoords2 = {2, 3};
  BOOST_CHECK_EQUAL_COLLECTIONS(coords2.begin(), coords2.end(),
                                expectedcoords2.begin(), expectedcoords2.end());

  // move
  img2 = std::move(img);
  BOOST_CHECK_EQUAL(img2.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img2.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img2.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  auto coordsmove = img2.GetCoords();
  auto expectedcoordsmove = {3, 4}; //-V112
  BOOST_CHECK_EQUAL_COLLECTIONS(coordsmove.begin(), coordsmove.end(),
                                expectedcoordsmove.begin(),
                                expectedcoordsmove.end());
}

BOOST_AUTO_TEST_CASE(viewoverimage) {
  poutre::DenseImage<poutre::pUINT8> img1({3, 4}); //-V112
  // start view
  auto vimg1 = poutre::view(img1);
  BOOST_CHECK_EQUAL(vimg1.size(), 12);
  BOOST_CHECK_EQUAL(vimg1.bound(), (poutre::bd2d{3, 4})); //-V112
  BOOST_CHECK_EQUAL(vimg1.stride(), (poutre::idx2d{4, 1})); //-V112
}

BOOST_AUTO_TEST_CASE(SetPixelGetPixel) {
  using ImageType = poutre::DenseImage<poutre::pINT32>;
  auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  auto img = dynamic_cast<ImageType *>(imgin.get());
  BOOST_REQUIRE(img);
  (*img).SetPixel(2, 0, 10);  // x then y
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
  BOOST_CHECK_EQUAL(val, 10);
  poutre::idx2d p = {3, 0}; // y then x
  auto val2 = (*img).GetPixel(p);
  BOOST_CHECK_EQUAL(val2, 8);
  poutre::pt2D_scoord p2 = {5, 3}; // x then y
  auto val3 = (*img).GetPixel(p2);
  BOOST_CHECK_EQUAL(val3, 9);
}

BOOST_AUTO_TEST_CASE(GetLineBuffer) {
  using ImageType = poutre::DenseImage<poutre::pINT32>;
  auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
  auto img = dynamic_cast<ImageType *>(imgin.get());
  BOOST_REQUIRE(img);
  auto line = (*img).GetLineBuffer(3);
  BOOST_CHECK_EQUAL(line[4], 5); //-V522

  BOOST_CHECK_EQUAL((*img).GetXSize(), 6);
  BOOST_CHECK_EQUAL((*img).GetYSize(), 5);
}
BOOST_AUTO_TEST_SUITE_END()
