
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

#include <poutreIO/poutreIO.hpp>
#include <poutreIO/poutreIOLoader.hpp>

#include <deque>

namespace poutre
{
  /**
   * @addtogroup image_processing_io_group Image Processing IO API
   * @ingroup image_processing_group
   *@{
   */

  /**
   * @brief Collection of @c ImageLoader
   *
   */
  class IO_API ImageProvider
  {
    private:
    using self_type      = ImageProvider;
    using container_type = std::deque<ImageLoader>;
    using iterator       = container_type::iterator;
    using const_iterator = container_type::const_iterator;
    container_type m_container_loader;

    public:
    /**
     * @brief Construct a new Image Provider object, fill it with @c AddImage
     *
     */
    ImageProvider();

    /**
     * @brief Check emptiness
     *
     * @return true if empty
     * @return false otherwise
     */
    bool IsEmpty() const;

    //! Accessor to the image collection @c begin() iterator.
    const_iterator begin() const;

    //! Accessor to the image collection @c end() iterator.
    const_iterator end() const;

    //! Allow operator @c [] for sequential access.
    const ImageLoader &operator[](size_t index) const;

    /**
     * @brief Get current imageloader
     *
     * @return ImageLoader
     */
    ImageLoader Fetch();

    /**
     * @brief Add a path of an image to load see @c Fetch
     *
     * @param i_imgpath
     */
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

  //! @} doxygroup: image_processing_io_group
} // namespace poutre

#endif /* POUTREIOPROVIDER_HPP */
