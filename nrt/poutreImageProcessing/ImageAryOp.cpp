
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
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessing/core/include/poutreImageProcessingArith.hpp>
#include <boost/lexical_cast.hpp>
#include <poutreIO/poutreIOString.hpp>

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
    img2.fill(3);
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
    poutre::t_ArithSaturatedAddConstant(v_img1,static_cast<poutre::pUINT8>(10),v_img2);

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
    poutre::DenseImage<poutre::pUINT32> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //call UnaryOpDispatcher array view template specialization,fall back ptr
    poutre::t_ArithSaturatedAddConstant(v_img1, static_cast<poutre::pUINT8>(10), v_img2);
    std::string expected = "Dense Scalar GINT32 2 5 6 \
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
    std::string expected = "Dense Scalar GINT32 2 5 6 \
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
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(250);
    poutre::DenseImage<poutre::pINT64> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    //call UnaryOpDispatcher primary template default two idx
    poutre::t_ArithSaturatedAddConstant(s_img1, static_cast<poutre::pUINT8>(10), s_img2);

    std::string expected = "Dense Scalar GINT64 2 5 6 \
3 3 3 3 3 3 \
3 255 255 255 3 3 \
3 255 255 255 3 3 \
3 3 3 3 3 3 \
3 3 3 3 3 3\
";
    auto imgstr = poutre::ImageToString(img2);
    BOOST_CHECK_EQUAL(imgstr, expected);
}


BOOST_AUTO_TEST_SUITE_END()



