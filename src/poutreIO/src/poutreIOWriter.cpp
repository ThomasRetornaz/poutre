
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
* File:   poutreIOLoader.hpp
* Author: thomas
*
* Created on 15 mars 2016
*/
#include <string>
#include <memory>

#include <poutreIO/poutreIO.hpp>
#include <poutreIO/include/poutreIOPNG.hpp>
#include <poutreIO/poutreIOWriter.hpp>
#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace poutre
{
    namespace bf = boost::filesystem;

    ImageWriter& ImageWriter::SetPath(const bf::path& i_imgpath)
    {
        m_imgPath = i_imgpath;
        boost::filesystem::path dir = m_imgPath.parent_path();
        if (!(bf::exists(dir)))
        {
            POUTRE_RUNTIME_ERROR(boost::format("ImageWriter: provided path %s doesn't exists") % dir);
        }
        return *this;
    }

    ImageWriter& ImageWriter::SetPath(bf::path&& i_imgpath)
    {
        m_imgPath = std::move(i_imgpath);//will call string(string&&)
        boost::filesystem::path dir = m_imgPath.parent_path();
        if (!(bf::exists(dir)))
        {
            POUTRE_RUNTIME_ERROR(boost::format("ImageWriter: provided path %s doesn't exists") % dir);
        }
        return *this;
    }

    //void ImageWriter::Write(IInterface&& i_img) const
    void ImageWriter::Write(IInterface&& i_img) const
    {
        //const IInterface& img = std::forward<IInterface>(i_img);
        //switch on extension
        auto extension = bf::extension(m_imgPath);
        boost::algorithm::to_lower(extension);
        if (extension == "png")
        {
            return  details::IOSavePng(m_imgPath, i_img);
        } //else if 
        else
        {
            POUTRE_RUNTIME_ERROR(
                (boost::format("ImageWriter::Write unsupported extension %s") % extension).str());
        }
    }

}//namespace poutre

