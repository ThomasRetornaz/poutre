
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_COPIECONVERT_HPP__
#define POUTRE_IMAGEPROCESSING_INTERFACE_COPIECONVERT_HPP__

/**
 * @file   poutreImageProcessingInterfaceCopieConvert.hpp
 * @author Thomas Retornaz
 * @brief  Copie/Convert image helpers
 * 
 * 
 */

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>
#endif

namespace poutre
  {
/**
 * @ingroup image_processing_interface_group 
 *@{
 */     
  //! Deep clone of provided image  
  IMP_API std::unique_ptr<IInterface> GetSame(const IInterface& i_img1);

  //! Create a clone of provided image: using same geometries  but with different @c  CompoundType and @c PType 
  IMP_API std::unique_ptr<IInterface> GetSameCoord(const IInterface& i_img1, CompoundType ctype, PType ptype);

  //! Create a clone of provided image using same geometries but with different  @c PType      
  IMP_API std::unique_ptr<IInterface> GetSameCoord(const IInterface& i_img1, PType ptype);
     
//! @} doxygroup: image_processing_group
  }
#endif//POUTRE_IMAGEPROCESSING_INTERFACE_COPIECONVERT_HPP__


