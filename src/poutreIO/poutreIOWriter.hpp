
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
 * File:   poutreIOWritter.hpp
 * Author: thomas
 *
 * Created on 15 mars 2016
 */

#ifndef POUTREIOWRITTER_HPP
#define POUTREIOWRITTER_HPP

#include <memory>
#include <string>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#endif

namespace poutre
{
    /**
     * @addtogroup image_processing_io_group Image Processing IO API
     * @ingroup image_processing_group
     *@{
     */

    /**
     * @brief Concrete ImageWritter
     * Powered by OpenImageIO toolset see https://sites.google.com/site/openimageio/home
     * so various formats are supported (and deduced from path)
     * at least the most common ones (PNG,JPEG,TIFF,BMP,hdf5,...)
     * @note for Image with more than 2 dimensions, only hdf5 format is supported
     * @note FOr hdf5 case the image is stored with "poutre_img_1" naming
     */
    class IO_API ImageWriter
    {
        using self_type = ImageWriter;
        std::string m_imgPath;
        bool m_isready;

      public:
        /**
         * @brief Construct a new Image Writer object
         *
         */
        ImageWriter() : m_imgPath(), m_isready(false)
        {
        }
        /**
         * @brief Destroy the Image Writer object
         *
         */
        virtual ~ImageWriter()
        {
        }

        // rule of 0
        // virtual ~ImageLoader() {}

        // ImageLoader(self_type &&rhs) = default;

        // self_type &operator=(self_type &&rhs) = default;

        // ImageLoader(const self_type& rhs) = default;
        //
        // self_type& operator=(const self_type& rhs) = default;

        /**
         * @brief Set the Path object
         *
         * @param i_imgpath path where to store the image, format is deduced from extention
         * @return self_type& chain
         * @note For image with more than 2 dimentions only hdf5 format is supported
         */
        self_type &SetPath(const std::string &i_imgpath);
        void Write(const IInterface &i_img) const;
    };
    //! @} doxygroup: image_processing_io_group
} // namespace poutre

#endif /* POUTREIOWRITTER_HPP */
