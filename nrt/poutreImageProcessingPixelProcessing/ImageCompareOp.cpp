
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

namespace bs = boost::simd;

BOOST_AUTO_TEST_SUITE(poutreImageProcessingCompareOp)



BOOST_AUTO_TEST_CASE(CompareOpssswholeimage_equal)
{
    //poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });

    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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

    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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

    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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

    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    //poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });

    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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

    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 5 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


    auto imgfalse= poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);


    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);

    auto imgtrue= poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);


    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);

    auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);


    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
	ImageType* img = dynamic_cast<ImageType*> (imgin.get());
	BOOST_REQUIRE(img);
	auto v_img = poutre::view(*img);

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
    ImageType* img = dynamic_cast<ImageType*> (imgin.get());
    BOOST_REQUIRE(img);
    auto v_img = poutre::view(*img);

    auto imgcomp= poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 5 5 5\
 2 2 2 5 5 5\
");
    ImageType* imgc = dynamic_cast<ImageType*> (imgcomp.get());
    BOOST_REQUIRE(imgc);
    auto v_imgcomp= poutre::view(*imgc);

    auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
    ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
    BOOST_REQUIRE(imgt);
    auto v_imgtrue = poutre::view(*imgt);


    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
 2 2 2 2 2 2\
");
	ImageType* imgt = dynamic_cast<ImageType*> (imgtrue.get());
	BOOST_REQUIRE(imgt);
	auto v_imgtrue = poutre::view(*imgt);


	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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

    auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
		auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
		using ImageType = poutre::DenseImage < poutre::pINT32>;
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

		auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
		auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
		using ImageType = poutre::DenseImage < poutre::pINT32>;
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

		auto imgtrue = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
    auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
    using ImageType = poutre::DenseImage < poutre::pINT32>;
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

    auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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
	auto imgin = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 1 1 1\
 1 1 1 5 5 5\
 1 1 1 5 5 5\
");
	using ImageType = poutre::DenseImage < poutre::pINT32>;
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

	auto imgfalse = poutre::ImageFromString("Dense Scalar GINT32 2 5 6\
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

BOOST_AUTO_TEST_CASE(benchmark)
{
	const std::vector<poutre::pUINT8, boost::simd::allocator<poutre::pUINT8>> myvect(1024 * 1024);
	const std::vector<poutre::pUINT8, boost::simd::allocator<poutre::pUINT8>> myvectTrue(1024 * 1024);
	std::vector<poutre::pUINT8, boost::simd::allocator<poutre::pUINT8>> myvectout(1024 * 1024);

	auto v_img = poutre::array_view< const poutre::pUINT8, 2>(myvect, { int(1024),int(1024) });
	auto v_true = poutre::array_view< const poutre::pUINT8, 2>(myvectTrue, { int(1024),int(1024) });
	auto v_imgout = poutre::array_view< poutre::pUINT8, 2>(myvectout, { int(1024),int(1024) });

	auto iteration = 1;
	poutre::Timer timer;
	timer.Start();
	for(auto i=0;i<iteration;++i)
		poutre::ViewCompare_sss_func_helper(v_img, poutre::CompOpType::CompOpEqual, 1, 1, 0, v_imgout);
	timer.Stop();
	std::cout << "Time ViewCompare_sss_func_helper CompOpEqual " << timer << std::endl;
	timer.Reset();

	timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::ViewCompare_iii_dispatch(v_img, poutre::CompOpType::CompOpEqual, v_true, v_true, v_true, v_imgout);
	timer.Stop();
	std::cout << "Time ViewCompare_iii_dispatch CompOpEqual " << timer << std::endl;
	timer.Reset();
}

//namespace
//{
//	template<typename T>
//	struct ArrayView
//	{
//
//	};
//
//	template<typename T>
//	struct StridedView
//	{
//
//	};
//
//	template<typename T, template <typename> class ViewIn, typename = void>
//	struct dispatch
//	{
//		void operator()(const ViewIn<T>& test)
//		{
//			std::cout << "Primary template";
//		}
//	};
//
//	template<typename T, template <typename> class ViewIn>
//	struct dispatch<T,ViewIn,std::enable_if_t<std::is_same<ViewIn<T>,ArrayView<T>>::value>>
//	{
//		void operator()(const ViewIn<T>& test)
//		{
//			std::cout << "Sepecialize array view";
//		}
//	};
//}
//BOOST_AUTO_TEST_CASE(SNIFAE)
//{
//	ArrayView<int32_t> myview;
//	dispatch<int32_t,ArrayView> op;
//	op(myview);
//}
BOOST_AUTO_TEST_SUITE_END()



