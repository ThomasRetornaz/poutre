
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
        namespace bf = boost::filesystem;

        /**
         * Load an image from file at filename.
         * Storage format is deduced from file ending.
         *
         * @param image_path The path of the file to load
         *
         * @return std::unique_ptr<IInterface> corresponding to the loaded image
         */
        std::unique_ptr<IInterface> LoadFromOIIO(const std::string &image_path)
        {
            bf::path localPath(image_path);
            if (!(bf::exists(localPath)))
            {
                POUTRE_RUNTIME_ERROR(boost::format("LoadFromOIIO: provided path %s doesn't exists") %
                                     localPath.string());
            }

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
        /**
         * @brief Way to pass options to
         *
         */
        struct StoreWithOIIOOptions
        {
            /**
             * @brief Construct a new options with default compression quality to 100
             *
             */
            StoreWithOIIOOptions() : spec(0, 0, 0), autoconvert(true)
            {
                spec.attribute("CompressionQuality", 100);
            }

            /**
             * @brief Forward an attribute to an OpenImageIO ImageSpec.
             *   See the documentation of OIIO::ImageSpec::attribute() for a list
             *   of supported attributes.
             *   Default: "CompressionQuality" = 100
             *
             * @tparam T
             * @param name
             * @param v
             * @return StoreWithOIIOOptions&
             */
            template <class T> StoreWithOIIOOptions &attribute(OIIO::string_view name, T const &v)
            {
                spec.attribute(name, v);
                return *this;
            }

            OIIO::ImageSpec spec;
            bool autoconvert;
        };

        /**
         * Save image to disk.
         * The desired image format is deduced from ``filename``.
         * Supported formats are those supported by OpenImageIO.
         * Most common formats are supported (jpg, png, gif, bmp, tiff).
         *
         * @param path The path to the desired file
         * @param image Image Interface
         * @param options Pass a dump_image_options object to fine-tune image export
         */
        void StoreWithOIIO(const std::string &path, const IInterface &image,
                           StoreWithOIIOOptions const &options = StoreWithOIIOOptions())
        {

            bf::path localPath(path);
            boost::filesystem::path dir = localPath.parent_path();
            if (!(bf::exists(dir)))
            {
                POUTRE_RUNTIME_ERROR(boost::format("StoreFromOIIO: provided path %s doesn't exists") % dir);
            }
            if (image.GetNumDims() > 2)
                POUTRE_RUNTIME_ERROR(boost::format("For nd images with n>2 use StoreWithHDF5") % dir);
        }
    } // namespace details

    //! @} doxygroup: image_processing_io_group_details
    //! @} doxygroup: image_processing_io_group

} // namespace poutre

#endif /* POUTREOIIO_HPP */