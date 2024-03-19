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
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>

#include <string>

namespace poutre
{
  std::ostream &operator<<(std::ostream &stream, const IInterface &img) // NOLINT(misc-unused-parameters)
  {
    stream << img.str();
    return stream;
  }

  bool AsSizesCompatible(const IInterface &i_img1, const IInterface &i_img2) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("AsSizesCompatible");
    if( i_img1.GetNumDims() != i_img2.GetNumDims() )
      return false;
    const auto &coord1 = i_img1.GetCoords();
    const auto &coord2 = i_img2.GetCoords();
    return std::equal(coord1.begin(), coord1.end(), coord2.begin());
  }

  bool AsTypesCompatible(const IInterface &i_img1, const IInterface &i_img2) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("AsTypesCompatible");
    if( i_img1.GetImgType() != i_img2.GetImgType() )
      return false;
    if( i_img1.GetCType() != i_img2.GetCType() )
      return false;
    if( i_img1.GetPType() != i_img2.GetPType() )
      return false;
    return true;
  }

  void AssertSizesCompatible(const IInterface  &i_img1,
                             const IInterface  &i_img2,
                             const std::string &i_msg) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("AssertSizesCompatible");
    if( !AsSizesCompatible(i_img1, i_img2) )
      POUTRE_RUNTIME_ERROR(i_msg);
  }

  void AssertAsTypesCompatible(const IInterface  &i_img1,
                               const IInterface  &i_img2,
                               const std::string &i_msg) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("AssertAsTypesCompatible");
    if( !AsTypesCompatible(i_img1, i_img2) )
      POUTRE_RUNTIME_ERROR(i_msg);
  }

  // TODO FACTORIZE DISPATCH

  // NDIMS
  template<size_t numDims>
  std::unique_ptr<IInterface> CreateDenseDispatchPTypeScalar(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
    // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<pUINT8, numDims>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<pINT32, numDims>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<pFLOAT, numDims>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<pINT64, numDims>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<pDOUBLE, numDims>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateDenseDispatchPTypeScalar:: Unsupported scalar type:{}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateDenseDispatchPType3PLanes(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
      // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<compound<pUINT8, 3>, numDims>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<compound<pINT32, 3>, numDims>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<compound<pFLOAT, 3>, numDims>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<compound<pINT64, 3>, numDims>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<compound<pDOUBLE, 3>, numDims>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateDenseDispatchPType3PLanes:: Unsupported compound type 3 with {}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateDenseDispatchPType4PLanes(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
      // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<compound<pUINT8, 4>, numDims>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<compound<pINT32, 4>, numDims>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<compound<pFLOAT, 4>, numDims>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<compound<pINT64, 4>, numDims>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<compound<pDOUBLE, 4>, numDims>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateDenseDispatchPType3PLanes:: Unsupported compound type 3 with {}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateDenseDispatchDims(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
  {
    switch( ctype )
    {
    case CompoundType::CompoundType_Scalar:
    {
      return CreateDenseDispatchPTypeScalar<numDims>(dims, ptype);
    }
    break;
    case CompoundType::CompoundType_3Planes:
    {
      return CreateDenseDispatchPType3PLanes<numDims>(dims, ptype);
    }
    break;
    case CompoundType::CompoundType_4Planes:
    {
      return CreateDenseDispatchPType4PLanes<numDims>(dims, ptype);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  // 1D
  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage1DDispatchPTypeScalar(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
    // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<pUINT8, 1>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<pINT32, 1>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<pFLOAT, 1>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<pINT64, 1>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<pDOUBLE, 1>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateImage1DDispatchPTypeScalar:: Unsupported scalar type:{}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage1DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
  {
    switch( ctype )
    {
    case CompoundType::CompoundType_Scalar:
    {
      return CreateImage1DDispatchPTypeScalar<numDims>(dims, ptype);
    }
    break;
    // case CompoundType::CompoundType_3Planes:
    //   {
    //   return  CreateDenseDispatchPType3PLanes<numDims>(dims, ptype);
    //   }break;
    // case CompoundType::CompoundType_4Planes:
    //   {
    //   return  CreateDenseDispatchPType4PLanes<numDims>(dims, ptype);
    //   }break;
    default:
    {
      // POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  // 2D
  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage2DDispatchPTypeScalar(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
    // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<pUINT8, 2>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<pINT32, 2>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<pFLOAT, 2>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<pINT64, 2>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<pDOUBLE, 2>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateImage2DDispatchPTypeScalar:: Unsupported scalar type:{}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage2DDispatchPType3PLanes(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
    // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<compound<pUINT8, 3>, 2>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<compound<pINT32, 3>, 2>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<compound<pFLOAT, 3>, 2>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<compound<pINT64, 3>, 2>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<compound<pDOUBLE, 3>, 2>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateImage2DDispatchPType3PLanes:: Unsupported scalar type:{}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage2DDispatchPType4PLanes(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
    // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<compound<pUINT8, 4>, 2>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<compound<pINT32, 4>, 2>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<compound<pFLOAT, 4>, 2>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<compound<pINT64, 4>, 2>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<compound<pDOUBLE, 4>, 2>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateImage2DDispatchPType4PLanes:: Unsupported scalar type:{}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage2DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
  {
    switch( ctype )
    {
    case CompoundType::CompoundType_Scalar:
    {
      return CreateImage2DDispatchPTypeScalar<numDims>(dims, ptype);
    }
    break;
    case CompoundType::CompoundType_3Planes:
    {
      return CreateImage2DDispatchPType3PLanes<numDims>(dims, ptype);
    }
    break;
    case CompoundType::CompoundType_4Planes:
    {
      return CreateImage2DDispatchPType4PLanes<numDims>(dims, ptype);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  // 3D
  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage3DDispatchPTypeScalar(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
    // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<pUINT8, 3>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<pINT32, 3>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<pFLOAT, 3>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<pINT64, 3>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<pDOUBLE, 3>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateImage3DDispatchPTypeScalar:: Unsupported scalar type:{}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage3DDispatchPType3PLanes(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
    // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<compound<pUINT8, 3>>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<compound<pINT32, 3>>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<compound<pFLOAT, 3>>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<compound<pINT64, 3>>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<compound<pDOUBLE, 3>>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateImage3DDispatchPType3PLanes:: Unsupported scalar type:{}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage3DDispatchPType4PLanes(const std::vector<std::size_t> &dims, PType ptype)
  {
    switch( ptype )
    {
    // todo think about bool/binary here
    case PType::PType_GrayUINT8: return std::make_unique<DenseImage<compound<pUINT8, 4>>>(dims);
    case PType::PType_GrayINT32: return std::make_unique<DenseImage<compound<pINT32, 4>>>(dims);
    case PType::PType_F32: return std::make_unique<DenseImage<compound<pFLOAT, 4>>>(dims);
    case PType::PType_GrayINT64: return std::make_unique<DenseImage<compound<pINT64, 4>>>(dims);
    case PType::PType_D64: return std::make_unique<DenseImage<compound<pDOUBLE, 4>>>(dims);
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("CreateImage3DDispatchPType4PLanes:: Unsupported scalar type:{}", ptype));
      return std::unique_ptr<IInterface>();
    }
    }
  }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateImage3DDispatch(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype)
  {
    switch( ctype )
    {
    case CompoundType::CompoundType_Scalar:
    {
      return CreateImage3DDispatchPTypeScalar<numDims>(dims, ptype);
    }
    break;
    case CompoundType::CompoundType_3Planes:
    {
      return CreateImage3DDispatchPType3PLanes<numDims>(dims, ptype);
    }
    break;
    case CompoundType::CompoundType_4Planes:
    {
      return CreateImage3DDispatchPType4PLanes<numDims>(dims, ptype);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR(std::format("Unsupported compound type:{}", ctype));
      return std::unique_ptr<IInterface>();
    }
    }
  }
  // main
  std::unique_ptr<IInterface>
  CreateDense(const std::vector<std::size_t> &dims, CompoundType ctype, PType ptype) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("CreateDense");
    const auto &numDims = dims.size();
    switch( numDims )
    {
    case 0:
    {
      POUTRE_RUNTIME_ERROR("Unsupported number of dims:0");
      return std::unique_ptr<IInterface>();
    }
    break;
    case 1:
    {
      return CreateImage1DDispatch<1>(dims, ctype, ptype);
    }
    break;
    case 2:
    {
      return CreateImage2DDispatch<2>(dims, ctype, ptype);
    }
    break;
    case 3:
    {
      return CreateImage3DDispatch<3>(dims, ctype, ptype);
    }
    break;
    case 4:
    {
      return CreateDenseDispatchDims<4>(dims, ctype, ptype);
    }
    break;
    default:
    {
      POUTRE_RUNTIME_ERROR("Unsupported number of dims");
      return std::unique_ptr<IInterface>();
    }
    }
  }
} // namespace poutre
