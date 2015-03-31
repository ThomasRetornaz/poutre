#ifndef POUTREIMAGEPROCESSINGTYPE_HPP__
#define POUTREIMAGEPROCESSINGTYPE_HPP__

#ifndef POUTREIMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRETYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

#include <limits>       // std::numeric_limits
#include <boost/simd/preprocessor/parameters.hpp> //default alignment

namespace poutre
  {
  enum class PType
    {
    PType_Undef = 0,       //!< Undefined type 
    PType_Bin = 1 << 0,    //!< data {0,1} encode in  [0,255]
    //PType_BinPack  =  1 << 1,  //!< 1 bit per pixel binary data (TODO ?)
    PType_GrayUINT8 = 1 << 2,  //!< 8 bits per pixel, unsigned, grayscale data in [0,255]
    PType_GrayINT32 = 1 << 3,  //!< 32 bits per pixel, signed, grayscale data in ]-2^31,+2^31[
    PType_F32 = 1 << 4,  //!< Floating-point pixel, single precision (float)
    PType_GrayINT64 = 1 << 5,  //!< Int64 pixel, integrale image and so on 
    _PixelType_Max = 1 << 5 //sync with the max value
    };

  //! operator<< for PType
  BASE_API std::ostream& operator<<(std::ostream&, PType);

  //! operator>> for PType
  BASE_API std::istream& operator>>(std::istream&, PType&);

  template<PType>
  struct TypeTraits{};

  template<> struct TypeTraits < PType::PType_GrayUINT8 >
    {
    using storage_type = pUINT8;
    using safe_signed_type = pINT32;
    using str_type = pUINT32;
    using accu_type = pINT64;

    static const size_t default_padding_size = BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);

    static const size_t alignement = BOOST_SIMD_CONFIG_ALIGNMENT;

    inline static storage_type lowest()      { return std::numeric_limits<storage_type>::lowest(); }
    inline static storage_type min()         { return std::numeric_limits<storage_type>::min(); }
    inline static storage_type max()         { return std::numeric_limits<storage_type>::max(); }
    };

    template<> struct TypeTraits < PType::PType_Bin >//TODO switch to bitapacking later
      {
      using storage_type = pUINT8;
      using safe_signed_type = pINT32;
      using str_type = pUINT32;
      using accu_type = pUINT64;

      static const size_t default_padding_size = BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
      static const size_t alignement = BOOST_SIMD_CONFIG_ALIGNMENT;

      inline static storage_type lowest()      { return std::numeric_limits<storage_type>::lowest(); }
      inline static storage_type min()         { return std::numeric_limits<storage_type>::min(); }
      inline static storage_type max()         { return std::numeric_limits<storage_type>::max(); }
      };


      template<> struct TypeTraits < PType::PType_GrayINT32 > 
        {
        using storage_type = pINT32;
        using safe_signed_type = pINT64;
        using str_type = pINT32;
        using accu_type = pUINT64;

        static const size_t default_padding_size = BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
        static const size_t alignement = BOOST_SIMD_CONFIG_ALIGNMENT;

        inline static storage_type lowest()      { return std::numeric_limits<storage_type>::lowest(); }
        inline static storage_type min()         { return std::numeric_limits<storage_type>::min(); }
        inline static storage_type max()         { return std::numeric_limits<storage_type>::max(); }
        };

        template<> struct TypeTraits < PType::PType_F32 > 
          {
          using storage_type = pFLOAT;
          using safe_signed_type = pFLOAT;
          using str_type = pFLOAT;
          using accu_type = pFLOAT; //todo long double ?

          static const size_t default_padding_size = BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
          static const size_t alignement = BOOST_SIMD_CONFIG_ALIGNMENT;

          inline static storage_type lowest()      { return std::numeric_limits<storage_type>::lowest(); }
          inline static storage_type min()         { return std::numeric_limits<storage_type>::min(); }
          inline static storage_type max()         { return std::numeric_limits<storage_type>::max(); }
          };

          template<> struct TypeTraits < PType::PType_GrayINT64 > 
            {
            using storage_type = pINT64;
            using safe_signed_type = pINT64;
            using str_type = pINT64;
            using accu_type = pINT64;

            static const size_t default_padding_size = BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);
            static const size_t alignement = BOOST_SIMD_CONFIG_ALIGNMENT;

            inline static storage_type lowest()      { return std::numeric_limits<storage_type>::lowest(); }
            inline static storage_type min()         { return std::numeric_limits<storage_type>::min(); }
            inline static storage_type max()         { return std::numeric_limits<storage_type>::max(); }
            };

  }
#endif //POUTREIMAGEPROCESSINGTYPE_HPP__