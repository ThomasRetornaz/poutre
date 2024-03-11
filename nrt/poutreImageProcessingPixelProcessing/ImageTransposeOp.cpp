// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"
#include <vector>
#include <iostream>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingTranspose.hpp>
#include <poutreIO/poutreIOString.hpp>
#include <random>
#include <poutreBase/poutreChronos.hpp>

TEST(poutreImageProcessingTransposeOp,arrayview)
{
   using ImageType = const poutre::DenseImage < poutre::pINT32,2>;
   const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 2 3 4 5 6\
 7 8 9 10 11 12\
 13 14 15 16 17 18\
 19 20 21 22 23 24\
 25 26 27 28 29 30\
");
   ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
   EXPECT_TRUE(img);

    poutre::DenseImage<poutre::pINT32,2> imgOut({ 6, 5 });
   //  auto v_img1 = poutre::view(*img); //-V522
   //  auto v_img2 = poutre::view(imgOut);
    poutre::t_transpose(*img,imgOut); //-V522

    std::string expected = "Dense Scalar GINT32 2 6 5 \
1 7 13 19 25 \
2 8 14 20 26 \
3 9 15 21 27 \
4 10 16 22 28 \
5 11 17 23 29 \
6 12 18 24 30";
    auto imgstr = poutre::ImageToString(imgOut);
    EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingTransposeOp,stridedview)
{
// TODO
//   using ImageType = const poutre::DenseImage < poutre::pINT32>;
//   const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 6
// 1 2 3 4 5 6
// 7 8 9 10 11 12
// 13 14 15 16 17 18
// 19 20 21 22 23 24
// 25 26 27 28 29 30
//")
//   ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
//   EXPECT_TRUE(img);
//   auto v_img1 = poutre::view(*img); //-V522
//
//      
//   auto section2din = v_img1.section((poutre::idx2d{ 2,1 }), (poutre::bd2d{ 3,2 }));
//   EXPECT_EQ(section2din.size(), 6);
//   EXPECT_EQ(section2din.bound(), (poutre::bd2d({ 3,2 })));
//   EXPECT_EQ(section2din.stride(), (poutre::idx2d({ 6,1 })));
//
//   poutre::DenseImage<poutre::pINT32> imgOut({ 5, 6 });
//   imgOut.fill(1);
//   auto v_img2 = poutre::view(imgOut);
//   auto section2dout = v_img2.section((poutre::idx2d{ 1,2 }), (poutre::bd2d{ 2,3 }));
//  
//
//   EXPECT_EQ(section2dout.bound(), (poutre::bd2d({ 2,3 })));
//   EXPECT_EQ(section2dout.stride(), (poutre::idx2d({ 6,1 })));
//
//   poutre::t_transpose(section2din, section2dout);
//
//   std::string expected = "Dense Scalar GINT32 2 5 6 
//1 1 1 1 1 1 
//1 1 14 20 26 1
//1 1 15 21 27 1 1
//1 1 1  1 1 1
//1 1 1 1 1 1";
//   auto imgstr = poutre::ImageToString(imgOut);
//   EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingTransposeOp,arrayviewfloat)
{
   using ImageType = const poutre::DenseImage < poutre::pFLOAT,2>;
   const auto imgIn = poutre::ImageFromString("Dense Scalar F32 2 9 9 \
1 2 3 4 5 6 7 8 9 \
1 2 3 4 5 6 7 8 9 \
1 2 3 4 5 6 7 8 9 \
1 2 3 4 5 6 7 8 9 \
1 2 3 4 5 6 7 8 9 \
1 2 3 4 5 6 7 8 9 \
1 2 3 4 5 6 7 8 9 \
1 2 3 4 5 6 7 8 9 \
1 2 3 4 5 6 7 8 9");
   ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
   EXPECT_TRUE(img);

   poutre::DenseImage<poutre::pFLOAT,2> imgOut({ 9, 9 });
   // auto v_img1 = poutre::view(*img); //-V522
   // auto v_img2 = poutre::view(imgOut);
   // poutre::t_transpose(v_img1, v_img2);
   poutre::t_transpose(*img,imgOut); //-V522

   std::string expected = "Dense Scalar F32 2 9 9 \
1 1 1 1 1 1 1 1 1 \
2 2 2 2 2 2 2 2 2 \
3 3 3 3 3 3 3 3 3 \
4 4 4 4 4 4 4 4 4 \
5 5 5 5 5 5 5 5 5 \
6 6 6 6 6 6 6 6 6 \
7 7 7 7 7 7 7 7 7 \
8 8 8 8 8 8 8 8 8 \
9 9 9 9 9 9 9 9 9";
   auto imgstr = poutre::ImageToString(imgOut);
   EXPECT_EQ(imgstr, expected);
}

TEST(poutreImageProcessingTransposeOp,arrayviewunint8)
{
   using ImageType = const poutre::DenseImage < poutre::pUINT8,2>;
   const auto imgIn = poutre::ImageFromString("Dense Scalar GUINT8 2 17 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17");
   ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
   EXPECT_TRUE(img);

   poutre::DenseImage<poutre::pUINT8,2> imgOut({ 17, 17 });
   // auto v_img1 = poutre::view(*img); //-V522
   // auto v_img2 = poutre::view(imgOut);
   // poutre::t_transpose(v_img1, v_img2);
   poutre::t_transpose(*img,imgOut); //-V522

   std::string expected = "Dense Scalar GUINT8 2 17 17 \
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 \
2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 \
3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \
4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 \
5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 \
6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 \
7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 \
8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 \
9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 \
10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 \
11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 \
12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 \
13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 \
14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 \
15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 \
16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 \
17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17";
   auto imgstr = poutre::ImageToString(imgOut);
   EXPECT_EQ(imgstr, expected);
}


