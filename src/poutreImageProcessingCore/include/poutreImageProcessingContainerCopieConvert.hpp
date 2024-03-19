
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file poutreImageProcessingContainerCopieConvert.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>

namespace poutre
{
  /**
   * @addtogroup image_processing_copy_group Image Processing Template Copy/Convert facilities
   * @ingroup image_processing_group
   *@{
   */
  // primary use strided view
  template<typename T1,
           typename T2,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2>
  struct CopyOpDispatcher
  {
    void operator()(const View1<T1, Rank> &i_vin, View2<T2, Rank> &o_vout) const
    {
      // More runtime dispatch
      auto vInbound   = i_vin.bound();
      auto vOutbound  = o_vout.bound();
      auto stridevIN  = i_vin.stride();
      auto stridevOut = o_vout.stride();

      if( vInbound == vOutbound && stridevIN == stridevOut ) // same bound + same stride -> one idx
      {
        auto beg1 = begin(vInbound);
        auto end1 = end(vInbound);
        for( ; beg1 != end1; ++beg1 )
        {
          o_vout[*beg1] = static_cast<T2>((i_vin[*beg1]));
        }
        // TODO stride==1 in least significant dimension
      }
      else // default two idx
      {
        // std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
        auto beg1 = begin(vInbound);
        auto end1 = end(vInbound);
        auto beg2 = begin(vOutbound);
        for( ; beg1 != end1; ++beg1, ++beg2 )
        {
          o_vout[*beg2] = static_cast<T2>(i_vin[*beg1]);
        }
      }
    }
  };

  // template specialization both array_view but different type
  template<typename T1, typename T2, ptrdiff_t Rank> struct CopyOpDispatcher<T1, T2, Rank, array_view, array_view>
  {
    void operator()(const array_view<T1, Rank> &i_vin, array_view<T2, Rank> &o_vout) const
    {
      auto i_vinbeg  = i_vin.data();
      auto i_vinend  = i_vin.data() + i_vin.size();
      auto i_voutbeg = o_vout.data();
      for( ; i_vinbeg != i_vinend; i_vinbeg++, i_voutbeg++ )
      {
        *i_voutbeg = static_cast<T2>(*i_vinbeg);
      }
    }
  };

  // template specialization both array_view and same type, use memcopy
  template<typename T, ptrdiff_t Rank> struct CopyOpDispatcher<T, T, Rank, array_view, array_view>
  {
    void operator()(const array_view<T, Rank> &i_vin, array_view<T, Rank> &o_vout) const
    {
      std::memcpy((void *)o_vout.data(), (void *)i_vin.data(), sizeof(T) * i_vin.size());
    }
  };

  // Quiet stupid here we rewrite Processing unary op without simd specialization
  template<typename T1,
           typename T2,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class View1,
           template<typename, ptrdiff_t>
           class View2>
  void t_Copy(const View1<T1, Rank> &i_vin, View2<T2, Rank> &o_vout)
  {
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    CopyOpDispatcher<T1, T2, Rank, View1, View2> dispatcher;
    dispatcher(i_vin, o_vout);
  }

  template<typename T1, typename T2, ptrdiff_t Rank>
  void t_Copy(const DenseImage<T1, Rank> &i_image, DenseImage<T2, Rank> &o_image)
  {
    auto viewIn  = lview(i_image);
    auto viewOut = lview(o_image);
    t_Copy(viewIn, viewOut);
  }

  template<typename T, ptrdiff_t Rank>
  std::unique_ptr<DenseImage<T, Rank>> t_CloneGeometry(const DenseImage<T, Rank> &i_image)
  {
    return std::make_unique<DenseImage<T, Rank>>(i_image.GetCoords());
  }

  template<typename T, ptrdiff_t Rank> std::unique_ptr<DenseImage<T, Rank>> t_Clone(const DenseImage<T, Rank> &i_image)
  {
    auto res = std::make_unique<DenseImage<T, Rank>>(i_image.GetCoords());
    t_Copy(i_image, *res);
    return res;
  }
  //! @} doxygroup: image_processing_copy_group
} // namespace poutre
