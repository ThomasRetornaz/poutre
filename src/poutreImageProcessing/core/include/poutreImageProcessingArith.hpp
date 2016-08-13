
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

#include <poutreImageProcessing/core/include/poutreImageProcessingUnaryOp.hpp>
#include <poutreImageProcessing/core/include/poutreImageProcessingBinaryOp.hpp>

#include <boost/simd/sdk/simd/pack.hpp>
//#include <boost/simd/include/functions/plus.hpp>
#include <boost/simd/include/functions/adds.hpp> //saturated add
#include <boost/simd/include/functions/subs.hpp> //saturated sub
#include <boost/simd/include/functions/negs.hpp> //invert
#include <boost/simd/include/functions/max.hpp> //sup
#include <boost/simd/include/functions/min.hpp> //inf

namespace poutre
{
    namespace bs = boost::simd;


    /*  template<class ViewType>
      using target_type = typename std::conditional<
          is_strided<ViewType>::value,
          typename ViewType::value_type,
          typename boost::simd::pack<ViewType::value_type>
      >;*/
      /***********************************************************************************************************************************/
      /*                                                          NEGATE/INVERT                                                         */
      /**********************************************************************************************************************************/
    template< typename T1, typename T2, class tag>
    struct op_Invert;

    template< typename T1, typename T2>
    struct op_Invert<T1, T2, tag_SIMD_disabled>
    {
    public:
        op_Invert() {}
        T2 operator()(T1 const &a0)
        {
            return -a0;
        }
    };

    template< typename T>
    struct op_Invert<T, T, tag_SIMD_enabled>
    {
    public:
        op_Invert() {}
        template< typename U>
        U operator()(U const &a0)
        {
            return bs::negs(a0);
        }
    };

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
    void t_ArithNegate(const View1<T1, Rank>& i_vin, View2<T2, Rank>& o_vout)
    {
        PixelWiseUnaryOp<T1, T2, Rank, View1, View2, op_Invert>(i_vin, o_vout);
    }

    /***********************************************************************************************************************************/
    /*                                                  SATURATED ADD CONSTANT                                                         */
    /**********************************************************************************************************************************/

    template< typename T1, typename T2, class tag>
    struct op_Saturated_Add_Constant;

    template< typename T1, typename T2>
    struct op_Saturated_Add_Constant<T1, T2, tag_SIMD_disabled>
    {
    private:
        T2 m_val, m_maxval;
        using accutype = typename TypeTraits<T2>::accu_type;
    public:
        //using real_op = op_Saturated_Add_Constant<T, tag_SIMD_disabled>;
        op_Saturated_Add_Constant(T2 val) :m_val(val), m_maxval(TypeTraits<T2>::max()) {}
        T2 operator()(T1 const &a0)
        {
            accutype res = static_cast<accutype>(m_val) + static_cast<accutype>(a0);
            if (res > m_maxval) return m_maxval;
            return static_cast<T2>(res);
        }
    };

    //todo benchmark, if slow specialize boost::simd::transform to load m_val as a pack
    template< typename T>
    struct op_Saturated_Add_Constant<T, T, tag_SIMD_enabled>
    {
    private:
        /*using p_t = bs::pack<T>;
        p_t m_val_pack;*/
        T m_val;
    public:
        //using real_op = op_Saturated_Add_Constant<T, tag_SIMD_disabled>;       
        //op_Saturated_Add_Constant(T val) :m_val(val) {}
        op_Saturated_Add_Constant(T val) :m_val(val) {}
        template< typename U>
        U operator()(U const &a0)
        {
            return bs::adds(a0, m_val);
        }
    };

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
    //@warning saturation is related to *View2 type*
    void t_ArithSaturatedAddConstant(const View1<T1, Rank>& i_vin, T2 val, View2<T2, Rank>& o_vout)
    {
        PixelWiseUnaryOpWithValue<T1, T2, Rank, View1, View2, op_Saturated_Add_Constant>(i_vin, val, o_vout);
    }


    /***********************************************************************************************************************************/
    /*                                                  SUPREMUM                                                                       */
    /**********************************************************************************************************************************/

    template< typename T1, typename T2, typename T3, class tag>
    struct op_Sup;

    template< typename T1, typename T2, typename T3>
    struct op_Sup<T1, T2, T3, tag_SIMD_disabled>
    {
    public:
        op_Sup() {}
        T3 operator()(T1 const &a0, T2 const &a1)
        {
            return static_cast<T3>(a0 > a1 ? a0 : a1);
        }
    };

    template< typename T>
    struct op_Sup<T, T, T, tag_SIMD_enabled>
    {
    public:
        op_Sup() {}
        template< typename U>
        U operator()(U const &a0, U const &a1)
        {
            return bs::max(a0, a1);
        }
    };

    template<typename T1, typename T2, typename T3, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class View3>
    void t_ArithSup(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, View3<T3, Rank>& o_vout)
    {
        PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3, op_Sup>(i_vin1, i_vin2, o_vout);
    }


}//namespace poutre

#endif//POUTRE_IMAGEPROCESSING_ARITH_HPP__