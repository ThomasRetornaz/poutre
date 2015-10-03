
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
#define	POUTREIOSTRING_HPP

#include <string>
#include <memory>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>
#endif

namespace poutre
{   
    IO_API std::string ImageToString(const poutre::IInterface& i_image);    
    IO_API std::unique_ptr<IInterface> ImageFromString(const std::string& i_str); 
    
}

#endif	/* POUTREIOSTRING_HPP */

