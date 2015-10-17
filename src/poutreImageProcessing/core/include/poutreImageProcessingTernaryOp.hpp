
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_TERNARYOP_HPP__
#define POUTRE_IMAGEPROCESSING_TERNARYOP_HPP__

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
  
  }
#endif //POUTRE_IMAGEPROCESSING_TERNARYOP_HPP__