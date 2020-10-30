
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

#include <poutreIO/poutreIO.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>

#include <memory>
#include <string>

namespace poutre
{
  /**
   * @addtogroup image_processing_io_group Image Processing IO API
   * @ingroup image_processing_group
   *@{
   */
  /**
   * @addtogroup image_processing_io_group_details Image Processing IO API details
   * @ingroup image_processing_io_group
   *@{
   */
  namespace details
  {
    // Scalar
    template<typename valuetype, std::ptrdiff_t NumDims = 2> // default dim == 2
    void t_ImageToTypedBufferScalar(const DenseImage<valuetype, NumDims> &im,
                                    const size_t                          size_buffer, // Size in bytes!!!
                                    valuetype *                           buffer)
    {
      const auto bytesperpixel = sizeof(valuetype); // return size in bytes,byte == octet == 8*bits
      const auto bound         = im.bound();

      if( bound.size() * bytesperpixel != size_buffer )
      {
        POUTRE_RUNTIME_ERROR("t_ImageToTypedBufferScalar:Bad size for the output buffer");
      }

      pUINT8 *ptr_buffer = reinterpret_cast<pUINT8 *>(buffer);
      memcpy(ptr_buffer, im.data(), size_buffer);
    }

    // compound 3 Interleaved or planes by planes ?
    template<typename valuetype, std::ptrdiff_t NumDims = 2> // default dim == 2
    void t_ImageToBufferCompound3(const DenseImage<compound<valuetype, 3>, NumDims> &im,
                                  const size_t                                       size_buffer, // Size in bytes!!!
                                  valuetype *                                        buffer)
    {
      const auto bytesperpixel = sizeof(valuetype) * 3;
      const auto bound         = im.bound();
      if( bound.size() * bytesperpixel != size_buffer )
      {
        POUTRE_RUNTIME_ERROR("t_ImageToBufferCompound3:Bad size for the output buffer");
      }
      // TODO
    }

    // compound 4
    template<typename valuetype, std::ptrdiff_t NumDims = 2> // default dim == 2
    void t_ImageToBufferCompound4(const DenseImage<compound<valuetype, 4>, NumDims> &im,
                                  const size_t                                       size_buffer, // Size in bytes!!!
                                  valuetype *                                        buffer)
    {
      const auto bytesperpixel = sizeof(valuetype) * 4;
      const auto bound         = im.bound();
      if( bound.size() * bytesperpixel != size_buffer )
      {
        POUTRE_RUNTIME_ERROR("t_ImageToBufferCompound4:Bad size for the output buffer");
      }
      // TODO
    }

    template<typename valuetype>
    void t_ImageToBufferForceType(const IInterface &i_im,
                                  const size_t      size_buffer, // Size in bytes!!!
                                  valuetype *       buffer)
    {
      POUTRE_ENTERING("t_ImageToBufferForceType");
      auto imgType = i_im.GetImgType();
      if( imgType != ImgType::ImgType_Dense )
      {
        POUTRE_RUNTIME_ERROR("t_ImageToBufferForceType : only dense image type is supported");
      }
      auto compoundType = i_im.GetCType();
      if( compoundType != CompoundType::CompoundType_Scalar ) // todo!
      {
        POUTRE_RUNTIME_ERROR("t_ImageToBufferForceType : only scalar type is supported");
      }
      typedef typename std::remove_cv_t<typename std::remove_pointer_t<valuetype>> inputType;
      if( TypeTraits<inputType>::pixel_type != i_im.GetPType() )
      {
        POUTRE_RUNTIME_ERROR("Image and buffer doesn't share the same scalar type");
      }

      auto numDims = i_im.GetNumDims();
      switch( numDims )
      {
      case 0:
      {
        POUTRE_RUNTIME_ERROR("Unsupported number of dims:0");
      }
      break;
      case 1:
      {
        using ImageType      = poutre::DenseImage<valuetype, 1>;
        const ImageType *tmp = dynamic_cast<const ImageType *>(&i_im);
        if( !tmp )
        {
          POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        t_ImageToTypedBufferScalar<valuetype, 1>(*tmp, size_buffer, buffer);
      }
      break;
      case 2:
      {
        using ImageType      = poutre::DenseImage<valuetype, 2>;
        const ImageType *tmp = dynamic_cast<const ImageType *>(&i_im);
        if( !tmp )
        {
          POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        t_ImageToTypedBufferScalar<valuetype, 2>(*tmp, size_buffer, buffer);
      }
      break;
      case 3:
      {
        using ImageType      = poutre::DenseImage<valuetype, 3>;
        const ImageType *tmp = dynamic_cast<const ImageType *>(&i_im);
        if( !tmp )
        {
          POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        t_ImageToTypedBufferScalar<valuetype, 3>(*tmp, size_buffer, buffer);
      }
      break;
      case 4:
      {
        using ImageType      = poutre::DenseImage<valuetype, 4>;
        const ImageType *tmp = dynamic_cast<const ImageType *>(&i_im);
        if( !tmp )
        {
          POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        t_ImageToTypedBufferScalar<valuetype, 4>(*tmp, size_buffer, buffer);
      }
      break;
      default:
      {
        POUTRE_RUNTIME_ERROR("t_ImageToBufferForceType::Unsupported number of dims");
      }
      }
    }

    template<typename valuetype, std::ptrdiff_t NumDims = 2> // default dim == 2
    void t_ImageFromBufferWithType(DenseImage<valuetype, NumDims> &im, const size_t size_buffer, const valuetype *buffer)
    {
      const auto bytesperpixel = sizeof(valuetype);
      const auto bound         = im.bound();

      if( bound.size() * bytesperpixel != size_buffer )
      {
        POUTRE_RUNTIME_ERROR("t_ImageFromBufferWithType:Bad size for the input buffer");
      }

      //   const pUINT8 *ptr_buffer = reinterpret_cast<const pUINT8 *>(buffer);
      //   memcpy(im.data(), ptr_buffer, size_buffer);
      auto index = 0;
      for( auto &it : im )
      {
        it = buffer[(size_t)index];
        ++index;
      }
    }

    template<typename valuetype>
    void t_ImageFromBufferWithType(IInterface &o_im, const size_t i_size_buffer, const valuetype *i_buffer)
    {
      POUTRE_ENTERING("t_ImageFromBufferWithType");
      auto imgType = o_im.GetImgType();
      if( imgType != ImgType::ImgType_Dense )
      {
        POUTRE_RUNTIME_ERROR("t_ImageFromBufferWithType : only dense image type is supported");
      }
      auto compoundType = o_im.GetCType();
      if( compoundType != CompoundType::CompoundType_Scalar ) // todo!
      {
        POUTRE_RUNTIME_ERROR("t_ImageFromBufferWithType : only scalar type is supported");
      }

      typedef typename std::remove_cv_t<typename std::remove_pointer_t<valuetype>> inputType;
      if( TypeTraits<inputType>::pixel_type != o_im.GetPType() )
      {
        POUTRE_RUNTIME_ERROR("Image and buffer doesn't share the same scalar type");
      }

      auto numDims = o_im.GetNumDims();
      switch( numDims )
      {
      case 1:
      {
        using ImageType = poutre::DenseImage<valuetype, 1>;
        ImageType *tmp  = dynamic_cast<ImageType *>(&o_im);
        if( !tmp )
        {
          POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        t_ImageFromBufferWithType<valuetype, 1>(*tmp, i_size_buffer, i_buffer);
      }
      break;
      case 2:
      {
        using ImageType = poutre::DenseImage<valuetype, 2>;
        ImageType *tmp  = dynamic_cast<ImageType *>(&o_im);
        if( !tmp )
        {
          POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        t_ImageFromBufferWithType<valuetype, 2>(*tmp, i_size_buffer, i_buffer);
      }
      break;
      case 3:
      {
        using ImageType = poutre::DenseImage<valuetype, 3>;
        ImageType *tmp  = dynamic_cast<ImageType *>(&o_im);
        if( !tmp )
        {
          POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        t_ImageFromBufferWithType<valuetype, 3>(*tmp, i_size_buffer, i_buffer);
      }
      break;
      case 4:
      {
        using ImageType = poutre::DenseImage<valuetype, 4>;
        ImageType *tmp  = dynamic_cast<ImageType *>(&o_im);
        if( !tmp )
        {
          POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        t_ImageFromBufferWithType<valuetype, 4>(*tmp, i_size_buffer, i_buffer);
      }
      break;
      default: POUTRE_RUNTIME_ERROR("t_ImageFromBufferWithType:Unsupported number of dims");
      }
    }

  } // namespace details
    //! @} doxygroup: image_processing_io_group_details
    //! @} doxygroup: image_processing_io_group
} // namespace poutre
#endif /* POUTREIOBUFFER_HPP */
