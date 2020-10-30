//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_COMPARE_HPP__
#define POUTRE_IMAGEPROCESSING_COMPARE_HPP__

/**
 * @file poutreImageProcessingCompare.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Comparaisons op over images and views
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <poutreBase/include/poutreSimdAlgorithm.hpp> //simd transform
#include <poutreBase/poutreContainerView.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingBinaryOp.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingQuaternaryOp.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingTernaryOp.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingUnaryOp.hpp>

namespace poutre
{
  /**
   * @addtogroup image_processing_linear_group
   *@{
   */

  // redef == != < > <= >= to enable simd when available
  template<typename T1, typename T2, typename = void> struct OpCompEqual
  {
    public:
    OpCompEqual() {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      return static_cast<bool>(a0 == a1);
    }
  };
  template<typename T> struct OpCompEqual<T, T, std::enable_if_t<std::is_arithmetic_v<T>>>
  {
    using simd_t      = typename TypeTraits<T>::simd_type;
    using simd_mask_t = typename TypeTraits<T>::simd_mask_type;
    OpCompEqual() {}
    POUTRE_ALWAYS_INLINE bool operator()(T a0, T a1) const POUTRE_NOEXCEPT { return static_cast<bool>(a0 == a1); }
    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs, const U &rhs) const POUTRE_NOEXCEPT
    {
      return lhs == rhs;
    }
  };

  template<typename T1, typename T2, typename = void> struct OpCompDiff
  {
    public:
    OpCompDiff() {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      return static_cast<bool>(a0 != a1);
    }
  };
  template<typename T> struct OpCompDiff<T, T, std::enable_if_t<std::is_arithmetic_v<T>>>
  {
    using simd_t      = typename TypeTraits<T>::simd_type;
    using simd_mask_t = typename TypeTraits<T>::simd_mask_type;
    OpCompDiff() {}
    POUTRE_ALWAYS_INLINE bool operator()(T a0, T a1) const POUTRE_NOEXCEPT { return static_cast<bool>(a0 != a1); }
    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs, const U &rhs) const POUTRE_NOEXCEPT
    {
      return lhs != rhs;
    }
  };

  template<typename T1, typename T2, typename = void> struct OpCompSup
  {
    public:
    OpCompSup() {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      return static_cast<bool>(a0 > a1);
    }
  };
  template<typename T> struct OpCompSup<T, T, std::enable_if_t<std::is_arithmetic_v<T>>>
  {
    using simd_t      = typename TypeTraits<T>::simd_type;
    using simd_mask_t = typename TypeTraits<T>::simd_mask_type;
    OpCompSup() {}
    POUTRE_ALWAYS_INLINE bool operator()(T a0, T a1) const POUTRE_NOEXCEPT { return static_cast<bool>(a0 > a1); }
    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs, const U &rhs) const POUTRE_NOEXCEPT
    {
      return lhs > rhs;
    }
  };

  template<typename T1, typename T2, typename = void> struct OpCompInf
  {
    public:
    OpCompInf() {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      return static_cast<bool>(a0 < a1);
    }
  };
  template<typename T> struct OpCompInf<T, T, std::enable_if_t<std::is_arithmetic_v<T>>>
  {
    using simd_t      = typename TypeTraits<T>::simd_type;
    using simd_mask_t = typename TypeTraits<T>::simd_mask_type;
    OpCompInf() {}
    POUTRE_ALWAYS_INLINE bool operator()(T a0, T a1) const POUTRE_NOEXCEPT { return static_cast<bool>(a0 < a1); }
    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs, const U &rhs) const POUTRE_NOEXCEPT
    {
      return lhs < rhs;
    }
  };

  template<typename T1, typename T2, typename = void> struct OpCompSupEqual
  {
    public:
    OpCompSupEqual() {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      return static_cast<bool>(a0 >= a1);
    }
  };
  template<typename T> struct OpCompSupEqual<T, T, std::enable_if_t<std::is_arithmetic_v<T>>>
  {
    using simd_t      = typename TypeTraits<T>::simd_type;
    using simd_mask_t = typename TypeTraits<T>::simd_mask_type;
    OpCompSupEqual() {}
    POUTRE_ALWAYS_INLINE bool operator()(T a0, T a1) const POUTRE_NOEXCEPT { return static_cast<bool>(a0 >= a1); }
    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs, const U &rhs) const POUTRE_NOEXCEPT
    {
      return lhs >= rhs;
    }
  };

  template<typename T1, typename T2, typename = void> struct OpCompInfEqual
  {
    public:
    OpCompInfEqual() {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 const &a0, T2 const &a1) const POUTRE_NOEXCEPT
    {
      return static_cast<bool>(a0 <= a1);
    }
  };
  template<typename T> struct OpCompInfEqual<T, T, std::enable_if_t<std::is_arithmetic_v<T>>>
  {
    using simd_t      = typename TypeTraits<T>::simd_type;
    using simd_mask_t = typename TypeTraits<T>::simd_mask_type;
    OpCompInfEqual() {}
    POUTRE_ALWAYS_INLINE bool operator()(T a0, T a1) const POUTRE_NOEXCEPT { return static_cast<bool>(a0 <= a1); }
    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs, const U &rhs) const POUTRE_NOEXCEPT
    {
      return lhs <= rhs;
    }
  };

  // redef == != < > <= >= with value to enable simd when available
  template<typename T1, typename = void> struct OpCompEqualValue
  {
    T1 m_val;
    explicit OpCompEqualValue(T1 ival) : m_val(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs == m_val; }
  };

  template<typename T1> struct OpCompEqualValue<T1, std::enable_if_t<std::is_arithmetic_v<T1>>>
  {
    T1 m_val;
    using simd_t      = typename TypeTraits<T1>::simd_type;
    using simd_mask_t = typename TypeTraits<T1>::simd_mask_type;
    simd_t m_val_pack;
    explicit OpCompEqualValue(T1 ival) : m_val(ival), m_val_pack(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs == m_val; }

    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs) const POUTRE_NOEXCEPT
    {
      return lhs == m_val_pack;
    }
  };

  //! Operator diff
  template<typename T1, typename = void> struct OpCompDiffValue
  {
    T1 m_val;
    explicit OpCompDiffValue(T1 ival) : m_val(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs != m_val; }
  };

  template<typename T1> struct OpCompDiffValue<T1, std::enable_if_t<std::is_arithmetic_v<T1>>>
  {
    T1 m_val;
    using simd_t      = typename TypeTraits<T1>::simd_type;
    using simd_mask_t = typename TypeTraits<T1>::simd_mask_type;
    simd_t m_val_pack;
    explicit OpCompDiffValue(T1 ival) : m_val(ival), m_val_pack(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs != m_val; }

    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs) const POUTRE_NOEXCEPT
    {
      return lhs != m_val_pack;
    }
  };

  //! Operator Sup
  template<typename T1, typename = void> struct OpCompSupValue
  {
    T1 m_val;
    explicit OpCompSupValue(T1 ival) : m_val(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs > m_val; }
  };

  template<typename T1> struct OpCompSupValue<T1, std::enable_if_t<std::is_arithmetic_v<T1>>>
  {
    T1 m_val;
    using simd_t      = typename TypeTraits<T1>::simd_type;
    using simd_mask_t = typename TypeTraits<T1>::simd_mask_type;
    simd_t m_val_pack;
    explicit OpCompSupValue(T1 ival) : m_val(ival), m_val_pack(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs > m_val; }

    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs) const POUTRE_NOEXCEPT
    {
      return lhs > m_val_pack;
    }
  };

  //! Operator SupEqual
  template<typename T1, typename = void> struct OpCompSupEqualValue
  {
    T1 m_val;
    explicit OpCompSupEqualValue(T1 ival) : m_val(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs >= m_val; }
  };

  template<typename T1> struct OpCompSupEqualValue<T1, std::enable_if_t<std::is_arithmetic_v<T1>>>
  {
    T1 m_val;
    using simd_t      = typename TypeTraits<T1>::simd_type;
    using simd_mask_t = typename TypeTraits<T1>::simd_mask_type;
    simd_t m_val_pack;
    explicit OpCompSupEqualValue(T1 ival) : m_val(ival), m_val_pack(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs >= m_val; }

    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs) const POUTRE_NOEXCEPT
    {
      return lhs >= m_val_pack;
    }
  };

  //! Operator Inf
  template<typename T1, typename = void> struct OpCompInfValue
  {
    T1 m_val;
    explicit OpCompInfValue(T1 ival) : m_val(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs < m_val; }
  };

  template<typename T1> struct OpCompInfValue<T1, std::enable_if_t<std::is_arithmetic_v<T1>>>
  {
    T1 m_val;
    using simd_t      = typename TypeTraits<T1>::simd_type;
    using simd_mask_t = typename TypeTraits<T1>::simd_mask_type;
    simd_t m_val_pack;
    explicit OpCompInfValue(T1 ival) : m_val(ival), m_val_pack(ival) {}
    POUTRE_ALWAYS_INLINE bool                             operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs < m_val; }
    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs) const POUTRE_NOEXCEPT
    {
      return lhs < m_val_pack;
    }
  };

  //! Operator InfEqual
  template<typename T1, typename = void> struct OpCompInfEqualValue
  {
    T1 m_val;
    explicit OpCompInfEqualValue(T1 ival) : m_val(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs <= m_val; }
  };

  template<typename T1> struct OpCompInfEqualValue<T1, std::enable_if_t<std::is_arithmetic_v<T1>>>
  {
    T1 m_val;
    using simd_t      = typename TypeTraits<T1>::simd_type;
    using simd_mask_t = typename TypeTraits<T1>::simd_mask_type;
    simd_t m_val_pack;
    explicit OpCompInfEqualValue(T1 ival) : m_val(ival), m_val_pack(ival) {}
    POUTRE_ALWAYS_INLINE bool operator()(T1 lhs) const POUTRE_NOEXCEPT { return lhs <= m_val; }

    template<typename U> POUTRE_ALWAYS_INLINE simd_mask_t operator()(const U &lhs) const POUTRE_NOEXCEPT
    {
      return lhs <= m_val_pack;
    }
  };

  /*************************************************************************************************************************************/
  /*                                                      SSS */
  /*************************************************************************************************************************************/
  template<typename Tin, typename Tout, class CompareOp, typename = void> struct compare_sss
  {
    const CompareOp cop;
    const Tout      m_valtrue, m_valfalse;
    compare_sss(const Tout &i_valtrue, const Tout &i_valfalse) : cop(), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}
    compare_sss(const Tout &i_valtrue, const Tout &i_valfalse, const CompareOp &op)
        : cop(op)
        , m_valtrue(i_valtrue)
        , m_valfalse(i_valfalse)
    {
    }
    POUTRE_ALWAYS_INLINE Tout operator()(Tin i_val) const POUTRE_NOEXCEPT { return (cop(i_val) ? m_valtrue : m_valfalse); }
  };

  template<typename Tin, typename Tout, class CompareOp>
  struct compare_sss<
      Tin,
      Tout,
      CompareOp,
      std::enable_if_t<std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Tout>> && std::is_arithmetic_v<Tin>>>
  {
    const CompareOp cop;
    using simd_t      = typename TypeTraits<Tin>::simd_type;
    using simd_mask_t = typename TypeTraits<Tout>::simd_mask_type;
    simd_t m_valtrue_pack, m_valfalse_pack;
    Tout   m_valtrue, m_valfalse;
    compare_sss(const Tout &i_valtrue, const Tout &i_valfalse)
        : cop()
        , m_valtrue_pack(i_valtrue)
        , m_valfalse_pack(i_valfalse)
        , m_valtrue(i_valtrue)
        , m_valfalse(i_valfalse)
    {
    }
    compare_sss(const Tout &i_valtrue, const Tout &i_valfalse, const CompareOp &op)
        : cop(op)
        , m_valtrue_pack(i_valtrue)
        , m_valfalse_pack(i_valfalse)
        , m_valtrue(i_valtrue)
        , m_valfalse(i_valfalse)
    {
    }

    POUTRE_ALWAYS_INLINE Tout operator()(Tin i_val) const POUTRE_NOEXCEPT { return (cop(i_val) ? m_valtrue : m_valfalse); }

    template<typename U> U operator()(U i_val) const POUTRE_NOEXCEPT // TODO inline ?
    {
      return xs::select(cop(i_val), m_valtrue_pack, m_valfalse_pack);
    }
  };

  template<typename Tin,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           class Op,
           template<typename, ptrdiff_t>
           class ViewOut>
  void
  t_ViewCompare_sss(const ViewIn<Tin, Rank> &i_vin, Op i_op, Tout i_valtrue, Tout i_valfalse, ViewOut<Tout, Rank> &o_vout)
  {
    using myop = compare_sss<Tin, Tout, Op>;
    myop op(i_valtrue, i_valfalse, i_op);
    PixelWiseUnaryOp(i_vin, op, o_vout);
  }

  template<typename Tin,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_sss_dispatch(const ViewIn<Tin, Rank> &i_vin,
                                CompOpType               compOpType,
                                Tin                      i_compval,
                                Tout                     i_valtrue,
                                Tout                     i_valfalse,
                                ViewOut<Tout, Rank> &    o_vout)
  {
    switch( compOpType )
    {
    case CompOpType::CompOpEqual:
    {
      OpCompEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpDiff:
    {
      OpCompDiffValue<Tin> myop(i_compval);
      return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSup:
    {
      OpCompSupValue<Tin> myop(i_compval);
      return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSupEqual:
    {
      OpCompSupEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInf:
    {
      OpCompInfValue<Tin> myop(i_compval);
      return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInfEqual:
    {
      OpCompInfEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("ViewCompare_sss_dispatch CompOpType not implemented");
    }
    }
  }

  template<typename Tin,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_sss_func_helper(const ViewIn<Tin, Rank> &i_vin,
                                   CompOpType               compOpType,
                                   Tin                      i_compval,
                                   Tout                     i_valtrue,
                                   Tout                     i_valfalse,
                                   ViewOut<Tout, Rank> &    o_vout)
  {
    return ViewCompare_sss_dispatch(i_vin, compOpType, i_compval, i_valtrue, i_valfalse, o_vout);
  }

  /*************************************************************************************************************************************/
  /*                                                      III */
  /*************************************************************************************************************************************/
  template<typename Tin, typename Tcomp, typename Ttrue, typename Tfalse, typename Tout, class CompareOp, typename = void>
  struct compare_iii
  {
    const CompareOp cop;
    using param_typein    = Tin;
    using param_typecomp  = Tcomp;
    using param_typetrue  = Ttrue;
    using param_typefalse = Tfalse;
    compare_iii() : cop() {}
    explicit compare_iii(const CompareOp &op) : cop(op) {}
    POUTRE_ALWAYS_INLINE Tout operator()(param_typein   i_val,
                                         param_typecomp i_comp,
                                         param_typetrue i_true,
                                         param_typetrue i_false) const POUTRE_NOEXCEPT // TODO inline ?
    {
      return (cop(i_val, i_comp) ? i_true : i_false);
    }
  };

  template<typename Tin, typename Tcomp, typename Ttrue, typename Tfalse, typename Tout, class CompareOp>
  struct compare_iii<
      Tin,
      Tcomp,
      Ttrue,
      Tfalse,
      Tout,
      CompareOp,
      std::enable_if_t<
          std::is_same_v<
              std::remove_const_t<Tin>,
              std::remove_const_t<
                  Tout>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Tfalse>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Ttrue>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Tcomp>> && std::is_arithmetic_v<Tin>>>

  {
    const CompareOp cop;
    using param_typein    = Tin;
    using param_typecomp  = Tcomp;
    using param_typetrue  = Ttrue;
    using param_typefalse = Tfalse;
    compare_iii() : cop() {}
    explicit compare_iii(const CompareOp &op) : cop(op) {}
    POUTRE_ALWAYS_INLINE Tout operator()(param_typein   i_val,
                                         param_typecomp i_comp,
                                         param_typetrue i_true,
                                         param_typetrue i_false) const POUTRE_NOEXCEPT // TODO inline ?
    {
      return (cop(i_val, i_comp) ? i_true : i_false);
    }
    template<typename U> U operator()(U i_val, U i_comp, U i_true, U i_false) const POUTRE_NOEXCEPT
    {
      return xs::select(cop(i_val, i_comp), i_true, i_false);
    }
  };

  template<typename Tin,
           typename Tcomp,
           typename Ttrue,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewFalse,
           class Op,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_ViewCompare_iii(const ViewIn<Tin, Rank> &      i_vin,
                         Op                             i_op,
                         const ViewComp<Tcomp, Rank> &  i_vcomp,
                         const ViewTrue<Ttrue, Rank> &  i_vtrue,
                         const ViewFalse<Tfalse, Rank> &i_vfalse,
                         ViewOut<Tout, Rank> &          o_vout)
  {
    using myop = compare_iii<Tin, Tcomp, Ttrue, Tfalse, Tout, Op>;
    myop op(i_op);
    PixelWiseQuaternaryOp(i_vin, op, i_vcomp, i_vtrue, i_vfalse, o_vout);
  }

  template<typename Tin,
           typename Tcomp,
           typename Ttrue,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewFalse,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_iii_dispatch(const ViewIn<Tin, Rank> &      i_vin,
                                CompOpType                     compOpType,
                                const ViewComp<Tcomp, Rank> &  i_vcomp,
                                const ViewTrue<Ttrue, Rank> &  i_vtrue,
                                const ViewFalse<Tfalse, Rank> &i_vfalse,
                                ViewOut<Tout, Rank> &          o_vout)
  {
    static_assert(std::is_convertible<Ttrue, Tout>(),
                  "ViewCompare_iii_dispatch Ttrue and Tout must be convertible "
                  "to each other");
    static_assert(std::is_convertible<Tfalse, Tout>(),
                  "ViewCompare_iii_dispatch Tfalse and Tout must be convertible "
                  "to each other");
    static_assert(std::is_convertible<Tcomp, Tin>(),
                  "ViewCompare_iii_dispatch Tcomp and Tin must be convertible to "
                  "each other");
    switch( compOpType )
    {
    case CompOpType::CompOpEqual:
    {
      return t_ViewCompare_iii(i_vin, OpCompEqual<Tin, Tout>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpDiff:
    {
      return t_ViewCompare_iii(i_vin, OpCompDiff<Tin, Tout>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSup:
    {
      return t_ViewCompare_iii(i_vin, OpCompSup<Tin, Tout>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSupEqual:
    {
      return t_ViewCompare_iii(i_vin, OpCompSupEqual<Tin, Tout>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInf:
    {
      return t_ViewCompare_iii(i_vin, OpCompInf<Tin, Tout>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInfEqual:
    {
      return t_ViewCompare_iii(i_vin, OpCompInfEqual<Tin, Tout>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("ViewCompare_iii_dispatch CompOpType not implemented");
    }
    }
  }

  template<typename Tin,
           typename Tcomp,
           typename Ttrue,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewFalse,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_iii_func_helper(const ViewIn<Tin, Rank> &      i_vin,
                                   CompOpType                     compOpType,
                                   const ViewComp<Tcomp, Rank> &  i_vcomp,
                                   const ViewTrue<Ttrue, Rank> &  i_vtrue,
                                   const ViewFalse<Tfalse, Rank> &i_vfalse,
                                   ViewOut<Tout, Rank> &          o_vout)
  {
    return ViewCompare_iii_dispatch(i_vin, compOpType, i_vcomp, i_vtrue, i_vfalse, o_vout);
  }
  /*************************************************************************************************************************************/
  /*                                                      SII */
  /*************************************************************************************************************************************/
  template<typename Tin, typename Ttrue, typename Tfalse, typename Tout, class CompareOp, typename = void> struct compare_sii
  {
    const CompareOp cop;
    const Tin       m_compval;

    using param_typein    = Tin;
    using param_typetrue  = Ttrue;
    using param_typefalse = Tfalse;
    explicit compare_sii(const Tin &i_compval) : cop(), m_compval(i_compval) {}
    explicit compare_sii(const Tin &i_compval, const CompareOp &op) : cop(op), m_compval(i_compval) {}
    POUTRE_ALWAYS_INLINE Tout operator()(param_typein    i_val,
                                         param_typetrue  i_true,
                                         param_typefalse i_false) const POUTRE_NOEXCEPT
    {
      return (cop(i_val, m_compval) ? i_true : i_false);
    }
  };

  template<typename Tin, typename Ttrue, typename Tfalse, typename Tout, class CompareOp>
  struct compare_sii<
      Tin,
      Ttrue,
      Tfalse,
      Tout,
      CompareOp,
      std::enable_if_t<
          std::is_same_v<
              std::remove_const_t<Tin>,
              std::remove_const_t<
                  Tout>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Tfalse>> && std::is_arithmetic_v<Tin>>>
  {
    const CompareOp cop;
    using simd_t      = typename TypeTraits<Tin>::simd_type;
    using simd_mask_t = typename TypeTraits<Tout>::simd_mask_type;

    using param_typein    = Tin;
    using param_typetrue  = Ttrue;
    using param_typefalse = Tfalse;
    const param_typein m_compval;
    const simd_t       m_compval_pack;

    explicit compare_sii(const Tin &i_compval) : cop(), m_compval(i_compval), m_compval_pack(i_compval) {}
    explicit compare_sii(const Tin &i_compval, const CompareOp &op)
        : cop(op)
        , m_compval(i_compval)
        , m_compval_pack(i_compval)
    {
    }
    POUTRE_ALWAYS_INLINE Tout operator()(param_typein    i_val,
                                         param_typetrue  i_true,
                                         param_typefalse i_false) const POUTRE_NOEXCEPT
    {
      return (cop(i_val, m_compval) ? i_true : i_false);
    }
    template<typename U> U operator()(U i_val, U i_true, U i_false) const POUTRE_NOEXCEPT
    {
      return xs::select(cop(i_val, m_compval_pack), i_true, i_false);
    }
  };

  template<typename Tin,
           typename Ttrue,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewFalse,
           class Op,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_ViewCompare_sii(const ViewIn<Tin, Rank> &      i_vin,
                         Op                             i_op,
                         Tin                            i_compval,
                         const ViewTrue<Ttrue, Rank> &  i_vtrue,
                         const ViewFalse<Tfalse, Rank> &i_vfalse,
                         ViewOut<Tout, Rank> &          o_vout)
  {
    using myop = compare_sii<Tin, Ttrue, Tfalse, Tout, Op>;
    myop op(i_compval, i_op);
    PixelWiseTernaryOp(i_vin, op, i_vtrue, i_vfalse, o_vout);
  }

  template<typename Tin,
           typename Ttrue,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewFalse,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_sii_dispatch(const ViewIn<Tin, Rank> &      i_vin,
                                CompOpType                     compOpType,
                                Tin                            i_compval,
                                const ViewTrue<Ttrue, Rank> &  i_vtrue,
                                const ViewFalse<Tfalse, Rank> &i_vfalse,
                                ViewOut<Tout, Rank> &          o_vout)
  {
    static_assert(std::is_convertible<Ttrue, Tout>(),
                  "ViewCompare_sii_dispatch Ttrue and Tout must be convertible "
                  "to each other");
    static_assert(std::is_convertible<Tfalse, Tout>(),
                  "ViewCompare_sii_dispatch Tfalse and Tout must be convertible "
                  "to each other");
    switch( compOpType )
    {
    case CompOpType::CompOpEqual:
    {
      return t_ViewCompare_sii(i_vin, OpCompEqual<Tin, Tout>(), i_compval, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpDiff:
    {
      return t_ViewCompare_sii(i_vin, OpCompDiff<Tin, Tout>(), i_compval, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSup:
    {
      return t_ViewCompare_sii(i_vin, OpCompSup<Tin, Tout>(), i_compval, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSupEqual:
    {
      return t_ViewCompare_sii(i_vin, OpCompSupEqual<Tin, Tout>(), i_compval, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInf:
    {
      return t_ViewCompare_sii(i_vin, OpCompInf<Tin, Tout>(), i_compval, i_vtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInfEqual:
    {
      return t_ViewCompare_sii(i_vin, OpCompInfEqual<Tin, Tout>(), i_compval, i_vtrue, i_vfalse, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("ViewCompare_sii_dispatch CompOpType not implemented");
    }
    }
  }

  template<typename Tin,
           typename Ttrue,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewFalse,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_sii_func_helper(const ViewIn<Tin, Rank> &      i_vin,
                                   CompOpType                     compOpType,
                                   Tin                            i_compval,
                                   const ViewTrue<Ttrue, Rank> &  i_vtrue,
                                   const ViewFalse<Tfalse, Rank> &i_vfalse,
                                   ViewOut<Tout, Rank> &          o_vout)
  {
    return ViewCompare_sii_dispatch(i_vin, compOpType, i_compval, i_vtrue, i_vfalse, o_vout);
  }

  /*************************************************************************************************************************************/
  /*                                                      SIS */
  /*************************************************************************************************************************************/
  template<typename Tin, typename Ttrue, typename Tout, class CompareOp, typename = void> struct compare_sis
  {
    const CompareOp cop;
    const Tin       m_compval;
    const Tout      m_valfalse;

    compare_sis(const Tout &i_valfalse) : cop(), m_valfalse(i_valfalse) {}
    compare_sis(const Tout &i_valfalse, const CompareOp &op) : cop(op), m_valfalse(i_valfalse) {}
    POUTRE_ALWAYS_INLINE Tout operator()(Tin i_val, Ttrue i_true) const POUTRE_NOEXCEPT
    {
      return (cop(i_val) ? i_true : m_valfalse);
    }
  };

  template<typename Tin, typename Ttrue, typename Tout, class CompareOp>
  struct compare_sis<
      Tin,
      Ttrue,
      Tout,
      CompareOp,
      std::enable_if_t<
          std::is_same_v<
              std::remove_const_t<Tin>,
              std::remove_const_t<
                  Tout>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Ttrue>> && std::is_arithmetic_v<Tin>>>
  {
    const CompareOp cop;

    using simd_t      = typename TypeTraits<Tin>::simd_type;
    using simd_mask_t = typename TypeTraits<Tout>::simd_mask_type;

    const Tout m_valfalse;
    simd_t     m_valfalse_pack;

    compare_sis(const Tout &i_valfalse) : cop(), m_valfalse(i_valfalse), m_valfalse_pack(i_valfalse) {}
    compare_sis(const Tout &i_valfalse, const CompareOp &op) : cop(op), m_valfalse(i_valfalse), m_valfalse_pack(i_valfalse)
    {
    }
    POUTRE_ALWAYS_INLINE Tout operator()(Tin i_val, Ttrue i_true) const POUTRE_NOEXCEPT
    {
      return (cop(i_val) ? i_true : m_valfalse);
    }

    template<typename U> U operator()(U i_val, U i_true) const POUTRE_NOEXCEPT
    {
      return xs::select(cop(i_val), i_true, m_valfalse_pack);
    }
  };

  template<typename Tin,
           typename Ttrue,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewTrue,
           class Op,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_ViewCompare_sis(const ViewIn<Tin, Rank> &    i_vin,
                         Op                           i_op,
                         const ViewTrue<Ttrue, Rank> &i_vtrue,
                         Tout                         i_valfalse,
                         ViewOut<Tout, Rank> &        o_vout)
  {
    using myop = compare_sis<Tin, Ttrue, Tout, Op>;
    myop op(i_valfalse, i_op);
    PixelWiseBinaryOp(i_vin, op, i_vtrue, o_vout);
  }

  template<typename Tin,
           typename Ttrue,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewOut,
           typename = void>
  void ViewCompare_sis_dispatch(const ViewIn<Tin, Rank> &    i_vin,
                                CompOpType                   compOpType,
                                Tin                          i_compval,
                                const ViewTrue<Ttrue, Rank> &i_vtrue,
                                Tout                         i_valfalse,
                                ViewOut<Tout, Rank> &        o_vout)
  {
    static_assert(std::is_convertible<Ttrue, Tout>(),
                  "ViewCompare_sis_dispatch Ttrue and Tout must be convertible "
                  "to each other");
    switch( compOpType )
    {
    case CompOpType::CompOpEqual:
    {
      OpCompEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_sis(i_vin, myop, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpDiff:
    {
      OpCompDiffValue<Tin> myop(i_compval);
      return t_ViewCompare_sis(i_vin, myop, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSup:
    {
      OpCompSupValue<Tin> myop(i_compval);
      return t_ViewCompare_sis(i_vin, myop, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSupEqual:
    {
      OpCompSupEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_sis(i_vin, myop, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInf:
    {
      OpCompInfValue<Tin> myop(i_compval);
      return t_ViewCompare_sis(i_vin, myop, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInfEqual:
    {
      OpCompInfEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_sis(i_vin, myop, i_vtrue, i_valfalse, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("ViewCompare_sis_dispatch CompOpType not implemented");
    }
    }
  }

  template<typename Tin,
           typename Ttrue,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_sis_func_helper(const ViewIn<Tin, Rank> &    i_vin,
                                   CompOpType                   compOpType,
                                   Tin                          i_compval,
                                   const ViewTrue<Ttrue, Rank> &i_vtrue,
                                   Tout                         i_valfalse,
                                   ViewOut<Tout, Rank> &        o_vout)
  {
    return ViewCompare_sis_dispatch(i_vin, compOpType, i_compval, i_vtrue, i_valfalse, o_vout);
  }

  /*************************************************************************************************************************************/
  /*                                                      ISI */
  /*************************************************************************************************************************************/
  template<typename Tin, typename Tcomp, typename Tfalse, typename Tout, class CompareOp, typename = void> struct compare_isi
  {
    const CompareOp cop;
    const Tout      m_valtrue;

    using param_typein    = Tin;
    using param_typecomp  = Tcomp;
    using param_typefalse = Tfalse;
    explicit compare_isi(const Tout &i_valtrue) : cop(), m_valtrue(i_valtrue) {}
    explicit compare_isi(const Tout &i_valtrue, const CompareOp &op) : cop(op), m_valtrue(i_valtrue) {}
    POUTRE_ALWAYS_INLINE Tout operator()(param_typein    i_val,
                                         param_typecomp  i_comp,
                                         param_typefalse i_false) const POUTRE_NOEXCEPT
    {
      return (cop(i_val, i_comp) ? m_valtrue : i_false);
    }
  };

  template<typename Tin, typename Tcomp, typename Tfalse, typename Tout, class CompareOp>
  struct compare_isi<
      Tin,
      Tcomp,
      Tfalse,
      Tout,
      CompareOp,
      std::enable_if_t<
          std::is_same_v<
              std::remove_const_t<Tin>,
              std::remove_const_t<
                  Tout>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Tcomp>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Tfalse>> && std::is_arithmetic_v<Tin>>>
  {
    const CompareOp cop;

    using simd_t      = typename TypeTraits<Tin>::simd_type;
    using simd_mask_t = typename TypeTraits<Tout>::simd_mask_type;

    const Tout m_valtrue;
    simd_t     m_valtrue_pack;

    explicit compare_isi(const Tout &i_valtrue) : cop(), m_valtrue(i_valtrue), m_valtrue_pack(i_valtrue) {}
    explicit compare_isi(const Tout &i_valtrue, const CompareOp &op)
        : cop(op)
        , m_valtrue(i_valtrue)
        , m_valtrue_pack(i_valtrue)
    {
    }

    POUTRE_ALWAYS_INLINE Tout operator()(Tin i_val, Tcomp i_comp, Tfalse i_false) const POUTRE_NOEXCEPT
    {
      return (cop(i_val, i_comp) ? m_valtrue : i_false);
    }

    template<typename U> U operator()(U i_val, U i_comp, U i_false) const POUTRE_NOEXCEPT
    {
      return xs::select(cop(i_val, i_comp), m_valtrue_pack, i_false);
    }
  };

  template<typename Tin,
           typename Tcomp,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewFalse,
           class Op,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_ViewCompare_isi(const ViewIn<Tin, Rank> &      i_vin,
                         Op                             i_op,
                         const ViewComp<Tcomp, Rank> &  i_vcomp,
                         Tout                           i_valtrue,
                         const ViewFalse<Tfalse, Rank> &i_vfalse,
                         ViewOut<Tout, Rank> &          o_vout)
  {
    using myop = compare_isi<Tin, Tcomp, Tfalse, Tout, Op>;
    myop op(i_valtrue, i_op);
    PixelWiseTernaryOp(i_vin, op, i_vcomp, i_vfalse, o_vout);
  }

  template<typename Tin,
           typename Tcomp,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_isi_dispatch(const ViewIn<Tin, Rank> &     i_vin,
                                CompOpType                    compOpType,
                                const ViewComp<Tcomp, Rank> & i_vcomp,
                                Tout                          i_valtrue,
                                const ViewTrue<Tfalse, Rank> &i_vfalse,
                                ViewOut<Tout, Rank> &         o_vout)
  {
    static_assert(std::is_convertible<Tfalse, Tout>(),
                  "ViewCompare_isi_dispatch Tfalse and Tout must be convertible "
                  "to each other");
    static_assert(std::is_convertible<Tin, Tcomp>(),
                  "ViewCompare_isi_dispatch Tin and Tcomp must be convertible to "
                  "each other");
    switch( compOpType )
    {
    case CompOpType::CompOpEqual:
    {
      return t_ViewCompare_isi(i_vin, OpCompEqual<Tin, Tout>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpDiff:
    {
      return t_ViewCompare_isi(i_vin, OpCompDiff<Tin, Tout>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSup:
    {
      return t_ViewCompare_isi(i_vin, OpCompSup<Tin, Tout>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSupEqual:
    {
      return t_ViewCompare_isi(i_vin, OpCompSupEqual<Tin, Tout>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInf:
    {
      return t_ViewCompare_isi(i_vin, OpCompInf<Tin, Tout>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInfEqual:
    {
      return t_ViewCompare_isi(i_vin, OpCompInfEqual<Tin, Tout>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("ViewCompare_isi_dispatch CompOpType not implemented");
    }
    }
  }

  template<typename Tin,
           typename Tcomp,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_isi_func_helper(const ViewIn<Tin, Rank> &     i_vin,
                                   CompOpType                    compOpType,
                                   const ViewComp<Tcomp, Rank> & i_vcomp,
                                   Tout                          i_valtrue,
                                   const ViewTrue<Tfalse, Rank> &i_vfalse,
                                   ViewOut<Tout, Rank> &         o_vout)
  {
    return ViewCompare_isi_dispatch(i_vin, compOpType, i_vcomp, i_valtrue, i_vfalse, o_vout);
  }
  /*************************************************************************************************************************************/
  /*                                                      IIS */
  /*************************************************************************************************************************************/
  template<typename Tin, typename Tcomp, typename Ttrue, typename Tout, class CompareOp, typename = void> struct compare_iis
  {
    const CompareOp cop;
    const Tout      m_valfalse;

    using param_typein   = Tin;
    using param_typecomp = Tcomp;
    using param_typetrue = Ttrue;
    explicit compare_iis(const Tout &i_valfalse) : cop(), m_valfalse(i_valfalse) {}
    explicit compare_iis(const Tout &i_valfalse, const CompareOp &op) : cop(op), m_valfalse(i_valfalse) {}
    POUTRE_ALWAYS_INLINE Tout operator()(param_typein   i_val,
                                         param_typecomp i_comp,
                                         param_typetrue i_true) const POUTRE_NOEXCEPT
    {
      return (cop(i_val, i_comp) ? i_true : m_valfalse);
    }
  };

  template<typename Tin, typename Tcomp, typename Ttrue, typename Tout, class CompareOp>
  struct compare_iis<
      Tin,
      Tcomp,
      Ttrue,
      Tout,
      CompareOp,
      std::enable_if_t<
          std::is_same_v<
              std::remove_const_t<Tin>,
              std::remove_const_t<
                  Tout>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Tcomp>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Ttrue>> && std::is_arithmetic_v<Tin>>>
  {
    const CompareOp cop;
    const Tout      m_valfalse;

    using param_typein   = Tin;
    using param_typecomp = Tcomp;
    using param_typetrue = Ttrue;
    using simd_t         = typename TypeTraits<Tin>::simd_type;
    using simd_mask_t    = typename TypeTraits<Tout>::simd_mask_type;
    simd_t m_valfalse_pack;

    explicit compare_iis(const Tout &i_valfalse) : cop(), m_valfalse(i_valfalse), m_valfalse_pack(i_valfalse) {}
    explicit compare_iis(const Tout &i_valfalse, const CompareOp &op)
        : cop(op)
        , m_valfalse(i_valfalse)
        , m_valfalse_pack(i_valfalse)
    {
    }
    POUTRE_ALWAYS_INLINE Tout operator()(param_typein   i_val,
                                         param_typecomp i_comp,
                                         param_typetrue i_true) const POUTRE_NOEXCEPT
    {
      return (cop(i_val, i_comp) ? i_true : m_valfalse);
    }
    template<typename U> U operator()(U i_val, U i_comp, U i_true) const POUTRE_NOEXCEPT
    {
      return xs::select(cop(i_val, i_comp), i_true, m_valfalse_pack);
    }
  };

  template<typename Tin,
           typename Tcomp,
           typename Ttrue,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewTrue,
           class Op,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_ViewCompare_iis(const ViewIn<Tin, Rank> &    i_vin,
                         Op                           i_op,
                         const ViewComp<Tcomp, Rank> &i_vcomp,
                         const ViewTrue<Ttrue, Rank> &i_vtrue,
                         Tout                         i_valfalse,
                         ViewOut<Tout, Rank> &        o_vout)
  {
    using myop = compare_iis<Tin, Tcomp, Ttrue, Tout, Op>;
    myop op(i_valfalse, i_op);
    PixelWiseTernaryOp(i_vin, op, i_vcomp, i_vtrue, o_vout);
  }

  template<typename Tin,
           typename Tcomp,
           typename Ttrue,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_iis_dispatch(const ViewIn<Tin, Rank> &    i_vin,
                                CompOpType                   compOpType,
                                const ViewComp<Tcomp, Rank> &i_vcomp,
                                const ViewTrue<Ttrue, Rank> &i_vtrue,
                                Tout                         i_valfalse,
                                ViewOut<Tout, Rank> &        o_vout)
  {
    static_assert(std::is_convertible<Ttrue, Tout>(),
                  "ViewCompare_iis_dispatch Ttrue and Tout must be convertible "
                  "to each other");
    static_assert(std::is_convertible<Tin, Tcomp>(),
                  "ViewCompare_iis_dispatch Tin and Tcomp must be convertible to "
                  "each other");
    switch( compOpType )
    {
    case CompOpType::CompOpEqual:
    {
      return t_ViewCompare_iis(i_vin, OpCompEqual<Tin, Tout>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpDiff:
    {
      return t_ViewCompare_iis(i_vin, OpCompDiff<Tin, Tout>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSup:
    {
      return t_ViewCompare_iis(i_vin, OpCompSup<Tin, Tout>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSupEqual:
    {
      return t_ViewCompare_iis(i_vin, OpCompSupEqual<Tin, Tout>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInf:
    {
      return t_ViewCompare_iis(i_vin, OpCompInf<Tin, Tout>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInfEqual:
    {
      return t_ViewCompare_iis(i_vin, OpCompInfEqual<Tin, Tout>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("ViewCompare_iis_dispatch CompOpType not implemented");
    }
    }
  }

  template<typename Tin,
           typename Tcomp,
           typename Ttrue,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewComp,
           template<typename, ptrdiff_t>
           class ViewTrue,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_iis_func_helper(const ViewIn<Tin, Rank> &    i_vin,
                                   CompOpType                   compOpType,
                                   const ViewComp<Tcomp, Rank> &i_vcomp,
                                   const ViewTrue<Ttrue, Rank> &i_vtrue,
                                   Tout                         i_valfalse,
                                   ViewOut<Tout, Rank> &        o_vout)
  {
    return ViewCompare_iis_dispatch(i_vin, compOpType, i_vcomp, i_vtrue, i_valfalse, o_vout);
  }

  /*************************************************************************************************************************************/
  /*                                                      SSI */
  /*************************************************************************************************************************************/
  template<typename Tin, typename Tfalse, typename Tout, class CompareOp, typename = void> struct compare_ssi
  {
    const CompareOp cop;
    const Tin       m_compval;
    const Tout      m_valtrue;

    using param_typein   = Tin;
    using param_typetrue = Tfalse;
    compare_ssi(const Tout &i_valtrue) : cop(), m_valtrue(i_valtrue) {}
    compare_ssi(const Tout &i_valtrue, const CompareOp &op) : cop(op), m_valtrue(i_valtrue) {}
    POUTRE_ALWAYS_INLINE Tout operator()(param_typein i_val, param_typetrue i_false) const POUTRE_NOEXCEPT
    {
      return (cop(i_val) ? m_valtrue : i_false);
    }
  };

  template<typename Tin, typename Tfalse, typename Tout, class CompareOp>
  struct compare_ssi<
      Tin,
      Tfalse,
      Tout,
      CompareOp,
      std::enable_if_t<
          std::is_same_v<
              std::remove_const_t<Tin>,
              std::remove_const_t<
                  Tfalse>> && std::is_same_v<std::remove_const_t<Tin>, std::remove_const_t<Tout>> && std::is_arithmetic_v<Tin>>>
  {
    const CompareOp cop;

    using simd_t      = typename TypeTraits<Tin>::simd_type;
    using simd_mask_t = typename TypeTraits<Tout>::simd_mask_type;

    const Tout m_valtrue;
    simd_t     m_valtrue_pack;

    compare_ssi(const Tout &i_valtrue) : cop(), m_valtrue(i_valtrue), m_valtrue_pack(i_valtrue) {}
    compare_ssi(const Tout &i_valtrue, const CompareOp &op) : cop(op), m_valtrue(i_valtrue), m_valtrue_pack(i_valtrue) {}
    POUTRE_ALWAYS_INLINE Tout operator()(Tin i_val, Tfalse i_false) const POUTRE_NOEXCEPT
    {
      return (cop(i_val) ? m_valtrue : i_false);
    }

    template<typename U> U operator()(U i_val, U i_false) const POUTRE_NOEXCEPT
    {
      return xs::select(cop(i_val), m_valtrue_pack, i_false);
    }
  };

  template<typename Tin,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           class Op,
           template<typename, ptrdiff_t>
           class ViewFalse,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_ViewCompare_ssi(const ViewIn<Tin, Rank> &i_vin,
                         Op                       i_op,
                         // Tin                            i_compval,
                         Tout                           i_valtrue,
                         const ViewFalse<Tfalse, Rank> &i_vfalse,
                         ViewOut<Tout, Rank> &          o_vout)
  {
    using myop = compare_ssi<Tin, Tfalse, Tout, Op>;
    myop op(i_valtrue, i_op);
    PixelWiseBinaryOp(i_vin, op, i_vfalse, o_vout);
  }

  template<typename Tin,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewFalse,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_ssi_dispatch(const ViewIn<Tin, Rank> &      i_vin,
                                CompOpType                     compOpType,
                                Tin                            i_compval,
                                Tout                           i_valtrue,
                                const ViewFalse<Tfalse, Rank> &i_vfalse,
                                ViewOut<Tout, Rank> &          o_vout)
  {
    static_assert(std::is_convertible<Tfalse, Tout>(),
                  "ViewCompare_ssi_dispatch Ttrue and Tout must be convertible "
                  "to each other");
    switch( compOpType )
    {
    case CompOpType::CompOpEqual:
    {
      OpCompEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_ssi(i_vin, myop, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpDiff:
    {
      OpCompDiffValue<Tin> myop(i_compval);
      return t_ViewCompare_ssi(i_vin, myop, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSup:
    {
      OpCompSupValue<Tin> myop(i_compval);
      return t_ViewCompare_ssi(i_vin, myop, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSupEqual:
    {
      OpCompSupEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_ssi(i_vin, myop, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInf:
    {
      OpCompInfValue<Tin> myop(i_compval);
      return t_ViewCompare_ssi(i_vin, myop, i_valtrue, i_vfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInfEqual:
    {
      OpCompInfEqualValue<Tin> myop(i_compval);
      return t_ViewCompare_ssi(i_vin, myop, i_valtrue, i_vfalse, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("ViewCompare_ssi_dispatch CompOpType not implemented");
    }
    }
  }

  template<typename Tin,
           typename Tfalse,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewFalse,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_ssi_func_helper(const ViewIn<Tin, Rank> &      i_vin,
                                   CompOpType                     compOpType,
                                   Tin                            i_compval,
                                   Tout                           i_valtrue,
                                   const ViewFalse<Tfalse, Rank> &i_vfalse,
                                   ViewOut<Tout, Rank> &          o_vout)
  {
    return ViewCompare_ssi_dispatch(i_vin, compOpType, i_compval, i_valtrue, i_vfalse, o_vout);
  }
  /*************************************************************************************************************************************/
  /*                                                      ISS */
  /*************************************************************************************************************************************/
  template<typename Tin1, typename Tin2, typename Tout, class CompareOp, typename = void> struct compare_iss
  {
    const CompareOp cop;
    const Tout      m_valtrue, m_valfalse;
    using param_type1 = Tin1;
    using param_type2 = Tin2;
    compare_iss(const Tout &i_valtrue, const Tout &i_valfalse) : cop(), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}
    compare_iss(const Tout &i_valtrue, const Tout &i_valfalse, const CompareOp &op)
        : cop(op)
        , m_valtrue(i_valtrue)
        , m_valfalse(i_valfalse)
    {
    }
    POUTRE_ALWAYS_INLINE Tout operator()(param_type1 i_val1, param_type2 i_val2) const POUTRE_NOEXCEPT
    {
      return (cop(i_val1, i_val2) ? m_valtrue : m_valfalse);
    }
  };

  template<typename Tin1, typename Tin2, typename Tout, class CompareOp>
  struct compare_iss<
      Tin1,
      Tin2,
      Tout,
      CompareOp,
      std::enable_if_t<
          std::is_same_v<
              std::remove_const_t<Tin1>,
              std::remove_const_t<
                  Tin2>> && std::is_same_v<std::remove_const_t<Tin1>, std::remove_const_t<Tout>> && std::is_arithmetic_v<Tin1>>>
  {
    const CompareOp cop;
    using param_type1 = Tin1;
    using param_type2 = Tin2;
    using simd_t      = typename TypeTraits<Tin1>::simd_type;
    using simd_mask_t = typename TypeTraits<Tout>::simd_mask_type;
    simd_t m_valtrue_pack, m_valfalse_pack;
    Tout   m_valtrue, m_valfalse;
    compare_iss(const Tout &i_valtrue, const Tout &i_valfalse)
        : cop()
        , m_valtrue_pack(i_valtrue)
        , m_valfalse_pack(i_valfalse)
        , m_valtrue(i_valtrue)
        , m_valfalse(i_valfalse)
    {
    }
    compare_iss(const Tout &i_valtrue, const Tout &i_valfalse, const CompareOp &op)
        : cop(op)
        , m_valtrue_pack(i_valtrue)
        , m_valfalse_pack(i_valfalse)
        , m_valtrue(i_valtrue)
        , m_valfalse(i_valfalse)
    {
    }

    POUTRE_ALWAYS_INLINE Tout operator()(param_type1 i_val1, param_type2 i_val2) const POUTRE_NOEXCEPT
    {
      return (cop(i_val1, i_val2) ? m_valtrue : m_valfalse);
    }

    template<typename U> U operator()(U i_val1, U i_val2) const POUTRE_NOEXCEPT
    {
      return xs::select(cop(i_val1, i_val2), m_valtrue_pack, m_valfalse_pack);
    }
  };

  template<typename Tin1,
           typename Tin2,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn1,
           template<typename, ptrdiff_t>
           class ViewIn2,
           class Op,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_ViewCompare_iss(const ViewIn1<Tin1, Rank> &i_vin1,
                         Op                         i_op,
                         const ViewIn2<Tin2, Rank> &i_vin2,
                         Tout                       i_valtrue,
                         Tout                       i_valfalse,
                         ViewOut<Tout, Rank> &      o_vout)
  {
    using myop = compare_iss<Tin1, Tin2, Tout, Op>;
    myop op(i_valtrue, i_valfalse, i_op);
    PixelWiseBinaryOp(i_vin1, op, i_vin2, o_vout);
  }

  template<typename Tin1,
           typename Tin2,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn1,
           template<typename, ptrdiff_t>
           class ViewIn2,
           template<typename, ptrdiff_t>
           class ViewOut,
           typename = void>
  void ViewCompare_iss_dispatch(const ViewIn1<Tin1, Rank> &i_vin1,
                                CompOpType                 compOpType,
                                const ViewIn2<Tin2, Rank> &i_vin2,
                                Tout                       i_valtrue,
                                Tout                       i_valfalse,
                                ViewOut<Tout, Rank> &      o_vout)
  {
    static_assert(std::is_convertible<Tin1, Tin2>(),
                  "ViewCompare_iss_dispatch Tin1 and Tin2 must be convertible to "
                  "each other");
    switch( compOpType )
    {
    case CompOpType::CompOpEqual:
    {
      return t_ViewCompare_iss(i_vin1, OpCompEqual<Tin1, Tin2>(), i_vin2, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpDiff:
    {
      return t_ViewCompare_iss(i_vin1, OpCompDiff<Tin1, Tin2>(), i_vin2, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSup:
    {
      return t_ViewCompare_iss(i_vin1, OpCompSup<Tin1, Tin2>(), i_vin2, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpSupEqual:
    {
      return t_ViewCompare_iss(i_vin1, OpCompSupEqual<Tin1, Tin2>(), i_vin2, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInf:
    {
      return t_ViewCompare_iss(i_vin1, OpCompInf<Tin1, Tin2>(), i_vin2, i_valtrue, i_valfalse, o_vout);
    }
    break;
    case CompOpType::CompOpInfEqual:
    {
      return t_ViewCompare_iss(i_vin1, OpCompInfEqual<Tin1, Tin2>(), i_vin2, i_valtrue, i_valfalse, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("ViewCompare_iss_dispatch CompOpType not implemented");
    }
    } // end case
  }

  template<typename Tin1,
           typename Tin2,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn1,
           template<typename, ptrdiff_t>
           class ViewIn2,
           template<typename, ptrdiff_t>
           class ViewOut>
  void ViewCompare_iss_func_helper(const ViewIn1<Tin1, Rank> &i_vin1,
                                   CompOpType                 compOpType,
                                   const ViewIn2<Tin2, Rank> &i_vin2,
                                   Tout                       i_valtrue,
                                   Tout                       i_valfalse,
                                   ViewOut<Tout, Rank> &      o_vout)
  {
    return ViewCompare_iss_dispatch(i_vin1, compOpType, i_vin2, i_valtrue, i_valfalse, o_vout);
  }
  //! @} doxygroup: image_processing_linear_group
} // namespace poutre
#endif // POUTRE_IMAGEPROCESSING_COMPARE_HPP__
