
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IO_HPP__
#define POUTRE_IO_HPP__

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#endif

#ifdef POUTRE_DYNAMIC   // defined if POUTRE is compiled as a DLL
#ifdef PoutreIO_EXPORTS // defined if we are building the POUTRE DLL (instead of using it)
#define IO_API MODULE_EXPORT
#else
#define IO_API MODULE_IMPORT
#endif // POUTRE_DLL_EXPORTS
#define IO_LOCAL MODULE_LOCAL
#else // POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define IO_API
#define IO_LOCAL
#endif // POUTRE_DYNAMIC

namespace poutre
{
    /**
     * @addtogroup image_processing_io_group Image Processing IO API
     * @ingroup image_processing_group
     *@{
     */

    /**
     * @brief Read 2D Image from path powered by OpenImageIO
     *
     * @param[in] image_path
     * @throw runtime_error in case of errors
     * @return std::unique_ptr<IInterface> fresh image instance
     * @warning Doesn't not support Image with more than 2 dimentions use @c LoadFromHDF5
     */
    IO_API std::unique_ptr<IInterface> LoadFromOIIO(const std::string &image_path);

    /**
     * Store image to disk using OpenImageIO
     * The desired image format is deduced from ``filename``.
     * Supported formats are those supported by OpenImageIO.
     * Most common formats are supported (jpg, png, gif, bmp, tiff).
     *
     * @param path[in] The path to the desired file
     * @param iimage[in] Image Interface
     * @throw runtime_error in case of errors
     * @warning Doesn't not support Image with more than 2 dimentions use @c StoreWithHDF5
     */
    IO_API void StoreWithOIIO(const std::string &path, const IInterface &iimage);

    /*!@brief Load from HDF5 file.
     * @param[in]  path absolute path
     * @param[in]  image_name the image name to read within the file.
     * @return fresh new image
     * @throw runtime_error in case of failure
     * @see StoreWithHDF5
     */
    IO_API std::unique_ptr<IInterface> LoadFromHDF5(const std::string &path,
                                                    const std::string &image_name = "poutre_img_1");

    /**
     * @brief Store image to disk as HDF5 file
     *
     * @param[in] path absolute path
     * @param[in] iimage the image to save
     * @param[in] image_name the name of the image into which the "image" will be saved
     * @throw runtime_error in case of failure
     * @see LoadFromHDF5
     */
    IO_API void StoreWithHDF5(const std::string &path, const IInterface &iimage,
                              const std::string &image_name = "poutre_img_1");
    //! @} doxygroup: image_processing_io_group
} // namespace poutre
#endif /* POUTRE_IO_HPP */
