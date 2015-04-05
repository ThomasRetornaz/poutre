#ifndef POUTREIMAGEPROCESSINGINTERFACECOPIECONVERT_HPP__
#define POUTREIMAGEPROCESSINGINTERFACECOPIECONVERT_HPP__

#ifndef POUTREIMAGEPROCESSINGINTERFACE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>
#endif

namespace poutre
  {
  
  BASE_API std::unique_ptr<IInterface> GetSame(const IInterface& i_img1);

  BASE_API std::unique_ptr<IInterface> GetSameCoordinates(const IInterface& i_img1,PType ptype);

  }
#endif POUTREIMAGEPROCESSINGINTERFACECOPIECONVERT_HPP__


