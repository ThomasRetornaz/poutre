
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
#define	POUTREIOWRITTER_HPP

#include <string>
#include <memory>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>
#endif

#include <boost/filesystem.hpp>

namespace poutre
{
    class IO_API ImageWriter
    {
        using self_type = ImageWriter;
        std::string m_imgPath;
        ImageIOFormat m_imgformat;
        bool m_isready;

    public:
        ImageWriter() :m_imgPath(), m_imgformat(ImageIOFormat::PNG), m_isready(false){}

        virtual ~ImageWriter() {}

        //rule of 0
        //virtual ~ImageLoader() {}

        //ImageLoader(self_type &&rhs) = default;

        //self_type &operator=(self_type &&rhs) = default;

        //ImageLoader(const self_type& rhs) = default;
        //
        //self_type& operator=(const self_type& rhs) = default;

        self_type& SetPath(const std::string& i_imgpath);
        void Write(const IInterface& i_img) const;
    };
}//poutre

#endif	/* POUTREIOWRITTER_HPP */

