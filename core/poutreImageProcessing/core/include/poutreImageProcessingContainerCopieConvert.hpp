#ifndef POUTREIMAGEPROCESSINGCONTAINERCOPIECONVERT_HPP__
#define POUTREIMAGEPROCESSINGCONTAINERCOPIECONVERT_HPP__


#ifndef POUTREIMAGEPROCESSINGCONTAINER_HPP__
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>
#endif

namespace poutre
  {

  template <PType ptypeOther, class allocator>
  struct get_same_allocator
    {
    };

  template <PType ptypeOther, PType ptype>
  struct get_same_allocator<ptypeOther, boost::simd::allocator < TypeTraits<ptype>::storage_type, TypeTraits<ptype>::alignement >
    {
    using type = boost::simd::allocator < TypeTraits<ptypeOther>::storage_type, TypeTraits<ptypeOther>::alignement >;
    };

  template <ImgType imgTypeOther, PType ptypeOther, class ImageIn>
  struct get_same_t
    {
    };

  template <PType ptypeOther, class ImageIn>
  struct get_same_t < ImgType::ImgType_Dense, ptypeOther, ImageIn >
    {
    using type = DenseImage <
      ptypeOther,
      ImageIn::m_numdims,
      get_same_allocator<ptypeOther, ImageIn::m_ptype>
    > ;

    };


  }
#endif POUTREIMAGEPROCESSINGCONTAINERCOPIECONVERT_HPP__
