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

#include <boost/filesystem.hpp>
#include <poutreIO/include/poutreHDF5.hpp>
#include <poutreIO/poutreIOLoader.hpp>
#include <poutreIO/poutreIOString.hpp>
#include <poutreIO/poutreIOWriter.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>

namespace bf = boost::filesystem;
BOOST_AUTO_TEST_SUITE(hdf5)

BOOST_AUTO_TEST_CASE(iohdf5UINT8)
{
    std::string str = "Dense Scalar GUINT8 2 2 3 \
0 76 149 \
28 176 255\
";

    // store
    auto expected = poutre::ImageFromString(str);
    bf::path tempDir = /*bf::path(POUTREIO_NRT_DATAS) /*/ "POUTRE_NRT_IO_TMP_DIR";
    if (!bf::is_directory(tempDir))
    {
        bf::create_directory(tempDir);
    }
    bf::path image_path = tempDir / "write_test_GRAY256.h5";
    auto writter = poutre::ImageWriter().SetPath(image_path.string());
    writter.Write(*expected);

    // load again and check
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    BOOST_CHECK_EQUAL(imgstr, str);
}

BOOST_AUTO_TEST_CASE(iohdf5RGB)
{
    std::string str = "Dense 3Planes GUINT8 2 2 3 \
0 0 0 255 0 0 0 255 0 \
0 0 255 228 135 255 255 255 255\
";

    // store
    auto expected = poutre::ImageFromString(str);
    bf::path tempDir = /*bf::path(POUTREIO_NRT_DATAS) /*/ "POUTRE_NRT_IO_TMP_DIR";
    if (!bf::is_directory(tempDir))
    {
        bf::create_directory(tempDir);
    }
    bf::path image_path = tempDir / "write_test_RGB.h5";
    auto writter = poutre::ImageWriter().SetPath(image_path.string());
    writter.Write(*expected);

    // load again and check
    auto loader = poutre::ImageLoader().SetPath(image_path.string());
    auto img = loader.Load();
    auto imgstr = poutre::ImageToString(*img);
    BOOST_CHECK_EQUAL(imgstr, str);
}

BOOST_AUTO_TEST_SUITE_END()