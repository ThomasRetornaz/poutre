// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>

#include <poutreBase/poutreTrace.hpp>
#include <poutreIO/include/poutreIOPNG.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>

#include <png.h> /* libpng header; includes zlib.h */

// http://www.boost.org/doc/libs/1_61_0/libs/filesystem/doc/reference.html
namespace bf = boost::filesystem;

// READ  http://www.libpng.org/pub/png/book/chapter13.html
// WRITE http://www.libpng.org/pub/png/book/chapter15.html
// http://zarb.org/~gc/html/libpng.html
namespace {
    using namespace poutre;

    // template<class image_type>
    // void helper_fill_image(IInterface& img,const png_structp& png_ptr)
    //{
    //    image_type* img_t = dynamic_cast<image_type*>(&img);
    //    if (!img_t) { POUTRE_RUNTIME_ERROR("helper_fill_image: downcast fail"); }

    //}

    std::unique_ptr<IInterface> loappngHelper(const bf::path &i_path) {
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
        auto path = i_path.string();

        FILE *infile = fopen(path.c_str(), "rb");
        if (infile == 0) {
            POUTRE_RUNTIME_ERROR(
                (boost::format("loappngHelper::unable to open image file %s") % i_path)
                .str());
        }

        png_byte sig[8];

        /* first do a quick check that the file really is a PNG image; could have used
         * slightly more general png_sig_cmp() function instead */
        fread(sig, 1, 8, infile);
        if (!png_sig_cmp(sig, 0,8)) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            POUTRE_RUNTIME_ERROR(
                (boost::format(
                    "loappngHelper::it seems that %s is not a valid  png image file") %
                    i_path)
                .str());
        }

        /* create utilities struct */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            POUTRE_RUNTIME_ERROR(
                "loappngHelper::unable to allocate memory for png struct");
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            POUTRE_RUNTIME_ERROR(
                "loappngHelper::unable to allocate memory for png info struct");
        }

        /* setjmp() must be called in every function that calls a PNG-reading  libpng
         * function */
        if (setjmp(png_jmpbuf(png_ptr))) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            POUTRE_RUNTIME_ERROR("loappngHelper:setjmp fail");
        }

        png_init_io(png_ptr, infile);
        png_set_sig_bytes(png_ptr, 8);    /* we already read the 8 signature bytes */
        png_read_info(png_ptr, info_ptr); /* read all PNG info up to image data */

        png_uint_32 width, height;
        int bit_depth, color_type;
        int interlace_method, compression_method, filter_method;
        png_byte channels;
        // png_byte  *image_data = nullptr;

        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
            &interlace_method, &compression_method, &filter_method);
        channels = png_get_channels(png_ptr, info_ptr);

        if (interlace_method != PNG_INTERLACE_NONE) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            POUTRE_RUNTIME_ERROR("loappngHelper:interlace mode not supported yet");
        }

        // Determinize the poutre imagetype
        ImgType imgtype = ImgType::ImgType_Dense;
        std::ptrdiff_t NumDims = 2;
        std::vector<std::size_t> dims(NumDims);
        dims[0] = static_cast<size_t>(width);
        dims[1] = static_cast<size_t>(height);

        CompoundType cptype = CompoundType::CompoundType_Undef;
        PType ptype = PType::PType_Undef;

        if (color_type == PNG_COLOR_TYPE_GRAY) {
            cptype = CompoundType::CompoundType_Scalar;
            if (bit_depth == 16) {
                // cppcheck-suppress doubleFree
                fclose(infile);
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                POUTRE_RUNTIME_ERROR("loappngHelper: bit_depth==16 not suported yet");
            }
            ptype = PType::PType_GrayUINT8;
            if (bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        else if (color_type == PNG_COLOR_TYPE_PALETTE) {
            if (bit_depth == 16) {
                // cppcheck-suppress doubleFree
                fclose(infile);
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                POUTRE_RUNTIME_ERROR("loappngHelper: bit_depth==16 not suported yet");
            }
            png_set_palette_to_rgb(png_ptr);
            cptype = CompoundType::CompoundType_3Planes;
            ptype = PType::PType_GrayUINT8;
        }
        else if (channels == 3) {
            if (bit_depth == 16) {
                // cppcheck-suppress doubleFree
                fclose(infile);
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                POUTRE_RUNTIME_ERROR("loappngHelper: bit_depth==16 not suported yet");
            }
            cptype = CompoundType::CompoundType_3Planes;
            ptype = PType::PType_GrayUINT8;
            png_set_palette_to_rgb(png_ptr);
        }
        else if (channels == 4)  // RGBA //-V112
        {
            if (bit_depth == 16) {
                // cppcheck-suppress doubleFree
                fclose(infile);
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                POUTRE_RUNTIME_ERROR("loappngHelper: bit_depth==16 not suported yet");
            }

            cptype = CompoundType::CompoundType_4Planes;
            ptype = PType::PType_GrayUINT8;
        }
        else {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            POUTRE_RUNTIME_ERROR("loappngHelper: not supported format");
        }

        // requested ?
        int number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);
        if (setjmp(png_jmpbuf(png_ptr))) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            POUTRE_RUNTIME_ERROR("loappngHelper:setjmp fail");
        }

        std::unique_ptr<IInterface> imgout(nullptr);

        // Enough information to create image out
        try {
            imgout = CreateDense(dims, cptype, ptype);
            png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);
            if (row_pointers == NULL) {
                // cppcheck-suppress doubleFree
                fclose(infile);
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                POUTRE_RUNTIME_ERROR("loappngHelper: empty image ?");
            }

            // dispatch
            switch (cptype) {
            case CompoundType::CompoundType_Scalar: {
                auto img_t = dynamic_cast<DenseImage<pUINT8> *>(&(*imgout));
                if (!img_t) {
                    POUTRE_RUNTIME_ERROR("helper_fill_image: downcast fail");
                }

                scoord linenb = 0, column = 0;
                pUINT8 *png_line_pix = nullptr;
                pUINT8 *line_pix = nullptr;

                for (linenb = 0; linenb < (scoord)height; linenb++) {
                    png_line_pix = reinterpret_cast<pUINT8 *>(
                        row_pointers[linenb]);                   // current line png
                    line_pix = img_t->data() + width * linenb;  // current line image
                    memcpy(line_pix, png_line_pix, width * sizeof(pUINT8));
                }

            } break;
            case CompoundType::CompoundType_3Planes: {
                auto img_t =
                    dynamic_cast<DenseImage<compound_pixel<pUINT8, 3>> *>(&(*imgout));
                if (!img_t) {
                    POUTRE_RUNTIME_ERROR("helper_fill_image: downcast fail");
                }

                scoord linenb = 0, column = 0;
                pUINT8 *png_line_pix = nullptr;
                compound_pixel<pUINT8, 3> *line_pix = nullptr;

                for (linenb = 0; linenb < (scoord)height; linenb++) {
                    png_line_pix = reinterpret_cast<pUINT8 *>(
                        row_pointers[linenb]);                   // current line png
                    line_pix = img_t->data() + width * linenb;  // current line image
                    for (column = 0; column < (scoord)width; ++column) {
                        line_pix[column][0] = png_line_pix[column * 3];
                        line_pix[column][1] = png_line_pix[column * 3 + 1];
                        line_pix[column][2] = png_line_pix[column * 3 + 2];
                    }
                }

            } break;
            case CompoundType::CompoundType_4Planes: {
                auto img_t =
                    dynamic_cast<DenseImage<compound_pixel<pUINT8, 4>> *>(&(*imgout));
                if (!img_t) {
                    POUTRE_RUNTIME_ERROR("helper_fill_image: downcast fail");
                }

                scoord linenb = 0, column = 0;
                pUINT8 *png_line_pix = nullptr;
                compound_pixel<pUINT8, 4> *line_pix = nullptr;

                for (linenb = 0; linenb < (scoord)height; linenb++) {
                    png_line_pix = reinterpret_cast<pUINT8 *>(
                        row_pointers[linenb]);                   // current line png
                    line_pix = img_t->data() + width * linenb;  // current line image
                    for (column = 0; column < (scoord)width; ++column) {
                        line_pix[column][0] = png_line_pix[column * 4]; //-V112
                        line_pix[column][1] = png_line_pix[column * 4 + 1]; //-V112
                        line_pix[column][2] = png_line_pix[column * 4 + 2]; //-V112
                        line_pix[column][3] = png_line_pix[column * 4 + 3]; //-V112
                    }
                }

            } break;
            default:
                POUTRE_RUNTIME_ERROR(
                    (boost::format("loappngHelper: not supported CompoundType %s") %
                        boost::lexical_cast<std::string>(cptype)));
            }
        }
        catch (...) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            POUTRE_RUNTIME_ERROR("loappngHelper: allocation failure");
        }

        png_read_end(png_ptr, info_ptr);
        // cppcheck-suppress doubleFree
        fclose(infile);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        return imgout;
    }

    void writepngHelper(const bf::path &i_path, const IInterface &i_img) {
        if (i_img.GetImgType() != ImgType::ImgType_Dense)
            POUTRE_RUNTIME_ERROR(
            (boost::format("writepnghelper: only denses images are supported")));

        if (i_img.GetNumDims() != 2)
            POUTRE_RUNTIME_ERROR(
            (boost::format("writepnghelper: can only save 2d images not %dd ") %
                i_img.GetNumDims()));

        if (i_img.GetPType() != PType::PType_GrayUINT8)
            POUTRE_RUNTIME_ERROR(
            (boost::format("writepnghelper: only GrayUINT8 supported not %s ") %
                boost::lexical_cast<std::string>(i_img.GetPType())));

        // prepare future
        if ((i_img.GetCType() != CompoundType::CompoundType_Scalar) &&
            (i_img.GetCType() != CompoundType::CompoundType_3Planes) &&
            (i_img.GetCType() != CompoundType::CompoundType_4Planes))
            POUTRE_RUNTIME_ERROR(
            (boost::format(
                "writepnghelper: only scalar/c3/c4 are supported not %s ") %
                boost::lexical_cast<std::string>(i_img.GetCType())));

        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
        auto path = i_path.string();

        FILE *infile = fopen(path.c_str(), "wb");
        if (infile == 0) {
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper::unable to open image file %s") % i_path)
                .str());
        }

        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper: png_create_write_struct failed"))
                .str());
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_write_struct(&png_ptr, NULL);
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper: png_create_info_struct failed")).str());
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper:Error during init_io")).str());
        }

        png_init_io(png_ptr, infile);

        /* write header */
        if (setjmp(png_jmpbuf(png_ptr))) {
            // cppcheck-suppress doubleFree
            fclose(infile);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper:Error during writing header")).str());
        }

        png_uint_32 width = (png_uint_32)i_img.GetCoords()[0], height = (png_uint_32)i_img.GetCoords()[1];
        int interlace_method = PNG_INTERLACE_NONE,
            compression_method = PNG_COMPRESSION_TYPE_BASE,
            filter_method = PNG_FILTER_TYPE_BASE;
        int bit_depth, color_type;
        // png_byte channels; ????????????????

        // only 8 bits, may 16 bits in the future
        bit_depth = 8;
        switch (i_img.GetCType()) {
        case CompoundType::CompoundType_Scalar: {
            color_type = PNG_COLOR_TYPE_GRAY;
        } break;
        case CompoundType::CompoundType_3Planes: {
            color_type = PNG_COLOR_TYPE_RGB;
        } break;
        case CompoundType::CompoundType_4Planes: {
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
        } break;
        default: {
            fclose(infile);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper:Unsuported Counpound type %s ") %
                    boost::lexical_cast<std::string>(i_img.GetCType()))
                .str());
        }
        }
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
            interlace_method, compression_method, filter_method);

        png_write_info(png_ptr, info_ptr);

        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr))) {
            fclose(infile);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper:Error during writing bytes")).str());
        }

        ///////////////////////////////////////////
        // dispatch
        switch (i_img.GetCType()) {
        case CompoundType::CompoundType_Scalar: {
            auto img_t = dynamic_cast<const DenseImage<pUINT8> *>(&(i_img));
            if (!img_t) {
                POUTRE_RUNTIME_ERROR("helper_fill_image: downcast fail");
            }
        } break;
        case CompoundType::CompoundType_3Planes: {
            auto img_t =
                dynamic_cast<const DenseImage<compound_pixel<pUINT8, 3>> *>(&(i_img));
            if (!img_t) {
                POUTRE_RUNTIME_ERROR("helper_fill_image: downcast fail");
            }
        } break;
        case CompoundType::CompoundType_4Planes: {
            auto img_t =
                dynamic_cast<const DenseImage<compound_pixel<pUINT8, 4>> *>(&(i_img));
            if (!img_t) {
                POUTRE_RUNTIME_ERROR("helper_fill_image: downcast fail");
            }

        } break;
        default:
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper: not supported CompoundType %s") %
                    boost::lexical_cast<std::string>(i_img.GetCType())));
        }



        ///////////////////////////////////////////
        /* end write */
        if (setjmp(png_jmpbuf(png_ptr))) {
            fclose(infile);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            POUTRE_RUNTIME_ERROR(
                (boost::format("writepngHelper:Error during end of write")).str());
        }

        png_write_end(png_ptr, info_ptr);

        fclose(infile);
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}
namespace poutre {
    namespace details {

        std::unique_ptr<IInterface> IOLoadPng(const bf::path &i_path) {
            if (!bf::exists(i_path)) {
                POUTRE_RUNTIME_ERROR(
                    (boost::format("IOLoadPng::unable to load image from path %s") % i_path)
                    .str());
            }

            if (!bf::is_regular_file(i_path)) {
                POUTRE_RUNTIME_ERROR(
                    (boost::format("IOLoadPng::%s is not a regular file") % i_path).str());
            }

            auto extension = bf::extension(i_path);
            boost::algorithm::to_lower(extension);
            if (extension != "png") {
                POUTRE_RUNTIME_ERROR(
                    (boost::format("IOLoadPng::wrong extension %s") % i_path).str());
            }

            return loappngHelper(i_path);
        }

        void IOSavePng(const bf::path &i_path, const poutre::IInterface &i_img) {
            bf::path dir = i_path.parent_path();

            if (!bf::exists(dir)) {
                POUTRE_RUNTIME_ERROR(
                    (boost::format("IOSavePng:: path don't exist %s") % dir).str());
            }

            auto extension = bf::extension(i_path);
            boost::algorithm::to_lower(extension);
            if (extension != "png") {
                POUTRE_RUNTIME_ERROR(
                    (boost::format("IOSavePng::wrong extension %s") % extension).str());
            }

            writepngHelper(i_path, i_img);
        }
    }

}  // namespace
