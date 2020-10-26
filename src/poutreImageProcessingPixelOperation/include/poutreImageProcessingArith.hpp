
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_ARITH_HPP__
#define POUTRE_IMAGEPROCESSING_ARITH_HPP__
/**
 * @file poutreImageProcessingArith.hpp
 * @author thomas.retornaz@mines_paris.org
 * @brief Arithmetic operations
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <poutreBase/poutreContainerView.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingBinaryOp.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingUnaryOp.hpp>

namespace poutre
{
  /**
   * @addtogroup image_processing_linear_group Image Processing Linear Operations
   * @ingroup image_processing_group
   *@{
   */

  /***********************************************************************************************************************************/
  /*                                                          NEGATE/INVERT */
  /**********************************************************************************************************************************/
  template<typename T1, typename T2, typename = void> struct op_Invert
  {
    public:
    op_Invert() {}
    POUTRE_ALWAYS_INLINE T2 operator()(T1 const &a0) const POUTRE_NOEXCEPT { return -a0; }
  };

  template<typename T1, typename T2>
  struct op_Invert<T1,
                   T2,
                   std::enable_if_t<std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value
                                    && std::is_arithmetic<T1>::value>>
  {
    public:
    op_Invert() {}

    POUTRE_ALWAYS_INLINE T1 operator()(T1 const &a0) const POUTRE_NOEXCEPT { return -a0; }

    template<typename U> POUTRE_ALWAYS_INLINE U operator()(U const &a0) const POUTRE_NOEXCEPT { return -a0; }
  };

  template<typename T1,
           typename T2,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2>
  void t_ArithNegate(const View1<T1, Rank> &i_vin, View2<T2, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_ArithNegate");
    using myop = op_Invert<T1, T2>;
    myop op;
    PixelWiseUnaryOp<T1, T2, Rank, View1, View2>(i_vin, op, o_vout);
  }

  template<typename T1, typename T2, ptrdiff_t Rank>
  void t_ArithNegate(const DenseImage<T1, Rank> &i_vin, DenseImage<T2, Rank> &o_vout)
  {
    auto viewIn  = lview(const_cast<DenseImage<T1, Rank> &>(i_vin));
    auto viewOut = lview(o_vout);
    return t_ArithNegate(viewIn, viewOut);
  }

  /***********************************************************************************************************************************/
  /*                                                  SATURATED SUB */
  /**********************************************************************************************************************************/
  template<typename T1, typename T2, typename T3, typename = void> struct op_Saturated_Sub
  {
    private:
    T3 m_minval;
    using accutype = typename TypeTraits<T3>::accu_type;

    public:
    op_Saturated_Sub() : m_minval(TypeTraits<T3>::min()) {}
    POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      accutype res = static_cast<accutype>(a0) - static_cast<accutype>(a1);
      if( res < static_cast<accutype>(m_minval) )
        return m_minval;
      return static_cast<T3>(res);
    }
  };

  template<typename T1, typename T2, typename T3>
  struct op_Saturated_Sub<T1,
                          T2,
                          T3,
                          std::enable_if_t<std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value
                                           && std::is_same<std::remove_const_t<T1>, std::remove_const_t<T3>>::value
                                           && std::is_arithmetic<T1>::value>>
  {
    private:
    T1 m_minval;
    using accutype = typename TypeTraits<T1>::accu_type;

    public:
    op_Saturated_Sub() : m_minval(TypeTraits<T1>::min()) {}

    POUTRE_ALWAYS_INLINE T1 operator()(T1 const &a0, T1 const &a1) const POUTRE_NOEXCEPT
    {
      accutype res = static_cast<accutype>(a0) - static_cast<accutype>(a1);
      if( res < static_cast<accutype>(m_minval) )
        return m_minval;
      return static_cast<T1>(res);
    }

    template<typename U> POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) const POUTRE_NOEXCEPT
    {
      return ssub(a0,
                  a1); // FIXME//boost::simd::saturated_(boost::simd::minus)(a0, a1);
    }
  };

  template<typename T1,
           typename T2,
           typename T3,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2,
           template<typename, ptrdiff_t>
           class View3>
  void t_ArithSaturatedSub(const View1<T1, Rank> &i_vin1, const View2<T2, Rank> &i_vin2, View3<T3, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_ArithSaturatedSub");
    using myop = op_Saturated_Sub<T1, T2, T3>;
    myop op;
    PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3>(i_vin1, op, i_vin2, o_vout);
  }

  template<typename T1, typename T2, typename T3, ptrdiff_t Rank>
  void
  t_ArithSaturatedSub(const DenseImage<T1, Rank> &i_vin1, const DenseImage<T2, Rank> &i_vin2, DenseImage<T3, Rank> &o_vout)
  {
    auto viewIn1 = lview(const_cast<DenseImage<T1, Rank> &>(i_vin1));
    auto viewIn2 = lview(const_cast<DenseImage<T1, Rank> &>(i_vin2));
    auto viewOut = lview(o_vout);
    return t_ArithSaturatedSub(viewIn1, viewIn2, viewOut);
  }
  /***********************************************************************************************************************************/
  /*                                                  SATURATED ADD */
  /**********************************************************************************************************************************/
  template<typename T1, typename T2, typename T3, typename = void> struct op_Saturated_Add
  {
    private:
    T3 m_maxval;
    using accutype = typename TypeTraits<T3>::accu_type;

    public:
    op_Saturated_Add() : m_maxval(TypeTraits<T3>::max()) {}
    POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      accutype res = static_cast<accutype>(a0) + static_cast<accutype>(a1);
      if( res > static_cast<accutype>(m_maxval) )
        return m_maxval;
      return static_cast<T3>(res);
    }
  };

  template<typename T1, typename T2, typename T3>
  struct op_Saturated_Add<T1,
                          T2,
                          T3,
                          std::enable_if_t<std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value
                                           && std::is_same<std::remove_const_t<T1>, std::remove_const_t<T3>>::value
                                           && std::is_arithmetic<T1>::value>>
  {
    private:
    T1 m_maxval;
    using accutype = typename TypeTraits<T1>::accu_type;

    public:
    op_Saturated_Add() : m_maxval(TypeTraits<T1>::max()) {}
    POUTRE_ALWAYS_INLINE T1 operator()(T1 const &a0, T1 const &a1) const POUTRE_NOEXCEPT
    {
      accutype res = static_cast<accutype>(a0) + static_cast<accutype>(a1);
      if( res > static_cast<accutype>(m_maxval) )
        return m_maxval;
      return static_cast<T1>(res);
    }
    template<typename U> POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) const POUTRE_NOEXCEPT
    {
      return sadd(a0, a1);
    }
  };

  template<typename T1,
           typename T2,
           typename T3,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2,
           template<typename, ptrdiff_t>
           class View3>
  void t_ArithSaturatedAdd(const View1<T1, Rank> &i_vin1, const View2<T2, Rank> &i_vin2, View3<T3, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_ArithSaturatedAdd");
    using myop = op_Saturated_Add<T1, T2, T3>;
    myop op;
    PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3>(i_vin1, op, i_vin2, o_vout);
  }

  template<typename T1, typename T2, typename T3, ptrdiff_t Rank>
  void
  t_ArithSaturatedAdd(const DenseImage<T1, Rank> &i_vin1, const DenseImage<T2, Rank> &i_vin2, DenseImage<T3, Rank> &o_vout)
  {
    auto viewIn1 = lview(const_cast<DenseImage<T1, Rank> &>(i_vin1));
    auto viewIn2 = lview(const_cast<DenseImage<T1, Rank> &>(i_vin2));
    auto viewOut = lview(o_vout);
    return t_ArithSaturatedAdd(viewIn1, viewIn2, viewOut);
  }

  /***********************************************************************************************************************************/
  /*                                                  SATURATED ADD CONSTANT */
  /**********************************************************************************************************************************/

  template<typename T1, typename T2, typename = void> struct op_Saturated_Add_Constant
  {
    private:
    T2 m_val, m_maxval;
    using accutype = typename TypeTraits<T2>::accu_type;

    public:
    op_Saturated_Add_Constant(T2 val) : m_val(val), m_maxval(TypeTraits<T2>::max()) {}
    POUTRE_ALWAYS_INLINE T2 operator()(T1 const &a0) const POUTRE_NOEXCEPT
    {
      accutype res = static_cast<accutype>(m_val) + static_cast<accutype>(a0);
      if( res > static_cast<accutype>(m_maxval) )
        return m_maxval;
      return static_cast<T2>(res);
    }
  };

  // todo benchmark, if slow specialize boost::simd::transform to load m_val as a
  // pack
  template<typename T1, typename T2>
  struct op_Saturated_Add_Constant<T1,
                                   T2,
                                   std::enable_if_t<std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value
                                                    && std::is_arithmetic<T1>::value>>
  {
    private:
    const T1                                 m_val, m_maxval;
    const typename TypeTraits<T1>::simd_type m_simd_val;
    using accutype = typename TypeTraits<T1>::accu_type;

    public:
    op_Saturated_Add_Constant(T1 val) : m_val(val), m_maxval(TypeTraits<T1>::max()), m_simd_val(val) {}
    POUTRE_ALWAYS_INLINE T1 operator()(T1 const &a0) const POUTRE_NOEXCEPT
    {
      accutype res = static_cast<accutype>(m_val) + static_cast<accutype>(a0);
      if( res > static_cast<accutype>(m_maxval) )
        return m_maxval;
      return static_cast<T1>(res);
    }
    template<typename U> POUTRE_ALWAYS_INLINE U operator()(U const &a0) const POUTRE_NOEXCEPT
    {
      return sadd(a0, m_simd_val);
    }
  };

  template<typename T1,
           typename T2,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2>
  //@warning saturation is related to *View2 type*
  void t_ArithSaturatedAddConstant(const View1<T1, Rank> &i_vin, T2 val, View2<T2, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_ArithSaturatedAddConstant");
    using myop = op_Saturated_Add_Constant<T1, T2>;
    myop op(val);
    PixelWiseUnaryOp(i_vin, op, o_vout);
  }

  template<typename T1, typename T2, ptrdiff_t Rank>
  void t_ArithSaturatedAddConstant(const DenseImage<T1, Rank> &i_vin, T2 val, DenseImage<T2, Rank> &o_vout)
  {
    auto viewIn  = lview(const_cast<DenseImage<T1, Rank> &>(i_vin));
    auto viewOut = lview(o_vout);
    return t_ArithSaturatedAddConstant(viewIn, val, viewOut);
  }

  /***********************************************************************************************************************************/
  /*                                                  SATURATED SUB CONSTANT */
  /**********************************************************************************************************************************/
  template<typename T1, typename T2, typename = void> struct op_Saturated_Sub_Constant
  {
    private:
    T2 m_val, m_minval;
    using accutype = typename TypeTraits<T2>::accu_type;

    public:
    op_Saturated_Sub_Constant(T2 val) : m_val(val), m_minval(TypeTraits<T2>::min()) {}
    POUTRE_ALWAYS_INLINE T2 operator()(T1 const &a0) const POUTRE_NOEXCEPT
    {
      accutype res = static_cast<accutype>(a0) - static_cast<accutype>(m_val);
      if( res < static_cast<accutype>(m_minval) )
        return m_minval;
      return static_cast<T2>(res);
    }
  };

  // todo benchmark, if slow specialize boost::simd::transform to load m_val as a
  // pack
  template<typename T1, typename T2>
  struct op_Saturated_Sub_Constant<T1,
                                   T2,
                                   std::enable_if_t<std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value
                                                    && std::is_arithmetic<T1>::value>>
  {
    private:
    const T1                                 m_val, m_minval;
    const typename TypeTraits<T1>::simd_type m_simd_val;
    using accutype = typename TypeTraits<T1>::accu_type;

    public:
    op_Saturated_Sub_Constant(T1 val) : m_val(val), m_minval(TypeTraits<T1>::min()), m_simd_val(val) {}

    POUTRE_ALWAYS_INLINE T1 operator()(T1 const &a0) const POUTRE_NOEXCEPT
    {
      accutype res = static_cast<accutype>(a0) - static_cast<accutype>(m_val);
      if( res < static_cast<accutype>(m_minval) )
        return m_minval;
      return static_cast<T1>(res);
    }

    template<typename U> POUTRE_ALWAYS_INLINE U operator()(U const &a0) const POUTRE_NOEXCEPT
    {
      return ssub(a0, m_simd_val);
    }
  };

  template<typename T1,
           typename T2,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2>
  //@warning saturation is related to *View2 type*
  void t_ArithSaturatedSubConstant(const View1<T1, Rank> &i_vin, T2 val, View2<T2, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_ArithSaturatedSubConstant");
    using myop = op_Saturated_Sub_Constant<T1, T2>;
    myop op(val);
    PixelWiseUnaryOp(i_vin, op, o_vout);
  }

  template<typename T1, typename T2, ptrdiff_t Rank>
  void t_ArithSaturatedSubConstant(const DenseImage<T1, Rank> &i_vin, T2 val, DenseImage<T2, Rank> &o_vout)
  {
    auto viewIn  = lview(const_cast<DenseImage<T1, Rank> &>(i_vin));
    auto viewOut = lview(o_vout);
    return t_ArithSaturatedSubConstant(viewIn, val, viewOut);
  }

  /***********************************************************************************************************************************/
  /*                                                  SUPREMUM */
  /**********************************************************************************************************************************/
  template<typename T1, typename T2, typename T3, typename = void> struct op_Sup
  {
    public:
    op_Sup() {}
    POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      return static_cast<T3>(a0 > a1 ? a0 : a1);
    }
  };

  template<typename T1, typename T2, typename T3>
  struct op_Sup<T1,
                T2,
                T3,
                std::enable_if_t<std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value
                                 && std::is_same<std::remove_const_t<T1>, std::remove_const_t<T3>>::value
                                 && std::is_arithmetic<T1>::value>>
  {
    public:
    op_Sup() {}
    POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT { return (a0 > a1 ? a0 : a1); }
    template<typename U> POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) const POUTRE_NOEXCEPT
    {
      return xs::max(a0, a1);
    }
  };

  template<typename T1,
           typename T2,
           typename T3,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2,
           template<typename, ptrdiff_t>
           class View3>
  void t_ArithSup(const View1<T1, Rank> &i_vin1, const View2<T2, Rank> &i_vin2, View3<T3, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_ArithSup");
    using myop = op_Sup<T1, T2, T3>;
    myop op;
    PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3>(i_vin1, op, i_vin2, o_vout);
  }

  template<typename T1, typename T2, typename T3, ptrdiff_t Rank>
  void t_ArithSup(const DenseImage<T1, Rank> &i_vin1, const DenseImage<T2, Rank> &i_vin2, DenseImage<T3, Rank> &o_vout)
  {
    auto viewIn1 = lview(const_cast<DenseImage<T1, Rank> &>(i_vin1));
    auto viewIn2 = lview(const_cast<DenseImage<T2, Rank> &>(i_vin2));
    auto viewOut = lview(o_vout);
    return t_ArithSup(viewIn1, viewIn2, viewOut);
  }

  /***********************************************************************************************************************************/
  /*                                                  INFIMUM */
  /************************************************************
   * **********************************************************************/
  template<typename T1, typename T2, typename T3, typename = void> struct op_Inf
  {
    public:
    op_Inf() {}
    POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      return static_cast<T3>(a0 < a1 ? a0 : a1);
    }
  };

  template<typename T1, typename T2, typename T3>
  struct op_Inf<T1,
                T2,
                T3,
                std::enable_if_t<std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value
                                 && std::is_same<std::remove_const_t<T1>, std::remove_const_t<T3>>::value
                                 && std::is_arithmetic<T1>::value>>
  {
    public:
    op_Inf() {}

    POUTRE_ALWAYS_INLINE T3 operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT { return (a0 < a1 ? a0 : a1); }

    template<typename U> POUTRE_ALWAYS_INLINE U operator()(U const &a0, U const &a1) const POUTRE_NOEXCEPT
    {
      return xs::min(a0, a1);
    }
  };

  template<typename T1,
           typename T2,
           typename T3,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2,
           template<typename, ptrdiff_t>
           class View3>
  void t_ArithInf(const View1<T1, Rank> &i_vin1, const View2<T2, Rank> &i_vin2, View3<T3, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_ArithInf");
    using myop = op_Inf<T1, T2, T3>;
    myop op;
    PixelWiseBinaryOp<T1, T2, T3, Rank, View1, View2, View3>(i_vin1, op, i_vin2, o_vout);
  }

  template<typename T1, typename T2, typename T3, ptrdiff_t Rank>
  void t_ArithInf(const DenseImage<T1, Rank> &i_vin1, const DenseImage<T2, Rank> &i_vin2, DenseImage<T3, Rank> &o_vout)
  {
    auto viewIn1 = lview(const_cast<DenseImage<T1, Rank> &>(i_vin1));
    auto viewIn2 = lview(const_cast<DenseImage<T2, Rank> &>(i_vin2));
    auto viewOut = lview(o_vout);
    return t_ArithInf(viewIn1, viewIn2, viewOut);
  }
  //! @} doxygroup: image_processing_linear_group
} // namespace poutre

#endif // POUTRE_IMAGEPROCESSING_ARITH_HPP__
