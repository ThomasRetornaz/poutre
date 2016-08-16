
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>

namespace poutre
  {

  //todo define macros
  template class DenseImage < pUINT8, 1 > ;
  template class DenseImage < pINT32, 1 > ;
  template class DenseImage < pFLOAT, 1 > ;
  template class DenseImage < pINT64, 1 > ;
  template class DenseImage < pDOUBLE, 1 > ;

  template class DenseImage < pUINT8, 2 > ;
  template class DenseImage < pINT32, 2 > ;
  template class DenseImage < pFLOAT, 2 > ;
  template class DenseImage < pINT64, 2 > ;
  template class DenseImage < pDOUBLE, 2 > ;

  template class DenseImage < compound_pixel < pUINT8, 3 > > ;
  template class DenseImage < compound_pixel < pINT32, 3 > > ;
  template class DenseImage < compound_pixel < pFLOAT, 3 > > ;
  template class DenseImage < compound_pixel < pINT64, 3 > > ;
  template class DenseImage < compound_pixel < pDOUBLE, 3 > > ;

  template class DenseImage < compound_pixel < pUINT8, 4 > > ;
  template class DenseImage < compound_pixel < pINT32, 4 > > ;
  template class DenseImage < compound_pixel < pFLOAT, 4 > > ;
  template class DenseImage < compound_pixel < pINT64, 4 > > ;
  template class DenseImage < compound_pixel < pDOUBLE, 4 > > ;

  template class DenseImage < pUINT8, 3 > ;
  template class DenseImage < pINT32, 3 > ;
  template class DenseImage < pFLOAT, 3 > ;
  template class DenseImage < pINT64, 3 > ;
  template class DenseImage < pDOUBLE, 3 > ;

  template class DenseImage < pUINT8, 4 > ;
  template class DenseImage < pINT32, 4 > ;
  template class DenseImage < pFLOAT, 4 > ;
  template class DenseImage < pINT64, 4 > ;
  template class DenseImage < pDOUBLE, 4 > ;

  }
