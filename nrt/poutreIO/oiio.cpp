// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreIO/poutreIOString.hpp>
#include <poutreIO/include/poutreOIIO.hpp>
#include <poutreIO/poutreIOLoader.hpp>
#include <poutreIO/poutreIOWriter.hpp>
#include <boost/filesystem.hpp>

namespace fs = std::filesystem;

TEST(oiio,iopngloadUINT8)
{
    std::string expected = "Dense Scalar GUINT8 2 2 3 \
0 76 149 \
28 176 255\
";
    fs::path image_path = fs::path(POUTREIO_NRT_DATAS) / "test_GRAY256.png";
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    EXPECT_EQ(imgstr, expected);
}
TEST(oiio,iopngloadRGB)
{
    std::string expected = "Dense 3Planes GUINT8 2 2 3 \
0 0 0 255 0 0 0 255 0 \
0 0 255 228 135 255 255 255 255\
";
    fs::path image_path= fs::path(POUTREIO_NRT_DATAS) / "test_RGB.png";
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    EXPECT_EQ(imgstr, expected);
}
TEST(oiio,iopngloadRGBA)
{
    std::string expected = "Dense 4Planes GUINT8 2 2 3 \
0 0 0 255 255 0 0 255 0 255 0 255 \
0 0 255 255 228 135 255 255 255 255 255 255\
";
    fs::path image_path = fs::path(POUTREIO_NRT_DATAS) / "test_RGBA.png";
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    EXPECT_EQ(imgstr, expected);
}

TEST(oiio,iopngsaveUINT8)
{
    std::string str = "Dense Scalar GUINT8 2 2 3 \
0 76 149 \
28 176 255\
";
    //store
    auto expected = poutre::ImageFromString(str);
    fs::path tempDir = /*fs::path(POUTREIO_NRT_DATAS) /*/ "POUTRE_NRT_IO_TMP_DIR";
    if (!fs::is_directory(tempDir))
    {
        fs::create_directory(tempDir);
    }
    fs::path image_path = tempDir / "write_test_GRAY256.png";
    auto writter = poutre::ImageWriter().SetPath(image_path.string());
    writter.Write(*expected);

    //load again and check
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    EXPECT_EQ(imgstr, str);
}

TEST(oiio,iopngsaveRGB)
{
    std::string str = "Dense 3Planes GUINT8 2 2 3 \
0 0 0 255 0 0 0 255 0 \
0 0 255 228 135 255 255 255 255\
";
    // store
    auto expected = poutre::ImageFromString(str);
    fs::path tempDir = /*fs::path(POUTREIO_NRT_DATAS) /*/ "POUTRE_NRT_IO_TMP_DIR";
    if (!fs::is_directory(tempDir))
    {
        fs::create_directory(tempDir);
    }
    fs::path image_path = tempDir / "write_test_GRAY256.png";
    auto writter = poutre::ImageWriter().SetPath(image_path.string());
    writter.Write(*expected);

    // load again and check
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    EXPECT_EQ(imgstr, str);

}
TEST(oiio,iopngsaveRGBA)
{
    std::string str = "Dense 4Planes GUINT8 2 2 3 \
0 0 0 255 255 0 0 255 0 255 0 255 \
0 0 255 255 228 135 255 255 255 255 255 255\
";

    // store
    auto expected = poutre::ImageFromString(str);
    fs::path tempDir = /*fs::path(POUTREIO_NRT_DATAS) /*/ "POUTRE_NRT_IO_TMP_DIR";
    if (!fs::is_directory(tempDir))
    {
        fs::create_directory(tempDir);
    }
    fs::path image_path = tempDir / "write_test_GRAY256.png";
    auto writter = poutre::ImageWriter().SetPath(image_path.string());
    writter.Write(*expected);

    // load again and check
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    EXPECT_EQ(imgstr, str);
}

TEST(oiio,iotifffloat)
{
    std::string str = "Dense 3Planes F32 2 2 3 \
0 0 0 255 0 0 0 255 0 \
0 0 255 228 135 255 255 255 255\
";
    // store
    auto expected = poutre::ImageFromString(str);
    fs::path tempDir = /*fs::path(POUTREIO_NRT_DATAS) /*/ "POUTRE_NRT_IO_TMP_DIR";
    if (!fs::is_directory(tempDir))
    {
        fs::create_directory(tempDir);
    }
    fs::path image_path = tempDir / "write_test_RGBFLOAT.tif";
    auto writter = poutre::ImageWriter().SetPath(image_path.string());
    writter.Write(*expected);
    // load again and check
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    EXPECT_EQ(imgstr, str);
}

TEST(oiio,exceptions)
{
    {
    
    std::string str = "Dense Scalar GUINT8 3 1 1 1 \
0\
";
    auto expected = poutre::ImageFromString(str);
    
    fs::path tempDir = /*fs::path(POUTREIO_NRT_DATAS) /*/ "POUTRE_NRT_IO_TMP_DIR";
    if (!fs::is_directory(tempDir))
    {
        fs::create_directory(tempDir);
    }
    fs::path image_path = tempDir / "write_test_3DGRAYMUSTFAIL.tif";
    EXPECT_THROW(poutre::StoreWithOIIO(image_path.string(), *expected), std::runtime_error);
    }

    {
        fs::path image_path = "bli.mouf";
        EXPECT_THROW(poutre::LoadFromOIIO(image_path.string()), std::runtime_error);
    }
  }
