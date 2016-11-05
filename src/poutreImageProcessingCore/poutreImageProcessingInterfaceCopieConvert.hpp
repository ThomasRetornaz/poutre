
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
 * @brief  Copy/Convert image helpers
 *
 *
 */

#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#endif

namespace poutre
{
    /**
     * @ingroup image_processing_interface_group
     *@{
     */
     //! Deep clone of provided image  
    IMP_CORE_API std::unique_ptr<IInterface> Clone(const IInterface& i_img1);

    //! Clone of provided image but no data copied 
    IMP_CORE_API std::unique_ptr<IInterface> CloneGeometry(const IInterface& i_img1);

    //! Convert the provided image: using same geometries  but with different @c  CompoundType and @c PType, no data is copied 
    IMP_CORE_API std::unique_ptr<IInterface> ConvertGeometry(const IInterface& i_img1, CompoundType ctype, PType ptype);

    //! Create a clone of provided image using same geometries but with different  @c PType, no data is copied     
    IMP_CORE_API std::unique_ptr<IInterface> ConvertGeometry(const IInterface& i_img1, PType ptype);

    //! Copy i_img1 in i_img2
    IMP_CORE_API void Copy(const IInterface& i_img1, IInterface& i_img2);

    //! @} doxygroup: image_processing_group
}
#endif//POUTRE_IMAGEPROCESSING_INTERFACE_COPIECONVERT_HPP__


