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

#include <boost/lexical_cast.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <string>


namespace poutre {
std::ostream &operator<<(std::ostream &stream, const IInterface &img) {
  stream << img.str();
  return stream;
}

bool AsSizesCompatible(const IInterface &i_img1, const IInterface &i_img2) {
  if (i_img1.GetNumDims() != i_img2.GetNumDims())
    return false;
  const auto &coord1 = i_img1.GetCoords();
  const auto &coord2 = i_img2.GetCoords();
  return std::equal(coord1.begin(), coord1.end(), coord2.begin());
}

bool AsTypesCompatible(const IInterface &i_img1, const IInterface &i_img2) {
  if (i_img1.GetImgType() != i_img2.GetImgType())
    return false;
  if (i_img1.GetCType() != i_img2.GetCType())
    return false;
  if (i_img1.GetPType() != i_img2.GetPType())
    return false;
  return true;
}

void AssertSizesCompatible(const IInterface &i_img1, const IInterface &i_img2,
                           const std::string &i_msg) {
  if (!AsSizesCompatible(i_img1, i_img2))
    POUTRE_RUNTIME_ERROR(i_msg);
}

void AssertAsTypesCompatible(const IInterface &i_img1, const IInterface &i_img2,
                             const std::string &i_msg) {
  if (!AsTypesCompatible(i_img1, i_img2))
    POUTRE_RUNTIME_ERROR(i_msg);
}

// TODO FACTORIZE DISPATCH

//NDIMS
template <size_t numDims>
std::unique_ptr<IInterface>
CreateDenseDispatchPTypeScalar(const std::vector<std::size_t> &dims,
                              PType ptype) {
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<DenseImage<pUINT8,numDims>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<DenseImage<pINT32,numDims>>(dims);
  case PType::PType_F32:
    return std::make_unique<DenseImage<pFLOAT,numDims>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<DenseImage<pINT64,numDims>>(dims);
  case PType::PType_D64:
    return std::make_unique<DenseImage<pDOUBLE,numDims>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateDenseDispatchPTypeScalar:: Unsupported scalar type:" +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateDenseDispatchPType3PLanes(const std::vector<std::size_t> &dims,
                               PType ptype) {
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<DenseImage<compound_pixel<pUINT8, 3>, numDims>>(
        dims);
  case PType::PType_GrayINT32:
    return std::make_unique<DenseImage<compound_pixel<pINT32, 3>, numDims>>(
        dims);
  case PType::PType_F32:
    return std::make_unique<DenseImage<compound_pixel<pFLOAT, 3>, numDims>>(
        dims);
  case PType::PType_GrayINT64:
    return std::make_unique<DenseImage<compound_pixel<pINT64, 3>, numDims>>(
        dims);
  case PType::PType_D64:
    return std::make_unique<DenseImage<compound_pixel<pDOUBLE, 3>, numDims>>(
        dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateDenseDispatchPType3PLanes:: Unsupported compound type 3 with " +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateDenseDispatchPType4PLanes(const std::vector<std::size_t> &dims,
                               PType ptype) {
  switch (ptype) {
    // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<DenseImage<compound_pixel<pUINT8, 4>, numDims>>(
        dims);
  case PType::PType_GrayINT32:
    return std::make_unique<DenseImage<compound_pixel<pINT32, 4>, numDims>>(
        dims);
  case PType::PType_F32:
    return std::make_unique<DenseImage<compound_pixel<pFLOAT, 4>, numDims>>(
        dims);
  case PType::PType_GrayINT64:
    return std::make_unique<DenseImage<compound_pixel<pINT64, 4>, numDims>>(
        dims);
  case PType::PType_D64:
    return std::make_unique<DenseImage<compound_pixel<pDOUBLE, 4>, numDims>>(
        dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateDenseDispatchPType3PLanes:: Unsupported compound type 3 with " +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateDenseDispatchDims(const std::vector<std::size_t> &dims, CompoundType ctype,
                       PType ptype) {
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    return CreateDenseDispatchPTypeScalar<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_3Planes: {
    return CreateDenseDispatchPType3PLanes<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_4Planes: {
    return CreateDenseDispatchPType4PLanes<numDims>(dims, ptype);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR(("Unsupported compound type:" +
                          boost::lexical_cast<std::string>(ctype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

//1D
template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage1DDispatchPTypeScalar(const std::vector<std::size_t> &dims,
                                PType ptype) {
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<Signal<pUINT8>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<Signal<pINT32>>(dims);
  case PType::PType_F32:
    return std::make_unique<Signal<pFLOAT>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<Signal<pINT64>>(dims);
  case PType::PType_D64:
    return std::make_unique<Signal<pDOUBLE>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateImage1DDispatchPTypeScalar:: Unsupported scalar type:" +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage1DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype,
                     PType ptype) {
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    return CreateImage1DDispatchPTypeScalar<numDims>(dims, ptype);
  } break;
  // case CompoundType::CompoundType_3Planes:
  //   {
  //   return  CreateDenseDispatchPType3PLanes<numDims>(dims, ptype);
  //   }break;
  // case CompoundType::CompoundType_4Planes:
  //   {
  //   return  CreateDenseDispatchPType4PLanes<numDims>(dims, ptype);
  //   }break;
  default: {
    POUTRE_RUNTIME_ERROR(("Unsupported compound type:" +
                          boost::lexical_cast<std::string>(ctype)));
    return std::unique_ptr<IInterface>();
  }
  }
}


//2D
template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage2DDispatchPTypeScalar(const std::vector<std::size_t> &dims,
                                PType ptype) {
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<Image2D<pUINT8>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<Image2D<pINT32>>(dims);
  case PType::PType_F32:
    return std::make_unique<Image2D<pFLOAT>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<Image2D<pINT64>>(dims);
  case PType::PType_D64:
    return std::make_unique<Image2D<pDOUBLE>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateImage2DDispatchPTypeScalar:: Unsupported scalar type:" +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage2DDispatchPType3PLanes(const std::vector<std::size_t> &dims,
                                PType ptype) {
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<Image2D<compound_pixel<pUINT8, 3>>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<Image2D<compound_pixel<pINT32,3>>>(dims);
  case PType::PType_F32:
    return std::make_unique<Image2D<compound_pixel<pFLOAT,3>>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<Image2D<compound_pixel<pINT64,3>>>(dims);
  case PType::PType_D64:
    return std::make_unique<Image2D<compound_pixel<pDOUBLE,3>>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateImage2DDispatchPType3PLanes:: Unsupported scalar type:" +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage2DDispatchPType4PLanes(const std::vector<std::size_t> &dims,
                                PType ptype) {
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<Image2D<compound_pixel<pUINT8, 4>>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<Image2D<compound_pixel<pINT32,4>>>(dims);
  case PType::PType_F32:
    return std::make_unique<Image2D<compound_pixel<pFLOAT,4>>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<Image2D<compound_pixel<pINT64,4>>>(dims);
  case PType::PType_D64:
    return std::make_unique<Image2D<compound_pixel<pDOUBLE,4>>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateImage2DDispatchPType4PLanes:: Unsupported scalar type:" +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage2DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype,
                     PType ptype) {
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    return CreateImage2DDispatchPTypeScalar<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_3Planes:
     {
     return  CreateImage2DDispatchPType3PLanes<numDims>(dims, ptype);
     }break;
   case CompoundType::CompoundType_4Planes:
     {
     return  CreateImage2DDispatchPType4PLanes<numDims>(dims, ptype);
     }break;
  default: {
    POUTRE_RUNTIME_ERROR(("Unsupported compound type:" +
                          boost::lexical_cast<std::string>(ctype)));
    return std::unique_ptr<IInterface>();
  }
  }
}


//3D
template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage3DDispatchPTypeScalar(const std::vector<std::size_t> &dims,
                                PType ptype) {
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<Image3D<pUINT8>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<Image3D<pINT32>>(dims);
  case PType::PType_F32:
    return std::make_unique<Image3D<pFLOAT>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<Image3D<pINT64>>(dims);
  case PType::PType_D64:
    return std::make_unique<Image3D<pDOUBLE>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateImage3DDispatchPTypeScalar:: Unsupported scalar type:" +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage3DDispatchPType3PLanes(const std::vector<std::size_t> &dims,
                                PType ptype) {
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<Image3D<compound_pixel<pUINT8, 3>>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<Image3D<compound_pixel<pINT32,3>>>(dims);
  case PType::PType_F32:
    return std::make_unique<Image3D<compound_pixel<pFLOAT,3>>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<Image3D<compound_pixel<pINT64,3>>>(dims);
  case PType::PType_D64:
    return std::make_unique<Image3D<compound_pixel<pDOUBLE,3>>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateImage3DDispatchPType3PLanes:: Unsupported scalar type:" +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage3DDispatchPType4PLanes(const std::vector<std::size_t> &dims,
                                PType ptype) {
  switch (ptype) {
  // todo think about bool/binary here
  case PType::PType_GrayUINT8:
    return std::make_unique<Image3D<compound_pixel<pUINT8, 4>>>(dims);
  case PType::PType_GrayINT32:
    return std::make_unique<Image3D<compound_pixel<pINT32,4>>>(dims);
  case PType::PType_F32:
    return std::make_unique<Image3D<compound_pixel<pFLOAT,4>>>(dims);
  case PType::PType_GrayINT64:
    return std::make_unique<Image3D<compound_pixel<pINT64,4>>>(dims);
  case PType::PType_D64:
    return std::make_unique<Image3D<compound_pixel<pDOUBLE,4>>>(dims);
  default: {
    POUTRE_RUNTIME_ERROR(
        ("CreateImage3DDispatchPType4PLanes:: Unsupported scalar type:" +
         boost::lexical_cast<std::string>(ptype)));
    return std::unique_ptr<IInterface>();
  }
  }
}

template <size_t numDims>
std::unique_ptr<IInterface>
CreateImage3DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype,
                     PType ptype) {
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    return CreateImage3DDispatchPTypeScalar<numDims>(dims, ptype);
  } break;
  case CompoundType::CompoundType_3Planes:
     {
     return  CreateImage3DDispatchPType3PLanes<numDims>(dims, ptype);
     }break;
   case CompoundType::CompoundType_4Planes:
     {
     return  CreateImage3DDispatchPType4PLanes<numDims>(dims, ptype);
     }break;
  default: {
    POUTRE_RUNTIME_ERROR(("Unsupported compound type:" +
                          boost::lexical_cast<std::string>(ctype)));
    return std::unique_ptr<IInterface>();
  }
  }
}
//main
std::unique_ptr<IInterface> CreateDense(const std::vector<std::size_t> &dims,
                                        CompoundType ctype, PType ptype) {
  const auto &numDims = dims.size();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims:0");
    return std::unique_ptr<IInterface>();
  } break;
  case 1: {
    return CreateImage1DDispatch<1>(dims, ctype, ptype);
  } break;
  case 2: {
    return CreateImage2DDispatch<2>(dims, ctype, ptype);
  } break;
  case 3: {
    return CreateImage3DDispatch<3>(dims, ctype, ptype);
  } break;
  case 4: {
    return CreateDenseDispatchDims<4>(dims, ctype, ptype);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("Unsupported number of dims");
    return std::unique_ptr<IInterface>();
  }
  }
}
} // namespace poutre
