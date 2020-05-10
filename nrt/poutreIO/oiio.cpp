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

#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreIO/poutreIOString.hpp>
//#include <poutreIO/include/poutreOIIO.hpp>
#include <poutreIO/poutreIOLoader.hpp>
#include <poutreIO/poutreIOWriter.hpp>
#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;

namespace
{
 /*   xarray<uint8_t> test_image_rgb = {{{0, 0, 0}, {255, 0, 0}, {0, 255, 0}},
                                      {{0, 0, 255}, {228, 135, 255}, {255, 255, 255}}};

    xarray<uint8_t> test_image_rgba = {{{0, 0, 0, 255}, {255, 0, 0, 255}, {0, 255, 0, 255}},
                                       {{0, 0, 255, 255}, {228, 135, 255, 255}, {255, 255, 255, 255}}};

    xarray<uint8_t> test_image_jpg = {{{0, 0, 35}, {255, 12, 0}, {0, 255, 0}},
                                      {{5, 10, 236}, {222, 124, 237}, {255, 253, 255}}};*/
}
BOOST_AUTO_TEST_SUITE(oiio)

BOOST_AUTO_TEST_CASE(iopngloadUINT8)
{
}
BOOST_AUTO_TEST_CASE(iopngloadRGB)
{
    std::string expected = "Dense 3Planes GUINT8 2 2 3 \
0 0 0 255 0 0 0 255 0 \
0 0 255 228 135 255 255 255 255\
";
    bf::path image_path= bf::path(POUTREIO_NRT_DATAS) / "test_RGB.png";
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_CASE(iopngloadRGBA)
{

    std::string expected = "Dense 4Planes GUINT8 2 2 3 \
0 0 0 255 255 0 0 255 0 255 0 255 \
0 0 255 255 228 135 255 255 255 255 255 255\
";
    bf::path image_path = bf::path(POUTREIO_NRT_DATAS) / "test_RGBA.png";
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(iopngsaveUINT8)
{
}
BOOST_AUTO_TEST_CASE(iopngsaveRGB)
{
}
BOOST_AUTO_TEST_CASE(iopngsaveRGBA)
{
}

BOOST_AUTO_TEST_SUITE_END()
