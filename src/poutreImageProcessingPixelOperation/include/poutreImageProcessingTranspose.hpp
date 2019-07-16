
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
#include "boost/simd/pack.hpp"
#include "boost/simd/function/load.hpp"
#include "boost/simd/function/store.hpp"
#include "boost/simd/function/aligned_load.hpp"
#include "boost/simd/function/aligned_store.hpp"
#else
#include <simdpp/simd.h>
#endif

namespace poutre
{
   /***********************************************************************************************************************************/
   /*                                                  2D transpose along x                                                           */
   /************************************************************ **********************************************************************/
   // primary use strided view
   template<typename T1, typename Tout, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class ViewOut, typename = void>
   struct transpose2DAlongXOp
   {
      // static_assert((
      //    std::is_same< View1<T1, 2>, strided_array_view<T1, 2> >::value
      //    || std::is_same< View1<T1, 2>, strided_array_view<const T1, 2> >::value
      //    || std::is_same< ViewOut<Tout, 2>, strided_array_view<Tout, 2> >::value
      //    ), "strided view only specialization fail for arrayview");

      // void operator()(const View1<T1, 2>& i_vin, ViewOut<Tout, 2>& o_vout) const
      // {
      //    static_assert(false, "Not implemented for strided view");
      // }
   };

   //template specialization both array_view
   template<typename T1, typename Tout>
   struct transpose2DAlongXOp<T1, Tout, array_view, array_view>
   {

      void operator()(const array_view<T1, 2>& i_vin1, array_view<Tout, 2>& o_vout) const
      {
         //chack bound compatibility
         auto ibd = i_vin1.bound();
         auto obd = o_vout.bound();
         scoord  ysize = ibd[0];
         scoord  xsize = ibd[1];
         scoord  oxsize = obd[1];

         POUTRE_CHECK(ibd[0] == obd[1], "ibd[0]!=obd[1] bound not compatible");
         POUTRE_CHECK(ibd[1] == obd[0], "ibd[1]!=obd[0] bound not compatible");

         auto i_vinbeg1 = i_vin1.data();
         auto i_voutbeg = o_vout.data();
         for (scoord y = 0u; y < ysize; ++y)
         {
            for (scoord  x = 0u; x < xsize; ++x)
            {
               i_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
            }
         }
      }

   };
   //TODO arm64 
#ifdef __AVX__
    //https ://software.intel.com/sites/landingpage/IntrinsicsGuide/
   #include <immintrin.h>
   template<>
   struct transpose2DAlongXOp<pUINT8, pUINT8, array_view, array_view>
   {

      void operator()(const array_view<pUINT8, 2>& i_vin1, array_view<pUINT8, 2>& o_vout) const
      {
         constexpr scoord loopStep = 16;
         auto ibd = i_vin1.bound();
         auto obd = o_vout.bound();
         POUTRE_CHECK(ibd[0] = obd[1], "ibd[0]!=obd[1] bound not compatible");
         POUTRE_CHECK(ibd[1] = obd[0], "ibd[1]!=obd[0] bound not compatible");
         auto i_vinbeg1 = i_vin1.data();
         auto o_voutbeg = o_vout.data();
         scoord ysize = ibd[0];
         scoord xsize = ibd[1];
         scoord oxsize = obd[1];
         scoord ySizeSimdLoop = ysize - (ysize%loopStep);
         scoord xSizeSimdLoop = xsize - (xsize%loopStep);
         scoord x, y;
         //enforce no loop in case one of the two coordinate have size below loopStep
         if (ySizeSimdLoop == 0 || xSizeSimdLoop == 0)
         {
            ySizeSimdLoop = 0;
            xSizeSimdLoop = 0;
         }
         __m128i  temp, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;
         for (y = 0; y < ySizeSimdLoop; y += loopStep)
         {
            for (x = 0; x < xSizeSimdLoop; x += loopStep)
            {
               /* __m256i temp0, temp1, temp2, temp3, temp4,temp5,temp6,temp7;
                temp0 = _mm256_lddqu_si256((__m256i const *)(i_vin1[y + 0].data() + x + 0));
                temp1 = _mm256_lddqu_si256((__m256i const *)(i_vin1[y + 0].data() + x + 0));*/
               r0 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 0].data() + x));
               r1 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 1].data() + x));
               r2 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 2].data() + x));
               r3 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 3].data() + x));
               r4 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 4].data() + x)); //-V112
               r5 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 5].data() + x));
               r6 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 6].data() + x));
               r7 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 7].data() + x));
               r8 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 8].data() + x));
               r9 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 9].data() + x));
               r10 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 10].data() + x));
               r11 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 11].data() + x));
               r12 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 12].data() + x));
               r13 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 13].data() + x));
               r14 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 14].data() + x));
               r15 = _mm_loadu_si128((__m128i const*)(i_vin1[y + 15].data() + x));

               //FIRST STEP GROUP Byte To Word
               temp = _mm_unpackhi_epi8(r0, r1);
               r1 = _mm_unpacklo_epi8(r0, r1);
               r0 = temp;

               temp = _mm_unpackhi_epi8(r2, r3);
               r3 = _mm_unpacklo_epi8(r2, r3);
               r2 = temp;

               temp = _mm_unpackhi_epi8(r4, r5);
               r5 = _mm_unpacklo_epi8(r4, r5);
               r4 = temp;

               temp = _mm_unpackhi_epi8(r6, r7);
               r7 = _mm_unpacklo_epi8(r6, r7);
               r6 = temp;

               temp = _mm_unpackhi_epi8(r8, r9);
               r9 = _mm_unpacklo_epi8(r8, r9);
               r8 = temp;
               temp = _mm_unpackhi_epi8(r10, r11);
               r11 = _mm_unpacklo_epi8(r10, r11);
               r10 = temp;

               temp = _mm_unpackhi_epi8(r12, r13);
               r13 = _mm_unpacklo_epi8(r12, r13);
               r12 = temp;

               temp = _mm_unpackhi_epi8(r14, r15);
               r15 = _mm_unpacklo_epi8(r14, r15);
               r14 = temp;

               //SECOND STEP GROUP Word to DWord
               temp = _mm_unpackhi_epi16(r0, r2);
               r2 = _mm_unpacklo_epi16(r0, r2);
               r0 = temp;

               temp = _mm_unpackhi_epi16(r1, r3);
               r3 = _mm_unpacklo_epi16(r1, r3);
               r1 = temp;

               temp = _mm_unpackhi_epi16(r4, r6);
               r6 = _mm_unpacklo_epi16(r4, r6);
               r4 = temp;

               temp = _mm_unpackhi_epi16(r5, r7);
               r7 = _mm_unpacklo_epi16(r5, r7);
               r5 = temp;

               temp = _mm_unpackhi_epi16(r8, r10);
               r10 = _mm_unpacklo_epi16(r8, r10);
               r8 = temp;

               temp = _mm_unpackhi_epi16(r9, r11);
               r11 = _mm_unpacklo_epi16(r9, r11);
               r9 = temp;

               temp = _mm_unpackhi_epi16(r12, r14);
               r14 = _mm_unpacklo_epi16(r12, r14);
               r12 = temp;

               temp = _mm_unpackhi_epi16(r13, r15);
               r15 = _mm_unpacklo_epi16(r13, r15);
               r13 = temp;

               //THIRD STEP GROUP DWord to QWord 
               temp = _mm_unpackhi_epi32(r0, r4);
               r4 = _mm_unpacklo_epi32(r0, r4);
               r0 = temp;

               temp = _mm_unpackhi_epi32(r1, r5);
               r5 = _mm_unpacklo_epi32(r1, r5);
               r1 = temp;

               temp = _mm_unpackhi_epi32(r2, r6);
               r6 = _mm_unpacklo_epi32(r2, r6);
               r2 = temp;

               temp = _mm_unpackhi_epi32(r3, r7);
               r7 = _mm_unpacklo_epi32(r3, r7);
               r3 = temp;

               temp = _mm_unpackhi_epi32(r8, r12);
               r12 = _mm_unpacklo_epi32(r8, r12);
               r8 = temp;

               temp = _mm_unpackhi_epi32(r9, r13);
               r13 = _mm_unpacklo_epi32(r9, r13);
               r9 = temp;

               temp = _mm_unpackhi_epi32(r10, r14);
               r14 = _mm_unpacklo_epi32(r10, r14);
               r10 = temp;

               temp = _mm_unpackhi_epi32(r11, r15);
               r15 = _mm_unpacklo_epi32(r11, r15);
               r11 = temp;

               //FOURTH STEP GROUP QWord to DQWord 
               temp = _mm_unpackhi_epi64(r0, r8);
               r8 = _mm_unpacklo_epi64(r0, r8);
               r0 = temp;

               temp = _mm_unpackhi_epi64(r1, r9);
               r9 = _mm_unpacklo_epi64(r1, r9);
               r1 = temp;

               temp = _mm_unpackhi_epi64(r2, r10);
               r10 = _mm_unpacklo_epi64(r2, r10);
               r2 = temp;

               temp = _mm_unpackhi_epi64(r3, r11);
               r11 = _mm_unpacklo_epi64(r3, r11);
               r3 = temp;

               temp = _mm_unpackhi_epi64(r4, r12);
               r12 = _mm_unpacklo_epi64(r4, r12);
               r4 = temp;

               temp = _mm_unpackhi_epi64(r5, r13);
               r13 = _mm_unpacklo_epi64(r5, r13);
               r5 = temp;

               temp = _mm_unpackhi_epi64(r6, r14);
               r14 = _mm_unpacklo_epi64(r6, r14);
               r6 = temp;

               temp = _mm_unpackhi_epi64(r7, r15);
               r15 = _mm_unpacklo_epi64(r7, r15);
               r7 = temp;

               _mm_storeu_si128((__m128i *)(o_vout[x + 0].data() + y), r15);
               _mm_storeu_si128((__m128i *)(o_vout[x + 1].data() + y), r14);
               _mm_storeu_si128((__m128i *)(o_vout[x + 2].data() + y), r13);
               _mm_storeu_si128((__m128i *)(o_vout[x + 3].data() + y), r12);
               _mm_storeu_si128((__m128i *)(o_vout[x + 4].data() + y), r11); //-V112
               _mm_storeu_si128((__m128i *)(o_vout[x + 5].data() + y), r10);
               _mm_storeu_si128((__m128i *)(o_vout[x + 6].data() + y), r9);
               _mm_storeu_si128((__m128i *)(o_vout[x + 7].data() + y), r8);
               _mm_storeu_si128((__m128i *)(o_vout[x + 8].data() + y), r7);
               _mm_storeu_si128((__m128i *)(o_vout[x + 9].data() + y), r6);
               _mm_storeu_si128((__m128i *)(o_vout[x + 10].data() + y), r5);
               _mm_storeu_si128((__m128i *)(o_vout[x + 11].data() + y), r4);
               _mm_storeu_si128((__m128i *)(o_vout[x + 12].data() + y), r3);
               _mm_storeu_si128((__m128i *)(o_vout[x + 13].data() + y), r2);
               _mm_storeu_si128((__m128i *)(o_vout[x + 14].data() + y), r1);
               _mm_storeu_si128((__m128i *)(o_vout[x + 15].data() + y), r0);
            }
         }
         //epilogue
         for (y = ySizeSimdLoop; y < ysize; y++)
         {
            for (x = 0; x < xSizeSimdLoop; x++)
            {
               o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
            }
         }
         for (y = 0; y < ySizeSimdLoop; y++)
         {
            for (x = xSizeSimdLoop; x < xsize; x++)
            {
               o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
            }
         }
         for (y = ySizeSimdLoop; y < ysize; y++)
         {
            for (x = xSizeSimdLoop; x < xsize; x++)
            {
               o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
            }
         }
      }
   };
   //see https://stackoverflow.com/questions/25622745/transpose-an-8x8-float-using-avx-avx2
   template<>
   struct transpose2DAlongXOp<pFLOAT, pFLOAT, array_view, array_view>
   {

      void operator()(const array_view<pFLOAT, 2>& i_vin1, array_view<pFLOAT, 2>& o_vout) const
      {
         constexpr scoord loopStep = 8;
         auto ibd = i_vin1.bound();
         auto obd = o_vout.bound();
         POUTRE_CHECK(ibd[0] = obd[1], "ibd[0]!=obd[1] bound not compatible");
         POUTRE_CHECK(ibd[1] = obd[0], "ibd[1]!=obd[0] bound not compatible");
         auto i_vinbeg1 = i_vin1.data();
         auto o_voutbeg = o_vout.data();
         scoord ysize = ibd[0];
         scoord xsize = ibd[1];
         scoord oxsize = obd[1];
         scoord ySizeSimdLoop = ysize - (ysize%loopStep);
         scoord xSizeSimdLoop = xsize - (xsize%loopStep);
         scoord x, y;
         //enforce no loop in case one of the two coordinate have size below loopStep
         if (ySizeSimdLoop == 0 || xSizeSimdLoop == 0)
         {
            ySizeSimdLoop = 0;
            xSizeSimdLoop = 0;
         }
         //boost::simd::pack<pFLOAT, 8> tmp, r1, r2, r3, r4, r5, r6, r7, r8;
         __m256  r0, r1, r2, r3, r4, r5, r6, r7;
         __m256  t0, t1, t2, t3, t4, t5, t6, t7;
         for (y = 0; y < ySizeSimdLoop; y += loopStep)
         {
            for (x = 0; x < xSizeSimdLoop; x += loopStep)
            {

               /*r1 = boost::simd::load<boost::simd::pack<pFLOAT, 8>>(i_vin1[y + 0].data() + x);
               r2 = boost::simd::load<boost::simd::pack<pFLOAT, 8>>(i_vin1[y + 1].data() + x);
               r3 = boost::simd::load<boost::simd::pack<pFLOAT, 8>>(i_vin1[y + 2].data() + x);
               r4 = boost::simd::load<boost::simd::pack<pFLOAT, 8>>(i_vin1[y + 3].data() + x);
               r5 = boost::simd::load<boost::simd::pack<pFLOAT, 8>>(i_vin1[y + 4].data() + x);
               r6 = boost::simd::load<boost::simd::pack<pFLOAT, 8>>(i_vin1[y + 5].data() + x);
               r7 = boost::simd::load<boost::simd::pack<pFLOAT, 8>>(i_vin1[y + 6].data() + x);
               r8 = boost::simd::load<boost::simd::pack<pFLOAT, 8>>(i_vin1[y + 7].data() + x);*/
               //boost::simd::store(r8,o_vout[x + 0].data() + y);
               //boost::simd::store(r7,o_vout[x + 1].data() + y);
               //boost::simd::store(r6,o_vout[x + 2].data() + y);
               //boost::simd::store(r5,o_vout[x + 3].data() + y);
               //boost::simd::store(r4,o_vout[x + 4].data() + y);
               //boost::simd::store(r3,o_vout[x + 5].data() + y);
               //boost::simd::store(r2,o_vout[x + 6].data() + y);
               //boost::simd::store(r1,o_vout[x + 7].data() + y);
               //todo check loading through avx
               r0 = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_load_ps(i_vin1[y + 0].data() + x + 0)), _mm_load_ps(i_vin1[y + 4].data() + x + 0), 1); //-V112
               r1 = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_load_ps(i_vin1[y + 1].data() + x + 0)), _mm_load_ps(i_vin1[y + 5].data() + x + 0), 1);
               r2 = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_load_ps(i_vin1[y + 2].data() + x + 0)), _mm_load_ps(i_vin1[y + 6].data() + x + 0), 1);
               r3 = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_load_ps(i_vin1[y + 3].data() + x + 0)), _mm_load_ps(i_vin1[y + 7].data() + x + 0), 1);
               r4 = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_load_ps(i_vin1[y + 0].data() + x + 4)), _mm_load_ps(i_vin1[y + 4].data() + x + 4), 1); //-V112
               r5 = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_load_ps(i_vin1[y + 1].data() + x + 4)), _mm_load_ps(i_vin1[y + 5].data() + x + 4), 1); //-V112
               r6 = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_load_ps(i_vin1[y + 2].data() + x + 4)), _mm_load_ps(i_vin1[y + 6].data() + x + 4), 1); //-V112
               r7 = _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_load_ps(i_vin1[y + 3].data() + x + 4)), _mm_load_ps(i_vin1[y + 7].data() + x + 4), 1); //-V112

               t0 = _mm256_unpacklo_ps(r0, r1);
               t1 = _mm256_unpackhi_ps(r0, r1);
               t2 = _mm256_unpacklo_ps(r2, r3);
               t3 = _mm256_unpackhi_ps(r2, r3);
               t4 = _mm256_unpacklo_ps(r4, r5);
               t5 = _mm256_unpackhi_ps(r4, r5);
               t6 = _mm256_unpacklo_ps(r6, r7);
               t7 = _mm256_unpackhi_ps(r6, r7);

               __m256 v;
               v = _mm256_shuffle_ps(t0, t2, 0x4E);
               r0 = _mm256_blend_ps(t0, v, 0xCC);
               r1 = _mm256_blend_ps(t2, v, 0x33);

               v = _mm256_shuffle_ps(t1, t3, 0x4E);
               r2 = _mm256_blend_ps(t1, v, 0xCC);
               r3 = _mm256_blend_ps(t3, v, 0x33);

               v = _mm256_shuffle_ps(t4, t6, 0x4E);
               r4 = _mm256_blend_ps(t4, v, 0xCC);
               r5 = _mm256_blend_ps(t6, v, 0x33);

               v = _mm256_shuffle_ps(t5, t7, 0x4E);
               r6 = _mm256_blend_ps(t5, v, 0xCC);
               r7 = _mm256_blend_ps(t7, v, 0x33);

               _mm256_store_ps(o_vout[x + 0].data() + y, r0);
               _mm256_store_ps(o_vout[x + 1].data() + y, r1);
               _mm256_store_ps(o_vout[x + 2].data() + y, r2);
               _mm256_store_ps(o_vout[x + 3].data() + y, r3);
               _mm256_store_ps(o_vout[x + 4].data() + y, r4); //-V112
               _mm256_store_ps(o_vout[x + 5].data() + y, r5);
               _mm256_store_ps(o_vout[x + 6].data() + y, r6);
               _mm256_store_ps(o_vout[x + 7].data() + y, r7);
            }
         }

         //epilogue
         for (y = ySizeSimdLoop; y < ysize; y++)
         {
            for (x = 0; x < xSizeSimdLoop; x++)
            {
               o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
            }
         }
         for (y = 0; y < ySizeSimdLoop; y++)
         {
            for (x = xSizeSimdLoop; x < xsize; x++)
            {
               o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
            }
         }
         for (y = ySizeSimdLoop; y < ysize; y++)
         {
            for (x = xSizeSimdLoop; x < xsize; x++)
            {
               o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
            }
         }
      }
   };
#endif

   template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2 >
   void t_transpose2DAlongX(const View1<T1, Rank>& i_vin1, View2<T2, Rank>& o_vout)
   {
      auto op = transpose2DAlongXOp<T1, T2, View1, View2>();
      op(i_vin1, o_vout);
   }
}//namespace poutre  

#endif//POUTRE_IMAGEPROCESSING_TRANSPOSE_HPP__  