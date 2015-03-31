#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>

namespace poutre
  {

  //todo define macros
  template class DenseImage < PType::PType_Bin, 1 >;
  template class DenseImage < PType::PType_GrayUINT8, 1 >;
  template class DenseImage < PType::PType_GrayINT32, 1 >;
  template class DenseImage < PType::PType_F32, 1 >;
  template class DenseImage < PType::PType_GrayINT64, 1 >;

  template class DenseImage < PType::PType_Bin, 2 >;
  template class DenseImage < PType::PType_GrayUINT8, 2 >;
  template class DenseImage < PType::PType_GrayINT32, 2 >;
  template class DenseImage < PType::PType_F32, 2 >;
  template class DenseImage < PType::PType_GrayINT64, 2 >;

  extern template class DenseImage < PType::PType_Bin, 3 >;
  extern template class DenseImage < PType::PType_GrayUINT8, 3 >;
  extern template class DenseImage < PType::PType_GrayINT32, 3 >;
  extern template class DenseImage < PType::PType_F32, 3 >;
  extern template class DenseImage < PType::PType_GrayINT64, 3 >;

  extern template class DenseImage < PType::PType_Bin, 4 >;
  extern template class DenseImage < PType::PType_GrayUINT8, 4 >;
  extern template class DenseImage < PType::PType_GrayINT32, 4 >;
  extern template class DenseImage < PType::PType_F32, 4 >;
  extern template class DenseImage < PType::PType_GrayINT64, 4 >;


  }
