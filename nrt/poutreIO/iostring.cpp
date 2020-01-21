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

#include <poutreIO/poutreIOString.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>

BOOST_AUTO_TEST_SUITE(iostring)

BOOST_AUTO_TEST_CASE(ioreadstring_scalar_GUINT8) {

  //*************************//
  // ImageType CompoundType Type NumDims pixvalue pixvalue .......
  std::string a = "Dense Scalar GUINT8 2 3 4\
 1 2 3 4\
 10 12 13 14\
 20 22 23 24\
";
  auto img = poutre::ImageFromString(a);
  BOOST_CHECK_EQUAL(img->GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img->GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img->GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL(img->GetNumDims(), 2);
  auto dims = img->GetCoords();
  std::vector<size_t> expectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(dims.begin(), dims.end(),
                                expectedcoords.begin(), expectedcoords.end());
  std::vector<poutre::pUINT8> expectedvalues = {1,  2,  3,  4,  10, 12,
                                                13, 14, 20, 22, 23, 24};
  poutre::DenseImage<poutre::pUINT8, 2> *img_t =
      dynamic_cast<poutre::DenseImage<poutre::pUINT8, 2> *>(&(*img));
  BOOST_REQUIRE(img_t);
  //   std::cout << "\n**************************************************\n";
  //   for (const auto &val : (*img_t)) {
  //     std::cout << "\nval\t"
  //               << (poutre::TypeTraits<poutre::pUINT8>::str_type)(val);
  //   }
  //   std::cout << "\n**************************************************\n";
  //   abort();
  BOOST_CHECK_EQUAL_COLLECTIONS((*img_t).begin(), (*img_t).end(),
                                expectedvalues.begin(), //-V522
                                expectedvalues.end()); //-V522
}

BOOST_AUTO_TEST_CASE(ioreadstring_scalar_INT32) {

  //*************************//
  // ImageType CompoundType Type NumDims pixvalue pixvalue .......
  std::string a = "Dense Scalar GINT32 2 3 4\
 -1 2 3 4\
 10 12 13 14\
 20 22 23 2400\
";
  auto img = poutre::ImageFromString(a);
  BOOST_CHECK_EQUAL(img->GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img->GetPType(), poutre::PType::PType_GrayINT32);
  BOOST_CHECK_EQUAL(img->GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL(img->GetNumDims(), 2);
  auto dims = img->GetCoords();
  std::vector<size_t> expectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(dims.begin(), dims.end(),
                                expectedcoords.begin(), expectedcoords.end());
  std::vector<poutre::pINT32> expectedvalues = {-1, 2,  3,  4,  10, 12,
                                                13, 14, 20, 22, 23, 2400};
  poutre::DenseImage<poutre::pINT32, 2> *img_t =
      dynamic_cast<poutre::DenseImage<poutre::pINT32, 2> *>(&(*img));
  BOOST_REQUIRE(img_t);
  //    for(const auto& val : (*img_t))
  //        {
  //        std::cout<<"\nval\t"<<(poutre::TypeTraits<poutre::pUINT8>::str_type)(val);
  //        }
  BOOST_CHECK_EQUAL_COLLECTIONS((*img_t).begin(), (*img_t).end(),
                                expectedvalues.begin(), //-V522
                                expectedvalues.end()); //-V522
}

BOOST_AUTO_TEST_CASE(ioreadstring_3Planes_UNT8) {
  // ImageType CompoundType Type NumDims pixvalue pixvalue .......
  std::string a = "Dense 3Planes GUINT8 2 3 4\
 1 1 1 2 2 2 3 3 3 4 4 4\
 10 10 10 12 12 12 13 13 13 14 14 14\
 20 20 20 22 22 22 23 23 23 24 24 24\
";
  auto img = poutre::ImageFromString(a);
  BOOST_CHECK_EQUAL(img->GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img->GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img->GetCType(),
                    poutre::CompoundType::CompoundType_3Planes);
  BOOST_CHECK_EQUAL(img->GetNumDims(), 2);
  auto dims = img->GetCoords();
  std::vector<size_t> expectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(dims.begin(), dims.end(),
                                expectedcoords.begin(), expectedcoords.end());
  std::vector<std::vector<poutre::pUINT8>> expectedvalues = {
      {1, 1, 1},    {2, 2, 2},    {3, 3, 3},    {4, 4, 4},
      {10, 10, 10}, {12, 12, 12}, {13, 13, 13}, {14, 14, 14},
      {20, 20, 20}, {22, 22, 22}, {23, 23, 23}, {24, 24, 24}};

  using ImgType = /*typename*/ poutre::DenseImage<
      typename poutre::TypeTraits<
          poutre::compound_pixel<poutre::pUINT8, 3>>::storage_type,
      2>;
  ImgType *img_t = dynamic_cast<ImgType *>(&(*img));
  BOOST_REQUIRE(img_t);
  //    for(const auto& pix : (*img_t))
  //        {
  //          std::cout<<"\n";
  //          for(const auto& val : pix)
  //          {
  //              std::cout<<"
  //              "<<(poutre::TypeTraits<poutre::pUINT8>::str_type)(val);
  //          }
  //        }

  for (size_t i = 0u; i < img_t->size(); ++i) //-V522
  { //-V522
    BOOST_CHECK_EQUAL_COLLECTIONS((*img_t)[i].begin(), (*img_t)[i].end(),
                                  expectedvalues[i].begin(),
                                  expectedvalues[i].end());
  }
}

BOOST_AUTO_TEST_CASE(ioreadstring_4Planes_double) {
  // ImageType CompoundType Type NumDims pixvalue pixvalue .......
  std::string a = "Dense 4Planes D64 2 3 4\
 1 1 1 1 2 2 2 2 3 3 3 3 4 4 4 4\
 10 10 10 10 12 12 12 12 13 13 13 13 14 14 14 14\
 20 20 20 20 22 22 22 22 23 23 23 23 24 24 24 24\
";
  auto img = poutre::ImageFromString(a);
  BOOST_CHECK_EQUAL(img->GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img->GetPType(), poutre::PType::PType_D64);
  BOOST_CHECK_EQUAL(img->GetCType(),
                    poutre::CompoundType::CompoundType_4Planes);
  BOOST_CHECK_EQUAL(img->GetNumDims(), 2);
  auto dims = img->GetCoords();
  std::vector<size_t> expectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(dims.begin(), dims.end(),
                                expectedcoords.begin(), expectedcoords.end());
  std::vector<std::vector<poutre::pUINT8>> expectedvalues = {
      {1, 1, 1, 1},     {2, 2, 2, 2},     {3, 3, 3, 3},     {4, 4, 4, 4},
      {10, 10, 10, 10}, {12, 12, 12, 12}, {13, 13, 13, 13}, {14, 14, 14, 14},
      {20, 20, 20, 20}, {22, 22, 22, 22}, {23, 23, 23, 23}, {24, 24, 24, 24}};

  using ImgType = /*typename*/ poutre::DenseImage<
      typename poutre::TypeTraits<
          poutre::compound_pixel<poutre::pDOUBLE, 4>>::storage_type,
      2>;
  ImgType *img_t = dynamic_cast<ImgType *>(&(*img));
  BOOST_REQUIRE(img_t);
  for (size_t i = 0u; i < img_t->size(); ++i) //-V522
  { //-V522
    BOOST_CHECK_EQUAL_COLLECTIONS((*img_t)[i].begin(), (*img_t)[i].end(),
                                  expectedvalues[i].begin(),
                                  expectedvalues[i].end());
  }
}

BOOST_AUTO_TEST_CASE(iowritestring_scalaruint8) {
  poutre::DenseImage<poutre::pUINT8> img({3, 4}); //-V112
  img.fill(10);
  BOOST_CHECK_EQUAL(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL(img.GetNumDims(), 2);
  BOOST_TEST_MESSAGE(img);

  auto imgstr = poutre::ImageToString(img);
  std::string expected = "Dense Scalar GUINT8 2 3 4\
 10 10 10 10\
 10 10 10 10\
 10 10 10 10\
";
  BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(iowritestring_scalarfloat) {
  poutre::DenseImage<poutre::pFLOAT> img({3, 4}); //-V112
  img.fill(-5.);
  BOOST_CHECK_EQUAL(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_F32);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL(img.GetNumDims(), 2);
  BOOST_TEST_MESSAGE(img);

  auto imgstr = poutre::ImageToString(img);
  std::string expected = "Dense Scalar F32 2 3 4\
 -5 -5 -5 -5\
 -5 -5 -5 -5\
 -5 -5 -5 -5\
";
  BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(iowritestring_compound3uint8) {
  poutre::DenseImage<poutre::compound_pixel<poutre::pUINT8, 3>> img(
      {3, 4}); //-V112
  img.fill({10, 11, 12});
  BOOST_CHECK_EQUAL(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_3Planes);
  BOOST_CHECK_EQUAL(img.size(), 12);
  BOOST_TEST_MESSAGE(img);

  auto imgstr = poutre::ImageToString(img);
  std::string expected = "Dense 3Planes GUINT8 2 3 4\
 10 11 12 10 11 12 10 11 12 10 11 12\
 10 11 12 10 11 12 10 11 12 10 11 12\
 10 11 12 10 11 12 10 11 12 10 11 12\
";
  BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(iowritestring_compound3double) {
  poutre::DenseImage<poutre::compound_pixel<poutre::pDOUBLE, 3>> img(
      {3, 4}); //-V112
  img.fill({10., -11., 1.2});
  BOOST_CHECK_EQUAL(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_D64);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_3Planes);
  BOOST_CHECK_EQUAL(img.size(), 12);
  BOOST_CHECK_EQUAL(img.max_size(), 12);
  BOOST_CHECK_EQUAL(img.GetNumDims(), 2);
  BOOST_TEST_MESSAGE(img);

  auto imgstr = poutre::ImageToString(img);
  std::string expected = "Dense 3Planes D64 2 3 4\
 10 -11 1.2 10 -11 1.2 10 -11 1.2 10 -11 1.2\
 10 -11 1.2 10 -11 1.2 10 -11 1.2 10 -11 1.2\
 10 -11 1.2 10 -11 1.2 10 -11 1.2 10 -11 1.2\
";
  BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_SUITE_END()
