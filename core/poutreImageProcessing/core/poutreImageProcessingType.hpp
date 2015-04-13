#ifndef POUTREIMAGEPROCESSINGTYPE_HPP__
#define POUTREIMAGEPROCESSINGTYPE_HPP__

#ifndef POUTREIMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRETYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
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

  enum class CompoundType
    {
    CompoundType_Undef,                                         //!< Undefined
    CompoundType_Scalar,                                        //!< scalar 
    CompoundType_3Planes,                                       //!< 3 channels 
    CompoundType_4Planes,                                       //!< 4 channels 
    CompoundType_Container,                                     //!< generic container
    };

  //! operator<< for ImgType
  BASE_API std::ostream& operator<<(std::ostream&, CompoundType);

  //! operator>> for ImgType
  BASE_API std::istream& operator>>(std::istream&, CompoundType&);

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

  //#ifdef POUTRE_64BITS
  //  typedef PType::PType_GrayINT64  PTypeLabel;
  //#else
  //  using PTypeLabel = PType::PType_GrayINT32;
  //#endif

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



  template <typename storage_type, std::size_t NumDims>
  class compound_pixel;

  template <typename storage_type>
  class compound_pixel < storage_type, 3 >
    {
    public:
      using  self_type = compound_pixel < storage_type, 3 > ;

      using  value_type = storage_type;
      using  pointer = value_type*;
      using  const_pointer = const value_type*;
      using  reference = value_type&;
      using  const_reference = value_type const &;
      using  difference_type = std::ptrdiff_t;
      using  size_type = std::size_t;

      static const size_t m_numdims = 3;

    public:
      value_type m_a0, m_a1, m_a2;

    public:
      compound_pixel( ) NOEXCEPT : m_a0(0), m_a1(0), m_a2(0)
        {
        }
      compound_pixel(const value_type& a0, const value_type& a1, const value_type& a2) NOEXCEPT : m_a0(a0), m_a1(a1), m_a2(a2)  {}
      compound_pixel(const value_type& a)  NOEXCEPT : m_a0(a), m_a1(a), m_a2(a)  {}
      compound_pixel(const self_type& rhs) NOEXCEPT : m_a0(rhs.m_a0), m_a1(rhs.m_a1), m_a2(rhs.m_a2)  {}

      //is this really needed ?
      compound_pixel(self_type&& rhs) NOEXCEPT
        { *this = std::move(rhs); }
        //is this really needed ?
        self_type& operator= (self_type&& rhs) NOEXCEPT
        { m_a0 = rhs.m_a0; m_a1 = rhs.m_a1; m_a2 = rhs.m_a2; return *this; }

        // foo2 overload is enabled via a parameter
        template<class other_storage_type
        , typename std::enable_if<std::is_convertible<other_storage_type, storage_type>::value>::type* = nullptr>
        compound_pixel(const compound_pixel<other_storage_type, m_numdims>& rhs) NOEXCEPT : m_a0(static_cast<storage_type>(rhs.m_a0)), m_a1(static_cast<storage_type>(rhs.m_a1)), m_a2(static_cast<storage_type>(rhs.m_a2))
        {
        }

      reference operator[] (size_type n) NOEXCEPTRELONLYNDEBUG
        {
        POUTRE_ASSERTCHECK(n >= 0, "compound_pixel 4 operator[n] n must be in [0,3[");
        POUTRE_ASSERTCHECK(n < 3, "compound_pixel 4 operator[n] n must be in [0,3[");
        if (n == 0) return m_a0;
        if (n == 1) return m_a1;
        if (n == 2) return m_a2;
        }
          const_reference operator[] (size_type n) const NOEXCEPTRELONLYNDEBUG
          {
          POUTRE_ASSERTCHECK(n >= 0, "compound_pixel 4 operator[n] n must be in [0,3[");
          POUTRE_ASSERTCHECK(n < 3, "compound_pixel 4 operator[n] n must be in [0,3[");
          if (n == 0) return m_a0;
          if (n == 1) return m_a1;
          if (n == 2) return m_a2;
          }

            bool operator==(const self_type& rhs) const NOEXCEPT
            {
            return  (m_a0 == rhs.m_a0) && (m_a1 == rhs.m_a1) && (m_a2 == rhs.m_a2);
            }
              bool operator!=(const self_type& rhs) const NOEXCEPT
              {
              return  (m_a0 != rhs.m_a0) || (m_a1 != rhs.m_a1) || (m_a2 != rhs.m_a2);
              }
    };




  template <typename storage_type>
  class compound_pixel < storage_type, 4 >
    {
    public:
      using  self_type = compound_pixel < storage_type, 4 > ;

      using  value_type = storage_type;
      using  pointer = value_type*;
      using  const_pointer = const value_type*;
      using  reference = value_type&;
      using  const_reference = value_type const &;
      using  difference_type = std::ptrdiff_t;
      using  size_type = std::size_t;

      static const size_t m_numdims = 4;

    public:
      value_type m_a0, m_a1, m_a2, m_a3;

    public:
      compound_pixel( ) NOEXCEPT : m_a0(0), m_a1(0), m_a2(0), m_a3(0)
        {
        }
      compound_pixel(const value_type& a0, const value_type& a1, const value_type& a2, const value_type& a3) NOEXCEPT : m_a0(a0), m_a1(a1), m_a2(a2), m_a3(a3)  {}
      compound_pixel(const value_type& a)  NOEXCEPT : m_a0(a), m_a1(a), m_a2(a), m_a3(a)  {}
      compound_pixel(const self_type& rhs) NOEXCEPT : m_a0(rhs.m_a0), m_a1(rhs.m_a1), m_a2(rhs.m_a2), m_a3(rhs.m_a3)  {}

      //is this really needed ?
      compound_pixel(self_type&& rhs) NOEXCEPT
        { *this = std::move(rhs); }

        //is this really needed ?
        self_type& operator= (self_type&& rhs) NOEXCEPT
        { m_a0 = rhs.m_a0; m_a1 = rhs.m_a1; m_a2 = rhs.m_a2; m_a3 = rhs.m_a3; return *this; }

        // foo2 overload is enabled via a parameter
        template<class other_storage_type
        , typename std::enable_if<std::is_convertible<other_storage_type, storage_type>::value>::type* = nullptr
        >
        compound_pixel(const compound_pixel<other_storage_type, m_numdims>& rhs) NOEXCEPT : m_a0(static_cast<storage_type>(rhs.m_a0)), m_a1(static_cast<storage_type>(rhs.m_a1)), m_a2(static_cast<storage_type>(rhs.m_a2)), m_a3(static_cast<storage_type>(rhs.m_a3))
        {
        }

      reference operator[] (size_type n) NOEXCEPTRELONLYNDEBUG
        {
        POUTRE_ASSERTCHECK(n >= 0, "compound_pixel 4 operator[n] n must be in [0,4[");
        POUTRE_ASSERTCHECK(n < 4, "compound_pixel 4 operator[n] n must be in [0,4[");
        if (n == 0) return m_a0;
        if (n == 1) return m_a1;
        if (n == 2) return m_a2;
        if (n == 3) return m_a3;
        }
          const_reference operator[] (size_type n) const NOEXCEPTRELONLYNDEBUG
          {
          POUTRE_ASSERTCHECK(n >= 0, "compound_pixel 4 operator[n] n must be in [0,4[");
          POUTRE_ASSERTCHECK(n < 4, "compound_pixel 4 operator[n] n must be in [0,4[");
          if (n == 0) return m_a0;
          if (n == 1) return m_a1;
          if (n == 2) return m_a2;
          if (n == 3) return m_a3;
          }

            bool operator==(const self_type& rhs) const NOEXCEPT
            {
            return  (m_a0 == rhs.m_a0) && (m_a1 == rhs.m_a1) && (m_a2 == rhs.m_a2) && (m_a3 == rhs.m_a3);
            }
              bool operator!=(const self_type& rhs) const NOEXCEPT
              {
              return  (m_a0 != rhs.m_a0) || (m_a1 != rhs.m_a1) || (m_a2 != rhs.m_a2) || (m_a3 != rhs.m_a3);
              }
    };


  extern template class compound_pixel < pUINT8, 3 > ;
  extern template class compound_pixel < pFLOAT, 3 > ;
  extern template class compound_pixel < pINT64, 3 > ;

  extern template class compound_pixel < pUINT8, 4 > ;
  extern template class compound_pixel < pFLOAT, 4 > ;
  extern template class compound_pixel < pINT64, 4 > ;

  template<PType>
  struct TypeTraits {};

  template<>
  struct TypeTraits < PType::PType_GrayUINT8 >
    {
    using storage_type = pUINT8;
    using safe_signed_type = pINT32;
    using str_type = pUINT32;
    using accu_type = pINT64;
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

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
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

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
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

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
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

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
    static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

    static const size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    static const size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    inline static storage_type lowest( ) { return std::numeric_limits<storage_type>::lowest( ); }
    inline static storage_type min( ) { return std::numeric_limits<storage_type>::min( ); }
    inline static storage_type max( ) { return std::numeric_limits<storage_type>::max( ); }
    };




  }
#endif //POUTREIMAGEPROCESSINGTYPE_HPP__