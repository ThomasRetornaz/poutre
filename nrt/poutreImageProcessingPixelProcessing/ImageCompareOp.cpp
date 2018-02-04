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
#include <boost/lexical_cast.hpp>

#include <poutreBase/poutreContainerView.hpp>
#include <poutreIO/poutreIOString.hpp>


#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingCompare.hpp>
#include <poutreBase/poutreChronos.hpp>

#include <simdpp/simd.h>

namespace simd = simdpp::SIMDPP_ARCH_NAMESPACE;

BOOST_AUTO_TEST_SUITE(poutreImageProcessingCompareOp)

BOOST_AUTO_TEST_CASE(CompareOpssswholeimage_equal)
{
    //poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });

    const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sss_func_helper(v_img1,poutre::CompOpType::CompOpEqual,1,1,0,v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 0 0 0 \
1 1 1 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssswholeimage_notequal)
{
    //poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });

	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sss_func_helper(v_img1, poutre::CompOpType::CompOpDiff, 1, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 1 1 1 \
0 0 0 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssswholeimage_sup)
{
    //poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });

	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sss_func_helper(v_img1, poutre::CompOpType::CompOpSup, 1, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 1 1 1 \
0 0 0 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssswholeimage_supequal)
{
    //poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });

	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sss_func_helper(v_img1, poutre::CompOpType::CompOpSupEqual, 1, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssswholeimage_inf)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sss_func_helper(v_img1, poutre::CompOpType::CompOpInf, 5, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 0 0 0 \
1 1 1 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssswholeimage_infequal)
{
    //poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });

	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sss_func_helper(v_img1, poutre::CompOpType::CompOpInfEqual, 5, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssssection)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 5 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);   
    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);

    //Extract section
    auto v_imgin = poutre::view(*img);
    auto v_imgout = poutre::view(imgout);

    auto s_img = v_imgin.section({ 1,1 }, { 3,2 });
    auto s_imgout = v_imgout.section({ 1,1 }, { 3,2 });

    poutre::ViewCompare_sss_func_helper(s_img, poutre::CompOpType::CompOpEqual, 1, 1, 0, s_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
4 4 4 4 4 4 \
4 1 1 4 4 4 \
4 1 0 4 4 4 \
4 1 1 4 4 4 \
4 4 4 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpisswholeimage_equal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);


	const auto imgin2 = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
	ImageType* img2 = dynamic_cast<ImageType*> (imgin2.get());
    BOOST_REQUIRE(img2);
    auto v_img2 = poutre::view(*img2);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_iss_dispatch(v_img1, poutre::CompOpType::CompOpEqual, v_img2,1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 0 0 0 \
1 1 1 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpisswholeimage_notequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);


	const auto imgin2 = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
	ImageType* img2 = dynamic_cast<ImageType*> (imgin2.get());
    BOOST_REQUIRE(img2);
    auto v_img2 = poutre::view(*img2);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_iss_dispatch(v_img1, poutre::CompOpType::CompOpDiff, v_img2, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 1 1 1 \
0 0 0 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpisswholeimage_sup)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);


    auto imgin2 = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
	ImageType* img2 = dynamic_cast<ImageType*> (imgin2.get());
    BOOST_REQUIRE(img2);
    auto v_img2 = poutre::view(*img2);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_iss_dispatch(v_img1, poutre::CompOpType::CompOpSup, v_img2, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 1 1 1 \
0 0 0 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpisswholeimage_supequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);


	const auto imgin2 = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
    ImageType* img2 = dynamic_cast<ImageType*> (imgin2.get());
    BOOST_REQUIRE(img2);
    auto v_img2 = poutre::view(*img2);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_iss_dispatch(v_img1, poutre::CompOpType::CompOpSupEqual, v_img2, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpisswholeimage_inf)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);


	const auto imgin2 = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
    ImageType* img2 = dynamic_cast<ImageType*> (imgin2.get());
    BOOST_REQUIRE(img2);
    auto v_img2 = poutre::view(*img2);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_iss_dispatch(v_img1, poutre::CompOpType::CompOpInf, v_img2, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpisswholeimage_infequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img1 = poutre::view(*img);


	const auto imgin2 = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
");
    ImageType* img2 = dynamic_cast<ImageType*> (imgin2.get());
    BOOST_REQUIRE(img2);
    auto v_img2 = poutre::view(*img2);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_iss_dispatch(v_img1, poutre::CompOpType::CompOpInfEqual, v_img2, 1, 0, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 0 0 0 \
1 1 1 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}



BOOST_AUTO_TEST_CASE(CompareOpsiswholeimage_equal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


    auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sis_dispatch(v_img, poutre::CompOpType::CompOpEqual,static_cast<poutre::pINT32>(1), v_imgtrue, static_cast<poutre::pINT32>(0),v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 0 0 0 \
2 2 2 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpsiswholeimage_notequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sis_dispatch(v_img, poutre::CompOpType::CompOpDiff, static_cast<poutre::pINT32>(1), v_imgtrue, static_cast<poutre::pINT32>(0), v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 4 4 4 \
0 0 0 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpsiswholeimage_sup)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img =  dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sis_dispatch(v_img, poutre::CompOpType::CompOpSup, static_cast<poutre::pINT32>(1), v_imgtrue, static_cast<poutre::pINT32>(0), v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 4 4 4 \
0 0 0 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpsiswholeimage_supequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sis_dispatch(v_img, poutre::CompOpType::CompOpSupEqual, static_cast<poutre::pINT32>(1), v_imgtrue, static_cast<poutre::pINT32>(0), v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 4 4 4 \
2 2 2 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpsiswholeimage_inf)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sis_dispatch(v_img, poutre::CompOpType::CompOpInf, static_cast<poutre::pINT32>(1), v_imgtrue, static_cast<poutre::pINT32>(0), v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpsiswholeimage_infequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sis_dispatch(v_img, poutre::CompOpType::CompOpInfEqual, static_cast<poutre::pINT32>(1), v_imgtrue, static_cast<poutre::pINT32>(0), v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 0 0 0 \
2 2 2 0 0 0\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssiwholeimage_equal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgfalse= poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_ssi_dispatch(v_img, poutre::CompOpType::CompOpEqual, static_cast<poutre::pINT32>(5), static_cast<poutre::pINT32>(1),v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 2 2 2 \
2 2 2 1 1 1 \
2 2 2 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}


BOOST_AUTO_TEST_CASE(CompareOpssiwholeimage_notequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_ssi_dispatch(v_img, poutre::CompOpType::CompOpDiff, static_cast<poutre::pINT32>(5), static_cast<poutre::pINT32>(1), v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 4 4 4 \
1 1 1 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssiwholeimage_sup)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_ssi_dispatch(v_img, poutre::CompOpType::CompOpSup, static_cast<poutre::pINT32>(5), static_cast<poutre::pINT32>(1), v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpssiwholeimage_supequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_ssi_dispatch(v_img, poutre::CompOpType::CompOpSupEqual, static_cast<poutre::pINT32>(5), static_cast<poutre::pINT32>(1), v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 1 1 1\
 2 2 2 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}


BOOST_AUTO_TEST_CASE(CompareOpssiwholeimage_inf)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_ssi_dispatch(v_img, poutre::CompOpType::CompOpInf, static_cast<poutre::pINT32>(5), static_cast<poutre::pINT32>(1), v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 4 4 4\
 1 1 1 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(CompareOpssiwholeimage_infequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_ssi_dispatch(v_img, poutre::CompOpType::CompOpInfEqual, static_cast<poutre::pINT32>(5), static_cast<poutre::pINT32>(1), v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}


BOOST_AUTO_TEST_CASE(CompareOpsiiwholeimage_equal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);

	const auto imgtrue= poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sii_dispatch(v_img, poutre::CompOpType::CompOpEqual, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpsiiwholeimage_notequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_sii_dispatch(v_img, poutre::CompOpType::CompOpDiff, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpsiiwholeimage_sup)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_sii_dispatch(v_img, poutre::CompOpType::CompOpSup, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);


}
BOOST_AUTO_TEST_CASE(CompareOpsiiwholeimage_supequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_sii_dispatch(v_img, poutre::CompOpType::CompOpSupEqual, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpsiiwholeimage_inf)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_sii_dispatch(v_img, poutre::CompOpType::CompOpInf, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpsiiwholeimage_infequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_sii_dispatch(v_img, poutre::CompOpType::CompOpInfEqual, static_cast<poutre::pINT32>(5), v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpiiiwholeimage_equal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);

	const auto imgcomp= poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
    ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
    BOOST_REQUIRE(imgc);
    auto v_imgcomp= poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_iii_dispatch(v_img, poutre::CompOpType::CompOpEqual, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpiiiwholeimage_notequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_iii_dispatch(v_img, poutre::CompOpType::CompOpDiff, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpiiiwholeimage_sup)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_iii_dispatch(v_img, poutre::CompOpType::CompOpSup, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpiiiwholeimage_supequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_iii_dispatch(v_img, poutre::CompOpType::CompOpSupEqual, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpiiiwholeimage_inf)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_iii_dispatch(v_img, poutre::CompOpType::CompOpInf, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpiiiwholeimage_infequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_iii_dispatch(v_img, poutre::CompOpType::CompOpInfEqual, v_imgcomp, v_imgtrue, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(CompareOpiiswholeimage_equal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
    ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
    BOOST_REQUIRE(imgc);
    auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_iis_dispatch(v_img, poutre::CompOpType::CompOpEqual, v_imgcomp, v_imgtrue, 4, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpiiswholeimage_notequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_iis_dispatch(v_img, poutre::CompOpType::CompOpDiff, v_imgcomp, v_imgtrue, 4, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpiiswholeimage_sup)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_iis_dispatch(v_img, poutre::CompOpType::CompOpSup, v_imgcomp, v_imgtrue, 4, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpiiswholeimage_supequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
		ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
		BOOST_REQUIRE(imgc);
		auto v_imgcomp = poutre::view(*imgc);

		const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
		ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
		BOOST_REQUIRE(imgt);
		auto v_imgtrue = poutre::view(*imgt);

		poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
		imgout.fill(4);
		auto v_imgout = poutre::view(imgout);

		poutre::ViewCompare_iis_dispatch(v_img, poutre::CompOpType::CompOpSupEqual, v_imgcomp, v_imgtrue, 4, v_imgout);

		std::string expected = "Dense Scalar GINT32 2 5 6\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 4 4 4\
 4 4 4 2 2 2\
 4 4 4 2 2 2\
";
		auto imgstr = poutre::ImageToString(imgout);
		BOOST_CHECK_EQUAL(imgstr, expected);


}
BOOST_AUTO_TEST_CASE(CompareOpiiswholeimage_inf)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_iis_dispatch(v_img, poutre::CompOpType::CompOpInf, v_imgcomp, v_imgtrue, 4, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 4 4 4\
 2 2 2 4 4 4\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_CASE(CompareOpiiswholeimage_infequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
		using ImageType = const poutre::DenseImage < poutre::pINT32>;
		ImageType* img = dynamic_cast<ImageType*> (imgin.get());
		BOOST_REQUIRE(img);
		auto v_img = poutre::view(*img);

		const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
		ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
		BOOST_REQUIRE(imgc);
		auto v_imgcomp = poutre::view(*imgc);

		const auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
		ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
		BOOST_REQUIRE(imgt);
		auto v_imgtrue = poutre::view(*imgt);

		poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
		imgout.fill(4);
		auto v_imgout = poutre::view(imgout);

		poutre::ViewCompare_iis_dispatch(v_img, poutre::CompOpType::CompOpInfEqual, v_imgcomp, v_imgtrue, 4, v_imgout);

		std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
";
		auto imgstr = poutre::ImageToString(imgout);
		BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(CompareOpisiwholeimage_equal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = const poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
    ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
    BOOST_REQUIRE(imgc);
    auto v_imgcomp = poutre::view(*imgc);

	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
    ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
    BOOST_REQUIRE(imgf);
    auto v_imgfalse = poutre::view(*imgf);

    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_imgout = poutre::view(imgout);

    poutre::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpEqual, v_imgcomp, 1, v_imgfalse,v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 1 1 1\
 2 2 2 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpisiwholeimage_notequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpDiff, v_imgcomp, 1, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 2 2 2\
 1 1 1 2 2 2\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpisiwholeimage_sup)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpSup, v_imgcomp, 1, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);

}
BOOST_AUTO_TEST_CASE(CompareOpisiwholeimage_supequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpSupEqual, v_imgcomp, 1, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 1 1 1\
 2 2 2 1 1 1\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(CompareOpisiwholeimage_inf)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpInf, v_imgcomp, 1, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 2 2 2\
 1 1 1 2 2 2\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(CompareOpisiwholeimage_infequal)
{
	const auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = const poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	const auto imgcomp = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
	ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
	BOOST_REQUIRE(imgc);
	auto v_imgcomp = poutre::view(*imgc);

	const auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgf = dynamic_cast<ImageType*> (imgfalse.get());
	BOOST_REQUIRE(imgf);
	auto v_imgfalse = poutre::view(*imgf);

	poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
	imgout.fill(4);
	auto v_imgout = poutre::view(imgout);

	poutre::ViewCompare_isi_dispatch(v_img, poutre::CompOpType::CompOpInfEqual, v_imgcomp, 1, v_imgfalse, v_imgout);

	std::string expected = "Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
";
	auto imgstr = poutre::ImageToString(imgout);
	BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(benchmarkViewCompare)
{
	const std::vector<poutre::pUINT8, simd::aligned_allocator<poutre::pUINT8, simd::typetraits<poutre::pUINT8>::alignment>> myvect(1000 * 1000);
	const std::vector<poutre::pUINT8, simd::aligned_allocator<poutre::pUINT8, simd::typetraits<poutre::pUINT8>::alignment>> myvectTrue(1000 * 1000);
	const std::vector<poutre::pUINT8, simd::aligned_allocator<poutre::pUINT8, simd::typetraits<poutre::pUINT8>::alignment>> myvectFalse(1000 * 1000);
	std::vector<poutre::pUINT8, simd::aligned_allocator<poutre::pUINT8, simd::typetraits<poutre::pUINT8>::alignment>> myvectout(1000 * 1000);
	std::vector<poutre::pFLOAT, simd::aligned_allocator<poutre::pFLOAT, simd::typetraits<poutre::pFLOAT>::alignment>> myvectoutfloat(1000 * 1000);

	auto v_img = poutre::array_view< const poutre::pUINT8, 2>(myvect, { int(1000),int(1000) });
	auto v_true = poutre::array_view< const poutre::pUINT8, 2>(myvectTrue, { int(1000),int(1000) });
	auto v_false = poutre::array_view< const poutre::pUINT8, 2>(myvectFalse, { int(1000),int(1000) });
	auto v_imgout = poutre::array_view< poutre::pUINT8, 2>(myvectout, { int(1000),int(1000) });
	auto v_imgoutfloat = poutre::array_view< poutre::pFLOAT, 2>(myvectoutfloat, { int(1000),int(1000) });

	auto iteration = 10000;
	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_sss_func_helper(v_img, poutre::CompOpType::CompOpEqual, 1, 1, 0, v_imgout);
		timer.Stop();
		std::cout << "Time ViewCompare_sss_func_helper CompOpEqual " << timer << std::endl;
		timer.Reset();
	}
	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_sss_func_helper(v_img, poutre::CompOpType::CompOpEqual, 1, 1, 0, v_imgoutfloat);
		timer.Stop();
		std::cout << "Time ViewCompare_sss_func_helper CompOpEqual float output" << timer << std::endl;
		timer.Reset();
	}

	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_sii_func_helper(v_img, poutre::CompOpType::CompOpEqual, 1, v_true, v_false, v_imgout);
		timer.Stop();
		std::cout << "Time ViewCompare_sii_func_helper CompOpEqual " << timer << std::endl;
		timer.Reset();
	}
	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_iii_func_helper(v_img, poutre::CompOpType::CompOpEqual, v_true, v_true, v_false, v_imgout);
		timer.Stop();
		std::cout << "Time ViewCompare_iii_dispatch CompOpEqual " << timer << std::endl;
		timer.Reset();
	}
	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_sis_func_helper(v_img, poutre::CompOpType::CompOpEqual, 1, v_true, 0, v_imgout);
		timer.Stop();
		std::cout << "Time ViewCompare_sis_func_helper CompOpEqual " << timer << std::endl;
		timer.Reset();
	}
	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_isi_func_helper(v_img, poutre::CompOpType::CompOpEqual, v_true, 1, v_false, v_imgout);
		timer.Stop();
		std::cout << "Time ViewCompare_isi_func_helper CompOpEqual " << timer << std::endl;
		timer.Reset();
	}

	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_iis_func_helper(v_img, poutre::CompOpType::CompOpEqual, v_true, v_true, 0, v_imgout);
		timer.Stop();
		std::cout << "Time ViewCompare_iis_func_helper CompOpEqual " << timer << std::endl;
		timer.Reset();
	}

	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_ssi_func_helper(v_img, poutre::CompOpType::CompOpEqual, 1, 0, v_false, v_imgout);
		timer.Stop();
		std::cout << "Time ViewCompare_ssi_func_helper CompOpEqual " << timer << std::endl;
		timer.Reset();
	}
	
	{
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			poutre::ViewCompare_iss_func_helper(v_img, poutre::CompOpType::CompOpEqual, v_true, 1, 0, v_imgout);
		timer.Stop();
		std::cout << "Time ViewCompare_iss_func_helper CompOpEqual " << timer << std::endl;
		timer.Reset();
	}
	
}

BOOST_AUTO_TEST_SUITE_END()



