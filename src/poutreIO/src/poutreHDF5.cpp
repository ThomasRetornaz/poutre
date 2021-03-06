// This is an open source non-commercial project. Dear PVS-Studio, please check
// it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2020 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreBase/poutreTrace.hpp>
#include <poutreIO/include/poutreHDF5.hpp>

#include <iostream>
#include <sstream>

namespace poutre
{
  void StoreWithHDF5(const std::string &path,
                     const IInterface & iimage,                          // NOLINT(misc-unused-parameters)
                     const std::string &image_name /*= "poutre_img_1"*/) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("StoreWithHDF5");
    /*
     * Turn off the auto-printing when failure occurs so that we can
     * handle the errors appropriately
     */
    H5::Exception::dontPrint();

    auto       ptype  = iimage.GetPType();
    auto       ctype  = iimage.GetCType();
    const auto coords = iimage.GetCoords();

    switch( coords.size() )
    {
    case 1:
    {
      switch( ctype )
      {
      case CompoundType::CompoundType_Scalar:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF5_helper<pUINT8, 1>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF5_helper<pINT32, 1>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF5_helper<pINT64, 1>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF5_helper<pFLOAT, 1>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF5_helper<pDOUBLE, 1>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_3Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF53Planes_helper<pUINT8, 1>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF53Planes_helper<pINT32, 1>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF53Planes_helper<pINT64, 1>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF53Planes_helper<pFLOAT, 1>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF53Planes_helper<pDOUBLE, 1>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_4Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF54Planes_helper<pUINT8, 1>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF54Planes_helper<pINT32, 1>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF54Planes_helper<pINT64, 1>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF54Planes_helper<pFLOAT, 1>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF54Planes_helper<pDOUBLE, 1>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported cTYpe {%s}", ctype)));
      }
    } // case 1
    break;
    case 2:
    {
      switch( ctype )
      {
      case CompoundType::CompoundType_Scalar:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF5_helper<pUINT8, 2>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF5_helper<pINT32, 2>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF5_helper<pINT64, 2>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF5_helper<pFLOAT, 2>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF5_helper<pDOUBLE, 2>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_3Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF53Planes_helper<pUINT8, 2>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF53Planes_helper<pINT32, 2>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF53Planes_helper<pINT64, 2>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF53Planes_helper<pFLOAT, 2>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF53Planes_helper<pDOUBLE, 2>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_4Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF54Planes_helper<pUINT8, 2>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF54Planes_helper<pINT32, 2>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF54Planes_helper<pINT64, 2>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF54Planes_helper<pFLOAT, 2>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF54Planes_helper<pDOUBLE, 2>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported cTYpe {%s}", ctype)));
      }
      break;
    } // case 2!
    case 3:
    {
      switch( ctype )
      {
      case CompoundType::CompoundType_Scalar:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF5_helper<pUINT8, 3>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF5_helper<pINT32, 3>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF5_helper<pINT64, 3>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF5_helper<pFLOAT, 3>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF5_helper<pDOUBLE, 3>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_3Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF53Planes_helper<pUINT8, 3>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF53Planes_helper<pINT32, 3>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF53Planes_helper<pINT64, 3>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF53Planes_helper<pFLOAT, 3>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF53Planes_helper<pDOUBLE, 3>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_4Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF54Planes_helper<pUINT8, 3>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF54Planes_helper<pINT32, 3>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF54Planes_helper<pINT64, 3>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF54Planes_helper<pFLOAT, 3>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF54Planes_helper<pDOUBLE, 3>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported cTYpe {%s}", ctype)));
      }
      break;
    } // case 3!
    case 4:
    {
      switch( ctype )
      {
      case CompoundType::CompoundType_Scalar:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF5_helper<pUINT8, 4>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF5_helper<pINT32, 4>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF5_helper<pINT64, 4>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF5_helper<pFLOAT, 4>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF5_helper<pDOUBLE, 4>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_3Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF53Planes_helper<pUINT8, 4>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF53Planes_helper<pINT32, 4>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF53Planes_helper<pINT64, 4>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF53Planes_helper<pFLOAT, 4>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF53Planes_helper<pDOUBLE, 4>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_4Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::StoreWithHDF54Planes_helper<pUINT8, 4>(iimage, path, image_name); break;

        case PType::PType_GrayINT32: details::StoreWithHDF54Planes_helper<pINT32, 4>(iimage, path, image_name); break;

        case PType::PType_GrayINT64: details::StoreWithHDF54Planes_helper<pINT64, 4>(iimage, path, image_name); break;
        case PType::PType_F32: details::StoreWithHDF54Planes_helper<pFLOAT, 4>(iimage, path, image_name); break;
        case PType::PType_D64: details::StoreWithHDF54Planes_helper<pDOUBLE, 4>(iimage, path, image_name); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported cTYpe {%s}", ctype)));
      }
      break;
    } // case 4!
    default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported number of dims {%d}", coords.size())));
    } // dims
  }

  std::unique_ptr<IInterface>
  LoadFromHDF5(const std::string &filename,                        // NOLINT(misc-unused-parameters)
               const std::string &image_name /* = poutre_img_1 */) // NOLINT(misc-unused-parameters)
  {
    POUTRE_ENTERING("LoadFromHDF5");
    /*
     * Turn off the auto-printing when failure occurs so that we can
     * handle the errors appropriately
     */
    H5::Exception::dontPrint();

    PType        ptype = PType::PType_Undef;
    CompoundType ctype = CompoundType::CompoundType_Undef;
    auto         res   = std::unique_ptr<IInterface>();

    H5::H5File  file(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    H5::DataSet dataset = file.openDataSet(image_name.c_str());

    // Handle sizes
    H5::DataSpace        dataspace = dataset.getSpace();
    int                  rank      = dataspace.getSimpleExtentNdims();
    std::vector<hsize_t> dims_out(rank);
    int                  ndims = dataspace.getSimpleExtentDims(&dims_out[0], NULL);

    // by hand attribute
    // get compoundtype
    H5std_string  attrTypeCompound;
    H5::Attribute resAttr = dataset.openAttribute("IMAGE_COMP_TYPE");
    H5::StrType   stype   = resAttr.getStrType();
    resAttr.read(stype, attrTypeCompound);
    if( attrTypeCompound == details::IMAGEScalar )
      ctype = CompoundType::CompoundType_Scalar;
    else if( attrTypeCompound == details::IMAGE3Planes )
      ctype = CompoundType::CompoundType_3Planes;
    else if( attrTypeCompound == details::IMAGE4Planes )
      ctype = CompoundType::CompoundType_4Planes;
    else
    {
      POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported compound type {%s}", attrTypeCompound)));
    }
    // get ptype
    H5std_string attrpType;
    resAttr = dataset.openAttribute("IMAGE_P_TYPE");
    stype   = resAttr.getStrType();
    resAttr.read(stype, attrpType);
    if( attrpType == details::PUINT8 )
      ptype = PType::PType_GrayUINT8;
    else if( attrpType == details::PINT32 )
      ptype = PType::PType_GrayINT32;
    else if( attrpType == details::PINT64 )
      ptype = PType::PType_GrayINT64;
    else if( attrpType == details::PFLOAT )
      ptype = PType::PType_F32;
    else if( attrpType == details::PDOUBLE )
      ptype = PType::PType_D64;
    else
    {
      POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported type {%s}", attrpType)));
    }

    std::vector<size_t> coords(dims_out.size());
    std::copy(dims_out.begin(), dims_out.end(), coords.begin());
    res = CreateDense(coords, ctype, ptype);

    switch( coords.size() )
    {
    case 1:
    {
      switch( ctype )
      {
      case CompoundType::CompoundType_Scalar:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF5_helper<pUINT8, 1>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF5_helper<pINT32, 1>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF5_helper<pINT64, 1>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF5_helper<pFLOAT, 1>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF5_helper<pDOUBLE, 1>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_3Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF53Planes_helper<pUINT8, 1>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF53Planes_helper<pINT32, 1>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF53Planes_helper<pINT64, 1>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF53Planes_helper<pFLOAT, 1>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF53Planes_helper<pDOUBLE, 1>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype))); break;
        }
        break;
      case CompoundType::CompoundType_4Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF54Planes_helper<pUINT8, 1>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF54Planes_helper<pINT32, 1>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF54Planes_helper<pINT64, 1>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF54Planes_helper<pFLOAT, 1>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF54Planes_helper<pDOUBLE, 1>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported cTYpe {%s}", ctype)));
      }
    } // case 1
    break;
    case 2:
    {
      switch( ctype )
      {
      case CompoundType::CompoundType_Scalar:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF5_helper<pUINT8, 2>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF5_helper<pINT32, 2>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF5_helper<pINT64, 2>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF5_helper<pFLOAT, 2>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF5_helper<pDOUBLE, 2>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      case CompoundType::CompoundType_3Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF53Planes_helper<pUINT8, 2>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF53Planes_helper<pINT32, 2>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF53Planes_helper<pINT64, 2>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF53Planes_helper<pFLOAT, 2>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF53Planes_helper<pDOUBLE, 2>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      case CompoundType::CompoundType_4Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF54Planes_helper<pUINT8, 2>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF54Planes_helper<pINT32, 2>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF54Planes_helper<pINT64, 2>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF54Planes_helper<pFLOAT, 2>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF54Planes_helper<pDOUBLE, 2>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break; // ptype

      default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported cTYpe {%s}", ctype)));
      }
      break;
    } // case 2!
    case 3:
    {
      switch( ctype )
      {
      case CompoundType::CompoundType_Scalar:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF5_helper<pUINT8, 3>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF5_helper<pINT32, 3>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF5_helper<pINT64, 3>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF5_helper<pFLOAT, 3>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF5_helper<pDOUBLE, 3>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      case CompoundType::CompoundType_3Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF53Planes_helper<pUINT8, 3>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF53Planes_helper<pINT32, 3>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF53Planes_helper<pINT64, 3>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF53Planes_helper<pFLOAT, 3>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF53Planes_helper<pDOUBLE, 3>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      case CompoundType::CompoundType_4Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF54Planes_helper<pUINT8, 3>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF54Planes_helper<pINT32, 3>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF54Planes_helper<pINT64, 3>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF54Planes_helper<pFLOAT, 3>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF54Planes_helper<pDOUBLE, 3>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break; // ptype

      default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported cTYpe {%s}", ctype)));
      }
      break;
    } // case 3!
    case 4:
    {
      switch( ctype )
      {
      case CompoundType::CompoundType_Scalar:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF5_helper<pUINT8, 4>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF5_helper<pINT32, 4>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF5_helper<pINT64, 4>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF5_helper<pFLOAT, 4>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF5_helper<pDOUBLE, 4>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      case CompoundType::CompoundType_3Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF53Planes_helper<pUINT8, 4>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF53Planes_helper<pINT32, 4>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF53Planes_helper<pINT64, 4>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF53Planes_helper<pFLOAT, 4>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF53Planes_helper<pDOUBLE, 4>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break;
      case CompoundType::CompoundType_4Planes:
        switch( ptype )
        {
        case PType::PType_GrayUINT8: details::LoadFromHDF54Planes_helper<pUINT8, 4>(*res, dataset); break;

        case PType::PType_GrayINT32: details::LoadFromHDF54Planes_helper<pINT32, 4>(*res, dataset); break;

        case PType::PType_GrayINT64: details::LoadFromHDF54Planes_helper<pINT64, 4>(*res, dataset); break;
        case PType::PType_F32: details::LoadFromHDF54Planes_helper<pFLOAT, 4>(*res, dataset); break;
        case PType::PType_D64: details::LoadFromHDF54Planes_helper<pDOUBLE, 4>(*res, dataset); break;
        default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported pTYpe {%s}", ptype)));
        }
        break; // ptype

      default: POUTRE_RUNTIME_ERROR((poutre::format("LoadFromHDF5: unsupported cTYpe {%s}", ctype)));
      }
      break;
    } // case 4!
    default: POUTRE_RUNTIME_ERROR((poutre::format("StoreWithHDF5: unsupported number of dims {}", coords.size())));
    } // dims

    return res;
  }

} // namespace poutre
