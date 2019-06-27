// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreIO/poutreIOProvider.hpp>
#include <poutreIO/poutreIOLoader.hpp>

namespace poutre
{
    ImageProvider::ImageProvider() :m_container_loader() {}

    bool ImageProvider::IsEmpty() const { return m_container_loader.empty(); }

    //! Accessor to the image collection @c begin() iterator.
    ImageProvider::const_iterator ImageProvider::begin() const { return m_container_loader.begin(); }

    //! Accessor to the image collection @c end() iterator.
    ImageProvider::const_iterator ImageProvider::end() const { return m_container_loader.end(); }

    //! Allow operator @c [] for sequential access.
    const ImageLoader& ImageProvider::operator[](size_t index) const
    {
        POUTRE_ASSERTCHECK(index < m_container_loader.size(), "out of bounds");
        return m_container_loader[index];
    }

    void ImageProvider::AddImage(const std::string& i_imgpath)
    {
        ImageLoader loader;
        loader.SetPath(i_imgpath);
        m_container_loader.push_back(loader);
    }

    ImageLoader ImageProvider::Fetch()
    {
        if (m_container_loader.empty()) POUTRE_RUNTIME_ERROR("ImageProvider::Fetch container empty");
        ImageLoader loader=m_container_loader.front();
        m_container_loader.pop_front();
        return loader;//RVO
    }
}