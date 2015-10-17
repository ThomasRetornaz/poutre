
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_BINARYOP_HPP__
#define POUTRE_IMAGEPROCESSING_BINARYOP_HPP__

#ifndef POUTRE_IMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
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

  //adapted from #include <boost/simd/sdk/simd/algorithm.hpp>

  template<PType ptypeIn, PType ptypeOut, class BinOp>
  struct pImageBinaryIterOp
    {
    using value_type = TypeTraits<ptypeIn>::storage_type;
    using pointer = value_type*;
    using bsvalue_type = bs::pack<value_type>;

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


  template<class ImageIn, class UnOp>
  struct pApplyImageUnaryIterOp
    {
    void operator()(ImageIn& img, UnOp f) const
      {
      //TODO use multithread approch here dispatch on dims and so on 
      pImageUnaryIterOp<ImageIn::m_ptype, UnOp>( )(img.begin( ), img.end( ), f);
      }
    };
  }
#endif //POUTRE_IMAGEPROCESSING_BINARYOP_HPP__