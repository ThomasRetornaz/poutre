#ifndef POUTREIMAGEPROCESSINGTYPE_HPP__
#define POUTREIMAGEPROCESSINGTYPE_HPP__

#ifndef POUTREIMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRETYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

#include <limits>
#include <type_traits>
#include <boost/simd/preprocessor/parameters.hpp> //default alignment
#include <stdexcept>

namespace poutre
  {
  enum class ImgType
    {
    ImgType_Undef = 0,          //!< Undefined type 
    ImgType_Dense = 1 << 0,     //!< Dense, contiguous  
    ImgType_Sparse = 1 << 1,    //!< Sparse
    };

  //! operator<< for ImgType
  BASE_API std::ostream& operator<<(std::ostream&, ImgType);

  //! operator>> for ImgType
  BASE_API std::istream& operator>>(std::istream&, ImgType&);

  enum class PType
    {
    PType_Undef = 0,       //!< Undefined type 
    PType_Bin = 1 << 0,    //!< data {0,1} encode in  [0,255]
    //PType_BinPack  =  1 << 1,  //!< data {0,1} binary packed 
    PType_GrayUINT8 = 1 << 2,  //!< 8 bits per pixel, unsigned, grayscale data in [0,255]
    PType_GrayINT32 = 1 << 3,  //!< 32 bits per pixel, signed, grayscale data in ]-2^31,+2^31[
    PType_F32 = 1 << 4,  //!< Floating-point pixel, single precision (float)
    PType_GrayINT64 = 1 << 5,  //!< Int64 pixel, think about integrale image
    _PixelType_Max = 1 << 5 //keep sync with the max value
    };

  //! operator<< for PType
  BASE_API std::ostream& operator<<(std::ostream&, PType);

  //! operator>> for PType
  BASE_API std::istream& operator>>(std::istream&, PType&);

  /*!
  * @brief Compare Operator
  */
  enum class CompOpType
    {
    CompOpUndef = 0,       //!< Undefined type 
    CompOpEqual = 1 << 0, //!p1==p2 
    CompOpDiff = 1 << 1,     //!p1!=p2 
    CompOpSup = 1 << 2,      //!p1>p2 
    CompOpSupEqual = 1 << 3, //!p1>=p2 
    CompOpInf = 1 << 4,      //!p1<p2
    CompOpInfEqual = 1 << 5,  //!p1<=p2
    _CompOp_Max = 1 << 5 //sync with the max value
    };

  //! operator<< for CompOpType
  BASE_API std::ostream& operator<<(std::ostream&, CompOpType);

  //! operator>> for CompOpType
  BASE_API std::istream& operator>>(std::istream&, CompOpType&);

  template<PType>
  struct TypeTraits {};

  template<>
  struct TypeTraits < PType::PType_GrayUINT8 >
    {
    using storage_type = pUINT8;
    using safe_signed_type = pINT32;
    using str_type = pUINT32;
    using accu_type = pINT64;

    static const size_t /*constexpr const size_t*/ default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);

    static const size_t /*constexpr const size_t*/ alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    inline static storage_type lowest( ) { return std::numeric_limits<storage_type>::lowest( ); }
    inline static storage_type min( ) { return std::numeric_limits<storage_type>::min( ); }
    inline static storage_type max( ) { return std::numeric_limits<storage_type>::max( ); }
    };

  template<>
  struct TypeTraits < PType::PType_Bin >//TODO switch to bitapacking later
    {
    using storage_type = pUINT8;
    using safe_signed_type = pINT32;
    using str_type = pUINT32;
    using accu_type = pUINT64;

    static const size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    static const size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    inline static storage_type lowest( ) { return std::numeric_limits<storage_type>::lowest( ); }
    inline static storage_type min( ) { return std::numeric_limits<storage_type>::min( ); }
    inline static storage_type max( ) { return std::numeric_limits<storage_type>::max( ); }
    };


  template<>
  struct TypeTraits < PType::PType_GrayINT32 >
    {
    using storage_type = pINT32;
    using safe_signed_type = pINT64;
    using str_type = pINT32;
    using accu_type = pUINT64;

    static const size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    static const size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    inline static storage_type lowest( ) { return std::numeric_limits<storage_type>::lowest( ); }
    inline static storage_type min( ) { return std::numeric_limits<storage_type>::min( ); }
    inline static storage_type max( ) { return std::numeric_limits<storage_type>::max( ); }
    };

  template<>
  struct TypeTraits < PType::PType_F32 >
    {
    using storage_type = pFLOAT;
    using safe_signed_type = pFLOAT;
    using str_type = pFLOAT;
    using accu_type = pFLOAT; //todo long double ?

    static const size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    static const size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    inline static storage_type lowest( ) { return std::numeric_limits<storage_type>::lowest( ); }
    inline static storage_type min( ) { return std::numeric_limits<storage_type>::min( ); }
    inline static storage_type max( ) { return std::numeric_limits<storage_type>::max( ); }
    };

  template<>
  struct TypeTraits < PType::PType_GrayINT64 >
    {
    using storage_type = pINT64;
    using safe_signed_type = pINT64;
    using str_type = pINT64;
    using accu_type = pINT64;

    static const size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    static const size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    inline static storage_type lowest( ) { return std::numeric_limits<storage_type>::lowest( ); }
    inline static storage_type min( ) { return std::numeric_limits<storage_type>::min( ); }
    inline static storage_type max( ) { return std::numeric_limits<storage_type>::max( ); }
    };




  }
#endif //POUTREIMAGEPROCESSINGTYPE_HPP__