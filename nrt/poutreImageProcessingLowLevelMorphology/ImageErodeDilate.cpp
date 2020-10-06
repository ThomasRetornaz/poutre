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
#include <poutreBase/poutreChronos.hpp>
#include <poutreIO/poutreIOString.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessingLowLevelMorphology/include/poutreImageProcessingEroDil.hpp>
#include <random>
#include <vector>

////////////////////////////DILATE
TEST(poutreImageProcessingErodeDilate,line_se_view_dilateX)
{
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 6 5\
 0 1 0 0 0 \
 11 21 31 41 0\
 42 32 22 12 2\
 13 43 0 33 23\
 31 21 0 0 51\
 0 0 0 0 0");
    using ImageType = const poutre::DenseImage<poutre::pINT32>;
    ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
    EXPECT_TRUE(img);
    auto v_img1 = poutre::view(*img); //-V522

    poutre::DenseImage<poutre::pINT32> imgOut({6,5});
    imgOut.fill(4); //-V112
    auto v_imgOut = poutre::view(imgOut);

    poutre::t_DilateX(v_img1, (ptrdiff_t)1, v_imgOut);

    std::string expected = "Dense Scalar GINT32 2 6 5\
 1 1 1 0 0 \
21 31 41 41 41 \
42 42 32 22 12 \
43 43 43 33 33 \
31 31 21 51 51 \
0 0 0 0 0";
    auto imgstr = poutre::ImageToString(imgOut);
    EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_dilate_SE2DSquare) {
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);
  auto v_img1 = poutre::view(*img); //-V522

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(4); //-V112
  auto v_imgOut = poutre::view(imgOut);

  poutre::t_Dilate(
      v_img1, poutre::se::NLS::NLS_c8_2d,
      v_imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_dilate_SE2DSquare) {
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(4); //-V112

  poutre::t_Dilate(
      *img, poutre::se::NLS::NLS_c8_2d, //-V522
      imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_dilate_SE2DCross) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Dilate(
      v_img1, poutre::se::NLS::NLS_c4_2d,
      v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 0\
 8 5 6 5 5\
 5 6 6 6 5\
 5 5 6 5 5\
 0 5 5 5 0\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_dilate_SE2DCross) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Dilate(
      *img, poutre::se::NLS::NLS_c4_2d, //-V522
      imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 0\
 8 5 6 5 5\
 5 6 6 6 5\
 5 5 6 5 5\
 0 5 5 5 0\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_dilate_SE2DSeg0) {
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);
  auto v_img1 = poutre::view(*img); //-V522

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(4); //-V112
  auto v_imgOut = poutre::view(imgOut);

  poutre::t_Dilate(v_img1,
                   poutre::se::NLS::NLS_c2_2dH,
                   v_imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 0 0 0\
 5 5 5 5 5\
 5 6 6 6 5\
 5 5 5 5 5\
 0 0 0 0 0\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_dilate_SE2DSeg0) {
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(4); //-V112

  poutre::t_Dilate(*img, //-V522
                   poutre::se::NLS::NLS_c2_2dH,
                   imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 0 0 0\
 5 5 5 5 5\
 5 6 6 6 5\
 5 5 5 5 5\
 0 0 0 0 0\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_dilate_SE2DSeg90) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Dilate(
      v_img1, poutre::se::NLS::NLS_c2_2dV,
      v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 0\
 8 5 6 5 0\
 0 5 6 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_dilate_SE2DSeg90) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Dilate(
      *img, poutre::se::NLS::NLS_c2_2dV, //-V522
      imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 0\
 8 5 6 5 0\
 0 5 6 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_dilate_SE2DSeg45) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Dilate(
      v_img1, poutre::se::NLS::NLS_c2_2d_Diag45,
      v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 0 0\
 5 8 5 5 0\
 5 5 6 5 5\
 0 5 5 6 5\
 0 0 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_dilate_SE2DSeg45) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Dilate(
      *img, poutre::se::NLS::NLS_c2_2d_Diag45, //-V522
      imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 0 0\
 5 8 5 5 0\
 5 5 6 5 5\
 0 5 5 6 5\
 0 0 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_dilate_SE2DSeg135) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Dilate(
      v_img1, poutre::se::NLS::NLS_c2_2d_Diag135,
      v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 0 5 5 5\
 0 5 5 6 5\
 5 5 6 5 5\
 5 6 5 5 0\
 5 5 5 0 0\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_dilate_SE2DSeg135) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Dilate(
      *img, poutre::se::NLS::NLS_c2_2d_Diag135, //-V522
      imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 0 5 5 5\
 0 5 5 6 5\
 5 5 6 5 5\
 5 6 5 5 0\
 5 5 5 0 0\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}
////////////////////////////ERODE

TEST(poutreImageProcessingErodeDilate,line_se_view_erodeX)
{
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 6 5\
 0 100 0 0 0\
 1 2 4 5 0\
 10 1 11 1 21\
 1 21 0 1 14\
 31 31 50 0 11\
 0 0 0 0 0");
    using ImageType = const poutre::DenseImage<poutre::pINT32>;
    ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
    EXPECT_TRUE(img);
    poutre::DenseImage<poutre::pINT32> imgOut({6, 5});
    imgOut.fill(4); //-V112
    poutre::t_ErodeX(*img, (ptrdiff_t)1, imgOut); //-V522

    std::string expected = "Dense Scalar GINT32 2 6 5\
 0 0 0 0 0\
 1 1 2 0 0\
 1 1 1 1 1\
 1 0 0 0 1\
 31 31 0 0 0\
 0 0 0 0 0";
    auto imgstr = poutre::ImageToString(imgOut);
    EXPECT_EQ(imgstr, expected);
}
TEST(poutreImageProcessingErodeDilate,static_se_view_erode_SE2DSquare) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Erode(
      v_img1, poutre::se::NLS::NLS_c8_2d,
      v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 5 5 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_erode_SE2DSquare) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Erode(
      *img, poutre::se::NLS::NLS_c8_2d, //-V522
      imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 5 5 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_erode_SE2DCross) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Erode(v_img1,
                  poutre::se::NLS::NLS_c4_2d,
                  v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 5 6 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_erode_SE2DCross) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Erode(*img, //-V522
                  poutre::se::NLS::NLS_c4_2d,
                  imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 5 6 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_erode_SE2DSeg0) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Erode(v_img1,
                  poutre::se::NLS::NLS_c2_2dH,
                  v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 8 6 6 5 5\
 5 5 6 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_erode_SE2DSeg0) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Erode(*img, //-V522
                  poutre::se::NLS::NLS_c2_2dH,
                  imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 8 6 6 5 5\
 5 5 6 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_erode_SE2DSeg90) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Erode(v_img1,
                  poutre::se::NLS::NLS_c2_2dV,
                  v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 5 6 5 5 5\
 5 6 6 6 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_erode_SE2DSeg90) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Erode(*img, //-V522
                  poutre::se::NLS::NLS_c2_2dV,
                  imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 5 6 5 5 5\
 5 6 6 6 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_erode_SE2DSeg45) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Erode(v_img1,
                  poutre::se::NLS::NLS_c2_2d_Diag45,
                  v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 6 5 5 5\
 6 6 6 5 5\
 5 6 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_erode_SE2DSeg45) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Erode(*img, //-V522
                  poutre::se::NLS::NLS_c2_2d_Diag45,
                  imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 6 5 5 5\
 6 6 6 5 5\
 5 6 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingErodeDilate,static_se_view_erode_SE2DSeg135) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  auto v_img1 = poutre::view(*img); //-V522
  auto v_img2 = poutre::view(imgOut);
  poutre::t_Erode(
      v_img1, poutre::se::NLS::NLS_c2_2d_Diag135,
      v_img2);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 5 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}
TEST(poutreImageProcessingErodeDilate,static_se_erode_SE2DSeg135) {
  using ImageType = const poutre::DenseImage<poutre::pINT32>;
  const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
  ImageType *img = dynamic_cast<ImageType *>(imgIn.get());
  EXPECT_TRUE(img);

  poutre::DenseImage<poutre::pINT32> imgOut({5, 5});
  imgOut.fill(-1);
  poutre::t_Erode(
      *img, poutre::se::NLS::NLS_c2_2d_Diag135, //-V522
      imgOut);

  std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 5 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
  auto imgstr = poutre::ImageToString(imgOut);
  EXPECT_EQ(imgstr, expected);
}
