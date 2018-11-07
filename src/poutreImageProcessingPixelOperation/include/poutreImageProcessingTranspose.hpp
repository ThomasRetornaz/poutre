
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_TRANSPOSE_HPP__
#define POUTRE_IMAGEPROCESSING_TRANSPOSE_HPP__

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


#ifdef USE_BOOSTSIMD
 //https://developer.numscale.com/boost.simd/documentation/develop/group__group-arithmetic.html
 //https://developer.numscale.com/bsimd/documentation/v1.17.6.0/

#else
#include <simdpp/simd.h>
#endif

namespace poutre
{
    /***********************************************************************************************************************************/
    /*                                                  2D transpose along x                                                           */
    /************************************************************ **********************************************************************/
    // primary use strided view
    template<typename T1, typename Tout, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class ViewOut,typename = void>
    struct transpose2DAlongXOp
    {
      static_assert((
         std::is_same< View1<T1, 2>, strided_array_view<T1, 2> >::value
         || std::is_same< View1<T1, 2>, strided_array_view<const T1, 2> >::value         
         || std::is_same< ViewOut<Tout, 2>, strided_array_view<Tout, 2> >::value
         ), "strided view only specilization fail for arrayview");

      void operator()(const View1<T1, 2>& i_vin,ViewOut<Tout, 2>& o_vout) const
      {
         auto vInbound = i_vin.bound();
         auto vOutbound = o_vout.bound();
         auto stridevIN = i_vin.stride();
         auto stridevOut = o_vout.stride();
         POUTRE_CHECK(vInbound[0] = vOutbound[1], "ibd[0]!=obd[1] bound not compatible");
         POUTRE_CHECK(vInbound[1] = vOutbound[0], "ibd[1]!=obd[0] bound not compatible");
         auto beg1 = begin(vInbound);
         auto end1 = end(vInbound);
         auto beg2 = begin(vOutbound);
         for (; beg1 != end1; ++beg1,++beg2)
         {
            
            auto idx = *beg1;
            auto idxout = *beg2;
            auto temp = idxout;
            idxout[0] = temp[1];
            idxout[1] = temp[0];
            std::cout << idx <<" "<< i_vin[idx]<<" "<< idxout<<std::endl;
            o_vout[idxout] = static_cast<Tout>(i_vin[idx]);
         }
      }
   };

    //template specialization both array_view
    template<typename T1, typename Tout>
    struct transpose2DAlongXOp<T1,Tout,array_view,array_view>
    {

       void operator()(const array_view<T1, 2>& i_vin1,array_view<Tout, 2>& o_vout) const
       {
          //chack bound compatibility
          auto ibd = i_vin1.bound();
          auto obd = o_vout.bound();
          auto xsize = ibd[0];
          auto ysize = ibd[1];
          auto oxsize = obd[0];

          POUTRE_CHECK(ibd[0] = obd[1], "ibd[0]!=obd[1] bound not compatible");
          POUTRE_CHECK(ibd[1] = obd[0], "ibd[1]!=obd[0] bound not compatible");
          auto i_vinbeg1 = i_vin1.data();
          auto i_voutbeg = o_vout.data();
          
          for (auto y = 0u; y < ysize; ++y)
          {
             for (auto x = 0u; x < xsize; ++x)
             {
                i_voutbeg[x*oxsize + y]= i_vinbeg1[x+y*xsize];
             }
          }
       }

    };

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2 >
    void t_transpose2DAlongX(const View1<T1, Rank>& i_vin1, View2<T2, Rank>& o_vout)
    {
       auto op = transpose2DAlongXOp<T1, T2, View1, View2>();
       op(i_vin1, o_vout);
    }
}//namespace poutre  

#endif//POUTRE_IMAGEPROCESSING_TRANSPOSE_HPP__  