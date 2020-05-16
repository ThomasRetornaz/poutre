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

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>

#include <poutreBase/poutreTrace.hpp>
#include <poutreIO/include/poutreHDF5.hpp>

namespace poutre
{
    void StoreWithHDF5(const std::string &path, const IInterface &iimage,
                       const std::string &image_name /*= "poutre_img_1"*/)
    {
        auto ptype = iimage.GetPType();
        auto ctype = iimage.GetCType();
        hid_t data_type_id = details::TypeToHDF5Type(ptype, ctype);

        // create a HDF5
        hid_t file_id = H5Fcreate(path.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        if (file_id < 0)
        {
            POUTRE_RUNTIME_ERROR((boost::format("StoreWithHDF5: H5Fcreate failed")).str());
        }

        // create an array with imgae coords
        const auto coords = iimage.GetCoords();
        hsize_t *dims = details::CoordToHDF5Dim(coords);
        hid_t data_space_id = H5Screate_simple((int)coords.size(), dims, NULL);
        delete[] dims;
        if (data_space_id < 0)
        {
            H5Sclose(data_type_id);
            H5Fclose(file_id);
            POUTRE_RUNTIME_ERROR((boost::format("StoreWithHDF5: H5Screate_simple failed")).str());
        }

        hid_t dataset_id =
            H5Dcreate2(file_id, image_name.c_str(), data_type_id, data_space_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        if (dataset_id < 0)
        {
            POUTRE_RUNTIME_ERROR((boost::format("StoreWithHDF5: H5Dcreate2 failed")).str());
        }
        switch (coords.size())
        {
        case 1: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<pUINT8, 1>>()(iimage, dataset_id, data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<pINT32, 1>>()(iimage, dataset_id, data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<pINT64, 1>>()(iimage, dataset_id, data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<pFLOAT, 1>>()(iimage, dataset_id, data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<pDOUBLE, 1>>()(iimage, dataset_id, data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                    break;
                }
                break;
            case CompoundType::CompoundType_3Planes:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 3>, 1>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT32, 3>, 1>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT64, 3>, 1>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 3>, 1>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 3>, 1>>()(iimage, dataset_id,
                                                                                                  data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                    break;
                }
                break;
            case CompoundType::CompoundType_4Planes:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 1>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 1>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 1>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 1>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 1>>()(iimage, dataset_id,
                                                                                                  data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break;
            default:
                POUTRE_RUNTIME_ERROR(
                    (boost::format("StoreWithHDF5: unsupported cTYpe %s") % boost::lexical_cast<std::string>(ctype))
                        .str());
            }
        } // case 1
        break;
        case 2: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<pUINT8, 2>>()(iimage, dataset_id, data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<pINT32, 2>>()(iimage, dataset_id, data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<pINT64, 2>>()(iimage, dataset_id, data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<pFLOAT, 2>>()(iimage, dataset_id, data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<pDOUBLE, 2>>()(iimage, dataset_id, data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break;
            case CompoundType::CompoundType_3Planes:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 3>, 2>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT32, 3>, 2>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT64, 3>, 2>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 3>, 2>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 3>, 2>>()(iimage, dataset_id,
                                                                                                  data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break;
            case CompoundType::CompoundType_4Planes:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 2>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 2>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 2>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 2>>()(iimage, dataset_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 2>>()(iimage, dataset_id,
                                                                                                  data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break; // ptype

            default:
                POUTRE_RUNTIME_ERROR(
                    (boost::format("StoreWithHDF5: unsupported cTYpe %s") % boost::lexical_cast<std::string>(ctype))
                        .str());
            }
            break;
        } // case 2!
        case 3: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<pUINT8, 3>>()(iimage, data_space_id, data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<pINT32, 3>>()(iimage, data_space_id, data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<pINT64, 3>>()(iimage, data_space_id, data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<pFLOAT, 3>>()(iimage, data_space_id, data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<pDOUBLE, 3>>()(iimage, data_space_id, data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break;
            case CompoundType::CompoundType_3Planes:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 3>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT32, 3>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT64, 3>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 3>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 3>, 3>>()(iimage, data_space_id,
                                                                                                  data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break;
            case CompoundType::CompoundType_4Planes:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 3>>()(iimage, data_space_id,
                                                                                                  data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break; // ptype

            default:
                POUTRE_RUNTIME_ERROR(
                    (boost::format("StoreWithHDF5: unsupported cTYpe %s") % boost::lexical_cast<std::string>(ctype))
                        .str());
            }
            break;
        } // case 3!
        case 4: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<pUINT8, 4>>()(iimage, data_space_id, data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<pINT32, 4>>()(iimage, data_space_id, data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<pINT64, 4>>()(iimage, data_space_id, data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<pFLOAT, 4>>()(iimage, data_space_id, data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<pDOUBLE, 4>>()(iimage, data_space_id, data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break;
            case CompoundType::CompoundType_3Planes:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 3>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 3>>()(iimage, data_space_id,
                                                                                                  data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break;
            case CompoundType::CompoundType_4Planes:
                switch (ptype)
                {
                case PType::PType_GrayUINT8:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 4>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 4>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;

                case PType::PType_GrayINT64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 4>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_F32:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 4>>()(iimage, data_space_id,
                                                                                                 data_type_id);
                    break;
                case PType::PType_D64:
                    details::StoreWithHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 4>>()(iimage, data_space_id,
                                                                                                  data_type_id);
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("StoreWithHDF5: unsupported pTYpe %s") % boost::lexical_cast<std::string>(ptype))
                            .str());
                }
                break; // ptype

            default:
                POUTRE_RUNTIME_ERROR(
                    (boost::format("StoreWithHDF5: unsupported cTYpe %s") % boost::lexical_cast<std::string>(ctype))
                        .str());
            }
            break;
        } // case 4!
        default:
            POUTRE_RUNTIME_ERROR((boost::format("StoreWithHDF5: unsupported number of dims %d") % coords.size()).str());
        } // dims
        H5Tclose(data_type_id);
        H5Dclose(dataset_id);
        H5Sclose(data_space_id);
        H5Fclose(file_id);
    }

    std::unique_ptr<IInterface> LoadFromHDF5(const std::string &filename,
                                             const std::string &image_name /* = poutre_img_1 */)
    {
        auto res = std::unique_ptr<IInterface>();
        hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
        if (file_id < 0)
        {
            POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: H5Fopen fail")).str());
        }

        hid_t dataset_id = H5Dopen2(file_id, image_name.c_str(), H5P_DEFAULT);
        if (dataset_id < 0)
        {
            H5Fclose(file_id);
            POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: H5Dopen2 fail")).str());
        }

        // datatype identifier
        hid_t data_type_id = H5Dget_type(dataset_id);

        // dataspace identifier
        hid_t data_space_id = H5Dget_space(dataset_id);
        try
        {
            // map
            auto types = details::HDF5TypeToType(data_type_id);

            // dimensions of the array
            int dim = H5Sget_simple_extent_ndims(data_space_id);

            const auto &coords = details::HDF5DimToCoord(data_space_id);

            auto ctype = types.first;
            auto ptype = types.second;
            res = CreateDense(coords, ctype, ptype);

            switch (coords.size())
            {
            case 1: {
                switch (ctype)
                {
                case CompoundType::CompoundType_Scalar:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<pUINT8, 1>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<pINT32, 1>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<pINT64, 1>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<pFLOAT, 1>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<pDOUBLE, 1>>()(*res, dataset_id, data_type_id,
                                                                                  data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                        break;
                    }
                    break;
                case CompoundType::CompoundType_3Planes:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 3>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT32, 3>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT64, 3>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 3>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 3>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                        break;
                    }
                    break;
                case CompoundType::CompoundType_4Planes:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 1>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break;
                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("LoadFromHDF5: unsupported cTYpe %s") % boost::lexical_cast<std::string>(ctype))
                            .str());
                }
            } // case 1
            break;
            case 2: {
                switch (ctype)
                {
                case CompoundType::CompoundType_Scalar:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<pUINT8, 2>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<pINT32, 2>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<pINT64, 2>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<pFLOAT, 2>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<pDOUBLE, 2>>()(*res, dataset_id, data_type_id,
                                                                                  data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break;
                case CompoundType::CompoundType_3Planes:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 3>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT32, 3>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT64, 3>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 3>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 3>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break;
                case CompoundType::CompoundType_4Planes:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 2>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break; // ptype

                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("LoadFromHDF5: unsupported cTYpe %s") % boost::lexical_cast<std::string>(ctype))
                            .str());
                }
                break;
            } // case 2!
            case 3: {
                switch (ctype)
                {
                case CompoundType::CompoundType_Scalar:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<pUINT8, 3>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<pINT32, 3>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<pINT64, 3>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<pFLOAT, 3>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<pDOUBLE, 3>>()(*res, dataset_id, data_type_id,
                                                                                  data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("StoreWithHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break;
                case CompoundType::CompoundType_3Planes:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 3>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT32, 3>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT64, 3>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 3>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 3>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break;
                case CompoundType::CompoundType_4Planes:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break; // ptype

                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("LoadFromHDF5: unsupported cTYpe %s") % boost::lexical_cast<std::string>(ctype))
                            .str());
                }
                break;
            } // case 3!
            case 4: {
                switch (ctype)
                {
                case CompoundType::CompoundType_Scalar:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<pUINT8, 4>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<pINT32, 4>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<pINT64, 4>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<pFLOAT, 4>>()(*res, dataset_id, data_type_id,
                                                                                 data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<pDOUBLE, 4>>()(*res, dataset_id, data_type_id,
                                                                                  data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("StoreWithHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break;
                case CompoundType::CompoundType_3Planes:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pDOUBLE, 4>, 3>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break;
                case CompoundType::CompoundType_4Planes:
                    switch (ptype)
                    {
                    case PType::PType_GrayUINT8:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pUINT8, 4>, 4>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT32, 4>, 4>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;

                    case PType::PType_GrayINT64:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pINT64, 4>, 4>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_F32:
                        details::LoadFromHDF5_helper_op<DenseImage<compound_pixel<pFLOAT, 4>, 4>>()(
                            *res, dataset_id, data_type_id, data_space_id);
                        break;
                    case PType::PType_D64:
                        details::LoadFromHDF5_helper_op<DenseImage<pDOUBLE, 4>>()(*res, dataset_id, data_type_id,
                                                                                  data_space_id);
                        break;
                    default:
                        POUTRE_RUNTIME_ERROR((boost::format("LoadFromHDF5: unsupported pTYpe %s") %
                                              boost::lexical_cast<std::string>(ptype))
                                                 .str());
                    }
                    break; // ptype

                default:
                    POUTRE_RUNTIME_ERROR(
                        (boost::format("LoadFromHDF5: unsupported cTYpe %s") % boost::lexical_cast<std::string>(ctype))
                            .str());
                }
                break;
            } // case 4!
            default:
                POUTRE_RUNTIME_ERROR(
                    (boost::format("StoreWithHDF5: unsupported number of dims %d") % coords.size()).str());
            } // dims
        }
        catch (const std::exception &e)
        {
            H5Sclose(data_space_id);
            H5Tclose(data_type_id);
            H5Dclose(dataset_id);
            H5Fclose(file_id);
            throw e;
            return res;
        }
        H5Sclose(data_space_id);
        H5Tclose(data_type_id);
        H5Dclose(dataset_id);
        H5Fclose(file_id);
        return res;
    }

} // namespace poutre
