
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_TRANSPOSE_HPP__
#define POUTRE_IMAGEPROCESSING_TRANSPOSE_HPP__

/**
 * @file poutreImageProcessingTranspose.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief 2D transpose
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#endif

namespace poutre
{
    /**
     * @addtogroup image_processing_linear_group
     *@{
     */
    /***********************************************************************************************************************************/
    /*                                                  2D transpose along x */
    /************************************************************
     * **********************************************************************/
    // primary use strided view
    template <typename T1, typename Tout, typename = void> struct t_transposeOp
    {
        // static_assert(false, "Not implemented for strided view");
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

    // template specialization both array_view
    template <typename T1, typename Tout> struct t_transposeOp<T1, Tout>
    {

        void operator()(const DenseImage<T1, 2> &i_vin1, DenseImage<Tout, 2> &o_vout) const
        {
            // check bound compatibility
            auto ibd = i_vin1.bound();
            auto obd = o_vout.bound();
            scoord oysize = obd[0];
            scoord oxsize = obd[1];
            scoord ysize = ibd[0];
            scoord xsize = ibd[1];

            scoord oxstep = o_vout.stride()[0];
            scoord xstep = i_vin1.stride()[0];

            POUTRE_CHECK(oysize == xsize, "ibd[0]!=obd[1] bound not compatible");
            POUTRE_CHECK(oxsize == ysize, "ibd[1]!=obd[0] bound not compatible");

            auto i_vinbeg1 = i_vin1.data();
            auto o_voutbeg = o_vout.data();

            for (scoord y = 0; y < xsize; y++)
                for (scoord x = 0; x < ysize; x++)
                    o_voutbeg[oxstep * y + x] = i_vinbeg1[xstep * x + y];
        }
    };

#ifdef __SSE2__
/*!
* @brief Transposition of 16x16 block using SSE2 approach
Get Row r0,r1,r2,r3,r4,r5,r6,r7,r8,	r9,r10,r11,r12,r13,r14,r15
and do transposition
* @return void
* @throw
* @note
* @warning
*/
#define tblock16x16(tmp1, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15)                        \
    /*FIRST STEP GROUP Byte To Word*/                                                                                  \
    tmp1 = _mm_unpackhi_epi8(r0, r1);                                                                                  \
    r1 = _mm_unpacklo_epi8(r0, r1);                                                                                    \
    r0 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi8(r2, r3);                                                                                  \
    r3 = _mm_unpacklo_epi8(r2, r3);                                                                                    \
    r2 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi8(r4, r5);                                                                                  \
    r5 = _mm_unpacklo_epi8(r4, r5);                                                                                    \
    r4 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi8(r6, r7);                                                                                  \
    r7 = _mm_unpacklo_epi8(r6, r7);                                                                                    \
    r6 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi8(r8, r9);                                                                                  \
    r9 = _mm_unpacklo_epi8(r8, r9);                                                                                    \
    r8 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi8(r10, r11);                                                                                \
    r11 = _mm_unpacklo_epi8(r10, r11);                                                                                 \
    r10 = tmp1;                                                                                                        \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi8(r12, r13);                                                                                \
    r13 = _mm_unpacklo_epi8(r12, r13);                                                                                 \
    r12 = tmp1;                                                                                                        \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi8(r14, r15);                                                                                \
    r15 = _mm_unpacklo_epi8(r14, r15);                                                                                 \
    r14 = tmp1;                                                                                                        \
                                                                                                                       \
    /*SECOND STEP GROUP Word to DWord*/                                                                                \
    tmp1 = _mm_unpackhi_epi16(r0, r2);                                                                                 \
    r2 = _mm_unpacklo_epi16(r0, r2);                                                                                   \
    r0 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi16(r1, r3);                                                                                 \
    r3 = _mm_unpacklo_epi16(r1, r3);                                                                                   \
    r1 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi16(r4, r6);                                                                                 \
    r6 = _mm_unpacklo_epi16(r4, r6);                                                                                   \
    r4 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi16(r5, r7);                                                                                 \
    r7 = _mm_unpacklo_epi16(r5, r7);                                                                                   \
    r5 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi16(r8, r10);                                                                                \
    r10 = _mm_unpacklo_epi16(r8, r10);                                                                                 \
    r8 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi16(r9, r11);                                                                                \
    r11 = _mm_unpacklo_epi16(r9, r11);                                                                                 \
    r9 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi16(r12, r14);                                                                               \
    r14 = _mm_unpacklo_epi16(r12, r14);                                                                                \
    r12 = tmp1;                                                                                                        \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi16(r13, r15);                                                                               \
    r15 = _mm_unpacklo_epi16(r13, r15);                                                                                \
    r13 = tmp1;                                                                                                        \
                                                                                                                       \
    /*THIRD STEP GROUP DWord to QWord */                                                                               \
    tmp1 = _mm_unpackhi_epi32(r0, r4);                                                                                 \
    r4 = _mm_unpacklo_epi32(r0, r4);                                                                                   \
    r0 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi32(r1, r5);                                                                                 \
    r5 = _mm_unpacklo_epi32(r1, r5);                                                                                   \
    r1 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi32(r2, r6);                                                                                 \
    r6 = _mm_unpacklo_epi32(r2, r6);                                                                                   \
    r2 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi32(r3, r7);                                                                                 \
    r7 = _mm_unpacklo_epi32(r3, r7);                                                                                   \
    r3 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi32(r8, r12);                                                                                \
    r12 = _mm_unpacklo_epi32(r8, r12);                                                                                 \
    r8 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi32(r9, r13);                                                                                \
    r13 = _mm_unpacklo_epi32(r9, r13);                                                                                 \
    r9 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi32(r10, r14);                                                                               \
    r14 = _mm_unpacklo_epi32(r10, r14);                                                                                \
    r10 = tmp1;                                                                                                        \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi32(r11, r15);                                                                               \
    r15 = _mm_unpacklo_epi32(r11, r15);                                                                                \
    r11 = tmp1;                                                                                                        \
                                                                                                                       \
    /*FOURTH STEP GROUP QWord to DQWord */                                                                             \
    tmp1 = _mm_unpackhi_epi64(r0, r8);                                                                                 \
    r8 = _mm_unpacklo_epi64(r0, r8);                                                                                   \
    r0 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi64(r1, r9);                                                                                 \
    r9 = _mm_unpacklo_epi64(r1, r9);                                                                                   \
    r1 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi64(r2, r10);                                                                                \
    r10 = _mm_unpacklo_epi64(r2, r10);                                                                                 \
    r2 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi64(r3, r11);                                                                                \
    r11 = _mm_unpacklo_epi64(r3, r11);                                                                                 \
    r3 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi64(r4, r12);                                                                                \
    r12 = _mm_unpacklo_epi64(r4, r12);                                                                                 \
    r4 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi64(r5, r13);                                                                                \
    r13 = _mm_unpacklo_epi64(r5, r13);                                                                                 \
    r5 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi64(r6, r14);                                                                                \
    r14 = _mm_unpacklo_epi64(r6, r14);                                                                                 \
    r6 = tmp1;                                                                                                         \
                                                                                                                       \
    tmp1 = _mm_unpackhi_epi64(r7, r15);                                                                                \
    r15 = _mm_unpacklo_epi64(r7, r15);                                                                                 \
    r7 = tmp1

// https://stackoverflow.com/questions/13257166/print-a-m128i-variable
// void p128_hex_u8(__m128i in)
//      {
//        alignas(16) uint8_t v[16];
//        _mm_store_si128((__m128i*)v, in);
//        printf("v16_u8: %x %x %x %x | %x %x %x %x | %x %x %x %x | %x %x %x
//        %x\n",
//         v[0], v[1],  v[2],  v[3],  v[4],  v[5],  v[6],  v[7],
//         v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
//      }

// https ://software.intel.com/sites/landingpage/IntrinsicsGuide/
#include <emmintrin.h>
    template <> struct t_transposeOp<pUINT8, pUINT8>
    {

        void operator()(const DenseImage<pUINT8, 2> &i_vin1, DenseImage<pUINT8, 2> &o_vout) const
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
            scoord oxstep = o_vout.stride()[0];
            scoord xstep = i_vin1.stride()[0];
            scoord ySizeSimdLoop = ysize - (ysize % loopStep);
            scoord xSizeSimdLoop = xsize - (xsize % loopStep);
            scoord x, y;
            // enforce no loop in case one of the two coordinate have size below
            // loopStep
            if (ySizeSimdLoop == 0 || xSizeSimdLoop == 0)
            {
                ySizeSimdLoop = 0;
                xSizeSimdLoop = 0;
            }
            __m128i tmp1, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;
            for (y = 0; y < ySizeSimdLoop; y += loopStep)
            {
                for (x = 0; x < xSizeSimdLoop; x += loopStep)
                {
                    r0 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 0) * xstep + x));
                    r1 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 1) * xstep + x));
                    r2 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 2) * xstep + x));
                    r3 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 3) * xstep + x));
                    r4 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 4) * xstep + x)); //-V112
                    r5 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 5) * xstep + x));
                    r6 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 6) * xstep + x));
                    r7 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 7) * xstep + x));
                    r8 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 8) * xstep + x));
                    r9 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 9) * xstep + x));
                    r10 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 10) * xstep + x));
                    r11 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 11) * xstep + x));
                    r12 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 12) * xstep + x));
                    r13 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 13) * xstep + x));
                    r14 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 14) * xstep + x));
                    r15 = _mm_loadu_si128((__m128i const *)(i_vinbeg1 + (y + 15) * xstep + x));
                    // p128_hex_u8(r0);
                    // p128_hex_u8(r1);

                    tblock16x16(tmp1, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15);

                    // p128_hex_u8(r15);
                    // p128_hex_u8(r14);

                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 0) * oxstep + y), r15);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 1) * oxstep + y), r7);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 2) * oxstep + y), r11);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 3) * oxstep + y), r3);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 4) * oxstep + y), r13); //-V112
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 5) * oxstep + y), r5);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 6) * oxstep + y), r9);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 7) * oxstep + y), r1);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 8) * oxstep + y), r14);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 9) * oxstep + y), r6);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 10) * oxstep + y), r10);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 11) * oxstep + y), r2);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 12) * oxstep + y), r12);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 13) * oxstep + y), r4);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 14) * oxstep + y), r8);
                    _mm_storeu_si128((__m128i *)(o_voutbeg + (x + 15) * oxstep + y), r0);
                }
            }
            // epilogue
            /*for(scoord y=0;y<xsize;y++)
               for(scoord x=0;x<ysize;x++)
                  o_voutbeg[oxsize*y+x] =i_vinbeg1[xsize*x+y];
            */
            for (y = ySizeSimdLoop; y < ysize; y++)
            {
                for (x = 0; x < xSizeSimdLoop; x++)
                {
                    o_voutbeg[oxstep * y + x] = i_vinbeg1[xstep * x + y];
                }
            }
            for (y = 0; y < ySizeSimdLoop; y++)
            {
                for (x = xSizeSimdLoop; x < xsize; x++)
                {
                    o_voutbeg[oxstep * y + x] = i_vinbeg1[xstep * x + y];
                }
            }
            for (y = ySizeSimdLoop; y < ysize; y++)
            {
                for (x = xSizeSimdLoop; x < xsize; x++)
                {
                    o_voutbeg[oxstep * y + x] = i_vinbeg1[xstep * x + y];
                }
            }
        }
    };
#endif //__SSE2__
       // Note slower than simple double loop dur probably to unaligned load
    // #ifdef __AVX__

    //    //see
    //    https://stackoverflow.com/questions/25622745/transpose-an-8x8-float-using-avx-avx2
    //    template<>
    //    struct t_transposeOp<pFLOAT, pFLOAT>
    //    {
    //       void operator()(const DenseImage<pFLOAT,2>&
    //       i_vin1,DenseImage<pFLOAT,2>& o_vout) const
    //       {
    //          constexpr scoord loopStep = 8;
    //          auto ibd = i_vin1.bound();
    //          auto obd = o_vout.bound();
    //          POUTRE_CHECK(ibd[0] = obd[1], "ibd[0]!=obd[1] bound not
    //          compatible"); POUTRE_CHECK(ibd[1] = obd[0], "ibd[1]!=obd[0] bound
    //          not compatible"); auto i_vinbeg1 = i_vin1.data(); auto o_voutbeg =
    //          o_vout.data(); scoord ysize = ibd[0]; scoord xsize = ibd[1]; scoord
    //          oxsize = obd[1]; scoord ySizeSimdLoop = ysize - (ysize%loopStep);
    //          scoord xSizeSimdLoop = xsize - (xsize%loopStep);
    //          scoord x, y;
    //          //enforce no loop in case one of the two coordinate have size below
    //          loopStep if (ySizeSimdLoop == 0 || xSizeSimdLoop == 0)
    //          {
    //             ySizeSimdLoop = 0;
    //             xSizeSimdLoop = 0;
    //          }
    //          __m256  r0, r1, r2, r3, r4, r5, r6, r7;
    //          __m256  t0, t1, t2, t3, t4, t5, t6, t7;
    //          for (y = 0; y < ySizeSimdLoop; y += loopStep)
    //          {
    //             for (x = 0; x < xSizeSimdLoop; x += loopStep)
    //             {

    //                //todo check loading through avx
    //                r0 =
    //                _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_loadu_ps(i_vin1[y
    //                + 0].data() + x + 0)), _mm_loadu_ps(i_vin1[y + 4].data() + x +
    //                0), 1); //-V112 r1 =
    //                _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_loadu_ps(i_vin1[y
    //                + 1].data() + x + 0)), _mm_loadu_ps(i_vin1[y + 5].data() + x +
    //                0), 1); r2 =
    //                _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_loadu_ps(i_vin1[y
    //                + 2].data() + x + 0)), _mm_loadu_ps(i_vin1[y + 6].data() + x +
    //                0), 1); r3 =
    //                _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_loadu_ps(i_vin1[y
    //                + 3].data() + x + 0)), _mm_loadu_ps(i_vin1[y + 7].data() + x +
    //                0), 1); r4 =
    //                _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_loadu_ps(i_vin1[y
    //                + 0].data() + x + 4)), _mm_loadu_ps(i_vin1[y + 4].data() + x +
    //                4), 1); //-V112 r5 =
    //                _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_loadu_ps(i_vin1[y
    //                + 1].data() + x + 4)), _mm_loadu_ps(i_vin1[y + 5].data() + x +
    //                4), 1); //-V112 r6 =
    //                _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_loadu_ps(i_vin1[y
    //                + 2].data() + x + 4)), _mm_loadu_ps(i_vin1[y + 6].data() + x +
    //                4), 1); //-V112 r7 =
    //                _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_loadu_ps(i_vin1[y
    //                + 3].data() + x + 4)), _mm_loadu_ps(i_vin1[y + 7].data() + x +
    //                4), 1); //-V112

    //                t0 = _mm256_unpacklo_ps(r0, r1);
    //                t1 = _mm256_unpackhi_ps(r0, r1);
    //                t2 = _mm256_unpacklo_ps(r2, r3);
    //                t3 = _mm256_unpackhi_ps(r2, r3);
    //                t4 = _mm256_unpacklo_ps(r4, r5);
    //                t5 = _mm256_unpackhi_ps(r4, r5);
    //                t6 = _mm256_unpacklo_ps(r6, r7);
    //                t7 = _mm256_unpackhi_ps(r6, r7);

    //                r0 = _mm256_shuffle_ps(t0,t2, 0x44);
    //                r1 = _mm256_shuffle_ps(t0,t2, 0xEE);
    //                r2 = _mm256_shuffle_ps(t1,t3, 0x44);
    //                r3 = _mm256_shuffle_ps(t1,t3, 0xEE);
    //                r4 = _mm256_shuffle_ps(t4,t6, 0x44);
    //                r5 = _mm256_shuffle_ps(t4,t6, 0xEE);
    //                r6 = _mm256_shuffle_ps(t5,t7, 0x44);
    //                r7 = _mm256_shuffle_ps(t5,t7, 0xEE);
    //                //__m256 v;
    //                //v = _mm256_shuffle_ps(t0, t2, 0x4E);
    //                //r0 = _mm256_blend_ps(t0, v, 0xCC);
    //                //r1 = _mm256_blend_ps(t2, v, 0x33);

    //                //v = _mm256_shuffle_ps(t1, t3, 0x4E);
    //                //r2 = _mm256_blend_ps(t1, v, 0xCC);
    //                //r3 = _mm256_blend_ps(t3, v, 0x33);

    //                //v = _mm256_shuffle_ps(t4, t6, 0x4E);
    //                //r4 = _mm256_blend_ps(t4, v, 0xCC);
    //                //r5 = _mm256_blend_ps(t6, v, 0x33);

    //                //v = _mm256_shuffle_ps(t5, t7, 0x4E);
    //                //r6 = _mm256_blend_ps(t5, v, 0xCC);
    //                //r7 = _mm256_blend_ps(t7, v, 0x33);

    //                _mm256_storeu_ps(o_vout[x + 0].data() + y, r0);
    //                _mm256_storeu_ps(o_vout[x + 1].data() + y, r1);
    //                _mm256_storeu_ps(o_vout[x + 2].data() + y, r2);
    //                _mm256_storeu_ps(o_vout[x + 3].data() + y, r3);
    //                _mm256_storeu_ps(o_vout[x + 4].data() + y, r4); //-V112
    //                _mm256_storeu_ps(o_vout[x + 5].data() + y, r5);
    //                _mm256_storeu_ps(o_vout[x + 6].data() + y, r6);
    //                _mm256_storeu_ps(o_vout[x + 7].data() + y, r7);
    //             }
    //          }

    //          //epilogue
    //          for (y = ySizeSimdLoop; y < ysize; y++)
    //          {
    //             for (x = 0; x < xSizeSimdLoop; x++)
    //             {
    //                o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
    //             }
    //          }
    //          for (y = 0; y < ySizeSimdLoop; y++)
    //          {
    //             for (x = xSizeSimdLoop; x < xsize; x++)
    //             {
    //                o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
    //             }
    //          }
    //          for (y = ySizeSimdLoop; y < ysize; y++)
    //          {
    //             for (x = xSizeSimdLoop; x < xsize; x++)
    //             {
    //                o_voutbeg[x*oxsize + y] = i_vinbeg1[x + y * xsize];
    //             }
    //          }
    //       }
    //    };
    // #endif //__AVX__
    template <typename T1, typename T2> //, template <typename> class ImageIn,
                                        // template <typename> class ImageOut >
    void t_transpose(const DenseImage<T1, 2> &i_vin1, DenseImage<T2, 2> &o_vout)
    {
        auto op = t_transposeOp<T1, T2>();
        op(i_vin1, o_vout);
    }
    //! @} doxygroup: image_processing_linear_group
} // namespace poutre

#endif // POUTRE_IMAGEPROCESSING_TRANSPOSE_HPP__
