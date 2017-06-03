
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
namespace bs = boost::simd;

////have a look https://searchcode.com/codesearch/view/15337430/
//nt2\modules\boost\simd\sdk\unit\simd\algorithm\transform.cpp



//template<class view_type>
//using target_type = std::conditional< is_strided::value, view_type::value_type, bs::simd::pack<view_type::value_type>>;

//template<class view_type1, class view_type2,class operator_default,class operator_simd>
//using op = std::conditional<
//                            std::is_same<
//                                typename poutre::details::extract_value_type<view_type1>::value_type,
//                                typename poutre::details::extract_value_type<view_type2>::value_type
//                                        >::value,
//                            operator_simd,
//                            operator_default >
//                            >;


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

		std::vector<unsigned char, boost::simd::allocator<unsigned char>> m_vect;
		m_vect.reserve(size);
		for (auto i = 0; i < size; ++i) {
			m_vect.push_back(dis(gen));
		}
		return m_vect;
	}
}


namespace
{
	/*decltype(auto) ConstructVector(size_t size)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);

		std::vector<unsigned char, boost::simd::allocator<unsigned char>> m_vect;
		m_vect.reserve(size);
		for (auto i = 0; i < size; ++i) {
			m_vect.push_back(dis(gen));
		}
		return m_vect;
	}*/

	struct tag_SIMD_disabled {};
	struct tag_SIMD_enabled {};

	template< typename T1, typename T2, typename T3, class tag>
	struct op_Inf;

	template< typename T1, typename T2, typename T3>
	struct op_Inf<T1, T2, T3, tag_SIMD_disabled>
	{
	public:
		op_Inf() {}
		POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) POUTRE_NOEXCEPT
		{
			//std::cout << "call min" << std::endl;
			return static_cast<T3>(a0 < a1 ? a0 : a1);
		}
	};

	template< typename T>
	struct op_Inf<T, T, T, tag_SIMD_enabled>
	{
	public:
		op_Inf() {}
		template< typename U>
		POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) POUTRE_NOEXCEPT
		{
			//std::cout << "call bs::min" << std::endl;
			//std::cout << "U type" << typeid(U).name() <<std::endl;
			return bs::min(a0, a1);
		}
	};

	void PerformInfScalar(const std::vector<unsigned char, boost::simd::allocator<unsigned char>>& vectin1, const std::vector<unsigned char, boost::simd::allocator<unsigned char>>& vectin2, std::vector<unsigned char, boost::simd::allocator<unsigned char>>& vectout)
	{
		//get the specialized operator
		using real_op = op_Inf<unsigned char, unsigned char, unsigned char, tag_SIMD_disabled>;
		real_op op;
		//std::cout << "\n" << "call PixelWiseBinaryOpDispatcherWithTag array view template specialization same type,fall back ptr";
		auto i_vinbeg1 = vectin1.data();
		auto i_vinend1 = vectin1.data() + vectin1.size();
		auto i_vinbeg2 = vectin2.data();
		auto i_voutbeg = vectout.data();
		for (; i_vinbeg1 != i_vinend1; ++i_vinbeg1, ++i_vinbeg2, ++i_voutbeg)
		{
			*i_voutbeg = op(*i_vinbeg1, *i_vinbeg2);
		}
	}

	void PerformInfSIMD(const std::vector<unsigned char, boost::simd::allocator<unsigned char>>& vectin1, const std::vector<unsigned char, boost::simd::allocator<unsigned char>>& vectin2, std::vector<unsigned char, boost::simd::allocator<unsigned char>>& vectout)
	{
		//get the specialized operator
		using real_op = op_Inf<unsigned char, unsigned char, unsigned char, tag_SIMD_enabled>;
		real_op op;
		//std::cout << "\n" << "call PixelWiseBinaryOpDispatcherWithTag array view template specialization same type,fall back ptr SIMD";
		auto i_vinbeg1 = vectin1.data();
		auto i_vinend1 = vectin1.data() + vectin1.size();
		auto i_vinbeg2 = vectin2.data();
		auto i_voutbeg = vectout.data();
		bs::transform(i_vinbeg1, i_vinend1, i_vinbeg2, i_voutbeg, op);
	}

}

BOOST_AUTO_TEST_CASE(operatorinfdispatch)
{
	const auto size = 1000*1000;
	//const auto size = 10 * 10;
	const auto inputVect1 = ConstructVector(size);
	const auto inputVect2 = ConstructVector(size);
	std::vector<unsigned char, boost::simd::allocator<unsigned char>> ouputVect(size);

	auto iteration = 10000;
	//auto iteration = 1;

	{
		poutre::Timer timer;
		std::cout << "********************************" << std::endl;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			PerformInfSIMD(inputVect1, inputVect2, ouputVect);
		timer.Stop();
		std::cout << "Time testInf simd" << timer << std::endl;
		timer.Reset();
		std::cout << "********************************" << std::endl;
	}
	{
		std::cout << "********************************" << std::endl;
		poutre::Timer timer;
		timer.Start();
		for (auto i = 0; i < iteration; ++i)
			PerformInfScalar(inputVect1, inputVect2, ouputVect);
		timer.Stop();
		std::cout << "Time testInf scalar " << timer << std::endl;
		timer.Reset();
		std::cout << "********************************" << std::endl;
	}


}

namespace
{
	/*template <class T,ptrdiff_t Rank>
	class view
	{
	};

	template <class T,ptrdiff_t Rank>
	class sview
	{
	};*/

	// primary use strided view
	template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, typename = void>
	struct PixelWiseUnaryDispatch
	{
		void operator()(const View1<T1, Rank>& i_vin, View2<T2, Rank>& o_vout) const
		{
			std::cout << "***************" << std::endl;
			std::cout << "primary template" << std::endl;
			std::cout << "***************" << std::endl;

		}
	};

	template <typename T1, typename T2, ptrdiff_t Rank>
	struct PixelWiseUnaryDispatch <T1, T2, Rank,poutre::array_view, poutre::array_view,
		std::enable_if_t<!std::is_same<std::remove_const_t<T1> , std::remove_const_t<T2>>::value>>
	{
		void operator()(poutre::array_view<T1,Rank> const & i_vin, poutre::array_view<T2, Rank> & o_vout) const
		{
			std::cout << "***************" << std::endl;
			std::cout << "both view != type" << std::endl;
			std::cout << "***************" << std::endl;
		}
	};
	template <typename T1, typename T2,ptrdiff_t Rank>
	struct PixelWiseUnaryDispatch<T1, T2,Rank, poutre::array_view, poutre::array_view,
		std::enable_if_t<
		std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value
		&& std::is_arithmetic<T1>::value>>
	{
		void operator()(poutre::array_view<T1, Rank> const & i_vin, poutre::array_view<T2, Rank> & o_vout) const
		{
			std::cout << "***************" << std::endl;
			std::cout << "both view same type" << std::endl;
			std::cout << "***************" << std::endl;

		}
	};

	template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
	void PixelWiseUnary(const View1<T1, Rank>& i_vin, View2<T2, Rank>& o_vout)
	{
		PixelWiseUnaryDispatch<T1, T2,Rank, View1, View2> dispatcher;
		dispatcher(i_vin, o_vout);
	}



}

BOOST_AUTO_TEST_CASE(teststemplate)
{
	poutre::array_view<const int,1> vin;
	poutre::array_view<const float,1> vinf;
	poutre::array_view<int,1> vout;
	poutre::strided_array_view<int,1> vsout;
	PixelWiseUnary(vin, vsout);
	PixelWiseUnary(vinf, vout);
	PixelWiseUnary(vin, vout);


}

BOOST_AUTO_TEST_CASE(benchmark)
{
	const auto size = 1024 * 1024;
	const auto inputVect1 = ConstructVector(size);
	const auto inputVect2 = ConstructVector(size);
	std::vector<poutre::pUINT8, boost::simd::allocator<poutre::pUINT8>> ouputVect(size);

	auto v_img1 = poutre::array_view< const poutre::pUINT8, 2>(inputVect1, { int(1024),int(1024) });
	auto v_img2 = poutre::array_view< const poutre::pUINT8, 2>(inputVect2, { int(1024),int(1024) });
	auto v_imgout = poutre::array_view< poutre::pUINT8, 2>(ouputVect, { int(1024),int(1024) });



	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	auto iteration = 1;
	std::cout << "********************************" << std::endl;
	poutre::Timer timer;
	timer.Start();
	for (auto i = 0; i<iteration; ++i)
		poutre::t_ArithNegate(v_img1,v_imgout);
	timer.Stop();
	std::cout << "Time t_ArithNegate " << timer << std::endl;
	timer.Reset();
	std::cout << "********************************" << std::endl;

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



