// This is an open source non-commercial project. Dear PVS-Studio, please check
// it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>

namespace poutre {

// todo define macros
template class Signal<pUINT8>;
template class Signal<pINT32>;
template class Signal<pFLOAT>;
template class Signal<pINT64>;
template class Signal<pDOUBLE>;

template class Image2D<pUINT8>;
template class Image2D<pINT32>;
template class Image2D<pFLOAT>;
template class Image2D<pINT64>;
template class Image2D<pDOUBLE>;

template class Image2D<compound_pixel<pUINT8, 3>>;
template class Image2D<compound_pixel<pINT32, 3>>;
template class Image2D<compound_pixel<pFLOAT, 3>>;
template class Image2D<compound_pixel<pINT64, 3>>;
template class Image2D<compound_pixel<pDOUBLE, 3>>;

template class Image2D<compound_pixel<pUINT8, 4>>;
template class Image2D<compound_pixel<pINT32, 4>>;
template class Image2D<compound_pixel<pFLOAT, 4>>;
template class Image2D<compound_pixel<pINT64, 4>>;
template class Image2D<compound_pixel<pDOUBLE, 4>>;

template class Image3D<pUINT8>;
template class Image3D<pINT32>;
template class Image3D<pFLOAT>;
template class Image3D<pINT64>;
template class Image3D<pDOUBLE>;

template class DenseImage<pUINT8, 4>;
template class DenseImage<pINT32, 4>;
template class DenseImage<pFLOAT, 4>;
template class DenseImage<pINT64, 4>;
template class DenseImage<pDOUBLE, 4>;

} // namespace poutre
