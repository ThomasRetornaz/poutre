
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>
#include <poutreBase/poutreTrace.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

namespace poutre
  {
	  std::ostream& operator<<(std::ostream& stream, const IInterface& img)
    {
    stream << img.str( );
    return stream;
    }

  bool AsSizesCompatible(const IInterface& i_img1, const IInterface& i_img2)
    {
    if (i_img1.GetNumDims( ) != i_img2.GetNumDims( ))
      return false;
    const auto& coord1 = i_img1.GetCoords( );
    const auto& coord2 = i_img2.GetCoords( );
    return std::equal(coord1.begin( ), coord1.end( ), coord2.begin( ));
    }

  bool AsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2)
    {
    if (i_img1.GetImgType( ) != i_img2.GetImgType( ))
      return false;
    if (i_img1.GetCType( ) != i_img2.GetCType( ))
      return false;
    if (i_img1.GetPType( ) != i_img2.GetPType( ))
      return false;
    return true;
    }

  void AssertSizesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg)
    {
    if (!AsSizesCompatible(i_img1, i_img2)) POUTRE_RUNTIME_ERROR(i_msg);
    }


  void AssertAsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg)
    {
    if (!AsTypesCompatible(i_img1, i_img2)) POUTRE_RUNTIME_ERROR(i_msg);
    }

  //TODO FACTOEIZE DISPATCH

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateDenseDipatchPTypeScalar(const std::vector<std::size_t>& dims, PType ptype)
    {
    switch (ptype)
      {
        //todo think about bool/binary here
        case PType::PType_GrayUINT8:
          return std::unique_ptr<IInterface>(new DenseImage < pUINT8, numDims>(dims));
        case PType::PType_GrayINT32:
          return std::unique_ptr<IInterface>(new DenseImage < pINT32, numDims>(dims));
        case PType::PType_F32:
          return std::unique_ptr<IInterface>(new DenseImage < pFLOAT, numDims>(dims));
        case PType::PType_GrayINT64:
          return std::unique_ptr<IInterface>(new DenseImage < pINT64, numDims>(dims));
        case PType::PType_D64:
          return std::unique_ptr<IInterface>(new DenseImage < pDOUBLE, numDims>(dims));
        default:
          {
          POUTRE_RUNTIME_ERROR(("CreateDenseDipatchPTypeScalar:: Unsupported scalar type:" + boost::lexical_cast<std::string>(ptype)));
          return std::unique_ptr < IInterface >( );
          }
      }
    }


  template<size_t numDims>
  std::unique_ptr<IInterface> CreateDenseDipatchPType3PLanes(const std::vector<std::size_t>& dims, PType ptype)
    {
    switch (ptype)
      {
      //todo think about bool/binary here
        case PType::PType_GrayUINT8:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pUINT8,3>, numDims>(dims));
        case PType::PType_GrayINT32:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pINT32,3>, numDims>(dims));
        case PType::PType_F32:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pFLOAT,3>, numDims>(dims));
        case PType::PType_GrayINT64:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pINT64,3>, numDims>(dims));
        case PType::PType_D64:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pDOUBLE,3>, numDims>(dims));
        default:
          {
          POUTRE_RUNTIME_ERROR(("CreateDenseDipatchPType3PLanes:: Unsupported compound type 3 with " + boost::lexical_cast<std::string>(ptype)));
          return std::unique_ptr < IInterface >( );
          }
      }
    }

  template<size_t numDims>
  std::unique_ptr<IInterface> CreateDenseDipatchPType4PLanes(const std::vector<std::size_t>& dims, PType ptype)
    {
    switch (ptype)
      {
      //todo think about bool/binary here
        case PType::PType_GrayUINT8:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pUINT8, 4>, numDims>(dims));
        case PType::PType_GrayINT32:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pINT32, 4>, numDims>(dims));
        case PType::PType_F32:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pFLOAT, 4>, numDims>(dims));
        case PType::PType_GrayINT64:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pINT64, 4>, numDims>(dims));
        case PType::PType_D64:
          return std::unique_ptr<IInterface>(new DenseImage < compound_pixel<pDOUBLE, 4>, numDims>(dims));
        default:
          {
          POUTRE_RUNTIME_ERROR(("CreateDenseDipatchPType3PLanes:: Unsupported compound type 3 with " + boost::lexical_cast<std::string>(ptype)));
          return std::unique_ptr < IInterface >( );
          }
      }
    }


  template<size_t numDims>
  std::unique_ptr<IInterface> CreateDenseDipatchDims(const std::vector<std::size_t>& dims,CompoundType ctype, PType ptype)
    {
    switch (ctype)
      {
        case CompoundType::CompoundType_Scalar:
          {
          return  CreateDenseDipatchPTypeScalar<numDims>(dims, ptype);
          }break;
        case CompoundType::CompoundType_3Planes:
          {
          return  CreateDenseDipatchPType3PLanes<numDims>(dims, ptype);
          }break;
        case CompoundType::CompoundType_4Planes:
          {
          return  CreateDenseDipatchPType4PLanes<numDims>(dims, ptype);
          }break;
        default:
          {
          POUTRE_RUNTIME_ERROR(("Unsupported compound type:" + boost::lexical_cast<std::string>(ctype)));
          return std::unique_ptr < IInterface >( );
          }
      }
    }

  std::unique_ptr<IInterface> CreateDense(const std::vector<std::size_t>& dims,CompoundType ctype, PType ptype)
    {
    const auto& numDims = dims.size( );
    switch (numDims)
      {
        case 0:
          {
          POUTRE_RUNTIME_ERROR("Unsupported number of dims:0"); return std::unique_ptr < IInterface >( );
          }break;
        case 1:
          {
          return CreateDenseDipatchDims<1>(dims,ctype,ptype);
          }break;
        case 2:
          {
          return CreateDenseDipatchDims<2>(dims,ctype,ptype);
          }break;
        case 3:
          {
          return CreateDenseDipatchDims<3>(dims,ctype, ptype);
          }break;
        case 4:
          {
          return CreateDenseDipatchDims<4>(dims,ctype, ptype);
          }break;
        default:
          {
          POUTRE_RUNTIME_ERROR("Unsupported number of dims"); return std::unique_ptr < IInterface >( );
          }
      }
    }
}//namespace
