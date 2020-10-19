
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_TYPE_HPP__
#define POUTRE_IMAGEPROCESSING_TYPE_HPP__

/**
 * @file   poutreImageProcessingType.hpp
 * @author Thomas Retornaz
 * @brief  Define type and alias for Image
 *
 *
 */

#include <limits>
#include <stdexcept>
#include <type_traits>

#include <poutreBase/include/poutreSimd.hpp>
#include <poutreBase/poutreConfig.hpp>
#include <poutreBase/poutreCoordinate.hpp>
#include <poutreBase/poutreStaticContainer.hpp>
#include <poutreBase/poutreTypes.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>

namespace poutre
{

    /**
     * @addtogroup image_processing_type_group Image Processing Types
     * @ingroup image_processing_group
     *@{
     */

    /*! Type of Image
     *
     */
    enum class ImgType
    {
        ImgType_Undef = 0,       //!< Undefined type
        ImgType_Dense = 1 << 0,  //!< Dense, contiguous
        ImgType_Sparse = 1 << 1, //!< Sparse
    };

    //! operator<< for ImgType
    IMP_CORE_API std::ostream &operator<<(std::ostream &, ImgType);

    //! operator>> for ImgType
    IMP_CORE_API std::istream &operator>>(std::istream &, ImgType &);

    enum class CompoundType
    {
        CompoundType_Undef = 0,        //!< Undefined
        CompoundType_Scalar = 1 << 0,  //!< scalar
        CompoundType_3Planes = 1 << 1, //!< 3 channels
        CompoundType_4Planes = 1 << 2, //!< 4 channels
                                       // CompoundType_Container,                                        //!< generic
                                       // container //LATER
    };

    //! operator<< for ImgType
    IMP_CORE_API std::ostream &operator<<(std::ostream &, CompoundType);

    //! operator>> for ImgType
    IMP_CORE_API std::istream &operator>>(std::istream &, CompoundType &);

    enum class PType
    {
        PType_Undef = 0, //!< Undefined type
        // PType_Bin = 1 << 0,    //!< data {0,1} encode in  [0,255]
        // PType_BinPack  =  1 << 1,  //!< data {0,1} binary packed
        PType_GrayUINT8 = 1 << 2, //!< 8 bits per pixel, unsigned, grayscale data in [0,255]
        PType_GrayINT32 = 1 << 3, //!< 32 bits per pixel, signed, grayscale data in ]-2^31,+2^31[
        PType_F32 = 1 << 4,       //!< Floating-point pixel, single precision (float)
        PType_GrayINT64 = 1 << 5, //!< Int64 pixel, think about integrale image
        PType_D64 = 1 << 6,       //!< Floating-point pixel, double precision (long double)
        _PixelType_Max = 1 << 6   // keep sync with the max value
    };

    //! operator<< for PType
    IMP_CORE_API std::ostream &operator<<(std::ostream &, PType);

    //! operator>> for PType
    IMP_CORE_API std::istream &operator>>(std::istream &, PType &);

    template <typename value_type, ptrdiff_t Rank> using compound = static_array<value_type, Rank>;

    using c3pUINT8 = compound<pUINT8, 3>;
    using c3pINT32 = compound<pINT32, 3>;
    using c3pFLOAT = compound<pFLOAT, 3>;
    using c3pINT64 = compound<pINT64, 3>;
    using c3pDOUBLE = compound<pDOUBLE, 3>;

    using c4pUINT8 = compound<pUINT8, 4>;
    using c4pINT32 = compound<pINT32, 4>;
    using c4pFLOAT = compound<pFLOAT, 4>;
    using c4pINT64 = compound<pINT64, 4>;
    using c4pDOUBLE = compound<pDOUBLE, 4>;

    //! Helper to retrieve dim off pixel storage type
    template <class storagetype> struct get_dim
    {
        POUTRE_STATIC_CONSTEXPR ptrdiff_t dim = 1;
    };

    //! Helper to retrieve dim off compound
    template <class storagetype, std::ptrdiff_t Rank> struct get_dim<compound<storagetype, Rank>>
    {
        POUTRE_STATIC_CONSTEXPR ptrdiff_t dim = Rank;
    };

    //! Define TypeTraits
    template <class valuetype> struct TypeTraits
    {
    };

    //! TypeTraits pUINT8
    template <> struct TypeTraits<pUINT8>
    {
        using storage_type = pUINT8;
        using safe_signed_type = pINT32;
        using str_type = pUINT32;
        using accu_type = pINT64;
        static const PType pixel_type = PType::PType_GrayUINT8;
        static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

        // POUTRE_STATIC_CONSTEXPR size_t default_padding_size =
        // (size_t)BOOST_SIMD_CONFIG_ALIGNMENT / sizeof(storage_type);

        POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_INT8_SIZE;
        using simd_type = typename xs::batch<storage_type, simd_loop_step>;
        using simd_mask_type = typename xs::batch_bool<storage_type, simd_loop_step>;
        POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(storage_type) * 8;

        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::lowest();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::max();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };

    template <> struct TypeTraits<const pUINT8> : public TypeTraits<pUINT8>
    {
    };

    //! TypeTraits pINT32
    template <> struct TypeTraits<pINT32>
    {
        using storage_type = pINT32;
        using safe_signed_type = pINT64;
        using str_type = pINT32;
        using accu_type = pINT64;

        static const PType pixel_type = PType::PType_GrayINT32;
        static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

        POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_INT32_SIZE;
        using simd_type = typename xs::batch<storage_type, simd_loop_step>;
        using simd_mask_type = typename xs::batch_bool<storage_type, simd_loop_step>;
        POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(storage_type) * 8;

        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::lowest();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::max();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };

    template <> struct TypeTraits<const pINT32> : public TypeTraits<pINT32>
    {
    };

    //! TypeTraits pFLOAT
    template <> struct TypeTraits<pFLOAT>
    {
        using storage_type = pFLOAT;
        using safe_signed_type = pFLOAT;
        using str_type = pFLOAT;
        using accu_type = pDOUBLE;

        static const PType pixel_type = PType::PType_F32;
        static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

        POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_FLOAT_SIZE;
        using simd_type = typename xs::batch<storage_type, simd_loop_step>;
        using simd_mask_type = typename xs::batch_bool<storage_type, simd_loop_step>;

        POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(pFLOAT) * 8;

        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::lowest();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::max();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };

    template <> struct TypeTraits<const pFLOAT> : public TypeTraits<pFLOAT>
    {
    };

    //! TypeTraits pDOUBLE
    template <> struct TypeTraits<pDOUBLE>
    {
        using storage_type = pDOUBLE;
        using safe_signed_type = pDOUBLE;
        using str_type = pDOUBLE;
        using accu_type = pDOUBLE;

        static const PType pixel_type = PType::PType_D64;
        static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

        POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_DOUBLE_SIZE;
        using simd_type = typename xs::batch<storage_type, simd_loop_step>;
        using simd_mask_type = typename xs::batch_bool<storage_type, simd_loop_step>;

        POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(pDOUBLE) * 8;

        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::lowest();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::max();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };

    template <> struct TypeTraits<const pDOUBLE> : public TypeTraits<pDOUBLE>
    {
    };

    //! TypeTraits pINT64
    template <> struct TypeTraits<pINT64>
    {
        using storage_type = pINT64;
        using safe_signed_type = pINT64;
        using str_type = pINT64;
        using accu_type = pINT64;

        static const PType pixel_type = PType::PType_GrayINT64;
        static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

        POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_INT64_SIZE;
        POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(pINT64) * 8;
        using simd_type = typename xs::batch<storage_type, simd_loop_step>;
        using simd_mask_type = typename xs::batch_bool<storage_type, simd_loop_step>;

        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::lowest();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return std::numeric_limits<storage_type>::max();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };

    template <> struct TypeTraits<const pINT64> : public TypeTraits<pINT64>
    {
    };

    //! TypeTraits compound
    template <class valuetype, std::ptrdiff_t Rank> struct TypeTraits<compound<valuetype, Rank>>
    {
    };

    //! TypeTraits compound 3 channel
    // template <class valuetype> struct TypeTraits<compound<valuetype, 3>>
    // {
    //     using storage_type = compound<valuetype, 3>;
    //     using safe_signed_type = compound<typename TypeTraits<valuetype>::safe_signed_type, 3>;
    //     using str_type = compound<typename TypeTraits<valuetype>::str_type, 3>;
    //     using accu_type = compound<typename TypeTraits<valuetype>::accu_type, 3>;

    //     static const PType pixel_type = TypeTraits<valuetype>::pixel_type;
    //     static const CompoundType compound_type = CompoundType::CompoundType_3Planes;

    //     POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
    //     POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

    //     // todo decltype
    //     POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
    //     {
    //         return compound<valuetype, 3>(std::numeric_limits<valuetype>::lowest(),
    //                                             std::numeric_limits<valuetype>::lowest(),
    //                                             std::numeric_limits<valuetype>::lowest());
    //     }

    //     // todo decltype
    //     POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
    //     {
    //         return compound<valuetype, 3>(std::numeric_limits<valuetype>::min(),
    //                                             std::numeric_limits<valuetype>::min(),
    //                                             std::numeric_limits<valuetype>::min());
    //     }

    //     // todo decltype
    //     POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
    //     {
    //         return compound<valuetype, 3>(std::numeric_limits<valuetype>::max(),
    //                                             std::numeric_limits<valuetype>::max(),
    //                                             std::numeric_limits<valuetype>::max());
    //     }
    //     POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
    //     {
    //         return min();
    //     }
    //     POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
    //     {
    //         return max();
    //     }
    // };
    template <> struct TypeTraits<compound<pUINT8, 3>>
    {
        using storage_type = compound<pUINT8, 3>;
        using safe_signed_type = compound<typename TypeTraits<pUINT8>::safe_signed_type, 3>;
        using str_type = compound<typename TypeTraits<pUINT8>::str_type, 3>;
        using accu_type = compound<typename TypeTraits<pUINT8>::accu_type, 3>;

        static const PType pixel_type = TypeTraits<pUINT8>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_3Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pUINT8, 3>(std::numeric_limits<pUINT8>::lowest(), std::numeric_limits<pUINT8>::lowest(),
                                       std::numeric_limits<pUINT8>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pUINT8, 3>(std::numeric_limits<pUINT8>::min(), std::numeric_limits<pUINT8>::min(),
                                       std::numeric_limits<pUINT8>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pUINT8, 3>(std::numeric_limits<pUINT8>::max(), std::numeric_limits<pUINT8>::max(),
                                       std::numeric_limits<pUINT8>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };

    template <> struct TypeTraits<const compound<pUINT8, 3>> : public TypeTraits<compound<pUINT8, 3>>
    {
    };

    template <> struct TypeTraits<compound<pINT32, 3>>
    {
        using storage_type = compound<pINT32, 3>;
        using safe_signed_type = compound<typename TypeTraits<pINT32>::safe_signed_type, 3>;
        using str_type = compound<typename TypeTraits<pINT32>::str_type, 3>;
        using accu_type = compound<typename TypeTraits<pINT32>::accu_type, 3>;

        static const PType pixel_type = TypeTraits<pINT32>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_3Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pINT32, 3>(std::numeric_limits<pINT32>::lowest(), std::numeric_limits<pINT32>::lowest(),
                                       std::numeric_limits<pINT32>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pINT32, 3>(std::numeric_limits<pINT32>::min(), std::numeric_limits<pINT32>::min(),
                                       std::numeric_limits<pINT32>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pINT32, 3>(std::numeric_limits<pINT32>::max(), std::numeric_limits<pINT32>::max(),
                                       std::numeric_limits<pINT32>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };
    template <> struct TypeTraits<const compound<pINT32, 3>> : public TypeTraits<compound<pINT32, 3>>
    {
    };
    template <> struct TypeTraits<compound<pINT64, 3>>
    {
        using storage_type = compound<pINT64, 3>;
        using safe_signed_type = compound<typename TypeTraits<pINT64>::safe_signed_type, 3>;
        using str_type = compound<typename TypeTraits<pINT64>::str_type, 3>;
        using accu_type = compound<typename TypeTraits<pINT64>::accu_type, 3>;

        static const PType pixel_type = TypeTraits<pINT64>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_3Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pINT64, 3>(std::numeric_limits<pINT64>::lowest(), std::numeric_limits<pINT64>::lowest(),
                                       std::numeric_limits<pINT64>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pINT64, 3>(std::numeric_limits<pINT64>::min(), std::numeric_limits<pINT64>::min(),
                                       std::numeric_limits<pINT64>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pINT64, 3>(std::numeric_limits<pINT64>::max(), std::numeric_limits<pINT64>::max(),
                                       std::numeric_limits<pINT64>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };
    template <> struct TypeTraits<const compound<pINT64, 3>> : public TypeTraits<compound<pINT64, 3>>
    {
    };
    template <> struct TypeTraits<compound<pFLOAT, 3>>
    {
        using storage_type = compound<pFLOAT, 3>;
        using safe_signed_type = compound<typename TypeTraits<pFLOAT>::safe_signed_type, 3>;
        using str_type = compound<typename TypeTraits<pFLOAT>::str_type, 3>;
        using accu_type = compound<typename TypeTraits<pFLOAT>::accu_type, 3>;

        static const PType pixel_type = TypeTraits<pFLOAT>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_3Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pFLOAT, 3>(std::numeric_limits<pFLOAT>::lowest(), std::numeric_limits<pFLOAT>::lowest(),
                                       std::numeric_limits<pFLOAT>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pFLOAT, 3>(std::numeric_limits<pFLOAT>::min(), std::numeric_limits<pFLOAT>::min(),
                                       std::numeric_limits<pFLOAT>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pFLOAT, 3>(std::numeric_limits<pFLOAT>::max(), std::numeric_limits<pFLOAT>::max(),
                                       std::numeric_limits<pFLOAT>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };
    template <> struct TypeTraits<const compound<pFLOAT, 3>> : public TypeTraits<compound<pFLOAT, 3>>
    {
    };
    template <> struct TypeTraits<compound<pDOUBLE, 3>>
    {
        using storage_type = compound<pDOUBLE, 3>;
        using safe_signed_type = compound<typename TypeTraits<pDOUBLE>::safe_signed_type, 3>;
        using str_type = compound<typename TypeTraits<pDOUBLE>::str_type, 3>;
        using accu_type = compound<typename TypeTraits<pDOUBLE>::accu_type, 3>;

        static const PType pixel_type = TypeTraits<pDOUBLE>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_3Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pDOUBLE, 3>(std::numeric_limits<pDOUBLE>::lowest(), std::numeric_limits<pDOUBLE>::lowest(),
                                        std::numeric_limits<pDOUBLE>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pDOUBLE, 3>(std::numeric_limits<pDOUBLE>::min(), std::numeric_limits<pDOUBLE>::min(),
                                        std::numeric_limits<pDOUBLE>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pDOUBLE, 3>(std::numeric_limits<pDOUBLE>::max(), std::numeric_limits<pDOUBLE>::max(),
                                        std::numeric_limits<pDOUBLE>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };
    template <> struct TypeTraits<const compound<pDOUBLE, 3>> : public TypeTraits<compound<pDOUBLE, 3>>
    {
    };
    ////! TypeTraits compound 4 channel
    // template <class valuetype> struct TypeTraits<compound<valuetype, 4>>
    //{
    //    using storage_type = compound<valuetype, 4>;
    //    using safe_signed_type = compound<typename TypeTraits<valuetype>::safe_signed_type, 4>;
    //    using str_type = compound<typename TypeTraits<valuetype>::str_type, 4>;
    //    using accu_type = compound<typename TypeTraits<valuetype>::accu_type, 4>;

    //    static const PType pixel_type = TypeTraits<valuetype>::pixel_type;
    //    static const CompoundType compound_type = CompoundType::CompoundType_4Planes;

    //    POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
    //    POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;
    //    // todo decltype
    //    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
    //    {
    //        return compound<valuetype, 4>(
    //            std::numeric_limits<valuetype>::lowest(), std::numeric_limits<valuetype>::lowest(),
    //            std::numeric_limits<valuetype>::lowest(), std::numeric_limits<valuetype>::lowest());
    //    }

    //    // todo decltype
    //    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
    //    {
    //        return compound<valuetype, 4>(
    //            std::numeric_limits<valuetype>::min(), std::numeric_limits<valuetype>::min(),
    //            std::numeric_limits<valuetype>::min(), std::numeric_limits<valuetype>::min());
    //    }

    //    // todo decltype
    //    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
    //    {
    //        return compound<valuetype, 4>(
    //            std::numeric_limits<valuetype>::max(), std::numeric_limits<valuetype>::max(),
    //            std::numeric_limits<valuetype>::max(), std::numeric_limits<valuetype>::max());
    //    }
    //    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
    //    {
    //        return min();
    //    }
    //    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
    //    {
    //        return max();
    //    }
    //};

    template <> struct TypeTraits<compound<pUINT8, 4>>
    {
        using storage_type = compound<pUINT8, 4>;
        using safe_signed_type = compound<typename TypeTraits<pUINT8>::safe_signed_type, 4>;
        using str_type = compound<typename TypeTraits<pUINT8>::str_type, 4>;
        using accu_type = compound<typename TypeTraits<pUINT8>::accu_type, 4>;

        static const PType pixel_type = TypeTraits<pUINT8>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_4Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pUINT8, 4>(std::numeric_limits<pUINT8>::lowest(), std::numeric_limits<pUINT8>::lowest(),
                                       std::numeric_limits<pUINT8>::lowest(), std::numeric_limits<pUINT8>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pUINT8, 4>(std::numeric_limits<pUINT8>::min(), std::numeric_limits<pUINT8>::min(),
                                       std::numeric_limits<pUINT8>::min(), std::numeric_limits<pUINT8>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pUINT8, 4>(std::numeric_limits<pUINT8>::max(), std::numeric_limits<pUINT8>::max(),
                                       std::numeric_limits<pUINT8>::max(), std::numeric_limits<pUINT8>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };

    template <> struct TypeTraits<const compound<pUINT8, 4>> : public TypeTraits<compound<pUINT8, 4>>
    {
    };

    template <> struct TypeTraits<compound<pINT32, 4>>
    {
        using storage_type = compound<pINT32, 4>;
        using safe_signed_type = compound<typename TypeTraits<pINT32>::safe_signed_type, 4>;
        using str_type = compound<typename TypeTraits<pINT32>::str_type, 4>;
        using accu_type = compound<typename TypeTraits<pINT32>::accu_type, 4>;

        static const PType pixel_type = TypeTraits<pINT32>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_4Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pINT32, 4>(std::numeric_limits<pINT32>::lowest(), std::numeric_limits<pINT32>::lowest(),
                                       std::numeric_limits<pINT32>::lowest(), std::numeric_limits<pINT32>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pINT32, 4>(std::numeric_limits<pINT32>::min(), std::numeric_limits<pINT32>::min(),
                                       std::numeric_limits<pINT32>::min(), std::numeric_limits<pINT32>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pINT32, 4>(std::numeric_limits<pINT32>::max(), std::numeric_limits<pINT32>::max(),
                                       std::numeric_limits<pINT32>::max(), std::numeric_limits<pINT32>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };
    template <> struct TypeTraits<const compound<pINT32, 4>> : public TypeTraits<compound<pINT32, 4>>
    {
    };
    template <> struct TypeTraits<compound<pINT64, 4>>
    {
        using storage_type = compound<pINT64, 4>;
        using safe_signed_type = compound<typename TypeTraits<pINT64>::safe_signed_type, 4>;
        using str_type = compound<typename TypeTraits<pINT64>::str_type, 4>;
        using accu_type = compound<typename TypeTraits<pINT64>::accu_type, 4>;

        static const PType pixel_type = TypeTraits<pINT64>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_4Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pINT64, 4>(std::numeric_limits<pINT64>::lowest(), std::numeric_limits<pINT64>::lowest(),
                                       std::numeric_limits<pINT64>::lowest(), std::numeric_limits<pINT64>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pINT64, 4>(std::numeric_limits<pINT64>::min(), std::numeric_limits<pINT64>::min(),
                                       std::numeric_limits<pINT64>::min(), std::numeric_limits<pINT64>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pINT64, 4>(std::numeric_limits<pINT64>::max(), std::numeric_limits<pINT64>::max(),
                                       std::numeric_limits<pINT64>::max(), std::numeric_limits<pINT64>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };
    template <> struct TypeTraits<const compound<pINT64, 4>> : public TypeTraits<compound<pINT64, 4>>
    {
    };
    template <> struct TypeTraits<compound<pFLOAT, 4>>
    {
        using storage_type = compound<pFLOAT, 4>;
        using safe_signed_type = compound<typename TypeTraits<pFLOAT>::safe_signed_type, 4>;
        using str_type = compound<typename TypeTraits<pFLOAT>::str_type, 4>;
        using accu_type = compound<typename TypeTraits<pFLOAT>::accu_type, 4>;

        static const PType pixel_type = TypeTraits<pFLOAT>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_4Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pFLOAT, 4>(std::numeric_limits<pFLOAT>::lowest(), std::numeric_limits<pFLOAT>::lowest(),
                                       std::numeric_limits<pFLOAT>::lowest(), std::numeric_limits<pFLOAT>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pFLOAT, 4>(std::numeric_limits<pFLOAT>::min(), std::numeric_limits<pFLOAT>::min(),
                                       std::numeric_limits<pFLOAT>::min(), std::numeric_limits<pFLOAT>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pFLOAT, 4>(std::numeric_limits<pFLOAT>::max(), std::numeric_limits<pFLOAT>::max(),
                                       std::numeric_limits<pFLOAT>::max(), std::numeric_limits<pFLOAT>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };
    template <> struct TypeTraits<const compound<pFLOAT, 4>> : public TypeTraits<compound<pFLOAT, 4>>
    {
    };
    template <> struct TypeTraits<compound<pDOUBLE, 4>>
    {
        using storage_type = compound<pDOUBLE, 4>;
        using safe_signed_type = compound<typename TypeTraits<pDOUBLE>::safe_signed_type, 4>;
        using str_type = compound<typename TypeTraits<pDOUBLE>::str_type, 4>;
        using accu_type = compound<typename TypeTraits<pDOUBLE>::accu_type, 4>;

        static const PType pixel_type = TypeTraits<pDOUBLE>::pixel_type;
        static const CompoundType compound_type = CompoundType::CompoundType_4Planes;

        POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
        POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
        {
            return compound<pDOUBLE, 4>(std::numeric_limits<pDOUBLE>::lowest(), std::numeric_limits<pDOUBLE>::lowest(),
                                        std::numeric_limits<pDOUBLE>::lowest(), std::numeric_limits<pDOUBLE>::lowest());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
        {
            return compound<pDOUBLE, 4>(std::numeric_limits<pDOUBLE>::min(), std::numeric_limits<pDOUBLE>::min(),
                                        std::numeric_limits<pDOUBLE>::min(), std::numeric_limits<pDOUBLE>::min());
        }

        // todo decltype
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
        {
            return compound<pDOUBLE, 4>(std::numeric_limits<pDOUBLE>::max(), std::numeric_limits<pDOUBLE>::max(),
                                        std::numeric_limits<pDOUBLE>::max(), std::numeric_limits<pDOUBLE>::max());
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
        {
            return min();
        }
        POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
        {
            return max();
        }
    };
    template <> struct TypeTraits<const compound<pDOUBLE, 4>> : public TypeTraits<compound<pDOUBLE, 4>>
    {
    };

    template <class valuetype, std::ptrdiff_t Rank = 2> class pixel_t
    {
      private:
        using point_t = index<Rank>;
        using value_type = valuetype;
        using self_type = pixel_t<valuetype, Rank>;
        point_t m_point;
        value_type m_value;
        using const_reference = const value_type &;
        using reference = value_type &;

      public:
        self_type &operator=(const self_type &other) = delete;
        self_type &operator=(self_type &&other) = delete;
        pixel_t() = delete;
        pixel_t(const self_type &) = default;
        pixel_t(self_type &&) = default;

        // read only
        POUTRE_CXX14_CONSTEXPR const point_t &point() const POUTRE_NOEXCEPT
        {
            return m_point;
        }
        // POUTRE_CXX14_CONSTEXPR const point_t &point() const POUTRE_NOEXCEPT
        //{
        //    return m_point;
        //}
        // read only
        POUTRE_CXX14_CONSTEXPR const value_type &value() const POUTRE_NOEXCEPT
        {
            return m_value;
        }
        // write allowed
        POUTRE_CXX14_CONSTEXPR value_type &value() POUTRE_NOEXCEPT
        {
            return m_value;
        }
    };

    // pixel 1D
    using px1du8 = pixel_t<pUINT8, 1>;  //! alias scalar pixel 1D UINT8
    using px1di32 = pixel_t<pINT32, 1>; //! alias scalar pixel 1D INT32
    using px1di64 = pixel_t<pINT64, 1>; //! alias scalar pixel 1D INT64
    using px1dF = pixel_t<pFLOAT, 1>;   //! alias scalar pixel 1D FLOAT
    using px1dD = pixel_t<pDOUBLE, 1>;  //! alias scalar pixel 1D FLOAT

    using px1dc3u8 = pixel_t<c3pUINT8, 1>;  //! alias compund pixel 1D 3*UINT8
    using px1dc3i32 = pixel_t<c3pINT32, 1>; //! alias compund pixel 1D 3*INT32
    using px1dc3i64 = pixel_t<c3pINT64, 1>; //! alias compund pixel 1D 3*INT64
    using px1dc3F = pixel_t<c3pFLOAT, 1>;   //! alias compund pixel 1D 3*FLOAT
    using px1dc3D = pixel_t<c3pDOUBLE, 1>;  //! alias compund pixel 1D 3*FLOAT

    using px1dc4u8 = pixel_t<c4pUINT8, 1>;  //! alias compund pixel 1D 3*UINT8
    using px1dc4i32 = pixel_t<c4pINT32, 1>; //! alias compund pixel 1D 3*INT32
    using px1dc4i64 = pixel_t<c4pINT64, 1>; //! alias compund pixel 1D 3*INT64
    using px1dc4F = pixel_t<c4pFLOAT, 1>;   //! alias compund pixel 1D 3*FLOAT
    using px1dc4D = pixel_t<c4pDOUBLE, 1>;  //! alias compund pixel 1D 3*FLOAT

    // pixel 2D
    using px2du8 = pixel_t<pUINT8, 2>;  //! alias scalar pixel 2D UINT8
    using px2di32 = pixel_t<pINT32, 2>; //! alias scalar pixel 2D INT32
    using px2di64 = pixel_t<pINT64, 2>; //! alias scalar pixel 2D INT64
    using px2dF = pixel_t<pFLOAT, 2>;   //! alias scalar pixel 2D FLOAT
    using px2dD = pixel_t<pDOUBLE, 2>;  //! alias scalar pixel 2D FLOAT

    using px2dc3u8 = pixel_t<c3pUINT8, 2>;  //! alias compund pixel 2D 3*UINT8
    using px2dc3i32 = pixel_t<c3pINT32, 2>; //! alias compund pixel 2D 3*INT32
    using px2dc3i64 = pixel_t<c3pINT64, 2>; //! alias compund pixel 2D 3*INT64
    using px2dc3F = pixel_t<c3pFLOAT, 2>;   //! alias compund pixel 2D 3*FLOAT
    using px2dc3D = pixel_t<c3pDOUBLE, 2>;  //! alias compund pixel 2D 3*FLOAT

    using px2dc4u8 = pixel_t<c4pUINT8, 2>;  //! alias compund pixel 2D 4*UINT8
    using px2dc4i32 = pixel_t<c4pINT32, 2>; //! alias compund pixel 2D 4*INT32
    using px2dc4i64 = pixel_t<c4pINT64, 2>; //! alias compund pixel 2D 4*INT64
    using px2dc4F = pixel_t<c4pFLOAT, 2>;   //! alias compund pixel 2D 4*FLOAT
    using px2dc4D = pixel_t<c4pDOUBLE, 2>;  //! alias compund pixel 2D 4*FLOAT

    // pixel 3D
    using px3du8 = pixel_t<pUINT8, 3>;  //! alias scalar pixel 3D UINT8
    using px3di32 = pixel_t<pINT32, 3>; //! alias scalar pixel 3D INT32
    using px3di64 = pixel_t<pINT64, 3>; //! alias scalar pixel 3D INT64
    using px3dF = pixel_t<pFLOAT, 3>;   //! alias scalar pixel 3D FLOAT
    using px3dD = pixel_t<pDOUBLE, 3>;  //! alias scalar pixel 3D FLOAT

    using px3dc3u8 = pixel_t<c3pUINT8, 3>;  //! alias compund pixel 3D 3*UINT8
    using px3dc3i32 = pixel_t<c3pINT32, 3>; //! alias compund pixel 3D 3*INT32
    using px3dc3i64 = pixel_t<c3pINT64, 3>; //! alias compund pixel 3D 3*INT64
    using px3dc3F = pixel_t<c3pFLOAT, 3>;   //! alias compund pixel 3D 3*FLOAT
    using px3dc3D = pixel_t<c3pDOUBLE, 3>;  //! alias compund pixel 3D 3*FLOAT

    using px3dc4u8 = pixel_t<c4pUINT8, 3>;  //! alias compund pixel 3D 4*UINT8
    using px3dc4i32 = pixel_t<c4pINT32, 3>; //! alias compund pixel 3D 4*INT32
    using px3dc4i64 = pixel_t<c4pINT64, 3>; //! alias compund pixel 3D 4*INT64
    using px3dc4F = pixel_t<c4pFLOAT, 3>;   //! alias compund pixel 3D 4*FLOAT
    using px3dc4D = pixel_t<c4pDOUBLE, 3>;  //! alias compund pixel 3D 4*FLOAT

    // pixel 4D
    using px4du8 = pixel_t<pUINT8, 4>;  //! alias scalar pixel 4D UINT8
    using px4di32 = pixel_t<pINT32, 4>; //! alias scalar pixel 4D INT32
    using px4di64 = pixel_t<pINT64, 4>; //! alias scalar pixel 4D INT64
    using px4dF = pixel_t<pFLOAT, 4>;   //! alias scalar pixel 4D FLOAT
    using px4dD = pixel_t<pDOUBLE, 4>;  //! alias scalar pixel 4D FLOAT

    using px4dc3u8 = pixel_t<c3pUINT8, 4>;  //! alias compund pixel 4D 3*UINT8
    using px4dc3i32 = pixel_t<c3pINT32, 4>; //! alias compund pixel 4D 3*INT32
    using px4dc3i64 = pixel_t<c3pINT64, 4>; //! alias compund pixel 4D 3*INT64
    using px4dc3F = pixel_t<c3pFLOAT, 4>;   //! alias compund pixel 4D 3*FLOAT
    using px4dc3D = pixel_t<c3pDOUBLE, 4>;  //! alias compund pixel 4D 3*FLOAT

    using px4dc4u8 = pixel_t<c4pUINT8, 4>;  //! alias compund pixel 4D 4*UINT8
    using px4dc4i32 = pixel_t<c4pINT32, 4>; //! alias compund pixel 4D 4*INT32
    using px4dc4i64 = pixel_t<c4pINT64, 4>; //! alias compund pixel 4D 4*INT64
    using px4dc4F = pixel_t<c4pFLOAT, 4>;   //! alias compund pixel 4D 4*FLOAT
    using px4dc4D = pixel_t<c4pDOUBLE, 4>;  //! alias compund pixel 4D 4*FLOAT
    //! @} doxygroup:
} // namespace poutre
#endif // POUTRE_IMAGEPROCESSING_TYPE_HPP__
