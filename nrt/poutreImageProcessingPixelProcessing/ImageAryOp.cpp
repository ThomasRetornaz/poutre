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
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingArith.hpp>
#include <boost/lexical_cast.hpp>
#include <poutreIO/poutreIOString.hpp>
#include <random>
#include <poutreBase/poutreChronos.hpp>



BOOST_AUTO_TEST_SUITE(poutreImageProcessingArithOp)

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType_negate)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    poutre::t_ArithNegate(v_img1, v_img2);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousDifferentPtrType_negate)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT64> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //call UnaryOpDispatcher array view template specialization,fall back ptr
    poutre::t_ArithNegate(v_img1, v_img2);
    std::string expected = "Dense Scalar GINT64 2 5 6 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1 \
-1 -1 -1 -1 -1 -1\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxSamePtrType_negate)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section(/*poutre::idx2d*/{ 1,1 });
    auto s_img2 = v_img2.section(/*poutre::idx2d*/{ 1,1 });
    BOOST_CHECK_EQUAL(s_img1.size(), 20);
    BOOST_CHECK_EQUAL(s_img1.bound(), (poutre::bd2d({ 4,5 })));
    BOOST_CHECK_EQUAL(s_img2.size(), 20);
    BOOST_CHECK_EQUAL(s_img2.bound(), (poutre::bd2d({ 4,5 })));
    //call UnaryOpDispatcher one idx
    poutre::t_ArithNegate(s_img1, s_img2);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxDiferentPtrType_negate)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT64> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 });
    auto s_img2 = v_img2.section({ 1,1 });
    BOOST_CHECK_EQUAL(s_img1.size(), 20);
    BOOST_CHECK_EQUAL(s_img1.bound(), (poutre::bd2d({ 4,5 })));
    BOOST_CHECK_EQUAL(s_img2.size(), 20);
    BOOST_CHECK_EQUAL(s_img2.bound(), (poutre::bd2d({ 4,5 })));

    poutre::t_ArithNegate(s_img1, s_img2);
    std::string expected = "Dense Scalar GINT64 2 5 6 \
3 3 3 3 3 3 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1 \
3 -1 -1 -1 -1 -1\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewCompatibleShiftedOffset_negate)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 2,2 });
    auto s_img2 = v_img2.section({ 3,3 }, { 2,2 });
    //call UnaryOpDispatcher one idx
    poutre::t_ArithNegate(s_img1, s_img2);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 -1 -1 3 \
3 3 3 -1 -1 3\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff_negate)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT64> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    //call UnaryOpDispatcher primary template default two idx
    poutre::t_ArithNegate(s_img1, s_img2);

    std::string expected = "Dense Scalar GINT64 2 5 6 \
3 3 3 3 3 3 \
3 -1 -1 -1 3 3 \
3 -1 -1 -1 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType_addconstsaturated)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    poutre::t_ArithSaturatedAddConstant(v_img1, static_cast<poutre::pUINT8>(10), v_img2);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousDifferentPtrType__addconstsaturated)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //call UnaryOpDispatcher array view template specialization,fall back ptr
    poutre::t_ArithSaturatedAddConstant(v_img1, static_cast<poutre::pUINT8>(10), v_img2);
    std::string expected = "Dense Scalar GUINT8 2 5 6 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxSamePtrType_addconstsaturated)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section(/*poutre::idx2d*/{ 1,1 });
    auto s_img2 = v_img2.section(/*poutre::idx2d*/{ 1,1 });
    //call UnaryOpDispatcher one idx
    poutre::t_ArithSaturatedAddConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);
    std::string expected = "Dense Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 255 255 255 255 255 \
3 255 255 255 255 255 \
3 255 255 255 255 255 \
3 255 255 255 255 255\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxDiferentPtrType_addconstsaturated)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 });
    auto s_img2 = v_img2.section({ 1,1 });

    poutre::t_ArithSaturatedAddConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);
    std::string expected = "Dense Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 255 255 255 255 255 \
3 255 255 255 255 255 \
3 255 255 255 255 255 \
3 255 255 255 255 255\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewCompatibleShiftedOffset_addconstsaturated)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 2,2 });
    auto s_img2 = v_img2.section({ 3,3 }, { 2,2 });
    //call UnaryOpDispatcher one idx
    poutre::t_ArithSaturatedAddConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 255 255 3 \
3 3 3 255 255 3\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff_addconstsaturated)
{
    poutre::DenseImage<poutre::pINT64> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    //call UnaryOpDispatcher primary template default two idx
    poutre::t_ArithSaturatedAddConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 255 255 255 3 3 \
3 255 255 255 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}



BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType_Subconstsaturated)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(5);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    poutre::t_ArithSaturatedSubConstant(v_img1, static_cast<poutre::pUINT8>(10), v_img2);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousDifferentPtrType__subconstsaturated)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(5);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //call UnaryOpDispatcher array view template specialization,fall back ptr
    poutre::t_ArithSaturatedSubConstant(v_img1, static_cast<poutre::pUINT8>(10), v_img2);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0\
";
    auto imgstr = poutre::ImageToString(img2);
    //std::cout << "********" << "\n" << img2<<"\n"<< imgstr;
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxSamePtrType_subconstsaturated)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(5);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section(/*poutre::idx2d*/{ 1,1 });
    auto s_img2 = v_img2.section(/*poutre::idx2d*/{ 1,1 });
    //call UnaryOpDispatcher one idx
    poutre::t_ArithSaturatedSubConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);
    std::string expected = "Dense Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 0 0 0 0 0 \
3 0 0 0 0 0 \
3 0 0 0 0 0 \
3 0 0 0 0 0\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxDiferentPtrType_subconstsaturated)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(5);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 });
    auto s_img2 = v_img2.section({ 1,1 });

    poutre::t_ArithSaturatedSubConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);
    std::string expected = "Dense Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 0 0 0 0 0 \
3 0 0 0 0 0 \
3 0 0 0 0 0 \
3 0 0 0 0 0\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewCompatibleShiftedOffset_subconstsaturated)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(5);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 2,2 });
    auto s_img2 = v_img2.section({ 3,3 }, { 2,2 });
    //call UnaryOpDispatcher one idx
    poutre::t_ArithSaturatedSubConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 3 0 0 3 \
3 3 3 0 0 3\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff_subconstsaturated)
{
    poutre::DenseImage<poutre::pINT64> img1({ 5, 6 });
    img1.fill(5);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    //call UnaryOpDispatcher primary template default two idx
    poutre::t_ArithSaturatedSubConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
3 3 3 3 3 3 \
3 0 0 0 3 3 \
3 0 0 0 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff_sup)
{
    poutre::DenseImage<poutre::pINT64> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(150);
    poutre::DenseImage<poutre::pINT32> img3({ 5, 6 });
    img3.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    auto s_img3 = v_img3.section({ 2,2 }, { 2,3 });

    poutre::t_ArithSup(s_img1, s_img2, s_img3);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 250 250 250 3 \
3 3 250 250 250 3 \
3 3 3 3 3 3\
";
    auto imgstr = poutre::ImageToString(img3);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType_sup)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(255);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(150);
    poutre::DenseImage<poutre::pUINT8> img3({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    poutre::t_ArithSup(v_img1, v_img2, v_img3);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255\
";
    auto imgstr = poutre::ImageToString(img3);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff_inf)
{
    poutre::DenseImage<poutre::pINT64> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(150);
    poutre::DenseImage<poutre::pINT32> img3({ 5, 6 });
    img3.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    auto s_img3 = v_img3.section({ 2,2 }, { 2,3 });

    poutre::t_ArithInf(s_img1, s_img2, s_img3);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 150 150 150 3 \
3 3 150 150 150 3 \
3 3 3 3 3 3\
";
    auto imgstr = poutre::ImageToString(img3);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType_inf)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(255);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(150);
    poutre::DenseImage<poutre::pUINT8> img3({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    poutre::t_ArithInf(v_img1, v_img2, v_img3);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
150 150 150 150 150 150 \
150 150 150 150 150 150 \
150 150 150 150 150 150 \
150 150 150 150 150 150 \
150 150 150 150 150 150\
";
    auto imgstr = poutre::ImageToString(img3);
    BOOST_CHECK_EQUAL(imgstr, expected);
}



BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff_add)
{
    poutre::DenseImage<poutre::pINT64> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(150);
    poutre::DenseImage<poutre::pINT32> img3({ 5, 6 });
    img3.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    auto s_img3 = v_img3.section({ 2,2 }, { 2,3 });

    poutre::t_ArithSaturatedAdd(s_img1, s_img2, s_img3);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 400 400 400 3 \
3 3 400 400 400 3 \
3 3 3 3 3 3\
";
    auto imgstr = poutre::ImageToString(img3);
    //std::cout << "***********************\n" << imgstr << "***********************\n";
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType_add)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(150);
    poutre::DenseImage<poutre::pUINT8> img3({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    poutre::t_ArithSaturatedAdd(v_img1, v_img2, v_img3);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255 \
255 255 255 255 255 255\
";
    auto imgstr = poutre::ImageToString(img3);
    BOOST_CHECK_EQUAL(imgstr, expected);
}



BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff_sub)
{
    poutre::DenseImage<poutre::pINT64> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(150);
    poutre::DenseImage<poutre::pINT32> img3({ 5, 6 });
    img3.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    auto s_img3 = v_img3.section({ 2,2 }, { 2,3 });

    poutre::t_ArithSaturatedSub(s_img1, s_img2, s_img3);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
3 3 3 3 3 3 \
3 3 3 3 3 3 \
3 3 100 100 100 3 \
3 3 100 100 100 3 \
3 3 3 3 3 3\
";
    auto imgstr = poutre::ImageToString(img3);
    //std::cout << "***********************\n" << imgstr << "***********************\n";
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType_sub)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(150);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(250);
    poutre::DenseImage<poutre::pUINT8> img3({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    poutre::t_ArithSaturatedSub(v_img1, v_img2, v_img3);

    std::string expected = "Dense Scalar GUINT8 2 5 6 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0 \
0 0 0 0 0 0\
";
    auto imgstr = poutre::ImageToString(img3);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

namespace
{
	decltype(auto) ConstructVector(size_t size)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);

		std::vector<poutre::pUINT8, poutre::aligned_allocator<poutre::pUINT8>> m_vect;
		m_vect.reserve(size);
		for (auto i = 0; i < size; ++i) {
			m_vect.push_back(dis(gen));
		}
		return m_vect;
	}
}


BOOST_AUTO_TEST_CASE(benchmark)
{
	const auto size = 1000 * 1000;
	const auto inputVect1 = ConstructVector(size);
	const auto inputVect2 = ConstructVector(size);
	std::vector<poutre::pUINT8,poutre::aligned_allocator<poutre::pUINT8>> ouputVect(size);

	auto v_img1 = poutre::array_view< const poutre::pUINT8, 2>(inputVect1, { int(1000),int(1000) });
	auto v_img2 = poutre::array_view< const poutre::pUINT8, 2>(inputVect2, { int(1000),int(1000) });
	auto v_imgout = poutre::array_view< poutre::pUINT8, 2>(ouputVect, { int(1000),int(1000) });



	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	auto iteration = 10000;
	std::cout << "********************************" << std::endl;
	poutre::Timer timer;
	
    /*timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::t_ArithNegate(v_img1,v_imgout);
	timer.Stop();
	std::cout << "Time t_ArithNegate " << timer << std::endl;
	timer.Reset();
	std::cout << "********************************" << std::endl;*/

	std::cout << "********************************"<<std::endl;
	timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::t_ArithSaturatedSub(v_img1,v_img2,v_imgout);
	timer.Stop();
	std::cout << "Time t_ArithSaturatedSub " << timer << std::endl;
	timer.Reset();
	std::cout << "********************************" << std::endl;

	std::cout << "********************************" << std::endl;
	timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::t_ArithSaturatedAdd(v_img1, v_img2, v_imgout);
	timer.Stop();
	std::cout << "Time t_ArithSaturatedAdd " << timer << std::endl;
	timer.Reset();
	std::cout << "********************************" << std::endl;

	std::cout << "********************************" << std::endl;
	timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::t_ArithSaturatedAddConstant(v_img1,static_cast<poutre::pUINT8>(10),v_imgout);
	timer.Stop();
	std::cout << "Time t_ArithSaturatedAddConstant " << timer << std::endl;
	timer.Reset();
	std::cout << "********************************" << std::endl;

	std::cout << "********************************" << std::endl;
	timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::t_ArithSaturatedSubConstant(v_img1, static_cast<poutre::pUINT8>(10), v_imgout);
	timer.Stop();
	std::cout << "Time t_ArithSaturatedSubConstant " << timer << std::endl;
	timer.Reset();
	std::cout << "********************************" << std::endl;

	std::cout << "********************************" << std::endl;
	timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::t_ArithSup(v_img1,v_img2, v_imgout);
	timer.Stop();
	std::cout << "Time t_ArithSup " << timer << std::endl;
	timer.Reset();
	std::cout << "********************************" << std::endl;

	std::cout << "********************************" << std::endl;
	timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::t_ArithInf(v_img1, v_img2, v_imgout);
	timer.Stop();
	std::cout << "Time t_ArithInf " << timer << std::endl;
	timer.Reset();
	std::cout << "********************************" << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()



