
//==============================================================================
//                  Copyright (c) 2020 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
 * File:   poutreHDF5.hpp
 * Author: thomas retornaz
 *
 * Created on 11 mai 2020
 */
#ifndef POUTREOIIO_HPP
#define POUTREOIIO_HPP

#include <memory>
#include <string>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#endif

#include <H5pubconf.h>
#include <H5public.h>
#include <hdf5_hl.h>

#include <boost/filesystem.hpp>

#include <memory>
#include <string>

namespace poutre
{
    /**
     * @addtogroup image_processing_io_group_details Image Processing IO API details
     * @ingroup image_processing_io_group
     *@{
     */
    namespace details
    {
        namespace bf = boost::filesystem;

        // stolen from yayi

        hid_t pTypeToHDF5ScalarType(const PType &p)
        {
            switch (p)
            {
            case PType::PType_GrayUINT8:
                return H5T_NATIVE_UCHAR;
            case PType::PType_GrayINT32:
                return H5T_NATIVE_INT32;
            case PType::PType_F32:
                return H5T_NATIVE_FLOAT;
            case PType::PType_D64:
                return H5T_NATIVE_DOUBLE;
            case PType::PType_GrayINT64:
                return H5T_NATIVE_LLONG;
            default:
                POUTRE_RUNTIME_ERROR((boost::format("pTypeToHDF5Tyoe: provided type %s unsupported") %
                                      boost::lexical_cast<std::string>(p))
                                         .str());
            }
            return -1;
        }

        PType HDF5ScalarTypeToPType(const hid_t &t)
        {
            if (H5Tequal(t, H5T_NATIVE_UCHAR) == 0)
                return PType::PType_GrayUINT8;
            if (H5Tequal(t, H5T_NATIVE_INT32) == 0)
                return PType::PType_GrayINT32;
            if (H5Tequal(t, H5T_NATIVE_FLOAT) == 0)
                return PType::PType_F32;
            if (H5Tequal(t, H5T_NATIVE_DOUBLE) == 0)
                return PType::PType_D64;
            if (H5Tequal(t, H5T_NATIVE_LLONG) == 0)
                return PType::PType_GrayINT64;
            POUTRE_RUNTIME_ERROR(
                (boost::format("pTypeToHDF5Tyoe: provided type %s unsupported") % boost::lexical_cast<std::string>(t))
                    .str());
            return PType::PType_Undef;
        }

        hid_t TypeToHDF5Type(const PType &p, const CompoundType &c)
        {
            hid_t scalar = pTypeToHDF5ScalarType(p); // throw

            herr_t status = H5Tset_order(scalar, H5T_ORDER_LE); // do something with this order
            if (status < 0)
            {
                H5Tclose(scalar);
                POUTRE_RUNTIME_ERROR((boost::format("TypeToHDF5Type: provided ptype %s unsupported") %
                                      boost::lexical_cast<std::string>(p))
                                         .str());
            }
            switch (c)
            {
            case CompoundType::CompoundType_Scalar:
                return scalar;
            case CompoundType::CompoundType_3Planes: {
                static const unsigned long long adims[] = {1, 1, 1};
                return H5Tarray_create2(scalar, 3, adims);
            }
            case CompoundType::CompoundType_4Planes: {
                static const unsigned long long adims[] = {1, 1, 1, 1};
                return H5Tarray_create2(scalar, 4, adims);
            }
            default:
                H5Tclose(scalar);
                POUTRE_RUNTIME_ERROR((boost::format("TypeToHDF5Type: provided ctype %s unsupported") %
                                      boost::lexical_cast<std::string>(c))
                                         .str());
            }
            return -1;
        }

        std::pair<CompoundType, PType> HDF5TypeToType(const hid_t &t)
        {
            PType resp = PType::PType_Undef;
            CompoundType resc = CompoundType::CompoundType_Undef;

            hid_t order = H5Tget_order(t);
            if (order != H5T_ORDER_LE)
            {
                POUTRE_RUNTIME_ERROR((boost::format("HDF5TypeToType: little endian support only not %s") %
                                      boost::lexical_cast<std::string>(order))
                                         .str());
            }

            resp = HDF5ScalarTypeToPType(t);

            hid_t scalar_hid = H5Tget_class(t);
            if (scalar_hid == H5T_INTEGER || scalar_hid == H5T_FLOAT)
            {
                return std::make_pair(CompoundType::CompoundType_Scalar, resp);
            }

            // dimensions
            int dims_data = H5Tget_array_ndims(t);
            if (dims_data < 1 || dims_data > 4)
                POUTRE_RUNTIME_ERROR(
                    (boost::format("HDF5TypeToType: unsupported number of data dimensions: %d") % dims_data).str());

            // size of dims
            auto s = std::vector<hsize_t>(dims_data);
            auto ss = s.data();
            if (H5Tget_array_dims2(t, ss) < 0)
            {
                POUTRE_RUNTIME_ERROR((boost::format("HDF5TypeToType: error occured in H5Tget_array_dims2")).str());
            }

            for (int i = 0; i < dims_data; i++)
            {
                if (ss[i] != 1)
                {
                    POUTRE_RUNTIME_ERROR((boost::format("HDF5TypeToType: dimention has not size of 1") % i).str());
                }
            }

            if (dims_data == 1)
                return std::make_pair(CompoundType::CompoundType_Scalar, resp);
            else if (dims_data == 3)
                return std::make_pair(CompoundType::CompoundType_3Planes, resp);
            else if (dims_data == 4)
                return std::make_pair(CompoundType::CompoundType_4Planes, resp);
            else
            {
                POUTRE_RUNTIME_ERROR(
                    (boost::format("HDF5TypeToType: unsupported number of dims %d") % dims_data).str());
            }
            return std::make_pair(resc, resp);
        }

        hsize_t *CoordToHDF5Dim(const std::vector<std::size_t> &s)
        {
            hsize_t *res = new hsize_t[s.size()];
            for (size_t i = 0; i < s.size(); ++i)
                res[i] = s[s.size() - i - (size_t)1];
            return res;
        }

        std::vector<std::size_t> HDF5DimToCoord(hid_t data_space_id)
        {
            int rank = H5Sget_simple_extent_ndims(data_space_id);
            std::vector<hsize_t> s(rank);
            std::vector<hsize_t> ms(rank);

            hsize_t *ss = s.data();
            hsize_t *mss = ms.data();
            if (H5Sget_simple_extent_dims(data_space_id, ss, mss) < 0)
            {
                return std::vector<std::size_t>();
            }

            std::vector<std::size_t> res(rank);

            for (size_t i = 0; i < rank; i++)
                res[i] = ss[(size_t)rank - i - 1];
            return res;
        }

        template <class Image> void StoreWithHDF5_helper(IInterface &iimage, hid_t &data_id, hid_t &data_type)
        {
            const Image *im_t = dynamic_cast<const Image *>(iimage);
            if (!im_t)
            {
                POUTRE_RUNTIME_ERROR("Dynamic cast fail");
            }
            herr_t status = H5Dwrite(data_id, data_type, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                                     reinterpret_cast<const void *>(&im_t->data()));
            if (!status)
            {
                POUTRE_RUNTIME_ERROR((boost::format("StoreWithHDF5_helper: H5Dwrite fail")).str());
            }
            return;
        }

    } // namespace details

    //! @} doxygroup: image_processing_io_group_details

} // namespace poutre

#endif // POUTREOIIO_HPP