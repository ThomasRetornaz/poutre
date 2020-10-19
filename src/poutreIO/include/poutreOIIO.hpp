
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

#include <poutreIO/poutreIO.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>

#include <boost/filesystem.hpp>
#include <memory>
#include <string>

namespace poutre
{
    namespace bf = boost::filesystem;
    /**
     * @addtogroup image_processing_io_group_details Image Processing IO API details
     * @ingroup image_processing_io_group
     *@{
     */
    namespace details
    {
        /**
         * @brief  Helper function which copy contant of input OIIO object in DenseImage 2D assuming scalar type
         *
         * @tparam T DenseImage pType
         * @param input preconstructed OIIO object
         * @param im Image where to copy data
         * @warning Unsafe method use @c ImageLoader for safety check
         */
        template <typename T> void FillImageFromOIIOScalar(OIIO::ImageInput &input, DenseImage<T, 2> &im)
        {
            input.read_image(OIIO::BaseTypeFromC<T>::value, im.data());
        }

        /**
         * @brief Helper function which copy contant of input file in DenseImage 2D assuming scalar type
         *
         * @tparam T  T DenseImage pType
         * @param path path of the image to load
         * Image where to copy data
         * @warning Unsafe method use @c ImageLoader for safety check
         */
        template <typename T> void FillImageFromOIIOScalar(const std::string &path, DenseImage<T, 2> &im)
        {
            bf::path localPath(path);
            if (!(bf::exists(localPath)))
            {
                POUTRE_RUNTIME_ERROR(
                    (poutre::format("FillImageFromOIIOScalar: provided path {%s} doesn't exists", localPath.string())));
            }
            auto in(OIIO::ImageInput::open(path));
            if (!in)
            {
                std::ostringstream errorstream;
                errorstream << " FillImageFromOIIOScalar load_image(): Error reading image '";
                errorstream << path;
                errorstream << "\n" << OIIO::geterror();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            }
            const OIIO::ImageSpec &spec = in->spec();
            if (spec.nchannels != 1)
            {
                POUTRE_RUNTIME_ERROR((poutre::format(
                    "FillImageFromOIIOScalar: wrong number of channels expected 1 found {%d}", spec.nchannels)));
            }
            if (spec.depth > 1)
            {
                POUTRE_RUNTIME_ERROR(
                    poutre::format("FillImageFromOIIOScalar: wrong number of dim expected 2 found depth dim"));
            }
            FillImageFromOIIOScalar(*in, im);

            in->close();
        }
        /**
         * @brief  Helper function which copy contant of input OIIO object in DenseImage 2D assuming RGB like color
         * space
         *
         * @tparam T DenseImage pType
         * @param input preconstructed OIIO object
         * @param im Image where to copy data
         * @warning Unsafe method use @c ImageLoader for safety check
         */
        template <typename T>
        void FillImageFromOIIOCompound3(OIIO::ImageInput &input, DenseImage<compound<T, 3>, 2> &im)
        {
            const OIIO::ImageSpec &spec = input.spec();
            if (spec.nchannels != 3)
            {
                POUTRE_RUNTIME_ERROR((poutre::format(
                    "FillImageFromOIIOCompound3: wrong number of channels expected 3 found {%d}", spec.nchannels)));
            }

            std::vector<T> tmp;
            tmp.resize((size_t)spec.width * (size_t)spec.height * (size_t)spec.nchannels);
            input.read_image(OIIO::BaseTypeFromC<T>::value, &tmp[0]);
            const auto ptr_tmp = tmp.data();
            auto ptr_img = im.data();
            for (size_t i = 0; i < tmp.size(); i += 3)
            {
                (*ptr_img)[0] = ptr_tmp[i];
                (*ptr_img)[1] = ptr_tmp[i + 1];
                (*ptr_img)[2] = ptr_tmp[i + 2];
                ptr_img++;
            }
        }
        /**
         * @brief Helper function which copy contant of input file in DenseImage 2D assuming like RGB color space
         *
         * @tparam T  T DenseImage pType
         * @param path path of the image to load
         * Image where to copy data
         * @warning Unsafe method use @c ImageLoader for safety check
         */
        template <typename T>
        void FillImageFromOIIOCompound3(const std::string &path, DenseImage<compound<T, 3>, 2> &im)
        {
            bf::path localPath(path);
            if (!(bf::exists(localPath)))
            {
                POUTRE_RUNTIME_ERROR((poutre::format("FillImageFromOIIOCompound3: provided path {%s} doesn't exists",
                                                     localPath.string())));
            }
            auto in(OIIO::ImageInput::open(path));
            if (!in)
            {
                std::ostringstream errorstream;
                errorstream << " FillImageFromOIIOCompound3 load_image(): Error reading image '";
                errorstream << path;
                errorstream << "\n" << OIIO::geterror();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            }
            const OIIO::ImageSpec &spec = in->spec();
            if (spec.nchannels != 3)
            {
                POUTRE_RUNTIME_ERROR((poutre::format(
                    "FillImageFromOIIOCompound3: wrong number of channels expected 3 found {%d}", spec.nchannels)));
            }
            if (spec.depth > 1)
            {
                POUTRE_RUNTIME_ERROR(
                    poutre::format("FillImageFromOIIOCompound3: wrong number of dim expected 2 found depth dim"));
            }
            FillImageFromOIIOCompound3(*in, im);

            in->close();
        }
        /**
         * @brief  Helper function which copy contant of input OIIO object in DenseImage 2D assuming RGBA like color
         * space
         *
         * @tparam T DenseImage pType
         * @param input preconstructed OIIO object
         * @param im Image where to copy data
         * @warning Unsafe method use @c ImageLoader for safety check
         */
        template <typename T>
        void FillImageFromOIIOCompound4(OIIO::ImageInput &input, DenseImage<compound<T, 4>, 2> &im)
        {
            const OIIO::ImageSpec &spec = input.spec();
            if (spec.nchannels != 4) //-V112
            {
                POUTRE_RUNTIME_ERROR((poutre::format(
                    "FillImageFromOIIOCompound4: wrong number of channels expected 4 found %d", spec.nchannels)));
            }

            std::vector<T> tmp;
            tmp.resize((size_t)spec.width * (size_t)spec.height * (size_t)spec.nchannels);
            input.read_image(OIIO::BaseTypeFromC<T>::value, &tmp[0]);
            const auto ptr_tmp = tmp.data();
            auto ptr_img = im.data();
            for (size_t i = 0; i < tmp.size(); i += 4) //-V112
            {
                (*ptr_img)[0] = ptr_tmp[i];
                (*ptr_img)[1] = ptr_tmp[i + 1];
                (*ptr_img)[2] = ptr_tmp[i + 2];
                (*ptr_img)[3] = ptr_tmp[i + 3];
                ptr_img++;
            }
        }

        /**
         * @brief Helper function which copy contant of input file in DenseImage 2D assuming like RGBA color space
         *
         * @tparam T  T DenseImage pType
         * @param path path of the image to load
         * Image where to copy data
         * @warning Unsafe method use @c ImageLoader for safety check
         */
        template <typename T>
        void FillImageFromOIIOCompound4(const std::string &path, DenseImage<compound<T, 4>, 2> &im)
        {
            bf::path localPath(path);
            if (!(bf::exists(localPath)))
            {
                POUTRE_RUNTIME_ERROR((poutre::format("FillImageFromOIIOCompound4: provided path {%s} doesn't exists",
                                                     localPath.string())));
            }
            auto in(OIIO::ImageInput::open(path));
            if (!in)
            {
                std::ostringstream errorstream;
                errorstream << " FillImageFromOIIOCompound4 load_image(): Error reading image '";
                errorstream << path;
                errorstream << "\n" << OIIO::geterror();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            }
            const OIIO::ImageSpec &spec = in->spec();
            if (spec.nchannels != 4) //-V112
            {
                POUTRE_RUNTIME_ERROR((poutre::format(
                    "FillImageFromOIIOCompound4: wrong number of channels expected 4 found {%d}", spec.nchannels)));
            }
            if (spec.depth > 1)
            {
                POUTRE_RUNTIME_ERROR(
                    poutre::format("FillImageFromOIIOCompound4: wrong number of dim expected 2 found depth dim"));
            }
            FillImageFromOIIOCompound4(*in, im);

            in->close();
        }

        /**
         * @brief Way to pass options to @c StoreWithOIIO
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
         * @brief  Helper function which copy contant of DenseImage into OIIO object assuming scalar type
         *
         * @tparam T DenseImage pType
         * @param im Image to copy into OIIO object
         * @param output preconstructed OIIO object
         * @warning Unsafe method use @c ImageWriter for safety check
         */
        template <typename T> void StoreWithOIIOScalar(const DenseImage<T, 2> &im, OIIO::ImageOutput &out)
        {
            const OIIO::ImageSpec &spec = out.spec();
            POUTRE_CHECK(spec.nchannels == 1, "Wrong number of channels");

            // stolen from https://github.com/xtensor-stack/xtensor-io/blob/master/include/xtensor-io/ximage.hpp
            if (out.spec().format != OIIO::BaseTypeFromC<T>::value)
            {
                // OpenImageIO changed the target type because the file format doesn't support value_type.
                // It will do automatic conversion, but the data should be in the range 0...1
                // for good results.

                //! TODO! make measurement module
                //! TODO! vectorize
                const auto iters = std::minmax_element(im.cbegin(), im.cend());
                const pDOUBLE min = (iters.first != im.cend() ? *iters.first : TypeTraits<pDOUBLE>::sup());
                const pDOUBLE max = (iters.second != im.cend() ? *iters.second : TypeTraits<pDOUBLE>::inf());

                if (max != min) //-V550
                {               //-V550
                    std::vector<pDOUBLE> normalized(im.size());
                    for (size_t i = 0; i < im.size(); ++i)
                        normalized[i] = (pDOUBLE(1.0) / (max - min)) * (im[i] - min);
                    out.write_image(OIIO::BaseTypeFromC<pDOUBLE>::value, &normalized[0]);
                }
                else
                {
                    out.write_image(OIIO::BaseTypeFromC<T>::value, im.data());
                }
            }
            else
            {
                out.write_image(OIIO::BaseTypeFromC<T>::value, im.data());
            }
            out.close();
        }

        /**
         * @brief  Helper function which copy contant of DenseImage on disk
         *
         * @tparam T DenseImage pType
         * @param im Image to serialize
         * @param path path where image is serialized
         * @warning Unsafe method use @c ImageWriter for safety check
         */
        template <typename T>
        void StoreWithOIIOScalar(const DenseImage<T, 2> &im, const std::string &path,
                                 StoreWithOIIOOptions const &options)
        {
            bf::path localPath(path);
            bf::path dir = localPath.parent_path();
            if (!(bf::exists(dir)))
            {
                POUTRE_RUNTIME_ERROR((poutre::format("StoreWithOIIOScalar: provided path {%s} doesn't exists", dir)));
            }
            const auto shape = im.shape();

            auto out(OIIO::ImageOutput::create(path));
            if (!out)
            {
                std::ostringstream errorstream;
                errorstream << " StoreWithOIIOScalar(): Error opening file '";
                errorstream << path;
                errorstream << "\nGet last error:\n";
                errorstream << OIIO::geterror();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            }
            OIIO::ImageSpec spec = options.spec;

            spec.width = static_cast<int>(shape[1]);
            spec.height = static_cast<int>(shape[0]);
            spec.nchannels = 1;
            spec.format = OIIO::BaseTypeFromC<T>::value;

            out->open(path, spec);

            StoreWithOIIOScalar(im, *out);
        }

        /**
         * @brief  Helper function which copy contant of DenseImage into OIIO object assuming RGB like color space
         *
         * @tparam T DenseImage pType
         * @param im Image to copy into OIIO object
         * @param output preconstructed OIIO object
         * @warning Unsafe method use @c ImageWriter for safety check
         */
        template <typename T>
        void StoreWithOIIOCompound3(const DenseImage<compound<T, 3>, 2> &im, OIIO::ImageOutput &out)
        {
            const OIIO::ImageSpec &spec = out.spec();
            POUTRE_CHECK(spec.nchannels == 3, "StoreWithOIIOCompound3(): Nb of channels must be 3");
            std::vector<T> buffer(spec.width * spec.height * spec.nchannels);
            const auto ptr_buffer = buffer.data();
            auto ptr_img = im.data();

            pDOUBLE min_0 = TypeTraits<pDOUBLE>::sup(), min_1 = TypeTraits<pDOUBLE>::sup(), //-V656
                min_2 = TypeTraits<pDOUBLE>::sup();                                         //-V656
            pDOUBLE max_0 = TypeTraits<pDOUBLE>::inf(), max_1 = TypeTraits<pDOUBLE>::inf(), //-V656
                max_2 = TypeTraits<pDOUBLE>::inf();                                         //-V656

            for (size_t i = 0; i < buffer.size(); i += 3)
            {
                ptr_buffer[i] = (*ptr_img)[0];
                min_0 = std::min(min_0, (pDOUBLE)ptr_buffer[i]);
                max_0 = std::max(max_0, (pDOUBLE)ptr_buffer[i]);

                ptr_buffer[i + 1] = (*ptr_img)[1];
                min_1 = std::min(min_1, (pDOUBLE)ptr_buffer[i + 1]);
                max_1 = std::max(max_1, (pDOUBLE)ptr_buffer[i + 1]);

                ptr_buffer[i + 2] = (*ptr_img)[2];
                min_2 = std::min(min_2, (pDOUBLE)ptr_buffer[i + 2]);
                max_2 = std::max(max_2, (pDOUBLE)ptr_buffer[i + 2]);

                ptr_img++;
            }

            // stolen from https://github.com/xtensor-stack/xtensor-io/blob/master/include/xtensor-io/ximage.hpp
            if (out.spec().format != OIIO::BaseTypeFromC<T>::value)
            {
                // OpenImageIO changed the target type because the file format doesn't support value_type.
                // It will do automatic conversion, but the data should be in the range 0...1
                // for good results.

                //! TODO! make measurement module
                //! TODO! vectorize

                if (max_0 != max_0 && max_1 != min_1 && max_2 != min_2) //-V550
                {                                                       //-V550
                    std::vector<pDOUBLE> normalized(buffer.size());
                    for (size_t i = 0; i < buffer.size(); i += 3)
                    {
                        normalized[i] = (pDOUBLE(1.0) / (max_0 - min_0)) * (buffer[i] - min_0);
                        normalized[i + 1] = (pDOUBLE(1.0) / (max_1 - min_1)) * (buffer[i + 1] - min_1);
                        normalized[i + 2] = (pDOUBLE(1.0) / (max_2 - min_2)) * (buffer[i + 2] - min_2);
                    }
                    out.write_image(OIIO::BaseTypeFromC<pDOUBLE>::value, &normalized[0]);
                }
                else
                {
                    out.write_image(OIIO::BaseTypeFromC<T>::value, &buffer[0]);
                }
            }
            else
            {
                out.write_image(OIIO::BaseTypeFromC<T>::value, &buffer[0]);
            }
            out.close();
        }

        /**
         * @brief  Helper function which copy contant of DenseImage on disk assuming RGB like color space
         *
         * @tparam T DenseImage pType
         * @param im Image to serialize
         * @param path path where image is serialized
         * @warning Unsafe method use @c ImageWriter for safety check
         */
        template <typename T>
        void StoreWithOIIOCompound3(const DenseImage<compound<T, 3>, 2> &im, const std::string &path,
                                    StoreWithOIIOOptions const &options)
        {
            bf::path localPath(path);
            bf::path dir = localPath.parent_path();
            if (!(bf::exists(dir)))
            {
                POUTRE_RUNTIME_ERROR(
                    (poutre::format("StoreWithOIIOCompound3: provided path {%s} doesn't exists", dir)));
            }
            const auto shape = im.shape();

            auto out(OIIO::ImageOutput::create(path));
            if (!out)
            {
                std::ostringstream errorstream;
                errorstream << " StoreWithOIIOCompound3(): Error opening file '";
                errorstream << path;
                errorstream << "\nGet last error:\n";
                errorstream << OIIO::geterror();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            }
            OIIO::ImageSpec spec = options.spec;

            spec.width = static_cast<int>(shape[1]);
            spec.height = static_cast<int>(shape[0]);
            spec.nchannels = 3;
            spec.format = OIIO::BaseTypeFromC<T>::value;

            out->open(path, spec);

            StoreWithOIIOCompound3(im, *out);
        }

        /**
         * @brief  Helper function which copy contant of DenseImage into OIIO object assuming RGBA like color space
         *
         * @tparam T DenseImage pType
         * @param im Image to copy into OIIO object
         * @param output preconstructed OIIO object
         * @warning Unsafe method use @c ImageWriter for safety check
         */
        template <typename T>
        void StoreWithOIIOCompound4(const DenseImage<compound<T, 4>, 2> &im, OIIO::ImageOutput &out)
        {
            const OIIO::ImageSpec &spec = out.spec();
            POUTRE_CHECK(spec.nchannels == 4, "StoreWithOIIOCompound4(): Nb of channels must be 4"); //-V112
            std::vector<T> buffer(spec.width * spec.height * spec.nchannels);
            const auto ptr_buffer = buffer.data();
            auto ptr_img = im.data();

            pDOUBLE min_0 = TypeTraits<pDOUBLE>::sup(), min_1 = TypeTraits<pDOUBLE>::sup(), //-V656
                min_2 = TypeTraits<pDOUBLE>::sup(), min_3 = TypeTraits<pDOUBLE>::sup();     //-V656
            pDOUBLE max_0 = TypeTraits<pDOUBLE>::inf(), max_1 = TypeTraits<pDOUBLE>::inf(), //-V656
                max_2 = TypeTraits<pDOUBLE>::inf(), max_3 = TypeTraits<pDOUBLE>::inf();     //-V656

            for (size_t i = 0; i < buffer.size(); i += 4) //-V112
            {
                ptr_buffer[i] = (*ptr_img)[0];
                min_0 = std::min(min_0, (pDOUBLE)ptr_buffer[i]);
                max_0 = std::max(max_0, (pDOUBLE)ptr_buffer[i]);

                ptr_buffer[i + 1] = (*ptr_img)[1];
                min_1 = std::min(min_1, (pDOUBLE)ptr_buffer[i + 1]);
                max_1 = std::max(max_1, (pDOUBLE)ptr_buffer[i + 1]);

                ptr_buffer[i + 2] = (*ptr_img)[2];
                min_2 = std::min(min_2, (pDOUBLE)ptr_buffer[i + 2]);
                max_2 = std::max(max_2, (pDOUBLE)ptr_buffer[i + 2]);

                ptr_buffer[i + 3] = (*ptr_img)[3];
                min_3 = std::min(min_3, (pDOUBLE)ptr_buffer[i + 2]);
                max_3 = std::max(max_3, (pDOUBLE)ptr_buffer[i + 2]);

                ptr_img++;
            }

            // stolen from https://github.com/xtensor-stack/xtensor-io/blob/master/include/xtensor-io/ximage.hpp
            if (out.spec().format != OIIO::BaseTypeFromC<T>::value)
            {
                // OpenImageIO changed the target type because the file format doesn't support value_type.
                // It will do automatic conversion, but the data should be in the range 0...1
                // for good results.

                //! TODO! make measurement module
                //! TODO! vectorize

                if (max_0 != max_0 && max_1 != min_1 && max_2 != min_2 && max_3 != min_3) //-V550
                {                                                                         //-V550
                    std::vector<pDOUBLE> normalized(buffer.size());
                    for (size_t i = 0; i < buffer.size(); i += 3)
                    {
                        normalized[i] = (pDOUBLE(1.0) / (max_0 - min_0)) * (buffer[i] - min_0);
                        normalized[i + 1] = (pDOUBLE(1.0) / (max_1 - min_1)) * (buffer[i + 1] - min_1);
                        normalized[i + 2] = (pDOUBLE(1.0) / (max_2 - min_2)) * (buffer[i + 2] - min_2);
                        normalized[i + 3] = (pDOUBLE(1.0) / (max_3 - min_3)) * (buffer[i + 3] - min_3);
                    }
                    out.write_image(OIIO::BaseTypeFromC<pDOUBLE>::value, &normalized[0]);
                }
                else
                {
                    out.write_image(OIIO::BaseTypeFromC<T>::value, &buffer[0]);
                }
            }
            else
            {
                out.write_image(OIIO::BaseTypeFromC<T>::value, &buffer[0]);
            }
            out.close();
        }

        /**
         * @brief  Helper function which copy contant of DenseImage on disk assuming RGBA like color space
         *
         * @tparam T DenseImage pType
         * @param im Image to serialize
         * @param path path where image is serialized
         * @warning Unsafe method use @c ImageWriter for safety check
         */
        template <typename T>
        void StoreWithOIIOCompound4(const DenseImage<compound<T, 4>, 2> &im, const std::string &path,
                                    StoreWithOIIOOptions const &options)
        {
            bf::path localPath(path);
            bf::path dir = localPath.parent_path();
            if (!(bf::exists(dir)))
            {
                std::ostringstream errorstream;
                errorstream << " StoreWithOIIOCompound4(): Error opening file '";
                errorstream << path;
                POUTRE_RUNTIME_ERROR(errorstream.str());
            }
            const auto shape = im.shape();

            auto out(OIIO::ImageOutput::create(path));
            if (!out)
            {

                std::ostringstream errorstream;
                errorstream << " StoreWithOIIOCompound3(): Error opening file '";
                errorstream << path;
                errorstream << "\nGet last error:\n";
                errorstream << OIIO::geterror();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            }
            OIIO::ImageSpec spec = options.spec;

            spec.width = static_cast<int>(shape[1]);
            spec.height = static_cast<int>(shape[0]);
            spec.nchannels = 4; //-V112
            spec.format = OIIO::BaseTypeFromC<T>::value;

            out->open(path, spec);

            StoreWithOIIOCompound4(im, *out);
        }

    } // namespace details

    //! @} doxygroup: image_processing_io_group_details

} // namespace poutre

#endif // POUTREOIIO_HPP