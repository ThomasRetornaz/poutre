
//==============================================================================
//                  Copyright (c) 2020 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
 * File:   poutreOIIO.hpp
 * Author: thomas retornaz
 *
 * Created on 02 mai 2020
 */
#ifndef POUTREOIIO_HPP
#define POUTREOIIO_HPP

#include <memory>
#include <string>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#endif

#ifdef __CLANG__
#pragma clang diagnostic push
// silencing warnings from OpenEXR 2.2.0 / OpenImageIO
#pragma clang diagnostic ignored "-Wdeprecated-register"
#pragma cling load("OpenImageIO")
#endif

#include "OpenImageIO/imageio.h"

#ifdef __CLANG__
#pragma clang diagnostic pop
#endif

namespace poutre
{
    namespace details
    {

    } // namespace details
} // namespace poutre
#endif /* POUTREOIIO_HPP */