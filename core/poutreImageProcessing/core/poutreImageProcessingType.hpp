
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_TYPE_HPP__
#define POUTRE_IMAGEPROCESSING_TYPE_HPP__

#include <limits>
#include <type_traits>
#include <boost/simd/preprocessor/parameters.hpp> //default alignment
#include <stdexcept>

#ifndef POUTRE_IMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRE_TYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifndef POUTRE_STATIC_CONTAINER_HPP__
#include <poutreBase/poutreStaticContainer.hpp>
#endif

namespace poutre
  {
  enum class ImgType
    {
    ImgType_Undef = 0,          //!< Undefined type 
    ImgType_Dense = 1 << 0,     //!< Dense, contiguous  
    ImgType_Sparse = 1 << 1,    //!< Sparse
    };

  //! operator<< for ImgType
  IMP_API std::ostream& operator<<(std::ostream&, ImgType);

  //! operator>> for ImgType
  IMP_API std::istream& operator>>(std::istream&, ImgType&);

  enum class CompoundType
    {
    CompoundType_Undef = 0 ,                                         //!< Undefined
    CompoundType_Scalar = 1 << 0 ,                                   //!< scalar 
    CompoundType_3Planes = 1 <<1 ,                                   //!< 3 channels 
    CompoundType_4Planes = 1 <<2 ,                                   //!< 4 channels 
    //CompoundType_Container,                                        //!< generic container //LATER
    };

  //! operator<< for ImgType
  IMP_API std::ostream& operator<<(std::ostream&, CompoundType);

  //! operator>> for ImgType
  IMP_API std::istream& operator>>(std::istream&, CompoundType&);

  enum class PType
    {
    PType_Undef = 0,       //!< Undefined type 
    //PType_Bin = 1 << 0,    //!< data {0,1} encode in  [0,255]
    //PType_BinPack  =  1 << 1,  //!< data {0,1} binary packed 
    PType_GrayUINT8 = 1 << 2,  //!< 8 bits per pixel, unsigned, grayscale data in [0,255]
    PType_GrayINT32 = 1 << 3,  //!< 32 bits per pixel, signed, grayscale data in ]-2^31,+2^31[
    PType_F32 = 1 << 4,  //!< Floating-point pixel, single precision (float)
    PType_GrayINT64 = 1 << 5,  //!< Int64 pixel, think about integrale image
    PType_D64 = 1 << 6,  //!< Floating-point pixel, double precision (long double)
    _PixelType_Max = 1 << 6 //keep sync with the max value
    };

  //! operator<< for PType
  IMP_API std::ostream& operator<<(std::ostream&, PType);

  //! operator>> for PType
  IMP_API std::istream& operator>>(std::istream&, PType&);

  template<typename value_type, int Rank> using compound_pixel = static_array<value_type, Rank>;

  //extern template class compound_pixel < pUINT8, 3 > ;
  //extern template class compound_pixel < pINT32, 3 >;
  //extern template class compound_pixel < pFLOAT, 3 > ;
  //extern template class compound_pixel < pINT64, 3 > ;
  //extern template class compound_pixel < pDOUBLE, 3 >;

  //extern template class compound_pixel < pUINT8, 4 > ;
  //extern template class compound_pixel < pINT32, 4 >;
  //extern template class compound_pixel < pFLOAT, 4 > ;
  //extern template class compound_pixel < pINT64, 4 > ;
  //extern template class compound_pixel < pDOUBLE, 4 >;

  typedef compound_pixel < pUINT8, 3 > c3pUINT8;
  typedef compound_pixel < pINT32, 3 > c3pINT32;
  typedef compound_pixel < pFLOAT, 3 > c3pFLOAT;
  typedef compound_pixel < pINT64, 3 > c3pINT64;
  typedef compound_pixel < pDOUBLE, 3 > c3pDOUBLE;

  typedef compound_pixel < pUINT8, 4 > c4pUINT8;
  typedef compound_pixel < pINT32, 3 > c4pINT32;
  typedef compound_pixel < pFLOAT, 4 > c4pFLOAT;
  typedef compound_pixel < pINT64, 4 > c4pINT64;
  typedef compound_pixel < pDOUBLE, 4 > c4pDOUBLE;

  template <class storagetype>
  struct get_dim
    {
    static const size_t dim= 1;
    };

  template <class storagetype, std::size_t NumDims>
  struct get_dim< compound_pixel<storagetype, NumDims> >
    {
    static const size_t dim = NumDims;
    };

  template<class valuetype>
  struct TypeTraits {};

  template<>
  struct TypeTraits <pUINT8>
    {
    using storage_type = pUINT8;
    using safe_signed_type = pINT32;
    using str_type = pUINT32;
    using accu_type = pINT64;
    static const PType pixel_type = PType::PType_GrayUINT8;
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

    POUTRE_STATIC_CONSTEXPR size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);

    POUTRE_STATIC_CONSTEXPR size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::lowest( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::min( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::max( ); }
    };

  template<>
  struct TypeTraits <pINT32>
    {
    using storage_type = pINT32;
    using safe_signed_type = pINT64;
    using str_type = pINT32;
    using accu_type = pUINT64;

    static const PType pixel_type = PType::PType_GrayINT32;
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

    POUTRE_STATIC_CONSTEXPR size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    POUTRE_STATIC_CONSTEXPR size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest( ) POUTRE_NOEXCEPT{ return std::numeric_limits<storage_type>::lowest( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min( ) POUTRE_NOEXCEPT{ return std::numeric_limits<storage_type>::min( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max( ) POUTRE_NOEXCEPT{ return std::numeric_limits<storage_type>::max( ); }
    };

  template<>
  struct TypeTraits <pFLOAT>
    {
    using storage_type = pFLOAT;
    using safe_signed_type = pFLOAT;
    using str_type = pFLOAT;
    using accu_type = pDOUBLE; 

    static const PType pixel_type = PType::PType_F32;
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

    POUTRE_STATIC_CONSTEXPR size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    POUTRE_STATIC_CONSTEXPR size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::lowest( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::min( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::max( ); }
    };

  template<>
  struct TypeTraits <pDOUBLE>
    {
    using storage_type = pDOUBLE;
    using safe_signed_type = pDOUBLE;
    using str_type = pDOUBLE;
    using accu_type = pDOUBLE;

    static const PType pixel_type = PType::PType_D64;
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

    POUTRE_STATIC_CONSTEXPR size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    POUTRE_STATIC_CONSTEXPR size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::lowest( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::min( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::max( ); }
    };

  template<>
  struct TypeTraits < pINT64 >
    {
    using storage_type = pINT64;
    using safe_signed_type = pINT64;
    using str_type = pINT64;
    using accu_type = pINT64;

    static const PType pixel_type = PType::PType_GrayINT64;
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

    POUTRE_STATIC_CONSTEXPR size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    POUTRE_STATIC_CONSTEXPR size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::lowest( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::min( ); }
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::max( ); }
    };


  template <class valuetype, std::size_t NumDims>
  struct TypeTraits < compound_pixel<valuetype, NumDims> >
    {
    };

  template <class valuetype>
  struct TypeTraits < compound_pixel<valuetype, 3> >
    {
    using storage_type = compound_pixel<valuetype, 3>;
    using safe_signed_type = compound_pixel<typename TypeTraits<valuetype>::safe_signed_type, 3>;
    using str_type = compound_pixel<typename TypeTraits<valuetype>::str_type, 3>;
    using accu_type = compound_pixel < typename TypeTraits<valuetype>::accu_type, 3 >;

    static const PType pixel_type = TypeTraits<valuetype>::pixel_type;
    static const CompoundType compound_type = CompoundType::CompoundType_3Planes;
    POUTRE_STATIC_CONSTEXPR size_t default_padding_size = 1;
    POUTRE_STATIC_CONSTEXPR size_t alignement = 1;

   //todo decltype
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest( ) POUTRE_NOEXCEPT
      { 
      return compound_pixel<valuetype, 3>(std::numeric_limits<valuetype>::lowest( ), std::numeric_limits<valuetype>::lowest( ), std::numeric_limits<valuetype>::lowest( ));
      }
   
    //todo decltype
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR  storage_type min( ) POUTRE_NOEXCEPT
      {
      return compound_pixel<valuetype, 3>(std::numeric_limits<valuetype>::min( ), std::numeric_limits<valuetype>::min( ), std::numeric_limits<valuetype>::min( ));
      }

    //todo decltype
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR  storage_type max( ) POUTRE_NOEXCEPT
      {
      return compound_pixel<valuetype, 3>(std::numeric_limits<valuetype>::max( ), std::numeric_limits<valuetype>::max( ), std::numeric_limits<valuetype>::max( ));
      }
    };

  template <class valuetype>
  struct TypeTraits < compound_pixel<valuetype, 4> >
    {
    using storage_type = compound_pixel<valuetype, 4>;
    using safe_signed_type = compound_pixel<typename  TypeTraits<valuetype>::safe_signed_type, 4>;
    using str_type = compound_pixel<typename  TypeTraits<valuetype>::str_type, 4>;
    using accu_type = compound_pixel < typename TypeTraits<valuetype>::accu_type, 4 >;

    static const PType pixel_type = TypeTraits<valuetype>::pixel_type;
    static const CompoundType compound_type = CompoundType::CompoundType_4Planes;

    POUTRE_STATIC_CONSTEXPR size_t default_padding_size = 1;
    POUTRE_STATIC_CONSTEXPR size_t alignement = 1;

    
    //todo decltype
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest( ) POUTRE_NOEXCEPT
      {
      return compound_pixel<valuetype, 4>(std::numeric_limits<valuetype>::lowest( ), std::numeric_limits<valuetype>::lowest( ), std::numeric_limits<valuetype>::lowest( ), std::numeric_limits<valuetype>::lowest( ));
      }

    //todo decltype
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR  storage_type min( ) POUTRE_NOEXCEPT
      {
      return compound_pixel<valuetype, 4>(std::numeric_limits<valuetype>::min( ), std::numeric_limits<valuetype>::min( ), std::numeric_limits<valuetype>::min( ), std::numeric_limits<valuetype>::min( ));
      }
        
    //todo decltype
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR  storage_type max( ) POUTRE_NOEXCEPT
      {
      return compound_pixel<valuetype, 4>(std::numeric_limits<valuetype>::max( ), std::numeric_limits<valuetype>::max( ), std::numeric_limits<valuetype>::max( ), std::numeric_limits<valuetype>::max( ));
      }
    };

  }
#endif //POUTRE_IMAGEPROCESSING_TYPE_HPP__