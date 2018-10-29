
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_ARITH_HPP__
#define POUTRE_IMAGEPROCESSING_ARITH_HPP__

#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_UNARYOP_HPP__
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingUnaryOp.hpp>
#endif
#ifndef POUTRE_IMAGEPROCESSING_BINARYOP_HPP__
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingBinaryOp.hpp>
#endif

#ifdef USE_BOOSTSIMD
 //https://developer.numscale.com/boost.simd/documentation/develop/group__group-arithmetic.html
 #include <boost/simd/function/saturated.hpp>
 #include <boost/simd/function/plus.hpp> //add
 #include <boost/simd/function/minus.hpp> //sub
 #include <boost/simd/function/negate.hpp> //invert
 #include <boost/simd/function/max.hpp> //sup
 #include <boost/simd/function/min.hpp> //inf
#else
#include <simdpp/simd.h>
#endif

namespace poutre
{
 
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
        POUTRE_ALWAYS_INLINE T2 operator()(T1 const &a0) const POUTRE_NOEXCEPT
        {
            return -a0;
        }
    };
    template< typename T>
    struct op_Invert<T, T, tag_SIMD_enabled>
    {
    public:
        op_Invert() {}

        POUTRE_ALWAYS_INLINE T operator()(T const &a0) const POUTRE_NOEXCEPT
        {
            return -a0;
        }

        template< typename U>
        POUTRE_ALWAYS_INLINE U operator()(U const &a0) const POUTRE_NOEXCEPT
        {
#ifdef USE_BOOSTSIMD
           return -a0;
#else
           return simd::neg(a0);
#endif
        }
    };

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
    void t_ArithNegate(const View1<T1, Rank>& i_vin, View2<T2, Rank>& o_vout)
    {
        PixelWiseUnaryOp<T1, T2, Rank, View1, View2, op_Invert>(i_vin, o_vout);
    }

    /***********************************************************************************************************************************/
    /*                                                  SATURATED SUB                                                                  */
    /**********************************************************************************************************************************/
    template< typename T1, typename T2, typename T3, class tag>
    struct op_Saturated_Sub;

    template< typename T1, typename T2, typename T3>
    struct op_Saturated_Sub<T1, T2, T3, tag_SIMD_disabled>
    {
    private:
        T3 m_minval;
        using accutype = typename TypeTraits<T3>::accu_type;
    public:
        op_Saturated_Sub() :m_minval(TypeTraits<T3>::min()) {}
        POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
        {
            accutype res = static_cast<accutype>(a0) - static_cast<accutype>(a1);
            if (res < static_cast<accutype>(m_minval)) return m_minval;
            return static_cast<T3>(res);
        }
    };

    template< typename T>
    struct op_Saturated_Sub<T, T, T, tag_SIMD_enabled>
    {
    private:
        T m_minval;
        using accutype = typename TypeTraits<T>::accu_type;
    public:
        op_Saturated_Sub() :m_minval(TypeTraits<T>::min()) {}

        POUTRE_ALWAYS_INLINE T operator()(T const &a0, T const &a1) const POUTRE_NOEXCEPT
        {
            accutype res = static_cast<accutype>(a0) - static_cast<accutype>(a1);
            if (res < static_cast<accutype>(m_minval)) return m_minval;
            return static_cast<T>(res);
        }

        template< typename U>
        POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) const POUTRE_NOEXCEPT
        {
#ifdef USE_BOOSTSIMD
            return boost::simd::saturated_(boost::simd::minus)(a0, a1);
#else
            return simd::sub_sat(a0, a1);
#endif
        }
    };

    template<typename T1, typename T2, typename T3, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class View3>
    void t_ArithSaturatedSub(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, View3<T3, Rank>& o_vout)
    {
        PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3, op_Saturated_Sub>(i_vin1, i_vin2, o_vout);
    }

    /***********************************************************************************************************************************/
    /*                                                  SATURATED ADD                                                                  */
    /**********************************************************************************************************************************/
    template< typename T1, typename T2, typename T3, class tag>
    struct op_Saturated_Add;

    template< typename T1, typename T2, typename T3>
    struct op_Saturated_Add<T1, T2, T3, tag_SIMD_disabled>
    {
    private:
        T3 m_maxval;
        using accutype = typename TypeTraits<T3>::accu_type;
    public:
        op_Saturated_Add() :m_maxval(TypeTraits<T3>::max()) {}
        POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
        {
            accutype res = static_cast<accutype>(a0) + static_cast<accutype>(a1);
            if (res > static_cast<accutype>(m_maxval)) return m_maxval;
            return static_cast<T3>(res);
        }
    };      

    template< typename T>
    struct op_Saturated_Add<T, T, T, tag_SIMD_enabled>
    {
    private:
        T m_maxval;
        using accutype = typename TypeTraits<T>::accu_type;
    public:
        op_Saturated_Add() :m_maxval(TypeTraits<T>::max()) {}
        POUTRE_ALWAYS_INLINE T operator()(T const &a0, T const &a1) const POUTRE_NOEXCEPT
        {
            accutype res = static_cast<accutype>(a0) + static_cast<accutype>(a1);
            if (res > static_cast<accutype>(m_maxval)) return m_maxval;
            return static_cast<T>(res);
        }
        template< typename U>
        POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) const POUTRE_NOEXCEPT
        {
#ifdef USE_BOOSTSIMD
           return boost::simd::saturated_(boost::simd::plus)(a0, a1);
#else
            return simd::add_sat(a0, a1);
#endif
        }
    };

    template<typename T1, typename T2, typename T3, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class View3>
    void t_ArithSaturatedAdd(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, View3<T3, Rank>& o_vout)
    {
        PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3, op_Saturated_Add>(i_vin1, i_vin2, o_vout);
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
        op_Saturated_Add_Constant(T2 val) :m_val(val), m_maxval(TypeTraits<T2>::max()) {}
        POUTRE_ALWAYS_INLINE T2 operator()(T1 const &a0) const  POUTRE_NOEXCEPT
        {
            accutype res = static_cast<accutype>(m_val) + static_cast<accutype>(a0);
            if (res > static_cast<accutype>(m_maxval)) return m_maxval;
            return static_cast<T2>(res);
        }
    };

    //todo benchmark, if slow specialize boost::simd::transform to load m_val as a pack
    template< typename T>
    struct op_Saturated_Add_Constant<T, T, tag_SIMD_enabled>
    {
    private:
        const T m_val, m_maxval;
        const typename TypeTraits<T>::simd_type m_simd_val;
        using accutype = typename TypeTraits<T>::accu_type;
    public:
#ifdef USE_BOOSTSIMD
        op_Saturated_Add_Constant(T val) :m_val(val), m_maxval(TypeTraits<T>::max()), m_simd_val(val){}
#else
       op_Saturated_Add_Constant(T val) : m_val(val), m_maxval(TypeTraits<T>::max()), m_simd_val(simd::splat(val)) {}
#endif
        POUTRE_ALWAYS_INLINE T operator()(T const &a0) const  POUTRE_NOEXCEPT
        {
            accutype res = static_cast<accutype>(m_val) + static_cast<accutype>(a0);
            if (res > static_cast<accutype>(m_maxval)) return m_maxval;
            return static_cast<T>(res);
        }
        template< typename U>
        POUTRE_ALWAYS_INLINE U operator()(U const &a0) const POUTRE_NOEXCEPT
        {
#ifdef USE_BOOSTSIMD
           return boost::simd::saturated_(boost::simd::plus)(a0, m_val);
#else
            return simd::add_sat(a0, m_simd_val);
#endif
        }
    };

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
    //@warning saturation is related to *View2 type*
    void t_ArithSaturatedAddConstant(const View1<T1, Rank>& i_vin, T2 val, View2<T2, Rank>& o_vout)
    {
        PixelWiseUnaryOpWithValue<T1, T2, Rank, View1, View2, op_Saturated_Add_Constant>(i_vin, val, o_vout);
    }

    /***********************************************************************************************************************************/
    /*                                                  SATURATED SUB CONSTANT                                                        */
    /**********************************************************************************************************************************/

    template< typename T1, typename T2, class tag>
    struct op_Saturated_Sub_Constant;

    template< typename T1, typename T2>
    struct op_Saturated_Sub_Constant<T1, T2, tag_SIMD_disabled>
    {
    private:
        T2 m_val, m_minval;
        using accutype = typename TypeTraits<T2>::accu_type;
    public:
        op_Saturated_Sub_Constant(T2 val) :m_val(val), m_minval(TypeTraits<T2>::min()) {}
        POUTRE_ALWAYS_INLINE T2 operator()(T1 const &a0) const POUTRE_NOEXCEPT
        {
            accutype res = static_cast<accutype>(a0) - static_cast<accutype>(m_val);
            if (res < static_cast<accutype>(m_minval)) return m_minval;
            return static_cast<T2>(res);
        }
    };

    //todo benchmark, if slow specialize boost::simd::transform to load m_val as a pack
    template< typename T>
    struct op_Saturated_Sub_Constant<T, T, tag_SIMD_enabled>
    {
    private:
        const T m_val, m_minval;
        const typename TypeTraits<T>::simd_type m_simd_val;
        using accutype = typename TypeTraits<T>::accu_type;
    public:
#ifdef USE_BOOSTSIMD
       op_Saturated_Sub_Constant(T val) :m_val(val), m_minval(TypeTraits<T>::min()), m_simd_val(val) {}
#else
        op_Saturated_Sub_Constant(T val) :m_val(val), m_minval(TypeTraits<T>::min()), m_simd_val(simd::splat(val)){}
#endif
        POUTRE_ALWAYS_INLINE T operator()(T const &a0) const POUTRE_NOEXCEPT
        {
            accutype res = static_cast<accutype>(a0) - static_cast<accutype>(m_val);
            if (res < static_cast<accutype>(m_minval)) return m_minval;
            return static_cast<T>(res);
        }

        template< typename U>
        POUTRE_ALWAYS_INLINE U operator()(U const &a0) const POUTRE_NOEXCEPT
        {
#ifdef USE_BOOSTSIMD
           return boost::simd::saturated_(boost::simd::minus)(a0, m_val);
#else
           return simd::sub_sat(a0, m_simd_val);
#endif
      
        }
    };

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
    //@warning saturation is related to *View2 type*
    void t_ArithSaturatedSubConstant(const View1<T1, Rank>& i_vin, T2 val, View2<T2, Rank>& o_vout)
    {
        PixelWiseUnaryOpWithValue<T1, T2, Rank, View1, View2, op_Saturated_Sub_Constant>(i_vin, val, o_vout);
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
        POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
        {
            return static_cast<T3>(a0 > a1 ? a0 : a1);
        }
    };

    template< typename T>
    struct op_Sup<T, T, T, tag_SIMD_enabled>
    {
    public:
        op_Sup() {}
        POUTRE_ALWAYS_INLINE T operator()(T const &a0, T const &a1) const POUTRE_NOEXCEPT
        {
            return (a0 > a1 ? a0 : a1);
        }
        template< typename U>
        POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) const POUTRE_NOEXCEPT
        {
#ifdef USE_BOOSTSIMD
           return boost::simd::max(a0, a1);
#else
           return simd::max(a0, a1);
#endif
      
        }
    };

    template<typename T1, typename T2, typename T3, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class View3>
    void t_ArithSup(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, View3<T3, Rank>& o_vout)
    {
        PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3, op_Sup>(i_vin1, i_vin2, o_vout);
    }

    /***********************************************************************************************************************************/
    /*                                                  INFIMUM                                                                        */
    /************************************************************ **********************************************************************/

    template< typename T1, typename T2, typename T3, class tag>
    struct op_Inf;

    template< typename T1, typename T2, typename T3>
    struct op_Inf<T1, T2, T3, tag_SIMD_disabled>
    {
    public:
        op_Inf() {}
        POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
        {
            return static_cast<T3>(a0 < a1 ? a0 : a1);
        }
    };

    template< typename T>
    struct op_Inf<T, T, T, tag_SIMD_enabled>
    {
    public:
        op_Inf() {}

        POUTRE_ALWAYS_INLINE T operator()(T const &a0, T const &a1) const POUTRE_NOEXCEPT
        {
            return (a0 < a1 ? a0 : a1);
        }

        template< typename U>
        POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) const POUTRE_NOEXCEPT
        {
#ifdef USE_BOOSTSIMD
           return boost::simd::min(a0, a1);
#else
           return simd::min(a0, a1);
#endif			
      
        }
    };

    template<typename T1, typename T2, typename T3, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class View3>
    void t_ArithInf(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, View3<T3, Rank>& o_vout)
    {
        PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3, op_Inf>(i_vin1, i_vin2, o_vout);
    }
}//namespace poutre

#endif//POUTRE_IMAGEPROCESSING_ARITH_HPP__