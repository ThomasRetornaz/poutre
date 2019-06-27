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
#include <poutreIO/poutreIOLoader.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace poutre
{
    namespace bf = boost::filesystem;

    ImageLoader& ImageLoader::SetPath(std::string&& i_imgpath)
    {
        m_imgPath = std::move(i_imgpath); //will call string(string&&)
        m_isready = true;
        return *this;
    }

    ImageLoader& ImageLoader::SetPath(const std::string& i_imgpath)
    {
        m_imgPath = i_imgpath;
        m_isready = true;
        return *this;
    }

    std::unique_ptr<IInterface> ImageLoader::Load() const
    {
        if (!m_isready) POUTRE_RUNTIME_ERROR("ImageLoader:  you must set path through SetPath");
        bf::path localPath(m_imgPath);
        if (!bf::exists(localPath)) {
            POUTRE_RUNTIME_ERROR(
                (boost::format("IOSavePng:: path don't exist %s") % m_imgPath).str());
        }

        auto extension = bf::extension(localPath);
        boost::algorithm::to_lower(extension);
        if (extension=="png")
        { 
            return  details::IOLoadPng(m_imgPath);
        } //else if 
        else
        {
            POUTRE_RUNTIME_ERROR(
                (boost::format("ImageLoader::Load unsupported extension %s") % extension).str());
        }
        return std::unique_ptr<IInterface>(); //shut up warning
    }

}//namespace poutre

