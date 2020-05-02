
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
 * File:   poutreIOBUFFER.hpp
 * Author: thomas retornaz
 *
 * Created on 19 mars 2016
 */

#ifndef POUTREIOBUFFER_HPP
#define POUTREIOBUFFER_HPP

#include <memory>
#include <string>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#endif

#include <boost/filesystem.hpp>

namespace poutre
{
    namespace details
    {
        // Scalar
        template <typename valuetype, std::ptrdiff_t NumDims = 2> // default dim == 2
        void t_ImageToTypedBufferScalar(const DenseImage<valuetype, NumDims> &im,
                                        const size_t size_buffer, // Size in bytes!!!
                                        valuetype *buffer)
        {
            const auto bytesperpixel = sizeof(valuetype); // return size in bytes,byte == octet == 8*bits
            const auto bound = im.bound();

            if (bound.size() * bytesperpixel != size_buffer)
            {
                POUTRE_RUNTIME_ERROR("t_ImageToTypedBufferScalar:Bad size for the output buffer");
            }

            // pUINT8 *ptr_buffer = reinterpret_cast<pUINT8 *>(buffer);
            // memcpy(ptr_buffer, im.data(), size_buffer);
            // Handle padding ...
            auto index = 0;
            for (const auto &it : im)
            {
                buffer[index] = it;
                ++index;
            }
        }

        // compound 3 Interleaved or planes by planes ?
        template <typename valuetype, std::ptrdiff_t NumDims = 2> // default dim == 2
        void t_ImageToBufferCompound3(const DenseImage<compound_pixel<valuetype, 3>, NumDims> &im,
                                      const size_t size_buffer, // Size in bytes!!!
                                      valuetype *buffer)
        {
            const auto bytesperpixel = sizeof(valuetype) * 3;
            const auto bound = im.bound();
            if (bound.size() * bytesperpixel != size_buffer)
            {
                POUTRE_RUNTIME_ERROR("t_ImageToBufferCompound3:Bad size for the output buffer");
            }
            // TODO
        }

        // compound 4
        template <typename valuetype, std::ptrdiff_t NumDims = 2> // default dim == 2
        void t_ImageToBufferCompound4(const DenseImage<compound_pixel<valuetype, 4>, NumDims> &im,
                                      const size_t size_buffer, // Size in bytes!!!
                                      valuetype *buffer)
        {
            const auto bytesperpixel = sizeof(valuetype) * 4;
            const auto bound = im.bound();
            if (bound.size() * bytesperpixel != size_buffer)
            {
                POUTRE_RUNTIME_ERROR("t_ImageToBufferCompound4:Bad size for the output buffer");
            }
            // TODO
        }

        template <typename valuetype>
        void t_ImageToBufferForceType(const IInterface &i_im,
                                      const size_t size_buffer, // Size in bytes!!!
                                      valuetype *buffer)
        {
            auto imgType = i_im.GetImgType();
            if (imgType != ImgType::ImgType_Dense)
            {
                POUTRE_RUNTIME_ERROR("t_ImageToBufferForceType : only dense image type is supported");
            }
            auto compoundType = i_im.GetCType();
            if (compoundType != CompoundType::CompoundType_Scalar) // todo!
            {
                POUTRE_RUNTIME_ERROR("t_ImageToBufferForceType : only scalar type is supported");
            }
            typedef typename boost::remove_cv<typename boost::remove_pointer<valuetype>::type>::type inputType;
            if (TypeTraits<inputType>::pixel_type != i_im.GetPType())
            {
                POUTRE_RUNTIME_ERROR("Image and buffer doesn't share the same scalar type");
            }

            auto numDims = i_im.GetNumDims();
            switch (numDims)
            {
            case 0: {
                POUTRE_RUNTIME_ERROR("Unsupported number of dims:0");
            }
            break;
            case 1: {
                using ImageType = poutre::DenseImage<valuetype, 1>;
                const ImageType *tmp = dynamic_cast<const ImageType *>(&i_im);
                if (!tmp)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                t_ImageToTypedBufferScalar<valuetype, 1>(*tmp, size_buffer, buffer);
            }
            break;
            case 2: {
                using ImageType = poutre::DenseImage<valuetype, 2>;
                const ImageType *tmp = dynamic_cast<const ImageType *>(&i_im);
                if (!tmp)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                t_ImageToTypedBufferScalar<valuetype, 2>(*tmp, size_buffer, buffer);
            }
            break;
            case 3: {
                using ImageType = poutre::DenseImage<valuetype, 3>;
                const ImageType *tmp = dynamic_cast<const ImageType *>(&i_im);
                if (!tmp)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                t_ImageToTypedBufferScalar<valuetype, 3>(*tmp, size_buffer, buffer);
            }
            break;
            case 4: {
                using ImageType = poutre::DenseImage<valuetype, 4>;
                const ImageType *tmp = dynamic_cast<const ImageType *>(&i_im);
                if (!tmp)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                t_ImageToTypedBufferScalar<valuetype, 4>(*tmp, size_buffer, buffer);
            }
            break;
            default: {
                POUTRE_RUNTIME_ERROR("t_ImageToBufferForceType::Unsupported number of dims");
            }
            }
        }

        template <typename valuetype, std::ptrdiff_t NumDims = 2> // default dim == 2
        void t_ImageFromBufferWithType(DenseImage<valuetype, NumDims> &im, const size_t size_buffer,
                                       const valuetype *buffer)
        {
            const auto bytesperpixel = sizeof(valuetype);
            const auto bound = im.bound();

            if (bound.size() * bytesperpixel != size_buffer)
            {
                POUTRE_RUNTIME_ERROR("t_ImageFromBufferWithType:Bad size for the input buffer");
            }

            //   const pUINT8 *ptr_buffer = reinterpret_cast<const pUINT8 *>(buffer);
            //   memcpy(im.data(), ptr_buffer, size_buffer);
            auto index = 0;
            for (auto &it : im)
            {
                it = buffer[index];
                ++index;
            }
        }

        template <typename valuetype>
        void t_ImageFromBufferWithType(IInterface &o_im, const size_t i_size_buffer, const valuetype *i_buffer)
        {
            auto imgType = o_im.GetImgType();
            if (imgType != ImgType::ImgType_Dense)
            {
                POUTRE_RUNTIME_ERROR("t_ImageFromBufferWithType : only dense image type is supported");
            }
            auto compoundType = o_im.GetCType();
            if (compoundType != CompoundType::CompoundType_Scalar) // todo!
            {
                POUTRE_RUNTIME_ERROR("t_ImageFromBufferWithType : only scalar type is supported");
            }
            typedef typename boost::remove_cv<typename boost::remove_pointer<valuetype>::type>::type inputType;
            if (TypeTraits<inputType>::pixel_type != o_im.GetPType())
            {
                POUTRE_RUNTIME_ERROR("Image and buffer doesn't share the same scalar type");
            }

            auto numDims = o_im.GetNumDims();
            switch (numDims)
            {
            case 1: {
                using ImageType = poutre::DenseImage<valuetype, 1>;
                ImageType *tmp = dynamic_cast<ImageType *>(&o_im);
                if (!tmp)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                t_ImageFromBufferWithType<valuetype, 1>(*tmp, i_size_buffer, i_buffer);
            }
            break;
            case 2: {
                using ImageType = poutre::DenseImage<valuetype, 2>;
                ImageType *tmp = dynamic_cast<ImageType *>(&o_im);
                if (!tmp)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                t_ImageFromBufferWithType<valuetype, 2>(*tmp, i_size_buffer, i_buffer);
            }
            break;
            case 3: {
                using ImageType = poutre::DenseImage<valuetype, 3>;
                ImageType *tmp = dynamic_cast<ImageType *>(&o_im);
                if (!tmp)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                t_ImageFromBufferWithType<valuetype, 3>(*tmp, i_size_buffer, i_buffer);
            }
            break;
            case 4: {
                using ImageType = poutre::DenseImage<valuetype, 4>;
                ImageType *tmp = dynamic_cast<ImageType *>(&o_im);
                if (!tmp)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                t_ImageFromBufferWithType<valuetype, 4>(*tmp, i_size_buffer, i_buffer);
            }
            break;
            default:
                POUTRE_RUNTIME_ERROR("t_ImageFromBufferWithType:Unsupported number of dims");
            }
        }

    } // namespace details
} // namespace poutre
#endif /* POUTREIOBUFFER_HPP */
