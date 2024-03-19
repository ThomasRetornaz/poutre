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
#ifdef POUTRE_USE_OIIO
#  include <poutreIO/include/poutreOIIO.hpp>
#endif
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
      POUTRE_RUNTIME_ERROR(std::format("ImageWriter: provided path {} doesn't exists", i_imgpath));
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
    std::string extension = localPath.extension().string();
    std::transform(
        extension.begin(), extension.end(), extension.begin(), [](unsigned char c) { return std::tolower(c); } // correct
    );
#ifdef POUTRE_USE_HDF5
    if( extension == ".h5" )
      return StoreWithHDF5(localPath.string(), i_img);
#endif
#ifdef POUTRE_USE_OIIO
    StoreWithOIIO(localPath.string(), i_img);
#endif
    throw std::runtime_error("ImageWrite::Write() no plugins found");
  }

} // namespace poutre
