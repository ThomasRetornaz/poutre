
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file poutreImageProcessingQuaternaryOp.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief QUaternary op over images and views
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>

namespace poutre
{
  /**
   * @addtogroup image_processing_linear_group
   *@{
   */

  /****************************************************************************************/
  /*                               PixelWiseQuaTernaryOp                                      */
  /****************************************************************************************/

  // primary use strided view
  template<typename T1,
           typename T2,
           typename T3,
           typename T4,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2,
           template<typename, ptrdiff_t>
           class View3,
           template<typename, ptrdiff_t>
           class View4,
           template<typename, ptrdiff_t>
           class ViewOut,
           class QuaterOp,
           typename = void>
  struct PixelWiseQuaternaryOpDispatcher
  {
    static_assert((std::is_same_v<View1<T1, Rank>, strided_array_view<T1, Rank>>
                   || std::is_same_v<View1<T1, Rank>, strided_array_view<const T1, Rank>>
                   || std::is_same_v<View2<T2, Rank>, strided_array_view<T2, Rank>>
                   || std::is_same_v<View2<T2, Rank>, strided_array_view<const T2, Rank>>
                   || std::is_same_v<View2<T3, Rank>, strided_array_view<T3, Rank>>
                   || std::is_same_v<View2<T3, Rank>, strided_array_view<const T3, Rank>>
                   || std::is_same_v<View2<T4, Rank>, strided_array_view<T4, Rank>>
                   || std::is_same_v<View2<T4, Rank>, strided_array_view<const T4, Rank>>
                   || std::is_same_v<ViewOut<Tout, Rank>, strided_array_view<Tout, Rank>>),
                  "strided view only specilization fail for arrayview");

    void operator()(const View1<T1, Rank> &i_vin1,
                    const QuaterOp        &op,
                    const View2<T2, Rank> &i_vin2,
                    const View3<T3, Rank> &i_vin3,
                    const View4<T4, Rank> &i_vin4,
                    ViewOut<Tout, Rank>   &o_vout) const POUTRE_NOEXCEPTONLYNDEBUG
    {
      POUTRE_ENTERING("PixelWiseQuaternaryOpDispatcher generic case");
      // More runtime dispatch
      auto vInbound1  = i_vin1.bound();
      auto vInbound2  = i_vin2.bound();
      auto vInbound3  = i_vin3.bound();
      auto vInbound4  = i_vin4.bound();
      auto vOutbound  = o_vout.bound();
      auto stridevIN1 = i_vin1.stride();
      auto stridevIN2 = i_vin2.stride();
      auto stridevIN3 = i_vin3.stride();
      auto stridevIN4 = i_vin4.stride();
      auto stridevOut = o_vout.stride();

      if( vInbound1 == vOutbound && vInbound2 == vOutbound && vInbound1 == vInbound3 && vInbound4 == vInbound3
          && stridevIN1 == stridevOut && stridevIN2 == stridevOut && stridevIN1 == stridevIN3
          && stridevIN4 == stridevIN3 ) // same bound + same stride -> one idx
      {
        auto beg1 = begin(vInbound1);
        auto end1 = end(vInbound1);
        for( ; beg1 != end1; ++beg1 )
        {
          o_vout[*beg1] = static_cast<Tout>(op(i_vin1[*beg1], i_vin2[*beg1], i_vin3[*beg1], i_vin4[*beg1]));
        }

        // TODO stride==1 in least significant dimension
      }
      else // default four idx
      {
        // std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
        auto beg1   = begin(vInbound1);
        auto end1   = end(vInbound1);
        auto beg2   = begin(vInbound2);
        auto beg3   = begin(vInbound3);
        auto beg4   = begin(vInbound4);
        auto begout = begin(vOutbound);

        for( ; beg1 != end1; ++beg1, ++beg2, ++beg3, ++begout )
        {
          o_vout[*begout] = static_cast<Tout>(op(i_vin1[*beg1], i_vin2[*beg2], i_vin3[*beg3], i_vin4[*beg4]));
        }
      }
    }
  };

  // template specialization both array_view but different type
  template<typename T1, typename T2, typename T3, typename T4, typename Tout, ptrdiff_t Rank, class QuaterOp>
  struct PixelWiseQuaternaryOpDispatcher<
      T1,
      T2,
      T3,
      T4,
      Tout,
      Rank,
      array_view,
      array_view,
      array_view,
      array_view,
      array_view,
      QuaterOp,
      std::enable_if_t<!std::is_same_v<std::remove_const_t<T1>, std::remove_const_t<T2>>
                       || !std::is_same_v<std::remove_const_t<T2>, std::remove_const_t<Tout>>
                       || !std::is_same_v<std::remove_const_t<T3>, std::remove_const_t<Tout>>
                       || !std::is_same_v<std::remove_const_t<T4>, std::remove_const_t<Tout>>>>
  {
    void operator()(const array_view<T1, Rank> &i_vin1,
                    const QuaterOp             &op,
                    const array_view<T2, Rank> &i_vin2,
                    const array_view<T3, Rank> &i_vin3,
                    const array_view<T4, Rank> &i_vin4,
                    array_view<Tout, Rank>     &o_vout) const POUTRE_NOEXCEPTONLYNDEBUG
    {
      POUTRE_ENTERING("PixelWiseQuaternaryOpDispatcher both arrayview");
      auto i_vinbeg1 = i_vin1.data();
      auto i_vinend1 = i_vin1.data() + i_vin1.size();
      auto i_vinbeg2 = i_vin2.data();
      auto i_vinbeg3 = i_vin3.data();
      auto i_vinbeg4 = i_vin4.data();
      auto i_voutbeg = o_vout.data();
      for( ; i_vinbeg1 != i_vinend1; ++i_vinbeg1, ++i_vinbeg2, ++i_vinbeg3, ++i_vinbeg4, ++i_voutbeg )
      {
        *i_voutbeg = static_cast<Tout>(op(*i_vinbeg1, *i_vinbeg2, *i_vinbeg3, *i_vinbeg4));
      }
    }
  };

  template<POUTRE_CONCEPT_BASE_VALUE_TYPE T1,
           POUTRE_CONCEPT_BASE_VALUE_TYPE T2,
           POUTRE_CONCEPT_BASE_VALUE_TYPE T3,
           POUTRE_CONCEPT_BASE_VALUE_TYPE T4,
           POUTRE_CONCEPT_BASE_VALUE_TYPE Tout,
           ptrdiff_t                      Rank,
           class QuaterOp>
  struct PixelWiseQuaternaryOpDispatcher<
      T1,
      T2,
      T3,
      T4,
      Tout,
      Rank,
      array_view,
      array_view,
      array_view,
      array_view,
      array_view,
      QuaterOp,
      std::enable_if_t<std::is_same_v<std::remove_const_t<T1>, std::remove_const_t<T2>>
                       && std::is_same_v<std::remove_const_t<T2>, std::remove_const_t<Tout>>
                       && std::is_same_v<std::remove_const_t<T3>, std::remove_const_t<Tout>>
                       && std::is_same_v<std::remove_const_t<T4>, std::remove_const_t<Tout>> && std::is_arithmetic_v<T1>>>
  {
    void operator()(const array_view<T1, Rank> &i_vin1,
                    const QuaterOp             &op,
                    const array_view<T2, Rank> &i_vin2,
                    const array_view<T3, Rank> &i_vin3,
                    const array_view<T4, Rank> &i_vin4,
                    array_view<Tout, Rank>     &o_vout) const POUTRE_NOEXCEPTONLYNDEBUG
    {
      POUTRE_ENTERING("PixelWiseQuaternaryOpDispatcher both arrayview");
      auto i_vinbeg1 = i_vin1.data();
      auto i_vinend1 = i_vin1.data() + i_vin1.size();
      auto i_vinbeg2 = i_vin2.data();
      auto i_vinbeg3 = i_vin3.data();
      auto i_vinbeg4 = i_vin4.data();
      auto i_voutbeg = o_vout.data();
      simd::transform(i_vinbeg1, i_vinend1, i_vinbeg2, i_vinbeg3, i_vinbeg4, i_voutbeg, op);
    }
  };

  template<typename T1,
           typename T2,
           typename T3,
           typename T4,
           typename Tout,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2,
           template<typename, ptrdiff_t>
           class View3,
           template<typename, ptrdiff_t>
           class View4,
           template<typename, ptrdiff_t>
           class ViewOut,
           class QuaterOp>
  void PixelWiseQuaternaryOp(const View1<T1, Rank> &i_vin1,
                             const QuaterOp        &op,
                             const View2<T2, Rank> &i_vin2,
                             const View3<T3, Rank> &i_vin3,
                             const View4<T4, Rank> &i_vin4,
                             ViewOut<Tout, Rank>   &o_vout) POUTRE_NOEXCEPTONLYNDEBUG
  {
    POUTRE_ASSERTCHECK(i_vin1.size() == i_vin2.size(), "Incompatible views size");
    POUTRE_ASSERTCHECK(i_vin2.size() == i_vin3.size(), "Incompatible views size");
    POUTRE_ASSERTCHECK(i_vin3.size() == i_vin4.size(), "Incompatible views size");
    POUTRE_ASSERTCHECK(o_vout.size() == i_vin4.size(), "Incompatible views size");
    PixelWiseQuaternaryOpDispatcher<T1, T2, T3, T4, Tout, Rank, View1, View2, View3, View4, ViewOut, QuaterOp> dispatcher;
    dispatcher(i_vin1, op, i_vin2, i_vin3, i_vin4, o_vout);
  }
  //! @} doxygroup: image_processing_linear_group
} // namespace poutre
