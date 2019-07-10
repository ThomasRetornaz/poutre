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
#include <poutreImageProcessingLowLevelMorphology/include/poutreImageProcessingEroDil.hpp>
#include <boost/lexical_cast.hpp>
#include <poutreIO/poutreIOString.hpp>
#include <random>
#include <poutreBase/poutreChronos.hpp>



BOOST_AUTO_TEST_SUITE(poutreImageProcessingErodeDilate)

////////////////////////////DILATE
BOOST_AUTO_TEST_CASE(static_se_dilate_SE2DSquare)
{
	const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img); //-V522

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(4); //-V112
    auto v_imgOut = poutre::view(imgOut);

	poutre::t_Dilate(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSquare, v_imgOut);

	std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
";
	auto imgstr = poutre::ImageToString(imgOut);
	BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_CASE(static_se_dilate_SE2DCross)
{
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
	ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
	BOOST_REQUIRE(img);

	poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
	imgOut.fill(-1);
	auto v_img1 = poutre::view(*img); //-V522
	auto v_img2 = poutre::view(imgOut);
	poutre::t_Dilate(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DCross, v_img2);

	std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 0\
 8 5 6 5 5\
 5 6 6 6 5\
 5 5 6 5 5\
 0 5 5 5 0\
";
	auto imgstr = poutre::ImageToString(imgOut);
	BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(static_se_dilate_SE2DSeg0)
{
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img); //-V522

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(4); //-V112
    auto v_imgOut = poutre::view(imgOut);

    poutre::t_Dilate(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSeg0, v_imgOut);

    std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 0 0 0\
 5 5 5 5 5\
 5 6 6 6 5\
 5 5 5 5 5\
 0 0 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_CASE(static_se_dilate_SE2DSeg90)
{
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(-1);
    auto v_img1 = poutre::view(*img); //-V522
    auto v_img2 = poutre::view(imgOut);
    poutre::t_Dilate(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSeg90, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 0\
 8 5 6 5 0\
 0 5 6 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(static_se_dilate_SE2DSeg45)
{
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(-1);
    auto v_img1 = poutre::view(*img); //-V522
    auto v_img2 = poutre::view(imgOut);
    poutre::t_Dilate(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSeg45, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 0 0\
 5 8 5 5 0\
 5 5 6 5 5\
 0 5 5 6 5\
 0 0 5 5 5\
";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(static_se_dilate_SE2DSeg135)
{
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 0 0 0 0\
 0 5 5 5 0\
 0 5 6 5 0\
 0 5 5 5 0\
 0 0 0 0 0\
");
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(-1);
    auto v_img1 = poutre::view(*img); //-V522
    auto v_img2 = poutre::view(imgOut);
    poutre::t_Dilate(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSeg135, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 5\
 8 0 5 5 5\
 0 5 5 6 5\
 5 5 6 5 5\
 5 6 5 5 0\
 5 5 5 0 0\
";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

////////////////////////////ERODE

BOOST_AUTO_TEST_CASE(static_se_erode_SE2DSquare)
{
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
	ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
	BOOST_REQUIRE(img);

	poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
	imgOut.fill(-1);
	auto v_img1 = poutre::view(*img); //-V522
	auto v_img2 = poutre::view(imgOut);
	poutre::t_Erode(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSquare, v_img2);

	std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 5 5 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
	auto imgstr = poutre::ImageToString(imgOut);
	BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_CASE(static_se_erode_SE2DCross)
{
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
	ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
	BOOST_REQUIRE(img);

	poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
	imgOut.fill(-1);
	auto v_img1 = poutre::view(*img); //-V522
	auto v_img2 = poutre::view(imgOut);
	poutre::t_Erode(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DCross, v_img2);

	std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 5 6 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
	auto imgstr = poutre::ImageToString(imgOut);
	BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(static_se_erode_SE2DSeg0)
{
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(-1);
    auto v_img1 = poutre::view(*img); //-V522
    auto v_img2 = poutre::view(imgOut);
    poutre::t_Erode(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSeg0, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 5\
 8 5 5 5 5\
 8 6 6 5 5\
 5 5 6 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_CASE(static_se_erode_SE2DSeg90)
{
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(-1);
    auto v_img1 = poutre::view(*img); //-V522
    auto v_img2 = poutre::view(imgOut);
    poutre::t_Erode(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSeg90, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 5 6 5 5 5\
 5 6 6 6 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(static_se_erode_SE2DSeg45)
{
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(-1);
    auto v_img1 = poutre::view(*img); //-V522
    auto v_img2 = poutre::view(imgOut);
    poutre::t_Erode(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSeg45, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 5\
 8 6 5 5 5\
 6 6 6 5 5\
 5 6 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_CASE(static_se_erode_SE2DSeg135)
{
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    const auto imgIn = poutre::ImageFromString("Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 8 6 6 5\
 5 6 6 6 5\
 5 6 6 6 5\
 5 5 5 5 5\
");
    ImageType* img = dynamic_cast<ImageType*> (imgIn.get());
    BOOST_REQUIRE(img);

    poutre::DenseImage<poutre::pINT32> imgOut({ 5, 5 });
    imgOut.fill(-1);
    auto v_img1 = poutre::view(*img); //-V522
    auto v_img2 = poutre::view(imgOut);
    poutre::t_Erode(v_img1, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DSeg135, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 5\
 8 8 5 5 5\
 8 5 5 5 5\
 5 5 6 5 5\
 5 5 5 5 5\
 5 5 5 5 5\
";
    auto imgstr = poutre::ImageToString(imgOut);
    BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_SUITE_END()