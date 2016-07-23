
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"
#include "poutreBase/poutreGeometry.hpp"
#include <vector>
#include <iostream>
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessing/core/include/poutreImageProcessingUnaryOp.hpp>
#include <boost/lexical_cast.hpp>


#include <boost/simd/sdk/simd/algorithm.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include <boost/simd/sdk/simd/pack.hpp>
#include <boost/simd/sdk/simd/io.hpp>
#include <boost/simd/include/functions/load.hpp>
#include <boost/simd/include/functions/evaluate.hpp>
#include <boost/simd/include/functions/sum.hpp>
#include <boost/simd/include/functions/load.hpp>
#include <boost/simd/include/functions/plus.hpp>
#include <boost/simd/include/functions/adds.hpp>
#include <boost/simd/include/functions/multiplies.hpp>
#include <boost/simd/include/constants/one.hpp>

namespace bs = boost::simd;

//TODO
//#include <boost/call_traits.hpp>
//Benchmark with http://www.boost.org/doc/libs/1_59_0/libs/utility/call_traits.htm param_type
struct dummy_unary_op
{
private:
    size_t count;

public:
    dummy_unary_op() :count(0) {}

    template<typename T>
    T operator()(const T &val) 
    {
        count++;
        return val;
    }

    size_t GetCount() const
    {
        return count;
    }
};

////have a look https://searchcode.com/codesearch/view/15337430/
//nt2\modules\boost\simd\sdk\unit\simd\algorithm\transform.cpp


template<class IN, class OUT>
struct op_plus_const_saturate_default
{
    IN m_const;
    op_plus_const_saturate_default(IN const &a0) : m_const(a0) {}

    OUT operator()(IN const& t0) const
    {
        return (a0+m_const); //TODO clip
    }
};

template<typename T>
struct op_plus_const_saturate_simd
{
    T m_const;
    op_plus_const_saturate_simd(T const &a0) : m_const(a0) {}

    template<class U>
    U operator()(U const& t0) const
    {
        return bs::adds(t0, m_const); //damit not vectorized at all :/
    }
};

struct op_plus_saturate
{
    template<class T>
    T operator()(T const& t0, T const& t1) const
    {
        return bs::adds(t0,t1);
    }
};


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


BOOST_AUTO_TEST_SUITE(poutreImageProcessingContainer)

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    img2.fill(3);
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //op_boost_utc_add<poutre::pUINT8> op(2);
  
    //poutre::UnaryOp(v_img1, v_img2, op);
    auto i_vinbeg = v_img1.data();
    auto i_vinend = v_img1.data() + v_img1.size();
    auto i_voutbeg = v_img2.data();
    poutre::UnaryOp(v_img1, v_img2, op_plus_const_saturate_simd<poutre::pUINT8>(2));


}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousDifferentPtrType)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //call UnaryOpDispatcher array view template specialization,fall back ptr
    poutre::UnaryOp(v_img1, v_img2, op_plus_const_saturate_simd<poutre::pUINT8>(2));
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxSamePtrType)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 });
    auto s_img2 = v_img2.section({ 1,1 });
    //call UnaryOpDispatcher one idx
    poutre::UnaryOp(s_img1, s_img2, op_plus_const_saturate_simd<poutre::pUINT8>(2));
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxDiferentPtrType)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 });
    auto s_img2 = v_img2.section({ 1,1 });
    //call UnaryOpDispatcher one idx
    poutre::UnaryOp(s_img1, s_img2, op_plus_const_saturate_simd<poutre::pUINT8>(2));
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewCompatibleShiftedOffset)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);

    //Extract section

    auto s_img1 = v_img1.section({ 1,1 }, { 2,2 });
    auto s_img2 = v_img2.section({ 3,3 }, { 2,2 });
    //call UnaryOpDispatcher one idx
    poutre::UnaryOp(s_img1, s_img2, op_plus_const_saturate_simd<poutre::pUINT8>(2));
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
    auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });
    //call UnaryOpDispatcher primary template default two idx
    poutre::UnaryOp(s_img1, s_img2, op_plus_const_saturate_simd<poutre::pUINT8>(2));
}





BOOST_AUTO_TEST_SUITE_END()



