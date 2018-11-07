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
#include <boost/lexical_cast.hpp>
#include <poutreIO/poutreIOString.hpp>
#include <random>
#include <poutreBase/poutreChronos.hpp>



BOOST_AUTO_TEST_SUITE(poutreImageProcessingTransposeOp)

BOOST_AUTO_TEST_CASE(arrayview)
{
   using ImageType = const poutre::DenseImage < poutre::pINT32>;
   const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 6 5\
 1 2 3 4 5 6\
 7 8 9 10 11 12\
 13 14 15 16 17 18\
 19 20 21 22 23 24\
 25 26 27 28 29 30\
");
   ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
   BOOST_REQUIRE(img);

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 6 });
    auto v_img1 = poutre::view(*img); //-V522
    auto v_img2 = poutre::view(imgOut);
    poutre::t_transpose2DAlongX(v_img1, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 7 13 19 25 \
2 8 14 20 26 \
3 9 15 21 27 \
4 10 16 22 28 \
5 11 17 23 29 \
6 12 18 24 30";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(stridedview)
{
   using ImageType = const poutre::DenseImage < poutre::pINT32>;
   const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 6 5\
 1 2 3 4 5 6\
 7 8 9 10 11 12\
 13 14 15 16 17 18\
 19 20 21 22 23 24\
 25 26 27 28 29 30\
");
   ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
   BOOST_REQUIRE(img);
   auto v_img1 = poutre::view(*img); //-V522


   auto section2din = v_img1.section((poutre::idx2d{ 2,1 }), (poutre::bd2d{ 3,2 }));
   BOOST_CHECK_EQUAL(section2din.size(), 6);
   BOOST_CHECK_EQUAL(section2din.bound(), (poutre::bd2d({ 3,2 })));
   BOOST_CHECK_EQUAL(section2din.stride(), (poutre::idx2d({ 5,1 })));

   poutre::DenseImage<poutre::pINT32> imgOut({ 6, 5 });
   imgOut.fill(1);
   auto v_img2 = poutre::view(imgOut);
   auto section2dout = v_img2.section((poutre::idx2d{ 1,2 }), (poutre::bd2d{ 2,3 }));
  

   BOOST_CHECK_EQUAL(section2dout.bound(), (poutre::bd2d({ 2,3 })));
   BOOST_CHECK_EQUAL(section2dout.stride(), (poutre::idx2d({ 5,1 })));

   poutre::t_transpose2DAlongX(section2din, section2dout);

   std::string expected = "Dense Scalar GINT32 2 6 5 \
1 1 1 1 1 1 \
1 1 1 1 1 1\
1 9 15 1 1 1\
1 10 16 1 1 1\
1 11 17 1 1 1";
   auto imgstr = poutre::ImageToString(imgOut);
   BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_SUITE_END()



