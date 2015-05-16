
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_COPIECONVERT_HPP__
#define POUTRE_IMAGEPROCESSING_INTERFACE_COPIECONVERT_HPP__

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>
#endif

namespace poutre
  {
  
  IMP_API std::unique_ptr<IInterface> GetSame(const IInterface& i_img1);

  IMP_API std::unique_ptr<IInterface> GetSameCoord(const IInterface& i_img1, CompoundType ctype, PType ptype);

  IMP_API std::unique_ptr<IInterface> GetSameCoord(const IInterface& i_img1, PType ptype);

  }
#endif//POUTRE_IMAGEPROCESSING_INTERFACE_COPIECONVERT_HPP__


