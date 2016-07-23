
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_ARITH_HPP__
#define POUTRE_IMAGEPROCESSING_ARITH_HPP__

#ifndef POUTRE_IMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

#include <boost/simd/sdk/simd/native.hpp>
#include <boost/simd/include/functions/aligned_load.hpp>
#include <boost/simd/include/functions/aligned_store.hpp>
#include <boost/simd/include/functions/load.hpp>
#include <boost/simd/include/functions/splat.hpp>
#include <boost/simd/memory/align_on.hpp>
#include <boost/simd/memory/is_aligned.hpp>
#include <boost/simd/sdk/simd/algorithm.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include <boost/config.hpp>

#include <boost/simd/include/functions/plus.hpp>
#include <boost/simd/include/functions/adds.hpp> //saturated add
#include <boost/simd/include/functions/subs.hpp> //saturated sub

namespace poutre
{
    namespace bs = boost::simd;


    template<class ViewType>
    using target_type = std::conditional<
        is_strided<ViewType>::value,
        typename ViewType::value_type,
        typename boost::simd::pack<ViewType::value_type>
    >;

    //!saturated add constant
    template <typename T>
    class op_adds
    {
    private
        T m_val;
        T m_maxval;
        typedef typename TypeTraits<T>::accu_type accutype;
    public:
        op_adds(T val):m_val(val),m_maxval(TypeTraits<T>::max())

        T operator()(T const &a0)
        {
            accutype res = static_cast<accutype>(m_val) + static_cast<accutype>(a0);
            if (res > m_max_val) return m_max_val;
            return static_cast<T>(res);
        }
    };

    //! simd saturated add constant
    template <typename T>
    class op_boost_adds
    {
        using p_t=bs::pack<T>;
        p_t m_val;
    public:
        op_adds(T val) :m_val(bs::splat<p_t>(val))

    public:
        p_t operator()(p_t const &a0)
        {
            return bs::adds(a0, m_val);
        }
    };

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
    void t_ArithAddConstSaturated(const View1<T1, Rank>& i_vin,T1 val,View2<T2, Rank>& o_vout)
    {
        op_boost_adds op(val);
        UnaryOp(i_vin, o_vout, op);
    }

    
}//namespace poutre

#endif//POUTRE_IMAGEPROCESSING_ARITH_HPP__