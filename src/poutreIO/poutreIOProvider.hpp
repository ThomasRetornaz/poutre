
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
 * File:   poutreIOProvider.hpp
 * Author: thomas
 *
 * Created on 25 Juillet 2016
 */

#ifndef POUTREIOPROVIDER_HPP
#define POUTREIOPROVIDER_HPP

#include <deque>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#include <boost/filesystem.hpp>

#ifndef POUTREIOLOADER_HPP
#include <poutreIO/poutreIOLoader.hpp>
#endif

namespace poutre
{
    class IO_API ImageProvider
    {
      private:
        using self_type = ImageProvider;
        using container_type = std::deque<ImageLoader>;
        using iterator = container_type::iterator;
        using const_iterator = container_type::const_iterator;
        container_type m_container_loader;

      public:
        ImageProvider();

        bool IsEmpty() const;

        //! Accessor to the image collection @c begin() iterator.
        const_iterator begin() const;

        //! Accessor to the image collection @c end() iterator.
        const_iterator end() const;

        //! Allow operator @c [] for sequential access.
        const ImageLoader &operator[](size_t index) const;

        ImageLoader Fetch();

        void AddImage(const std::string &i_imgpath);

        // TODO
        // void AddImages(const std::string& path);

        // rule of 0
        // virtual ~ImageProvider() {}

        // ImageProvider(self_type &&rhs) = default;

        // self_type &operator=(self_type &&rhs) = default;

        // ImageProvider(const self_type& rhs) = default;
        //
        // self_type& operator=(const self_type& rhs) = default;
    };
} // namespace poutre

#endif /* POUTREIOPROVIDER_HPP */
