
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_COPIECONVERT_HPP__
#define POUTRE_IMAGEPROCESSING_CONTAINER_COPIECONVERT_HPP__


#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>
#endif

namespace poutre
  {

  /*
  template <PType ptypeOther, PType ptype, class allocator>
  struct get_same_allocator_t
    {
    };

  template <PType ptypeOther, PType ptype>
  struct get_same_allocator_t<ptypeOther, typename boost::simd::allocator < TypeTraits<ptype>::storage_type, TypeTraits<ptype>::alignement >>
    {
    using type = boost::simd::allocator < typename  TypeTraits<ptypeOther>::storage_type, TypeTraits<ptypeOther>::alignement >;
    };

  template <PType ptypeOther, class ImageIn>
  struct get_same_t < ImgType::ImgType_Dense, ptypeOther, ImageIn >
    {
    using type = DenseImage <
      ptypeOther,
      ImageIn::m_numdims,
      typename get_same_allocator_t<ptypeOther, ImageIn::m_ptype, typename ImageIn::allocator_type>::type>
      >;
    };
  */

  }
#endif //POUTRE_IMAGEPROCESSING_CONTAINER_COPIECONVERT_HPP__
