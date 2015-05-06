#ifndef POUTREIMAGEPROCESSINGUNARYOP_HPP__
#define POUTREIMAGEPROCESSINGUNARYOP_HPP__

#ifndef POUTREIMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTREIMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif


#include <boost/simd/sdk/simd/native.hpp>
#include <boost/simd/include/functions/aligned_load.hpp>
#include <boost/simd/include/functions/aligned_store.hpp>
#include <boost/simd/include/functions/load.hpp>
#include <boost/simd/include/functions/splat.hpp>
#include <boost/simd/memory/align_on.hpp>
#include <boost/simd/memory/is_aligned.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include <boost/config.hpp>


namespace poutre
  {
  namespace bs = boost::simd;

  //stolen from
  //#include <boost/simd/sdk/simd/algorithm.hpp>
  template<PType ptypeIn, class UnOp>
  struct pImageUnaryIterOpInPlace
    {
    using value_type = TypeTraits<ptypeIn>::storage_type;
    using pointer = value_type*;
    using bsvalue_type = bs::pack < value_type > ;

    void operator()(pointer begin, pointer end, UnOp f) const
      {
      POUTRE_ASSERTCHECK(begin != nullptr, "pImageUnaryIterOp: begin null ptr");
      POUTRE_ASSERTCHECK(end != nullptr, "pImageUnaryIterOp: end null ptr");


      static const std::size_t N = bsvalue_type::static_size;
      const std::size_t shift = simd::align_on(begin, N * sizeof(value_type)) - begin;
      pointer end2 = begin + std::min<size_t>(shift, end - begin);
      pointer end3 = end2 + (end - end2) / N*N;

      // prologue
      for (; begin != end2; ++begin)
        *begin = f(*begin);

      //main loop 
      //TODO UNROLL ?
      if (simd::is_aligned(begin, N * sizeof(value_type)))
        {
        for (; begin != end3; begin += N, out += N)
          simd::aligned_store(f(simd::aligned_load<value_type>(begin)), begin);
        }
      else
        {
        for (; begin != end3; begin += N, out += N)
          simd::aligned_store(f(simd::load<value_type>(begin)), begin);
        }

      // epilogue
      for (; begin != end; ++begin, ++out)
        *begin = f(*begin);

      }
    };


  template<PType ptypeIn, PType ptypeOut, class UnOp>
  struct pImageUnaryIterOp
    {
    using value_typeIn = TypeTraits<ptypeIn>::storage_type;
    using value_typeOut = TypeTraits<ptypeOut>::storage_type;
    using pointerin = value_typeIn *;
    using pointerout = value_typeOut *;
    using const_pointerin = const value_typeIn *;
    //using const_pointerout = const value_typeOut *;
    using bsvalue_typeIn = bs::pack < value_typeIn > ;
    using bsvalue_typeOut = bs::pack < value_typeOut > ;

    void operator()(const_pointerin beginin, const_pointerin endin, pointerout beginout, UnOp f) const
      {
      BOOST_MPL_ASSERT_MSG(bsvalue_typeIn::static_size == bsvalue_typeOut::static_size
        , BOOST_SIMD_TRANSFORM_INPUT_OUTPUT_NOT_SAME_SIZE
        , (value_typeIn, value_typeOut)
        );
      static const std::size_t N = bsvalue_typeIn::static_size;

      const std::size_t shift = simd::align_on(beginout, N * sizeof(value_typeOut) - beginout;
      const_pointerin end2 = beginin + std::min<size_t>(shift, endin - beginin);
      const_pointerin end3 = end2 + (endin - end2) / N*N;

      // prologue
      for (; beginin != end2; ++beginin, ++beginout)
        *beginout = f(*beginin);

      //main loop
      //TODO UNROLL ?
      if (simd::is_aligned(beginin, N * sizeof(value_typeIn)) && simd::is_aligned(beginout, N * sizeof(value_typeOut)))
        {
        for (; beginin != end3; beginin += N, beginout += N)
          simd::aligned_store(f(simd::aligned_load<value_typeIn>(begin1)), beginout);
        }
      else
        {
        for (; beginin != end3; beginin += N, beginout += N)
          simd::store(f(simd::load<value_typeIn>(begin1)), beginout);
        }

      // epilogue
      for (; beginin != end; ++beginin, ++beginout)
        *beginout = f(*beginin);
      }
    };


  template<class ImageIn, class UnOp>
  struct pApplyImageUnaryIterOpInPlace
    {
    void operator()(ImageIn& img, UnOp f) const
      {
      //TODO use multithread approch here dispatch on dims and so on 
      //also dispatch on iterator categories
      pImageUnaryIterOp<ImageIn::m_ptype, ImageIn::m_ptype, UnOp>( )(img.begin( ), img.end( ), img.begin( ),f);
      }
    };

  template<class ImageIn,class ImageOut, class UnOp>
  struct pApplyImageUnaryIterOp
    {
    void operator()(const ImageIn& imgIn, ImageOut& imgOut, UnOp f) const
      {
      //TODO PRECONDITIONS
      //TODO use multithread approch here dispatch on dims and so on 
      //also dispatch on iterator categories and types
      pImageUnaryIterOp<ImageIn::m_ptype, ImageOut::m_ptype, UnOp>( )(imgIn.begin( ), imgIn.end( ), imgOut.begin( ), f);
      }
    };

  }
#endif POUTREIMAGEPROCESSINGUNARYOP_HPP__