
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/**
 * @file poutreIOLoader.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Common image loader
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef POUTREIOLOADER_HPP
#define POUTREIOLOADER_HPP

#include <poutreIO/poutreIO.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace poutre
{
  class ImageProvider;
}

namespace poutre
{
  namespace fs = std::filesystem;

  /**
   * @addtogroup image_processing_io_group Image Processing IO API
   * @ingroup image_processing_group
   *@{
   */

  /**
   * @brief Concrete ImageLoader
   * Powered by OpenImageIO toolset see https://sites.google.com/site/openimageio/home
   * so various formats are supported (and deduced from path)
   * at least the most common ones (PNG,JPEG,TIFF,BMP,hdf5,...)
   * @note for Image with more than 2 dimensions, only hdf5 format is supported
   * @warning in HDF5 case, only one image is loaded correspond to name "poutre_img_1", @c
   * StoreWithHDF5 for advanced options
   */
  class IO_API ImageLoader
  {
    using self_type = ImageLoader;
    friend class ImageProvider;
    std::string m_imgPath;
    bool        m_isready;

    public:
    /**
     * @brief Construct a new Image Loader object
     *
     */
    ImageLoader() : m_imgPath(), m_isready(false) {}

    // rule of 0
    // virtual ~ImageLoader() {}

    // ImageLoader(self_type &&rhs) = default;

    // self_type &operator=(self_type &&rhs) = default;

    // ImageLoader(const self_type& rhs) = default;
    //
    // self_type& operator=(const self_type& rhs) = default;

    /**
     * @brief Set the Path to the image to load
     *
     * @param i_imgpath universal reference to path to the image to load
     * @return self_type& return reference to object to chain call
     */
    self_type &SetPath(std::string &&i_imgpath);

    /**
     * @brief Set the Path to the image to load
     *
     * @param i_imgpath const reference to path to the image to load
     * @return self_type& return reference to object to chain call
     */
    self_type &SetPath(const std::string &i_imgpath);

    /**
     * @brief Load the image according to SetPath
     *
     * @return std::unique_ptr<IInterface>
     */
    std::unique_ptr<IInterface> Load() const;
  };

  //! @} doxygroup: image_processing_io_group
} // namespace poutre

#endif /* POUTREIOLOADER_HPP */
