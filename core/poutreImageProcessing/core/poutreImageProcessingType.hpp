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
#include <array>

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
    CompoundType_Undef = 0 ,                                         //!< Undefined
    CompoundType_Scalar = 1 << 0 ,                                   //!< scalar 
    CompoundType_3Planes = 1 <<1 ,                                   //!< 3 channels 
    CompoundType_4Planes = 1 <<2 ,                                   //!< 4 channels 
    //CompoundType_Container,                                        //!< generic container //LATER
    };

  //! operator<< for ImgType
  BASE_API std::ostream& operator<<(std::ostream&, CompoundType);

  //! operator>> for ImgType
  BASE_API std::istream& operator>>(std::istream&, CompoundType&);

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
  BASE_API std::ostream& operator<<(std::ostream&, PType);

  //! operator>> for PType
  BASE_API std::istream& operator>>(std::istream&, PType&);

  


  template <typename storage_type, std::size_t NumDims>
  class compound_pixel
    {
    public:
      using  self_type = compound_pixel < storage_type, NumDims > ;
      using  value_type = storage_type;
      using  pointer = value_type*;
      using  const_pointer = const value_type*;
      using  reference = value_type&;
      using  const_reference = value_type const &;
      using  difference_type = std::ptrdiff_t;
      using  size_type = std::size_t;
      POUTRE_STATIC_CONSTEXPR size_t m_numdims = NumDims;
      using pixcontainer = std::array<storage_type, NumDims>;
    public:
      pixcontainer m_pixcontainer;
    public:
      POUTRE_CONSTEXPR compound_pixel( ) POUTRE_NOEXCEPT : m_pixcontainer( )
        {
        }

      POUTRE_CONSTEXPR compound_pixel(const value_type& a) POUTRE_NOEXCEPT : m_pixcontainer( )
          {
          m_pixcontainer.assign(a);
          }

       POUTRE_CONSTEXPR compound_pixel(const std::initializer_list<value_type>& values) :m_pixcontainer()
            {
            if (values.size( ) != m_numdims) 
              POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of compound_pixel container");
            std::copy(values.begin( ), values.end(), m_pixcontainer.begin());
            }

       POUTRE_CONSTEXPR compound_pixel(const self_type& rhs) POUTRE_NOEXCEPT
          {
          std::copy(rhs.begin( ), rhs.end( ), m_pixcontainer.begin( ));
          }

            //is this really needed ?
            compound_pixel(self_type&& rhs) /*POUTRE_NOEXCEPT*/
            {
            *this = std::move(rhs);
            }

          //is this really needed ?
          self_type& operator= (self_type&& rhs) /*POUTRE_NOEXCEPT*/
            {
            if (this != &rhs) // ?? http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
              {
              m_pixcontainer = std::move(rhs::m_pixcontainer);
              }
            return *this;
            }

          //template<class other_container_type
          //  , typename std::enable_if<std::is_convertible<other_container_type::value_type, storage_type>::value>::type* = nullptr>
          //  compound_pixel(const other_container_type& values) : m_pixcontainer{}
          //  {
          //  if (container.size( ) != m_numdims) POUTRE_RUNTIME_ERROR("Invalid input container regarding NumDims of compound_pixel container");
          //  std::copy(values.begin( ), values.end( ), m_pixcontainer.begin( ));
          //  }


          reference operator[] (size_type n) POUTRE_NOEXCEPTONLYNDEBUG
            {
            POUTRE_ASSERTCHECK(n >= 0, "compound_pixel operator[n] n must be in >=0");
            POUTRE_ASSERTCHECK(n < m_numdims, "compound_pixel operator[n] n must be in [0,m_numdims[");
            return m_pixcontainer[n];
            }
              const_reference operator[] (size_type n) const POUTRE_NOEXCEPTONLYNDEBUG
              {
              POUTRE_ASSERTCHECK(n >= 0, "compound_pixel operator[n] n must be in >=0");
              POUTRE_ASSERTCHECK(n < m_numdims, "compound_pixel operator[n] n must be in [0,m_numdims[");
              return m_pixcontainer[n];
              }

                bool operator==(const self_type& rhs) const POUTRE_NOEXCEPT
                {
                return  std::equal(m_pixcontainer.cbegin( ), m_pixcontainer.cend( ), rhs.m_pixcontainer.cbegin());
                }
                  bool operator!=(const self_type& rhs) const POUTRE_NOEXCEPT
                  {
                  return  !(std::equal(m_pixcontainer.cbegin( ), m_pixcontainer.cend( ), rhs.m_pixcontainer.cbegin( )));
                  }

      /* template<typename storagetype, size_t NumDim>
       friend std::istream& operator<<(std::istream& in, const compound_pixel < storage_type, NumDims>& rhs) POUTRE_NOEXCEPT;*/

         friend std::ostream& operator<<(std::ostream& out, const compound_pixel < storage_type, NumDims>& rhs) POUTRE_NOEXCEPT
         {
         for (const auto& val : rhs.m_pixcontainer)
           {
           out << val;
           out << ",";
           }
         return out;
         }

    };

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
      POUTRE_CONSTEXPR compound_pixel( ) POUTRE_NOEXCEPT : m_a0(0), m_a1(0), m_a2(0)
        {
        }
      POUTRE_CONSTEXPR compound_pixel(const value_type& a0, const value_type& a1, const value_type& a2) POUTRE_NOEXCEPT : m_a0(a0), m_a1(a1), m_a2(a2)  {}
      POUTRE_CONSTEXPR compound_pixel(const value_type& a)  POUTRE_NOEXCEPT : m_a0(a), m_a1(a), m_a2(a)  {}
      POUTRE_CONSTEXPR compound_pixel(const self_type& rhs) POUTRE_NOEXCEPT : m_a0(rhs.m_a0), m_a1(rhs.m_a1), m_a2(rhs.m_a2)  {}

      //is this really needed ?
      compound_pixel(self_type&& rhs) POUTRE_NOEXCEPT
        { *this = std::move(rhs); }
        //is this really needed ?
        self_type& operator= (self_type&& rhs) POUTRE_NOEXCEPT
        { m_a0 = rhs.m_a0; m_a1 = rhs.m_a1; m_a2 = rhs.m_a2; return *this; }

        
        template<class other_storage_type
        , typename std::enable_if<std::is_convertible<other_storage_type, storage_type>::value>::type* = nullptr>
        compound_pixel(const compound_pixel<other_storage_type, m_numdims>& rhs) POUTRE_NOEXCEPT : m_a0(static_cast<storage_type>(rhs.m_a0)), m_a1(static_cast<storage_type>(rhs.m_a1)), m_a2(static_cast<storage_type>(rhs.m_a2))
        {
        }

      reference operator[] (size_type n) POUTRE_NOEXCEPTONLYNDEBUG
        {
        POUTRE_ASSERTCHECK(n >= 0, "compound_pixel 4 operator[n] n must be in [0,3[");
        POUTRE_ASSERTCHECK(n < 3, "compound_pixel 4 operator[n] n must be in [0,3[");
        if (n == 0) return m_a0;
        if (n == 1) return m_a1;
        if (n == 2) return m_a2;
        }
          const_reference operator[] (size_type n) const POUTRE_NOEXCEPTONLYNDEBUG
          {
          POUTRE_ASSERTCHECK(n >= 0, "compound_pixel 4 operator[n] n must be in [0,3[");
          POUTRE_ASSERTCHECK(n < 3, "compound_pixel 4 operator[n] n must be in [0,3[");
          if (n == 0) return m_a0;
          if (n == 1) return m_a1;
          if (n == 2) return m_a2;
          }

            bool operator==(const self_type& rhs) const POUTRE_NOEXCEPT
            {
            return  (m_a0 == rhs.m_a0) && (m_a1 == rhs.m_a1) && (m_a2 == rhs.m_a2);
            }
              bool operator!=(const self_type& rhs) const POUTRE_NOEXCEPT
              {
              return  (m_a0 != rhs.m_a0) || (m_a1 != rhs.m_a1) || (m_a2 != rhs.m_a2);
              }
      /*template<typename storagetype>
      friend std::istream& operator<<(std::istream& in, const compound_pixel < storage_type, 3>& rhs) POUTRE_NOEXCEPT;*/

      friend std::ostream& operator<<(std::ostream& out, const compound_pixel < storage_type, 3>& rhs) POUTRE_NOEXCEPT
        {
        out << rhs.m_a0;
        out << ",";
        out << rhs.m_a1;
        out << ",";
        out << rhs.m_a2;
        out << ",";
        return out;
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
      POUTRE_CONSTEXPR compound_pixel( ) POUTRE_NOEXCEPT : m_a0(0), m_a1(0), m_a2(0), m_a3(0)
        {
        }
      POUTRE_CONSTEXPR compound_pixel(const value_type& a0, const value_type& a1, const value_type& a2, const value_type& a3) POUTRE_NOEXCEPT : m_a0(a0), m_a1(a1), m_a2(a2), m_a3(a3)  {}
      POUTRE_CONSTEXPR compound_pixel(const value_type& a)  POUTRE_NOEXCEPT : m_a0(a), m_a1(a), m_a2(a), m_a3(a)  {}
      POUTRE_CONSTEXPR compound_pixel(const self_type& rhs) POUTRE_NOEXCEPT : m_a0(rhs.m_a0), m_a1(rhs.m_a1), m_a2(rhs.m_a2), m_a3(rhs.m_a3)  {}

      //is this really needed ?
      compound_pixel(self_type&& rhs) POUTRE_NOEXCEPT
        { *this = std::move(rhs); }

        //is this really needed ?
        self_type& operator= (self_type&& rhs) POUTRE_NOEXCEPT
        { m_a0 = rhs.m_a0; m_a1 = rhs.m_a1; m_a2 = rhs.m_a2; m_a3 = rhs.m_a3; return *this; }

        // foo2 overload is enabled via a parameter
        template<class other_storage_type
        , typename std::enable_if<std::is_convertible<other_storage_type, storage_type>::value>::type* = nullptr
        >
        compound_pixel(const compound_pixel<other_storage_type, m_numdims>& rhs) POUTRE_NOEXCEPT : m_a0(static_cast<storage_type>(rhs.m_a0)), m_a1(static_cast<storage_type>(rhs.m_a1)), m_a2(static_cast<storage_type>(rhs.m_a2)), m_a3(static_cast<storage_type>(rhs.m_a3))
        {
        }

      reference operator[] (size_type n) POUTRE_NOEXCEPTONLYNDEBUG
        {
        POUTRE_ASSERTCHECK(n >= 0, "compound_pixel 4 operator[n] n must be in [0,4[");
        POUTRE_ASSERTCHECK(n < 4, "compound_pixel 4 operator[n] n must be in [0,4[");
        if (n == 0) return m_a0;
        if (n == 1) return m_a1;
        if (n == 2) return m_a2;
        if (n == 3) return m_a3;
        
        }
          const_reference operator[] (size_type n) const POUTRE_NOEXCEPTONLYNDEBUG
          {
          POUTRE_ASSERTCHECK(n >= 0, "compound_pixel 4 operator[n] n must be in [0,4[");
          POUTRE_ASSERTCHECK(n < 4, "compound_pixel 4 operator[n] n must be in [0,4[");
          if (n == 0) return m_a0;
          if (n == 1) return m_a1;
          if (n == 2) return m_a2;
          if (n == 3) return m_a3;          
          }

            bool operator==(const self_type& rhs) const POUTRE_NOEXCEPT
            {
            return  (m_a0 == rhs.m_a0) && (m_a1 == rhs.m_a1) && (m_a2 == rhs.m_a2) && (m_a3 == rhs.m_a3);
            }
              bool operator!=(const self_type& rhs) const POUTRE_NOEXCEPT
              {
              return  (m_a0 != rhs.m_a0) || (m_a1 != rhs.m_a1) || (m_a2 != rhs.m_a2) || (m_a3 != rhs.m_a3);
              }
     /* template<typename storagetype>
      friend std::istream& operator<<(std::istream& in, const compound_pixel < storage_type, 4>& rhs) POUTRE_NOEXCEPT;*/

      friend std::ostream& operator<<(std::ostream& out, const compound_pixel < storage_type, 4>& rhs) POUTRE_NOEXCEPT
        {
        out << rhs.m_a0;
        out << ",";
        out << rhs.m_a1;
        out << ",";
        out << rhs.m_a2;
        out << ",";
        out << rhs.m_a3;
        out << ",";
        return out;
        }

    };

  extern template class compound_pixel < pUINT8, 3 > ;
  extern template class compound_pixel < pINT32, 3 >;
  extern template class compound_pixel < pFLOAT, 3 > ;
  extern template class compound_pixel < pINT64, 3 > ;
  extern template class compound_pixel < pDOUBLE, 3 >;

  extern template class compound_pixel < pUINT8, 4 > ;
  extern template class compound_pixel < pINT32, 4 >;
  extern template class compound_pixel < pFLOAT, 4 > ;
  extern template class compound_pixel < pINT64, 4 > ;
  extern template class compound_pixel < pDOUBLE, 4 >;

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

    BOOST_STATIC_CONSTEXPR size_t default_padding_size = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
    BOOST_STATIC_CONSTEXPR size_t alignement = (size_t)BOOST_SIMD_CONFIG_ALIGNMENT;

    BOOST_STATIC_CONSTEXPR storage_type lowest( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::lowest( ); }
    BOOST_STATIC_CONSTEXPR storage_type min( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::min( ); }
    BOOST_STATIC_CONSTEXPR storage_type max( ) POUTRE_NOEXCEPT { return std::numeric_limits<storage_type>::max( ); }
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
#endif //POUTREIMAGEPROCESSINGTYPE_HPP__