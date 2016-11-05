
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
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingTernaryOp.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingQuaternaryOp.hpp>

namespace bs = boost::simd;

namespace
{
    template<typename T1, typename T2, typename T3, typename Tout, class tag>
    class opTernaryDummy;

    template<typename T1, typename T2, typename T3, typename Tout>
    class opTernaryDummy<T1, T2, T3, Tout, poutre::tag_SIMD_disabled>
    {

    public:
        opTernaryDummy() {}
        Tout operator()(T1 const& a0, T2 const& a1, T3 const& a2)
        {
            return Tout(a0 + a1 + a2);
        }
    };

    template<typename T1, typename T2, typename T3, typename Tout>
    class opTernaryDummy<T1, T2, T3, Tout, poutre::tag_SIMD_enabled> :public  opTernaryDummy<T1, T2, T3, Tout, poutre::tag_SIMD_disabled>
    {
    };

    template<typename T1, typename T2, typename T3, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class View3, template <typename, ptrdiff_t> class ViewOut>
    void t_ApplyTernaryOp(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, const View3<T3, Rank>& i_vin3, ViewOut<Tout, Rank>& o_vout)
    {
        poutre::PixelWiseTernaryOp<T1, T2, T3, Tout, Rank, View1, View2, View3, ViewOut, opTernaryDummy>(i_vin1, i_vin2, i_vin3, o_vout);
    }



    template<typename T1, typename T2, typename T3, typename T4, typename Tout, class tag>
    class opQuaternaryDummy;

    template<typename T1, typename T2, typename T3, typename T4, typename Tout>
    class opQuaternaryDummy<T1, T2, T3, T4, Tout, poutre::tag_SIMD_disabled>
    {

    public:
        opQuaternaryDummy() {}
        Tout operator()(T1 const& a0, T2 const& a1, T3 const& a2, T4 const& a3)
        {
            return Tout(a0 + a1 + a2 + a3);
        }
    };

    template<typename T1, typename T2, typename T3, typename T4, typename Tout>
    class opQuaternaryDummy<T1, T2, T3, T4, Tout, poutre::tag_SIMD_enabled> :public  opQuaternaryDummy<T1, T2, T3, T4, Tout, poutre::tag_SIMD_disabled>
    {
    };

    template<typename T1, typename T2, typename T3, typename T4, typename Tout, ptrdiff_t Rank,
        template <typename, ptrdiff_t> class View1,
        template <typename, ptrdiff_t> class View2,
        template <typename, ptrdiff_t> class View3,
        template <typename, ptrdiff_t> class View4,
        template <typename, ptrdiff_t> class ViewOut>
    void t_ApplyQuaternaryOp(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, const View3<T3, Rank>& i_vin3, const View4<T4, Rank>& i_vin4, ViewOut<Tout, Rank>& o_vout)
    {
        poutre::PixelWiseQuaternaryOp<T1, T2, T3, T4, Tout, Rank, View1, View2, View3, View4, ViewOut, opQuaternaryDummy>(i_vin1, i_vin2, i_vin3, i_vin4, o_vout);
    }
}//namespace

BOOST_AUTO_TEST_SUITE(poutreImageProcessingTernaryOp)

BOOST_AUTO_TEST_CASE(ternaryop_contigous)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    img2.fill(2);
    poutre::DenseImage<poutre::pINT32> img3({ 5, 6 });
    img3.fill(3);
    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    auto v_imgout = poutre::view(imgout);

    t_ApplyTernaryOp(v_img1, v_img2, v_img3, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
6 6 6 6 6 6 \
6 6 6 6 6 6 \
6 6 6 6 6 6 \
6 6 6 6 6 6 \
6 6 6 6 6 6\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(ternaryop_alldiff)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(2);
    poutre::DenseImage<poutre::pINT64> img3({ 5, 6 });
    img3.fill(3);
    poutre::DenseImage<poutre::pFLOAT> imgout({ 5, 6 });
    imgout.fill(4);

    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    auto v_imgout = poutre::view(imgout);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    auto s_img3 = v_img3.section({ 2,2 }, { 2,3 });
    auto s_imgout = v_imgout.section({ 1,1 }, { 2,3 });
    t_ApplyTernaryOp(s_img1, s_img2, s_img3, s_imgout);

    std::string expected = "Dense Scalar F32 2 5 6 \
4 4 4 4 4 4 \
4 6 6 6 4 4 \
4 6 6 6 4 4 \
4 4 4 4 4 4 \
4 4 4 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(ternaryop_arrayviewdifferentptrtype)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(2);
    poutre::DenseImage<poutre::pINT64> img3({ 5, 6 });
    img3.fill(3);
    poutre::DenseImage<poutre::pFLOAT> imgout({ 5, 6 });
    imgout.fill(4);

    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    auto v_imgout = poutre::view(imgout);

    t_ApplyTernaryOp(v_img1, v_img2, v_img3, v_imgout);

    std::string expected = "Dense Scalar F32 2 5 6 \
6 6 6 6 6 6 \
6 6 6 6 6 6 \
6 6 6 6 6 6 \
6 6 6 6 6 6 \
6 6 6 6 6 6\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(ternaryop_compatibleview)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(2);
    poutre::DenseImage<poutre::pINT64> img3({ 5, 6 });
    img3.fill(3);
    poutre::DenseImage<poutre::pFLOAT> imgout({ 5, 6 });
    imgout.fill(4);

    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    auto v_imgout = poutre::view(imgout);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 3,2 });
    auto s_img3 = v_img3.section({ 1,1 }, { 3,2 });
    auto s_imgout = v_imgout.section({ 1,1 }, { 3,2 });
    t_ApplyTernaryOp(s_img1, s_img2, s_img3, s_imgout);

    std::string expected = "Dense Scalar F32 2 5 6 \
4 4 4 4 4 4 \
4 6 6 4 4 4 \
4 6 6 4 4 4 \
4 6 6 4 4 4 \
4 4 4 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);
}
BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(poutreImageProcessingQuaternaryOp)

BOOST_AUTO_TEST_CASE(quaternaryop_contigous)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    img2.fill(2);
    poutre::DenseImage<poutre::pINT32> img3({ 5, 6 });
    img3.fill(3);
    poutre::DenseImage<poutre::pINT32> img4({ 5, 6 });
    img4.fill(4);
    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });

    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    auto v_img4 = poutre::view(img4);
    auto v_imgout = poutre::view(imgout);

    t_ApplyQuaternaryOp(v_img1, v_img2, v_img3, v_img4, v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
10 10 10 10 10 10 \
10 10 10 10 10 10 \
10 10 10 10 10 10 \
10 10 10 10 10 10 \
10 10 10 10 10 10\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}

BOOST_AUTO_TEST_CASE(quaternaryop_alldiff)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(2);
    poutre::DenseImage<poutre::pINT64> img3({ 5, 6 });
    img3.fill(3);
    poutre::DenseImage<poutre::pINT64> img4({ 5, 6 });
    img4.fill(4);
    poutre::DenseImage<poutre::pFLOAT> imgout({ 5, 6 });
    imgout.fill(4);

    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    auto v_img4 = poutre::view(img4);
    auto v_imgout = poutre::view(imgout);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    auto s_img3 = v_img3.section({ 2,2 }, { 2,3 });
    auto s_img4 = v_img4.section({ 2,2 }, { 2,3 });
    auto s_imgout = v_imgout.section({ 1,1 }, { 2,3 });
    t_ApplyQuaternaryOp(s_img1, s_img2, s_img3, s_img4, s_imgout);

    std::string expected = "Dense Scalar F32 2 5 6 \
4 4 4 4 4 4 \
4 10 10 10 4 4 \
4 10 10 10 4 4 \
4 4 4 4 4 4 \
4 4 4 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(quaternaryop_arrayviewdifferentptrtype)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(2);
    poutre::DenseImage<poutre::pINT64> img3({ 5, 6 });
    img3.fill(3);
    poutre::DenseImage<poutre::pINT64> img4({ 5, 6 });
    img4.fill(4);
    poutre::DenseImage<poutre::pFLOAT> imgout({ 5, 6 });
    imgout.fill(4);

    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    auto v_img4 = poutre::view(img4);
    auto v_imgout = poutre::view(imgout);

    t_ApplyQuaternaryOp(v_img1, v_img2, v_img3, v_img4, v_imgout);

    std::string expected = "Dense Scalar F32 2 5 6 \
10 10 10 10 10 10 \
10 10 10 10 10 10 \
10 10 10 10 10 10 \
10 10 10 10 10 10 \
10 10 10 10 10 10\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_CASE(quaternaryop_compatibleview)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(2);
    poutre::DenseImage<poutre::pINT64> img3({ 5, 6 });
    img3.fill(3);
    poutre::DenseImage<poutre::pINT64> img4({ 5, 6 });
    img4.fill(4);
    poutre::DenseImage<poutre::pFLOAT> imgout({ 5, 6 });
    imgout.fill(4);

    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    auto v_img3 = poutre::view(img3);
    auto v_img4 = poutre::view(img4);
    auto v_imgout = poutre::view(imgout);

    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 3,2 });
    auto s_img3 = v_img3.section({ 1,1 }, { 3,2 });
    auto s_img4 = v_img4.section({ 1,1 }, { 3,2 });
    auto s_imgout = v_imgout.section({ 1,1 }, { 3,2 });
    t_ApplyQuaternaryOp(s_img1, s_img2, s_img3, s_img4, s_imgout);

    std::string expected = "Dense Scalar F32 2 5 6 \
4 4 4 4 4 4 \
4 10 10 4 4 4 \
4 10 10 4 4 4 \
4 10 10 4 4 4 \
4 4 4 4 4 4\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);
}

BOOST_AUTO_TEST_SUITE_END()



