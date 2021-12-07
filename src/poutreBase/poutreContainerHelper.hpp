
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   poutreContainerHelper.hpp
 * @author Thomas Retornaz
 * @brief  Boiler plate code to unroolling operator @c ArithOpType,@c
 * CompOpType,@c AssignOp for small size
 *
 *
 */

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4244) // 4244 'conversion' conversion from 'type1' to 'type2', possible
                                  // loss of data -> due to cast in operator / %  ...
#endif

#include <poutreBase/poutreConfig.hpp>
#include <poutreBase/poutreTypes.hpp>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace poutre
{
  /**
   * @addtogroup unrolling_helper_group Unrolling for array
   * @ingroup poutre_base_group
   *@{
   */
  namespace details
  {
    // unrolling helper for small rank eg from 1 to 4
    // All of this may be over engineered until c++14 constexpr capabilities were implemented in all
    // compiler NOTE c++14 we need return void see
    // http://en.cppreference.com/w/cpp/language/constexpr NOTE c++14 we need return assignment see
    // http://en.cppreference.com/w/cpp/language/constexpr cf
    // https://isocpp.org/files/papers/N3652.html GCC5.1 seems ok ,but fail for Visual 2015 .... see
    // http://blogs.msdn.com/b/vcblog/archive/2015/04/29/c-11-14-17-features-in-vs-2015-rc.aspx
    // Those specialization and design was inspired by ms amp tricks

    template<typename value_type, ArithOpType op> struct helper_arith_op
    {
    };

    // todo macro ?
    template<typename value_type> struct helper_arith_op<value_type, ArithOpType::ArithOpPlus>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(const value_type &A0, const value_type &A1, value_type &res) POUTRE_NOEXCEPT
      {
        res = (A0 + A1);
      }
    };

    template<typename value_type> struct helper_arith_op<value_type, ArithOpType::ArithOpMinus>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(const value_type &A0, const value_type &A1, value_type &res) POUTRE_NOEXCEPT
      {
        res = (A0 - A1);
      }
    };

    template<typename value_type> struct helper_arith_op<value_type, ArithOpType::ArithOpMul>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(const value_type &A0, const value_type &A1, value_type &res) POUTRE_NOEXCEPT
      {
        res = (A0 * A1);
      }
    };

    template<typename value_type> struct helper_arith_op<value_type, ArithOpType::ArithOpDiv>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(const value_type &A0, const value_type &A1, value_type &res) POUTRE_NOEXCEPT
      {
        res = (A0 / A1);
      }
    };

    template<typename t1, typename t2, AssignOpType op> struct helper_assign_op
    {
    };

    // todo macro ?
    template<typename t1, typename t2> struct helper_assign_op<t1, t2, AssignOpType::AssignOp>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(const t1 &A0, t2 &result) POUTRE_NOEXCEPT { result = A0; }
    };

    template<typename t1, typename t2> struct helper_assign_op<t1, t2, AssignOpType::AssignOpAdd>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(const t1 &A0, t2 &result) POUTRE_NOEXCEPT { result += A0; }
    };

    template<typename t1, typename t2> struct helper_assign_op<t1, t2, AssignOpType::AssignOpMinus>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(const t1 &A0, t2 &result) POUTRE_NOEXCEPT { result -= A0; }
    };

    template<typename t1, typename t2> struct helper_assign_op<t1, t2, AssignOpType::AssignOpMul>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(const t1 &A0, t2 &result) POUTRE_NOEXCEPT { result *= A0; }
    };

    template<typename t1, typename t2> struct helper_assign_op<t1, t2, AssignOpType::AssignOpDiv>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(const t1 &A0, t2 &result) POUTRE_NOEXCEPT { result /= A0; }
    };

    template<typename t1, typename t2> struct helper_assign_op<t1, t2, AssignOpType::AssignOpMod>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(const t1 &A0, t2 &result) POUTRE_NOEXCEPT { result %= A0; }
    };

    template<typename t1, typename t2> struct helper_assign_op<t1, t2, AssignOpType::AssignOpNegate>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(const t1 &A0, t2 &result) POUTRE_NOEXCEPT { result = -A0; }
    };

    template<typename value_type, CompOpType op> struct helper_comp_op
    {
    };

    // todo macro ?
    template<typename value_type> struct helper_comp_op<value_type, CompOpType::CompOpEqual>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
        return (A0 == A1);
      }
    };

    template<typename value_type> struct helper_comp_op<value_type, CompOpType::CompOpDiff>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
        return (A0 != A1);
      }
    };

    template<typename value_type> struct helper_comp_op<value_type, CompOpType::CompOpInf>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
        return (A0 < A1);
      }
    };

    template<typename value_type> struct helper_comp_op<value_type, CompOpType::CompOpSup>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
        return (A0 > A1);
      }
    };

    template<typename value_type> struct helper_comp_op<value_type, CompOpType::CompOpSupEqual>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
        return (A0 >= A1);
      }
    };

    template<typename value_type> struct helper_comp_op<value_type, CompOpType::CompOpInfEqual>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
        return (A0 <= A1);
      }
    };

    template<class container, ptrdiff_t Rank = container::rank> struct helper_comp_lexicographic_less_container_op
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(container const &A0, container const &A1) POUTRE_NOEXCEPT
      {
        for( ptrdiff_t i = Rank - 1; i >= 0; --i )
        {
          if( A0[i] == A1[i] )
            continue;
          return (A0[i] < A1[i]);
        }
        return false;
      }
    };

    template<class container, ptrdiff_t Rank = container::rank> struct helper_comp_lexicographic_less_equal_container_op
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(container const &A0, container const &A1) POUTRE_NOEXCEPT
      {
        for( ptrdiff_t i = Rank - 1; i >= 0; --i )
        {
          if( A0[i] > A1[i] )
            return false;
        }
        return true;
      }
    };

    template<class container, ptrdiff_t Rank = container::rank> struct helper_comp_lexicographic_sup_container_op
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(container const &A0, container const &A1) POUTRE_NOEXCEPT
      {
        for( ptrdiff_t i = Rank - 1; i >= 0; i-- )
        {
          if( A0[i] == A1[i] )
            continue;
          return (A0[i] > A1[i]);
        }
        return false;
      }
    };

    template<class container, ptrdiff_t Rank = container::rank> struct helper_comp_lexicographic_sup_equal_container_op
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(container const &A0, container const &A1) POUTRE_NOEXCEPT
      {
        for( ptrdiff_t i = Rank - 1; i >= 0; i-- )
        {
          if( A0[i] < A1[i] )
            return false;
        }
        return true;
      }
    };

    template<class container, ptrdiff_t Rank = container::rank> struct helper_comp_equal_container_op
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(container const &A0, container const &A1) POUTRE_NOEXCEPT
      {
        for( size_t i = 0; i < Rank; i++ )
        {
          if( A0[i] != A1[i] )
            return false;
        }
        return true;
      }
    };

    template<class container> struct helper_comp_equal_container_op<container, 1>
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(container const &A0, container const &A1) POUTRE_NOEXCEPT
      {
        return A0[0] == A1[0];
      }
    };

    template<class container> struct helper_comp_equal_container_op<container, 2>
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(container const &A0, container const &A1) POUTRE_NOEXCEPT
      {
        return ((A0[0] == A1[0]) && (A0[1] == A1[1]));
      }
    };

    template<class container> struct helper_comp_equal_container_op<container, 3>
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(container const &A0, container const &A1) POUTRE_NOEXCEPT
      {
        return ((A0[0] == A1[0]) && (A0[1] == A1[1]) && (A0[2] == A1[2]));
      }
    };

    template<class container, ArithOpType arithop, ptrdiff_t Rank = container::rank> struct helper_arith_container_op
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR void op(container const &A0, container const &A1, container &res) POUTRE_NOEXCEPT
      {
        for( size_t i = 0; i < Rank; i++ )
        {
          helper_arith_op<value_type, arithop>::op(A0[i], A1[i], res[i]);
        }
      }
    };

    // with proper support of constexpr may above specialization are unnecessary
    template<class container, ArithOpType arithop> struct helper_arith_container_op<container, arithop, 1>
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(container const &A0, container const &A1, container &res) POUTRE_NOEXCEPT
      {
        helper_arith_op<value_type, arithop>::op(A0[0], A1[0], res[0]);
      }
    };

    template<class container, ArithOpType arithop> struct helper_arith_container_op<container, arithop, 2>
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(container const &A0, container const &A1, container &res) POUTRE_NOEXCEPT
      {
        helper_arith_op<value_type, arithop>::op(A0[0], A1[0], res[0]);
        helper_arith_op<value_type, arithop>::op(A0[1], A1[1], res[1]);
      }
    };

    template<class container, ArithOpType arithop> struct helper_arith_container_op<container, arithop, 3>
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(container const &A0, container const &A1, container &res) POUTRE_NOEXCEPT
      {
        helper_arith_op<value_type, arithop>::op(A0[0], A1[0], res[0]);
        helper_arith_op<value_type, arithop>::op(A0[1], A1[1], res[1]);
        helper_arith_op<value_type, arithop>::op(A0[2], A1[2], res[2]);
      }
    };

    template<class container, ArithOpType arithop> struct helper_arith_container_op<container, arithop, 4>
    {
      using value_type = typename container::value_type;
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(container const &A0, container const &A1, container &res) POUTRE_NOEXCEPT
      {
        helper_arith_op<value_type, arithop>::op(A0[0], A1[0], res[0]);
        helper_arith_op<value_type, arithop>::op(A0[1], A1[1], res[1]);
        helper_arith_op<value_type, arithop>::op(A0[2], A1[2], res[2]);
        helper_arith_op<value_type, arithop>::op(A0[3], A1[3], res[3]);
      }
    };

    template<class container, AssignOpType assignop, ptrdiff_t Rank = container::rank> struct helper_assign_container_op
    {
      using value_type = typename container::value_type;
      template<class containerother>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(containerother const &A0, container &res) POUTRE_NOEXCEPT
      {
        for( size_t i = 0; i < Rank; i++ )
        {
          helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[1], res[1]);
        }
      }
    };

    // with proper support of constexpr may above specialization are unnecessary
    template<class container, AssignOpType assignop> struct helper_assign_container_op<container, assignop, 1>
    {
      using value_type = typename container::value_type;
      template<class containerother>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(containerother const &A0, container &res) POUTRE_NOEXCEPT
      {
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[0], res[0]);
      }
    };

    template<class container, AssignOpType assignop> struct helper_assign_container_op<container, assignop, 2>
    {
      using value_type = typename container::value_type;
      template<class containerother>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(containerother const &A0, container &res) POUTRE_NOEXCEPT
      {
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[0], res[0]);
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[1], res[1]);
      }
    };

    template<class container, AssignOpType assignop> struct helper_assign_container_op<container, assignop, 3>
    {
      using value_type = typename container::value_type;
      template<class containerother>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(containerother const &A0, container &res) POUTRE_NOEXCEPT
      {
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[0], res[0]);
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[1], res[1]);
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[2], res[2]);
      }
    };

    template<class container, AssignOpType assignop> struct helper_assign_container_op<container, assignop, 4>
    {
      using value_type = typename container::value_type;
      template<class containerother>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(containerother const &A0, container &res) POUTRE_NOEXCEPT
      {
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[0], res[0]);
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[1], res[1]);
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[2], res[2]);
        helper_assign_op<typename containerother::value_type, value_type, assignop>::op(A0[3], res[3]);
      }
    };

    template<class container, AssignOpType assignop, ptrdiff_t Rank = container::rank> struct helper_assign_container_valueop
    {
      using value_type = typename container::value_type;
      template<typename othervaluetype>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(othervaluetype const &A0, container &res) POUTRE_NOEXCEPT
      {
        for( size_t i = 0; i < Rank; i++ )
        {
          helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[i]);
        }
      }
    };

    template<class container, AssignOpType assignop> struct helper_assign_container_valueop<container, assignop, 1>
    {
      using value_type = typename container::value_type;
      template<typename othervaluetype>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(othervaluetype const &A0, container &res) POUTRE_NOEXCEPT
      {
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[0]);
      }
    };

    template<class container, AssignOpType assignop> struct helper_assign_container_valueop<container, assignop, 2>
    {
      using value_type = typename container::value_type;
      template<typename othervaluetype>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(othervaluetype const &A0, container &res) POUTRE_NOEXCEPT
      {
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[0]);
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[1]);
      }
    };

    template<class container, AssignOpType assignop> struct helper_assign_container_valueop<container, assignop, 3>
    {
      using value_type = typename container::value_type;
      template<typename othervaluetype>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(othervaluetype const &A0, container &res) POUTRE_NOEXCEPT
      {
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[0]);
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[1]);
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[2]);
      }
    };

    template<class container, AssignOpType assignop> struct helper_assign_container_valueop<container, assignop, 4>
    {
      using value_type = typename container::value_type;
      template<typename othervaluetype>
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void op(othervaluetype const &A0, container &res) POUTRE_NOEXCEPT
      {
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[0]);
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[1]);
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[2]);
        helper_assign_op<othervaluetype, value_type, assignop>::op(A0, res[3]);
      }
    };

    template<class bnds, class idx, ptrdiff_t Rank = bnds::rank> struct helper_contains_container_op
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(bnds const &bnd, idx const &id) POUTRE_NOEXCEPT
      {
        static_assert(bnds::rank == idx::rank, "helper_contains_container_op: all provided container must share same rank");
        for( size_t i = 0; i < Rank; i++ )
        {
          if( id[i] < 0 )
            return false;
          if( id[i] >= bnd[i] )
            return false;
        }
        return true;
      }
    };

    template<class bnds, class idx> struct helper_contains_container_op<bnds, idx, 1>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(bnds const &bnd, idx const &id) POUTRE_NOEXCEPT
      {
        return (id[0] >= 0) && (id[0] < bnd[0]);
      }
    };

    template<class bnds, class idx> struct helper_contains_container_op<bnds, idx, 2>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(bnds const &bnd, idx const &id) POUTRE_NOEXCEPT
      {
        return ((id[0] >= 0) && (id[0] < bnd[0]) && (id[1] >= 0) && (id[1] < bnd[1]));
      }
    };

    template<class bnds, class idx> struct helper_contains_container_op<bnds, idx, 3>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR bool op(bnds const &bnd, idx const &id) POUTRE_NOEXCEPT
      {
        return (id[0] >= 0) && (id[0] < bnd[0]) && (id[1] >= 0) && (id[1] < bnd[1]) && (id[2] >= 0) && (id[2] < bnd[2]);
      }
    };

    template<class bnds, class idx, ptrdiff_t Rank = bnds::rank> struct get_offset_from_coord_nostride
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR offset op(bnds const &i_bnd, idx const &i_idx) POUTRE_NOEXCEPT
      {
        static_assert(bnds::rank == idx::rank, "get_offset: all provided container must share same rank");
        offset    current_offset = i_idx[Rank - 1]; // at least rank==1
        ptrdiff_t stride         = i_bnd[Rank - 1];
        for( auto i = Rank - 2; i >= 0; --i )
        {
          current_offset += i_idx[i] * stride;
          stride *= i_bnd[i];
        }
        return current_offset;
      }
    };

    // specialize 1D
    template<class bnds, class idx> struct get_offset_from_coord_nostride<bnds, idx, 1>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR offset op(bnds const &i_bnd, idx const &i_idx) POUTRE_NOEXCEPT
      {
        return i_idx[0];
      }
    };

    // specialize 2D
    template<class bnds, class idx> struct get_offset_from_coord_nostride<bnds, idx, 2>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR offset op(const bnds &i_bnd, const idx &i_idx) POUTRE_NOEXCEPT
      {
        return (i_idx[1] + i_idx[0] * i_bnd[1]);
      }
    };

    template<class bnds, class idx, ptrdiff_t Rank = bnds::rank> struct get_coord_from_offset_nostride
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(bnds const &i_bnd, offset off, idx &o_idx) POUTRE_NOEXCEPTONLYNDEBUG
      {
        static_assert(bnds::rank == idx::rank, "get_offset: all provided container must share same rank");
        POUTRE_ASSERTCHECK(off >= 0, "get_offset: provided offset must be >0");
#  ifndef NDEBUG
        // compute max allowed offset

#  endif
        // recreate idx
        auto i = Rank - 1;
        for( ; i >= 0 && off != 0; --i )
        {
          POUTRE_ASSERTCHECK(i_bnd[i] >= 0, "get_offset: bnd[i] must be >0");
          auto dv  = std::div(off, i_bnd[i]);
          o_idx[i] = dv.rem;  // offset % current;
          off      = dv.quot; // offset=offset/current
        }
        for( ; i >= 0; --i )
        {
          o_idx[i] = 0;
        }
      }
    };

    template<class bnds, class idx> struct get_coord_from_offset_nostride<bnds, idx, 1>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(bnds const &i_bnd, offset off, idx &o_idx) POUTRE_NOEXCEPTONLYNDEBUG
      {
        POUTRE_ASSERTCHECK(off >= 0, "get_offset: provided offset must be >0");
        POUTRE_ASSERTCHECK(off < i_bnd[0], "get_offset: provided offset out of bound");
        o_idx[0] = off;
      }
    };

    template<class bnds, class idx> struct get_coord_from_offset_nostride<bnds, idx, 2>
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(bnds const &i_bnd, offset off, idx &o_idx) POUTRE_NOEXCEPTONLYNDEBUG
      {
        POUTRE_ASSERTCHECK(off >= 0, "get_offset: provided offset must be >0");
        auto dv  = std::div(off, i_bnd[1]);
        o_idx[1] = dv.rem;  // offset % current;
        o_idx[0] = dv.quot; // offset=offset/current
        return;
      }
    };

    template<class bnds, class idx, ptrdiff_t Rank = bnds::rank> struct shift_op
    {
      POUTRE_ALWAYS_INLINE static POUTRE_CONSTEXPR void
      op(bnds const &i_bnd, idx const &i_idx, offset shift, idx &o_idx) POUTRE_NOEXCEPT
      {
        static_assert(bnds::rank == idx::rank, "shift_op: all provided container must share same rank");
        // quick exit
        if( shift == 0 )
        {
          helper_assign_container_op<idx, AssignOpType::AssignOp>::op(i_idx, o_idx);
          return;
        }
        // current offset belong to in
        offset current_offset = get_offset_from_coord_nostride<bnds, idx>::op(i_bnd, i_idx);
        // shift
        current_offset += shift;
        // recreate idx
        get_coord_from_offset_nostride<bnds, idx>::op(i_bnd, current_offset, o_idx);
      }
    };

  } // namespace details
    /**
    //! @} doxygroup: unrolling_helper_group
    */
} // namespace poutre
#endif // DOXYGEN_SHOULD_SKIP_THIS

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
