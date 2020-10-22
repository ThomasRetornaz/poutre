
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
 * File:   poutreIOString.hpp
 * Author: thomas
 *
 * Created on 2 octobre 2015, 21:15
 */

#ifndef POUTREIOSTRING_HPP
#define POUTREIOSTRING_HPP

#include <memory>
#include <poutreIO/poutreIO.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#include <string>

namespace poutre
{
  /**
   * @addtogroup image_processing_io_group Image Processing IO API
   * @ingroup image_processing_group
   *@{
   */

  /**
   * @brief Convert IInterface to human readable string
   *
   * @param i_image
   * @return std::string
   */
  IO_API std::string ImageToString(const IInterface &i_image);

  /**
   * @brief From human readable string create an IInterface see @c ImageToString
   *
   * @param i_str
   * @return Image
   */
  IO_API std::unique_ptr<IInterface> ImageFromString(const std::string &i_str);

  //! @} doxygroup: image_processing_io_group

} // namespace poutre

#endif /* POUTREIOSTRING_HPP */
