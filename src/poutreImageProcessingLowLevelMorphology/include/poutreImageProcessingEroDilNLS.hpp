//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_ERO_DIL_NLS_HPP__
#define POUTRE_IMAGEPROCESSING_ERO_DIL_NLS_HPP__

/**
 * @file   poutreImageProcessingEroDil.hpp
 * @author Thomas Retornaz
 * @brief  Define erosion/dilatation function
 *
 *
 */
#include <poutreBase/poutreContainerView.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainerCopieConvert.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingArith.hpp>
#include <poutreImageProcessingSE/poutreImageProcessingSENeighborList.hpp>

namespace poutre
{
  /**
   * @addtogroup image_processing_mm_group Image Processing Morphology
   * @ingroup image_processing_group
   *@{
   */

  /**
   * @addtogroup image_processing_erodil_group Image Processing Erosion Dilatation
   *functions
   * @ingroup image_processing_mm_group
   *@{
   */

  //----2D array_view/DenseImage<T,2>
  template<se::NLS nl, typename T1, typename T2, class BinOp>
  void t_ErodeDilateIterateBorderArrayView2DHelper(const T1 *&i_vin,
                                                   T2 *&      o_vout,
                                                   scoord     xsize,
                                                   scoord     ysize,
                                                   scoord     stride,
                                                   ptrdiff_t  i_XCenter,
                                                   ptrdiff_t  i_YCenter)
  {
    auto      NlList     = se::NeighborListStaticSETraits<nl>::NlListTransposed;
    ptrdiff_t NbNeighbor = se::NeighborListStaticSETraits<nl>::NbNeighbor;
    auto      i_vinbeg   = i_vin;
    auto      o_voutbeg  = o_vout;
    auto      val        = BinOp::neutral;
    idx2d     center(i_YCenter, i_XCenter);
    for( ptrdiff_t idxNB = 0; idxNB < NbNeighbor; ++idxNB )
    {
      idx2d  idxnl(NlList[idxNB]);
      scoord currentx = center[1] + idxnl[1];
      scoord currenty = center[0] + idxnl[0];
      if( currentx < 0 )
        continue;
      if( currenty < 0 )
        continue;
      if( currentx >= xsize )
        continue;
      if( currenty >= ysize )
        continue;
      val = BinOp::process(val, i_vinbeg[stride * currenty + currentx]);
    }
    o_voutbeg[stride * i_YCenter + i_XCenter] = static_cast<T2>(val);
  }

  template<se::NLS nl, typename T1, typename T2, class BinOp>
  struct t_ErodeDilateOpispatcher<nl, T1, T2, 2, array_view, array_view, BinOp>
  {
    static_assert(2 == se::NeighborListStaticSETraits<nl>::Rank, "SE and view have not the same Rank==2");
    void operator()(const array_view<T1, 2> &i_vin, array_view<T2, 2> &o_vout)
    {
      POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
      auto   ibd     = i_vin.bound();
      auto   obd     = o_vout.bound();
      auto   istride = i_vin.stride();
      auto   ostride = o_vout.stride();
      scoord ysize   = ibd[0];
      scoord xsize   = ibd[1];

      POUTRE_CHECK(ibd == obd, "bound not compatible");
      POUTRE_CHECK(istride == ostride, "stride not compatible");
      auto      NlList              = se::NeighborListStaticSETraits<nl>::NlListTransposed;
      ptrdiff_t NbNeighbor          = se::NeighborListStaticSETraits<nl>::NbNeighbor;
      ptrdiff_t suroundingHalfSizeX = se::NeighborListStaticSETraits<nl>::surroundingSquareHalfSize[1];
      ptrdiff_t suroundingHalfSizeY = se::NeighborListStaticSETraits<nl>::surroundingSquareHalfSize[0];
      auto      i_vinbeg            = i_vin.data();
      auto      o_voutbeg           = o_vout.data();
      // handling the upper lines
      for( ptrdiff_t y = 0; y < suroundingHalfSizeY; ++y )
      {
        for( ptrdiff_t x = 0; x < xsize; ++x )
        {
          t_ErodeDilateIterateBorderArrayView2DHelper<nl, T1, T2, BinOp>(i_vinbeg, o_voutbeg, xsize, ysize, x, y);
        }
      }
      // handling the lower lines
      for( ptrdiff_t y = ysize - suroundingHalfSizeY; y < ysize; ++y )
      {
        for( ptrdiff_t x = 0; x < xsize; ++x )
        {
          t_ErodeDilateIterateBorderArrayView2DHelper<nl, T1, T2, BinOp>(i_vinbeg, o_voutbeg, xsize, ysize, x, y);
        }
      }
      // Main lines area
      for( ptrdiff_t y = suroundingHalfSizeY; y < ysize - suroundingHalfSizeY; ++y )
      {
        // handling the first columns
        for( ptrdiff_t x = 0; x < suroundingHalfSizeX; ++x )
        {
          t_ErodeDilateIterateBorderArrayView2DHelper<nl, T1, T2, BinOp>(i_vinbeg, o_voutbeg, xsize, ysize, x, y);
        }
        for( ptrdiff_t x = suroundingHalfSizeX; x < xsize - suroundingHalfSizeX; ++x )
        {
          auto  val = BinOp::neutral;
          idx2d center(y, x);
          // ver slow
          // for (const auto& idxnl : NlList)
          //{
          //    auto currentidx = center+ idxnl;
          //    max = std::max<T1>(max, i_vinbeg[xsize*currentidx[0]+
          //    currentidx[1]]);
          //}
          for( ptrdiff_t idxNB = 0; idxNB < NbNeighbor; ++idxNB )
          {
            idx2d  idxnl(NlList[idxNB]);
            scoord currentx = center[1] + idxnl[1];
            scoord currenty = center[0] + idxnl[0];
            val             = BinOp::process(val, i_vinbeg[xsize * currenty + currentx]);
          }
          o_voutbeg[xsize * y + x] = static_cast<T2>(val);
        }
        // handling the last columns
        for( ptrdiff_t x = xsize - suroundingHalfSizeX; x < xsize; ++x )
        {
          t_ErodeDilateIterateBorderArrayView2DHelper<nl, T1, T2, BinOp>(i_vinbeg, o_voutbeg, xsize, ysize, x, y);
        }
      }
    }
  };

  template<typename T>
  void t_LineBufferShiftRight(const T *i_viewlinein, scoord lenghtline, scoord nbshift, T paddValue, T *o_viewlineout)
  {
    //   POUTRE_ASSERTCHECK(i_viewlinein.size() == o_viewlineout.size(),
    //                      "Inconsistent array views sizes");
    //   scoord lenghtline = i_viewlinein.size();
    scoord   i;
    const T *linein  = i_viewlinein;
    T *      lineout = o_viewlineout;

    // Case handling error
    if( nbshift >= lenghtline )
    {
      // then padd right border
      for( i = 0; i < lenghtline; i++ )
      {
        lineout[i] = paddValue;
      }
      return;
    }

    // padd left border before shift
    for( i = 0; i < nbshift; i++ )
    {
      lineout[i] = paddValue;
    }
    // shift to right
    std::memcpy(lineout + nbshift,
                linein,
                (lenghtline - nbshift) * sizeof(T)); //<---faster than explicit call to simd::transform
  }

  template<typename T>
  void t_LineBufferShiftLeft(const T *i_viewlinein, scoord lenghtline, scoord nbshift, T paddValue, T *o_viewlineout)
  {
    //   POUTRE_ASSERTCHECK(i_viewlinein.size() == o_viewlineout.size(),
    //                      "Inconsistent array views sizes");
    //   scoord lenghtline = i_viewlinein.size();
    scoord   i;
    const T *linein  = i_viewlinein;
    T *      lineout = o_viewlineout;
    // Case handling error
    if( nbshift >= lenghtline )
    {
      // then padd right border
      for( i = 0; i < lenghtline; i++ )
      {
        lineout[i] = paddValue;
      }
      return;
    }

    // shift to left
    std::memcpy(lineout, linein + nbshift, (lenghtline - nbshift) * sizeof(T));

    // then padd right border
    for( i = lenghtline - nbshift; i < lenghtline; i++ )
    {
      lineout[i] = paddValue;
    }
  }

  // Helper structure Shift buffer left then right and apply arithmetic operator
  // Dilate
  template<typename T> struct LineBufferShiftAndArithDilateHelperOp
  {
    public:
    using linePtr       = T *;
    using const_linePtr = const T *;
    using viewIn        = const poutre::array_view<T, 1>; // FIXME const
    using viewOut       = poutre::array_view<T, 1>;

    static constexpr T m_paddingValue = poutre::TypeTraits<T>::inf();
    static constexpr T neutral        = m_paddingValue;
    using self                        = LineBufferShiftAndArithDilateHelperOp<T>;
    static T process(const T &A0, const T &A1) { return std::max<T>(A0, A1); }

    static void ApplyArith(const_linePtr lineIn1, const_linePtr lineIn2, scoord size, linePtr lineout)
    {
      simd::transform(lineIn1, lineIn1 + size, lineIn2, lineout, op_Sup<T, T, T>());
    }
    static void ShiftRightLeftAndArith(const_linePtr linein,
                                       scoord        size,
                                       scoord        nbStepShiftLeft,
                                       scoord        nbStepShiftRight,
                                       linePtr       linetmp, // to reuse external allocation
                                       linePtr       lineout)
    {
      // shift to left and arith
      t_LineBufferShiftLeft<T>(linein, size, nbStepShiftLeft, m_paddingValue, linetmp);
      self::ApplyArith(linein, linetmp, size, lineout);

      // shift to right and arith with previous computed arith
      t_LineBufferShiftRight<T>(linein, size, nbStepShiftRight, m_paddingValue, linetmp);
      self::ApplyArith(lineout, linetmp, size, lineout);
    }
  };

  // Helper structure Shift buffer left then right and apply arithmetic operator
  // Dilate
  template<typename T> struct LineBufferShiftAndArithErodeHelperOp
  {
    public:
    using linePtr       = T *;
    using const_linePtr = const T *;
    using viewIn        = const poutre::array_view<T, 1>; // FIXME const
    using viewOut       = poutre::array_view<T, 1>;

    static constexpr T m_paddingValue = poutre::TypeTraits<T>::sup();
    static constexpr T neutral        = m_paddingValue;
    using self                        = LineBufferShiftAndArithErodeHelperOp<T>;
    static T process(const T &A0, const T &A1) { return std::min<T>(A0, A1); }

    static void ApplyArith(const_linePtr lineIn1, const_linePtr lineIn2, scoord size, linePtr lineout)
    {
      simd::transform(lineIn1, lineIn1 + size, lineIn2, lineout, op_Inf<T, T, T>());
    }
    static void ShiftRightLeftAndArith(const_linePtr linein,
                                       scoord        size,
                                       scoord        nbStepShiftLeft,
                                       scoord        nbStepShiftRight,
                                       linePtr       linetmp, // to reuse external allocation
                                       linePtr       lineout)
    {
      // shift to left and arith
      t_LineBufferShiftLeft<T>(linein, size, nbStepShiftLeft, m_paddingValue, linetmp);
      self::ApplyArith(linein, linetmp, size, lineout);

      // shift to right and arith with previous computed arith
      t_LineBufferShiftRight<T>(linein, size, nbStepShiftRight, m_paddingValue, linetmp);
      self::ApplyArith(lineout, linetmp, size, lineout);
    }
  };

  /////Square
  template<typename TIn, typename TOut, class HelperOp> struct NLS_c8_2dErodeDilateCentralPartFunctor
  {
    using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
    using lineView  = TIn *__restrict;
    using clineView = const TIn *__restrict;

    tmpBuffer tempLine, tempLine1, tempLine2, tempLine3;
    lineView  bufTempLine, bufTempLine1, bufTempLine2, bufTempLine3;
    lineView  bufInputPreviousLine;
    lineView  bufInputCurrentLine;
    lineView  bufInputNextLine;
    lineView  bufOuputCurrentLine;
    scoord    m_xsize;
    explicit NLS_c8_2dErodeDilateCentralPartFunctor(scoord xsize)
        : tempLine(xsize)
        , tempLine1(xsize)
        , tempLine2(xsize)
        , tempLine3(xsize)
        , bufTempLine(tempLine.data())
        , bufTempLine1(tempLine1.data())
        , bufTempLine2(tempLine2.data())
        , bufTempLine3(tempLine3.data())
        , m_xsize(xsize)
    {
    }
    void operator()(lineView lineDataIn, lineView lineDataOut, scoord start, scoord stop)
    {
      POUTRE_ASSERTCHECK(start >= 2, "out of bounds");
    }
  };
  // FIXME check TIn, Tout equals cv
  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateOpispatcher<se::NLS::NLS_c8_2d, TIn, TOut, 2, array_view, array_view, HelperOp>
  {
    void operator()(const array_view<TIn, 2> &i_vin, array_view<TOut, 2> &o_vout)
    {
      POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
      auto   ibd     = i_vin.bound();
      auto   obd     = o_vout.bound();
      auto   istride = i_vin.stride();
      auto   ostride = o_vout.stride();
      scoord ysize   = ibd[0];
      scoord xsize   = ibd[1];
      POUTRE_ASSERTCHECK(ibd == obd, "bound not compatible");
      POUTRE_ASSERTCHECK(istride == ostride, "stride not compatible");
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      using lineView  = TIn *__restrict; // array_view<TIn, 1>;

      tmpBuffer tempLine(xsize), tempLine1(xsize), tempLine2(xsize), tempLine3(xsize);
      lineView  bufTempLine(tempLine.data()), bufTempLine1(tempLine1.data()), bufTempLine2(tempLine2.data()),
          bufTempLine3(tempLine3.data());
      lineView bufInputPreviousLine;
      lineView bufInputCurrentLine;
      lineView bufInputNextLine;
      lineView bufOuputCurrentLine;

      // quick run one line
      if( ysize == 1 )
      {
        bufInputPreviousLine = i_vin.data();
        bufOuputCurrentLine  = o_vout.data();
        // dilate/erode line 0
        HelperOp::ShiftRightLeftAndArith(bufInputPreviousLine, xsize, 1, 1, bufTempLine, bufOuputCurrentLine);
        return;
      }

      // compute first line
      // translate to clipped connection
      // x . x
      // x x x
      bufInputPreviousLine = i_vin.data();
      // dilate/erode line 0
      HelperOp::ShiftRightLeftAndArith(bufInputPreviousLine, xsize, 1, 1, bufTempLine, bufTempLine1);

      bufInputNextLine    = i_vin.data() + xsize;
      bufOuputCurrentLine = o_vout.data();
      // dilate/erode line 1
      HelperOp::ShiftRightLeftAndArith(bufInputNextLine, xsize, 1, 1, bufTempLine, bufTempLine2);

      // sup(dilate line 1,dilate line 0) or inf(erode line 1,erode line 0)
      HelperOp::ApplyArith(bufTempLine1, bufTempLine2, xsize, bufOuputCurrentLine);

      bufInputCurrentLine = bufInputNextLine;
      // then loop
      for( scoord y = 2; y < ysize; y++ )
      {
        // Invariant of the loop
        // bufTempLine1 contains dilation/erosion of previous line Y-2
        // bufTempLine2 contains dilation/erosion of current line Y-1
        // bufTempLine3 contains dilation/erosion of next line ie Y

        // actual computation
        // 1 2 3   <--- bufInputPreviousLine y-2
        // 4 5 6   <--- bufInputCurrentLine  y-1
        // 7 8 9   <--- bufInputNextLine     y

        bufOuputCurrentLine = o_vout.data() + (y - 1) * xsize;
        bufInputNextLine    = i_vin.data() + (y)*xsize;
        // dilate/erode(y)
        HelperOp::ShiftRightLeftAndArith(bufInputNextLine, xsize, 1, 1, bufTempLine, bufTempLine3);

        // sup(dilate(y-2),dilate(y-1)) or inf(erode(y-2),erode(y-1))
        HelperOp::ApplyArith(bufTempLine1, bufTempLine2, xsize, bufTempLine);
        // sup(dilate(y-2),dilate(y-1),dilate(y)) or
        // inf(erode(y-2),erode(y-1),erode(y))
        HelperOp::ApplyArith(bufTempLine, bufTempLine3, xsize, bufOuputCurrentLine);

        // so use swap to don't loose ref on bufTempLine3
        std::swap(bufTempLine3, bufTempLine2);
        std::swap(bufTempLine3, bufTempLine1);
      }
      // end last line
      // translate to clipped connection
      // x x x
      // x . x
      bufOuputCurrentLine = o_vout.data() + xsize * (ysize - 1);
      HelperOp::ApplyArith(bufTempLine1, bufTempLine2, xsize, bufOuputCurrentLine);
    }
  };

  /////Cross

  // FIXME check TIn, Tout equals cv
  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateOpispatcher<se::NLS::NLS_c4_2d, TIn, TOut, 2, array_view, array_view, HelperOp>
  {
    void operator()(const array_view<TIn, 2> &i_vin, array_view<TOut, 2> &o_vout)
    {
      POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
      auto   ibd     = i_vin.bound();
      auto   obd     = o_vout.bound();
      auto   istride = i_vin.stride();
      auto   ostride = o_vout.stride();
      scoord ysize   = ibd[0];
      scoord xsize   = ibd[1];
      POUTRE_ASSERTCHECK(ibd == obd, "bound not compatible");
      POUTRE_ASSERTCHECK(istride == ostride, "stride not compatible");
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      // using lineView = array_view<TIn, 1>;
      using lineView = TIn *__restrict; // array_view<TIn, 1>;

      tmpBuffer tempLine(xsize), tempLine1(xsize), tempLine2(xsize), tempLine3(xsize);
      lineView  bufTempLine(tempLine.data()), bufTempLine1(tempLine1.data()), bufTempLine2(tempLine2.data()),
          bufTempLine3(tempLine3.data());
      lineView bufInputPreviousLine;
      lineView bufInputCurrentLine;
      lineView bufInputNextLine;
      lineView bufOuputCurrentLine;

      // quick run one line
      if( ysize == 1 )
      {
        bufInputPreviousLine = i_vin.data();
        bufOuputCurrentLine  = o_vout.data();
        // dilate/erode line 0
        HelperOp::ShiftRightLeftAndArith(bufInputPreviousLine, xsize, 1, 1, bufTempLine, bufOuputCurrentLine);
        return;
      }

      // compute first line
      // translate to clipped connection
      // x . x
      // ? x ?
      bufInputPreviousLine = i_vin.data();
      // dilate/erode line 0
      HelperOp::ShiftRightLeftAndArith(bufInputPreviousLine, xsize, 1, 1, bufTempLine, bufTempLine2);

      bufInputNextLine    = i_vin.data() + xsize;
      bufOuputCurrentLine = o_vout.data();

      // inf/sup between dilate line 0 and line 1
      HelperOp::ApplyArith(bufTempLine2, bufInputNextLine, xsize, bufOuputCurrentLine);

      // then loop
      bufInputCurrentLine = bufInputNextLine;
      for( scoord y = 2; y < ysize; y++ )
      {
        // actual computation
        // 1 2 3   <--- bufInputPreviousLine y-2
        // 4 5 6   <--- bufInputCurrentLine  y-1
        // 7 8 9   <--- bufInputNextLine     y
        bufOuputCurrentLine = o_vout.data() + (y - 1) * xsize;
        bufInputNextLine    = i_vin.data() + (y)*xsize;
        // dilate(y-1)/erode(y-1)
        HelperOp::ShiftRightLeftAndArith(bufInputCurrentLine, xsize, 1, 1, bufTempLine, bufTempLine2);

        // sup(y-2,dilate(y-1)),inf(y-2,erode(y-1))
        HelperOp::ApplyArith(bufInputPreviousLine, bufTempLine2, xsize, bufTempLine);
        // sup(sup(y-2,dilate(y-1),y) || inf(inf(y-2,erode(y-1),y)
        HelperOp::ApplyArith(bufTempLine, bufInputNextLine, xsize, bufOuputCurrentLine);
        // swap
        bufInputPreviousLine = bufInputCurrentLine; // previous <--current
        bufInputCurrentLine  = bufInputNextLine;    // current <-- next
      }
      // end last line
      // translate to clipped connection
      //. x .
      // x x x
      // note that bufInputCurrentLine already point on ySize-1 (see above)
      bufOuputCurrentLine = o_vout.data() + xsize * (ysize - 1);
      // dilate/erode line y-1
      HelperOp::ShiftRightLeftAndArith(bufInputCurrentLine, xsize, 1, 1, bufTempLine, bufTempLine2);
      HelperOp::ApplyArith(bufInputPreviousLine, bufTempLine2, xsize, bufOuputCurrentLine);
    }
  };

  /////Seg0

  // FIXME check TIn, Tout equals cv
  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2dH, TIn, TOut, 2, array_view, array_view, HelperOp>
  {
    void operator()(const array_view<TIn, 2> &i_vin, array_view<TOut, 2> &o_vout)
    {
      POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
      auto   ibd     = i_vin.bound();
      auto   obd     = o_vout.bound();
      auto   istride = i_vin.stride();
      auto   ostride = o_vout.stride();
      scoord ysize   = ibd[0];
      scoord xsize   = ibd[1];
      POUTRE_ASSERTCHECK(ibd == obd, "bound not compatible");
      POUTRE_ASSERTCHECK(istride == ostride, "stride not compatible");
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      using lineView  = TIn *__restrict; // array_view<TIn, 1>;

      tmpBuffer tempLine(xsize);
      lineView  bufTempLine(tempLine.data());
      lineView  bufInputCurrentLine;
      lineView  bufOuputCurrentLine;

      // quick exit one column
      if( xsize == 1 )
      {
        t_Copy(i_vin, o_vout);
        return;
      }

      for( scoord y = 0; y < ysize; y++ )
      {
        bufInputCurrentLine = i_vin.data() + y * xsize;
        bufOuputCurrentLine = o_vout.data() + y * xsize;
        HelperOp::ShiftRightLeftAndArith(bufInputCurrentLine, xsize, 1, 1, bufTempLine, bufOuputCurrentLine);
      }
    }
  };

  /////Seg90

  // FIXME check TIn, Tout equals cv
  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2dV, TIn, TOut, 2, array_view, array_view, HelperOp>
  {
    void operator()(const array_view<TIn, 2> &i_vin, array_view<TOut, 2> &o_vout)
    {
      POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
      auto   ibd     = i_vin.bound();
      auto   obd     = o_vout.bound();
      auto   istride = i_vin.stride();
      auto   ostride = o_vout.stride();
      scoord ysize   = ibd[0];
      scoord xsize   = ibd[1];
      POUTRE_ASSERTCHECK(ibd == obd, "bound not compatible");
      POUTRE_ASSERTCHECK(istride == ostride, "stride not compatible");
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      // using lineView = array_view<TIn, 1>;
      using lineView = TIn *__restrict;
      tmpBuffer tempLine(xsize);
      lineView  bufTempLine(tempLine.data());
      lineView  bufInputPreviousLine;
      lineView  bufInputCurrentLine;
      lineView  bufInputNextLine;
      lineView  bufOuputCurrentLine;

      // quick run one line
      if( ysize == 1 )
      {
        t_Copy(i_vin, o_vout);
        return;
      }

      // compute first line
      // translate to clipped connection
      // ? . ?
      // ? x ?
      bufInputPreviousLine = i_vin.data();
      bufInputNextLine     = i_vin.data() + xsize;
      ;
      bufOuputCurrentLine = o_vout.data();
      // inf/sup between dilate line 0 and line 1
      HelperOp::ApplyArith(bufInputPreviousLine, bufInputNextLine, xsize, bufOuputCurrentLine);

      // then loop
      bufInputCurrentLine = bufInputNextLine;
      for( scoord y = 2; y < ysize; y++ )
      {
        // actual computation
        // 1   <--- bufInputPreviousLine y-2
        // 4   <--- bufInputCurrentLine  y-1
        // 7   <--- bufInputNextLine     y
        bufOuputCurrentLine = o_vout.data() + xsize * (y - 1);

        bufInputNextLine     = i_vin.data() + xsize * (y);
        bufInputPreviousLine = i_vin.data() + xsize * (y - 2);
        bufInputCurrentLine  = i_vin.data() + xsize * (y - 1);

        HelperOp::ApplyArith(bufInputPreviousLine, bufInputCurrentLine, xsize, bufTempLine);
        HelperOp::ApplyArith(bufTempLine, bufInputNextLine, xsize, bufOuputCurrentLine);
      }
      // end last line
      // translate to clipped connection
      //? x ?
      //? x ?
      bufInputPreviousLine = i_vin.data() + xsize * (ysize - 2);
      bufInputCurrentLine  = i_vin.data() + xsize * (ysize - 1);
      bufOuputCurrentLine  = o_vout.data() + xsize * (ysize - 1);
      HelperOp::ApplyArith(bufInputPreviousLine, bufInputCurrentLine, xsize, bufOuputCurrentLine);
    }
  };

  /////Seg45
  // FIXME check TIn, Tout equals cv
  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2d_Diag45, TIn, TOut, 2, array_view, array_view, HelperOp>
  {
    void operator()(const array_view<TIn, 2> &i_vin, array_view<TOut, 2> &o_vout)
    {
      POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
      auto   ibd     = i_vin.bound();
      auto   obd     = o_vout.bound();
      auto   istride = i_vin.stride();
      auto   ostride = o_vout.stride();
      scoord ysize   = ibd[0];
      scoord xsize   = ibd[1];
      POUTRE_ASSERTCHECK(ibd == obd, "bound not compatible");
      POUTRE_ASSERTCHECK(istride == ostride, "stride not compatible");
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      // using lineView = array_view<TIn, 1>;
      using lineView = TIn *__restrict;
      tmpBuffer tempLine(xsize), tempLine2(xsize), tempLine3(xsize);
      lineView  bufTempLine(tempLine.data()), bufTempLine2(tempLine2.data()), bufTempLine3(tempLine3.data());
      lineView  bufInputPreviousLine;
      lineView  bufInputCurrentLine;
      lineView  bufInputNextLine;
      lineView  bufOuputCurrentLine;

      // quick run one line/one column
      if( ysize == 1 || xsize == 1 )
      {
        t_Copy(i_vin, o_vout);
        return;
      }

      // compute first line
      // translate to clipped connection
      // ? . ?
      // ? ? x
      bufInputPreviousLine = i_vin.data();
      bufInputNextLine     = i_vin.data() + xsize;
      bufOuputCurrentLine  = o_vout.data();
      // shift to left
      t_LineBufferShiftLeft<TIn>(bufInputNextLine, xsize, 1, HelperOp::m_paddingValue, bufTempLine);
      // Take min(Erode), max(dilate)
      HelperOp::ApplyArith(bufInputPreviousLine, bufTempLine, xsize, bufOuputCurrentLine);

      // then loop
      bufInputCurrentLine = bufInputNextLine;
      for( scoord y = 1; y < ysize; y++ )
      {
        // actual computation
        // 1 2 3   <--- bufInputPreviousLine y-1
        // 4 5 6   <--- bufInputCurrentLine  y
        // 7 8 9   <--- bufInputNextLine     y+1
        bufInputPreviousLine = i_vin.data() + xsize * (y - 1);
        bufInputCurrentLine  = i_vin.data() + xsize * (y);
        bufInputNextLine     = i_vin.data() + xsize * (y + 1);
        bufOuputCurrentLine  = o_vout.data() + xsize * (y);
        // shift to right previous line
        t_LineBufferShiftRight<TIn>(bufInputPreviousLine, xsize, 1, HelperOp::m_paddingValue, bufTempLine);
        // shift to left next line
        t_LineBufferShiftLeft<TIn>(bufInputNextLine, xsize, 1, HelperOp::m_paddingValue, bufTempLine2);
        // Take min(Erode), max(dilate)
        HelperOp::ApplyArith(bufInputCurrentLine, bufTempLine, xsize, bufTempLine3);
        HelperOp::ApplyArith(bufTempLine3, bufTempLine2, xsize, bufOuputCurrentLine);
      }
      // end last line
      // translate to clipped connection
      // x ? ?
      //? . ?
      bufInputPreviousLine = i_vin.data() + xsize * (ysize - 2);
      bufInputCurrentLine  = i_vin.data() + xsize * (ysize - 1);
      bufOuputCurrentLine  = o_vout.data() + xsize * (ysize - 1);

      // shift to right
      t_LineBufferShiftRight<TIn>(bufInputPreviousLine, xsize, 1, HelperOp::m_paddingValue, bufTempLine);
      // Take min(Erode), max(dilate)
      HelperOp::ApplyArith(bufInputCurrentLine, bufTempLine, xsize, bufOuputCurrentLine);
    }
  };

  /////Seg135
  // FIXME check TIn, Tout equals cv
  template<typename TIn, typename TOut, class HelperOp>
  struct t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2d_Diag135, TIn, TOut, 2, array_view, array_view, HelperOp>
  {
    void operator()(const array_view<TIn, 2> &i_vin, array_view<TOut, 2> &o_vout)
    {
      POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
      auto   ibd     = i_vin.bound();
      auto   obd     = o_vout.bound();
      auto   istride = i_vin.stride();
      auto   ostride = o_vout.stride();
      scoord ysize   = ibd[0];
      scoord xsize   = ibd[1];
      POUTRE_ASSERTCHECK(ibd == obd, "bound not compatible");
      POUTRE_ASSERTCHECK(istride == ostride, "stride not compatible");
      using tmpBuffer = std::vector<TIn, xs::aligned_allocator<TIn, SIMD_IDEAL_MAX_ALIGN_BYTES>>;
      // using lineView = array_view<TIn, 1>;
      using lineView = TIn *__restrict;
      tmpBuffer tempLine(xsize), tempLine2(xsize), tempLine3(xsize);
      lineView  bufTempLine(tempLine.data()), bufTempLine2(tempLine2.data()), bufTempLine3(tempLine3.data());
      lineView  bufInputPreviousLine;
      lineView  bufInputCurrentLine;
      lineView  bufInputNextLine;
      lineView  bufOuputCurrentLine;

      // quick run one line
      if( ysize == 1 || xsize == 1 )
      {
        t_Copy(i_vin, o_vout);
        return;
      }

      // compute first line
      // translate to clipped connection
      // 0 0 X     0 . 0
      // 0 . 0   ->x 0 0
      // X 0 0
      bufInputPreviousLine = i_vin.data();
      bufInputNextLine     = i_vin.data() + xsize;
      bufOuputCurrentLine  = o_vout.data();

      // shift to rigt
      t_LineBufferShiftRight<TIn>(bufInputNextLine, xsize, 1, HelperOp::m_paddingValue, bufTempLine);
      // Take min(Erode), max(dilate)
      HelperOp::ApplyArith(bufInputPreviousLine, bufTempLine, xsize, bufOuputCurrentLine);

      for( scoord y = 1; y < ysize; y++ )
      {
        // actual computation
        // 1 2 3   <--- bufInputPreviousLine y-2
        // 4 5 6   <--- bufInputCurrentLine  y-1
        // 7 8 9   <--- bufInputNextLine     y
        bufInputPreviousLine = i_vin.data() + xsize * (y - 1);
        bufInputCurrentLine  = i_vin.data() + xsize * (y);
        bufInputNextLine     = i_vin.data() + xsize * (y + 1);
        bufOuputCurrentLine  = o_vout.data() + xsize * (y);
        // shift to left previous line
        t_LineBufferShiftLeft<TIn>(bufInputPreviousLine, xsize, 1, HelperOp::m_paddingValue, bufTempLine);
        // shift to right next line
        t_LineBufferShiftRight<TIn>(bufInputNextLine, xsize, 1, HelperOp::m_paddingValue, bufTempLine2);
        // Take min(Erode), max(dilate)
        HelperOp::ApplyArith(bufInputCurrentLine, bufTempLine, xsize, bufTempLine3);
        HelperOp::ApplyArith(bufTempLine3, bufTempLine2, xsize, bufOuputCurrentLine);
      }
      // end last line
      // translate to clipped connection
      // 0 0 X     0 0 x
      // 0 . 0   ->0 . 0
      // X 0 0
      bufInputPreviousLine = i_vin.data() + xsize * (ysize - 2);
      bufInputNextLine     = i_vin.data() + xsize * (ysize - 1);
      bufOuputCurrentLine  = o_vout.data() + xsize * (ysize - 1);

      // shift to left
      t_LineBufferShiftLeft<TIn>(bufInputPreviousLine, xsize, 1, HelperOp::m_paddingValue, bufTempLine);
      // Take min(Erode), max(dilate)
      HelperOp::ApplyArith(bufInputNextLine, bufTempLine, xsize, bufOuputCurrentLine);
    }
  };

  //**********************************************************/

  template<typename TIn,
           typename TOut,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_Dilate(const ViewIn<TIn, Rank> &i_vin, se::NLS nl, ViewOut<TOut, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_Dilate");
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    switch( nl )
    {
    case se::NLS::NLS_c8_2d:
    {
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c8_2d, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c4_2d:
    {
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c4_2d, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c2_2dH:
    {
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2dH, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c2_2dV:
    {
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2dV, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c2_2d_Diag45:
    {
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2d_Diag45, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c2_2d_Diag135:
    {
      using LineOp = LineBufferShiftAndArithDilateHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2d_Diag135, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_Dilate se::NLS not implemented");
    }
    }
  }

  template<typename TIn, typename TOut, ptrdiff_t Rank>
  void t_Dilate(const DenseImage<TIn, Rank> &i_vin, se::NLS nl, DenseImage<TOut, Rank> &o_vout)
  {
    AssertSizesCompatible(i_vin, o_vout, "t_Dilate incompatible size");
    AssertAsTypesCompatible(i_vin, o_vout, "t_Dilate incompatible types");
    auto viewIn  = view(const_cast<DenseImage<TIn, Rank> &>(i_vin));
    auto viewOut = view(o_vout);
    return t_Dilate(viewIn, nl, viewOut);
  }

  template<typename TIn,
           typename TOut,
           ptrdiff_t Rank,
           template<typename, ptrdiff_t>
           class ViewIn,
           template<typename, ptrdiff_t>
           class ViewOut>
  void t_Erode(const ViewIn<TIn, Rank> &i_vin, se::NLS nl, ViewOut<TOut, Rank> &o_vout)
  {
    POUTRE_ENTERING("t_Erode");
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    switch( nl )
    {
    case se::NLS::NLS_c8_2d:
    {
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c8_2d, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c4_2d:
    {
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c4_2d, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c2_2dH:
    {
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2dH, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c2_2dV:
    {
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2dV, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c2_2d_Diag45:
    {
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2d_Diag45, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    case se::NLS::NLS_c2_2d_Diag135:
    {
      using LineOp = LineBufferShiftAndArithErodeHelperOp<TIn>;
      t_ErodeDilateOpispatcher<se::NLS::NLS_c2_2d_Diag135, TIn, TOut, Rank, ViewIn, ViewOut, LineOp> dispatcher;
      dispatcher(i_vin, o_vout);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_Erode se::NLS not implemented");
    }
    }
  }
  template<typename TIn, typename TOut, ptrdiff_t Rank>
  void t_Erode(const DenseImage<TIn, Rank> &i_vin, se::NLS nl, DenseImage<TOut, Rank> &o_vout)
  {
    AssertSizesCompatible(i_vin, o_vout, "t_Erode incompatible size");
    AssertAsTypesCompatible(i_vin, o_vout, "t_Erode incompatible types");
    auto viewIn  = view(const_cast<DenseImage<TIn, Rank> &>(i_vin));
    auto viewOut = view(o_vout);
    return t_Erode(viewIn, nl, viewOut);
  }

  //! @} doxygroup: image_processing_erodil_group
  //! @} doxygroup: image_processing_group
} // namespace poutre
#endif // POUTRE_IMAGEPROCESSING_ERO_DIL_NLS_HPP__
