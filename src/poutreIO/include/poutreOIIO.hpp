
//==============================================================================
//                  Copyright (c) 2020 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
 * File:   poutreOIIO.hpp
 * Author: thomas retornaz
 *
 * Created on 02 mai 2020
 */
#ifndef POUTREOIIO_HPP
#define POUTREOIIO_HPP

#include <memory>
#include <string>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#endif

#ifdef __CLANG__
#pragma clang diagnostic push
// silencing warnings from OpenEXR 2.2.0 / OpenImageIO
#pragma clang diagnostic ignored "-Wdeprecated-register"
#pragma clang load("OpenImageIO")
#endif

#include "OpenImageIO/imageio.h"

#ifdef __CLANG__
#pragma clang diagnostic pop
#endif

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#endif

#include <boost/filesystem.hpp>

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
        std::unique_ptr<IInterface> LoadFromOIIO(const std::string &image_path)
        {
            auto in(OIIO::ImageInput::open(image_path));
            if (!in)
            {
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << image_path;
                POUTRE_RUNTIME_ERROR(errorstream.str());
            }

            const OIIO::ImageSpec &spec = in->spec();
            std::vector<std::size_t> dims;
            if (spec.width > 1)
                dims.push_back((size_t)spec.width);
            if (spec.height > 1)
                dims.push_back((size_t)spec.height);
            if (spec.depth > 1)
                dims.push_back((size_t)spec.depth);

            CompoundType ctype = CompoundType::CompoundType_Undef;
            switch (spec.nchannels)
            {
            case 1:
                ctype = CompoundType::CompoundType_Scalar;
                break;
            case 3:
                ctype = CompoundType::CompoundType_3Planes;
                break;
            case 4:
                ctype = CompoundType::CompoundType_4Planes;
                break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << image_path;
                errorstream << " unsupported number of channels ";
                errorstream << spec.nchannels;
                errorstream << " see desc \n" << spec.to_xml();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
            PType ptype = PType::PType_Undef;
            switch (spec.format.basetype)
            {
            case OpenImageIO_v2_2::TypeDesc::BASETYPE::UINT8:
                ptype = PType::PType_GrayUINT8;
                break;
            case OpenImageIO_v2_2::TypeDesc::BASETYPE::INT32:
                ptype = PType::PType_GrayINT32;
                break;
            case OpenImageIO_v2_2::TypeDesc::BASETYPE::FLOAT:
                ptype = PType::PType_F32;
                break;
            case OpenImageIO_v2_2::TypeDesc::BASETYPE::INT64:
                ptype = PType::PType_GrayINT64;
                break;
            case OpenImageIO_v2_2::TypeDesc::BASETYPE::DOUBLE:
                ptype = PType::PType_D64;
                break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << image_path;
                errorstream << " unsupported type ";
                errorstream << spec.format.basetype;
                errorstream << " see desc \n" << spec.to_xml();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
            auto iimage = CreateDense(dims, ctype, ptype);

            // now fill it
            in->close();

            return iimage;
        }
    } // namespace details

    //! @} doxygroup: image_processing_io_group_details
    //! @} doxygroup: image_processing_io_group

} // namespace poutre

#endif /* POUTREOIIO_HPP */