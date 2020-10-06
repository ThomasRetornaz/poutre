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

#include <boost/filesystem.hpp>
#include <poutreBase/poutreTrace.hpp>
#include <poutreIO/include/poutreOIIO.hpp>

namespace poutre
{
    namespace bf = boost::filesystem;
    /**
     * Load an image from file at filename.
     * Storage format is deduced from file.
     *
     * @param image_path The path of the file to load
     *
     * @return std::unique_ptr<IInterface> corresponding to the loaded image
     */
    std::unique_ptr<IInterface> LoadFromOIIO(const std::string &image_path)
    {
        bf::path localPath(image_path);
        if (!(bf::exists(localPath)))
        {
            POUTRE_RUNTIME_ERROR(boost::format("LoadFromOIIO: unable to locate provided path " + localPath.string()));
        }

        auto in(OIIO::ImageInput::open(image_path));
        if (!in)
        {
            std::ostringstream errorstream;
            errorstream << " load_image(): Error reading image '";
            errorstream << image_path;
            POUTRE_RUNTIME_ERROR(errorstream.str());
        }

        const OIIO::ImageSpec &spec = in->spec();
        std::vector<std::size_t> dims;
        if (spec.height > 1)
            dims.push_back((size_t)spec.height);
        if (spec.width > 1)
            dims.push_back((size_t)spec.width);
        if (spec.depth > 1)
            POUTRE_RUNTIME_ERROR(
                boost::format("LoadFromOIIO: nD with n >2  format require hdf5 ext, use LoadFromHDF5"));
        if (spec.channelformats.size() != 0)
            POUTRE_RUNTIME_ERROR(boost::format("LoadFromOIIO: different format over channels is not supported"));
        CompoundType ctype = CompoundType::CompoundType_Undef;
        switch (spec.nchannels)
        {
        case 1:
            ctype = CompoundType::CompoundType_Scalar;
            break;
        case 3:
            ctype = CompoundType::CompoundType_3Planes;
            break;
        case 4:
            ctype = CompoundType::CompoundType_4Planes;
            break;
        default:
            std::ostringstream errorstream;
            errorstream << " load_image(): Error reading image '";
            errorstream << image_path;
            errorstream << " unsupported number of channels ";
            errorstream << spec.nchannels;
            errorstream << " see desc \n" << spec.to_xml();
            POUTRE_RUNTIME_ERROR(errorstream.str());
        };
        PType ptype = PType::PType_Undef;
        switch (spec.format.basetype)
        {
        case OpenImageIO_v2_2::TypeDesc::BASETYPE::UINT8:
            ptype = PType::PType_GrayUINT8;
            break;
        case OpenImageIO_v2_2::TypeDesc::BASETYPE::INT32:
            ptype = PType::PType_GrayINT32;
            break;
        case OpenImageIO_v2_2::TypeDesc::BASETYPE::FLOAT:
            ptype = PType::PType_F32;
            break;
        case OpenImageIO_v2_2::TypeDesc::BASETYPE::INT64:
            ptype = PType::PType_GrayINT64;
            break;
        case OpenImageIO_v2_2::TypeDesc::BASETYPE::DOUBLE:
            ptype = PType::PType_D64;
            break;
        default:
            std::ostringstream errorstream;
            errorstream << " load_image(): Error reading image '";
            errorstream << image_path;
            errorstream << " unsupported type ";
            errorstream << spec.format.basetype;
            errorstream << " see desc \n" << spec.to_xml();
            POUTRE_RUNTIME_ERROR(errorstream.str());
        };
        auto iimage = CreateDense(dims, ctype, ptype);
        switch (ptype)
        {
        case PType::PType_GrayUINT8: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pUINT8, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOScalar(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pUINT8, 3>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound3(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pUINT8, 4>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound4(*in, *img_t);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << image_path;
                errorstream << " unsupported number of channels ";
                errorstream << spec.nchannels;
                errorstream << " see desc \n" << spec.to_xml();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;
#ifdef _WIN32
            //#FIXME translate to long under minux which no supported by OIIO

        case PType::PType_GrayINT32: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pINT32, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOScalar(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pINT32, 3>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound3(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pINT32, 4>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound4(*in, *img_t);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << image_path;
                errorstream << " unsupported number of channels ";
                errorstream << spec.nchannels;
                errorstream << " see desc \n" << spec.to_xml();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;
#endif
        case PType::PType_F32: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pFLOAT, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOScalar(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pFLOAT, 3>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound3(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pFLOAT, 4>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound4(*in, *img_t);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << image_path;
                errorstream << " unsupported number of channels ";
                errorstream << spec.nchannels;
                errorstream << " see desc \n" << spec.to_xml();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;
#ifdef _WIN32
        case PType::PType_GrayINT64: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pINT64, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOScalar(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pINT64, 3>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound3(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pINT64, 4>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound4(*in, *img_t);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << image_path;
                errorstream << " unsupported number of channels ";
                errorstream << spec.nchannels;
                errorstream << " see desc \n" << spec.to_xml();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;
#endif
        case PType::PType_D64: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pDOUBLE, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOScalar(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pDOUBLE, 3>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound3(*in, *img_t);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pDOUBLE, 4>, 2>;
                ImageType_t *img_t = dynamic_cast<ImageType_t *>(iimage.get());
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::FillImageFromOIIOCompound4(*in, *img_t);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << image_path;
                errorstream << " unsupported number of channels ";
                errorstream << spec.nchannels;
                errorstream << " see desc \n" << spec.to_xml();
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;

        default:
            std::ostringstream errorstream;
            errorstream << " load_image(): Error reading image '";
            errorstream << image_path;
            errorstream << " unsupported type ";
            errorstream << spec.format.basetype;
            errorstream << " see desc \n" << spec.to_xml();
            POUTRE_RUNTIME_ERROR(errorstream.str());
        };
        // now fill it
        in->close();

        return iimage;
    }

    void StoreWithOIIO(const std::string &path, const IInterface &iimage)
    {
        const details::StoreWithOIIOOptions options;
        bf::path localPath(path);
        boost::filesystem::path dir = localPath.parent_path();
        if (!(bf::exists(dir)))
        {
            POUTRE_RUNTIME_ERROR(boost::format("StoreFromOIIO: provided path %s doesn't exists") % dir);
        }
        if (iimage.GetNumDims() > 2)
            POUTRE_RUNTIME_ERROR(boost::format("For nd images with n>2 use StoreWithHDF5"));

        auto ptype = iimage.GetPType();
        auto ctype = iimage.GetCType();
        switch (ptype)
        {
        case PType::PType_GrayUINT8: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pUINT8, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOScalar(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pUINT8, 3>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound3(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pUINT8, 4>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound4(*img_t, path, options);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << path;
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;
#ifdef _WIN32
        case PType::PType_GrayINT32: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pINT32, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOScalar(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pINT32, 3>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound3(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pINT32, 4>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound4(*img_t, path, options);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << path;

                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;
#endif

        case PType::PType_F32: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pFLOAT, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOScalar(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pFLOAT, 3>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound3(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pFLOAT, 4>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound4(*img_t, path, options);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << path;

                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;
#ifdef _WIN32
        case PType::PType_GrayINT64: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pINT64, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOScalar(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pINT64, 3>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound3(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pINT64, 4>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound4(*img_t, path, options);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << path;
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;
#endif
        case PType::PType_D64: {
            switch (ctype)
            {
            case CompoundType::CompoundType_Scalar: {
                using ImageType_t = poutre::DenseImage<poutre::pDOUBLE, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOScalar(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_3Planes: {
                using ImageType_t = poutre::DenseImage<compound<pDOUBLE, 3>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound3(*img_t, path, options);
            }
            break;
            case CompoundType::CompoundType_4Planes: {
                using ImageType_t = poutre::DenseImage<compound<pDOUBLE, 4>, 2>;
                const ImageType_t *img_t = dynamic_cast<const ImageType_t *>(&iimage);
                if (!img_t)
                {
                    POUTRE_RUNTIME_ERROR("Dynamic cast fail");
                }
                details::StoreWithOIIOCompound4(*img_t, path, options);
            }
            break;
            default:
                std::ostringstream errorstream;
                errorstream << " load_image(): Error reading image '";
                errorstream << path;
                POUTRE_RUNTIME_ERROR(errorstream.str());
            };
        }
        break;

        default:
            std::ostringstream errorstream;
            errorstream << " load_image(): Error reading image '";
            errorstream << path;
            POUTRE_RUNTIME_ERROR(errorstream.str());
        };
    }

} // namespace poutre
