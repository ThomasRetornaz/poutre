// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace poutre
{
    namespace bf = boost::filesystem;

    ImageWriter& ImageWriter::SetPath(const std::string& i_imgpath)
    {
        m_imgPath = i_imgpath;
        bf::path localPath(m_imgPath);
        boost::filesystem::path dir = localPath.parent_path();
        if (!(bf::exists(dir)))
        {
            POUTRE_RUNTIME_ERROR(boost::format("ImageWriter: provided path %s doesn't exists") % dir);
        }
        m_isready=true;
        return *this;
    }

    void ImageWriter::Write(const IInterface& i_img) const
    {   
        if (!m_isready) POUTRE_RUNTIME_ERROR("ImageWritter:  you must set path through SetPath");
        bf::path localPath(m_imgPath);
        //switch on extension
        auto extension = bf::extension(localPath);
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

