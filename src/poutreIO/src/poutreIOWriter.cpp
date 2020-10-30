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
#include <poutreIO/include/poutreOIIO.hpp>
#include <poutreIO/poutreIO.hpp>
#include <poutreIO/poutreIOWriter.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace poutre
{
  namespace fs = std::filesystem;

  ImageWriter &ImageWriter::SetPath(const std::string &i_imgpath)
  {
    m_imgPath = i_imgpath;
    fs::path localPath(m_imgPath);
    fs::path dir = localPath.parent_path();
    if( !(fs::exists(dir)) )
    {
      POUTRE_RUNTIME_ERROR((poutre::format("ImageWriter: provided path {%s} doesn't exists", dir)));
    }
    m_isready = true;
    return *this;
  }

  void ImageWriter::Write(const IInterface &i_img) const
  {
    POUTRE_ENTERING("ImageLoader::Write()");
    if( !m_isready )
      POUTRE_RUNTIME_ERROR("ImageWritter:  you must set path through SetPath");
    fs::path localPath(m_imgPath);
    // switch on extension
    auto extension = localPath.extension().string();
    std::for_each(extension.begin(), extension.end(), [](char &c) { c = ::tolower(c); });
    if( extension == ".h5" )
      return StoreWithHDF5(localPath.string(), i_img);
    StoreWithOIIO(localPath.string(), i_img);
  }

} // namespace poutre
