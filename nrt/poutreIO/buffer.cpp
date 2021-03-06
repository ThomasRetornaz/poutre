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

#include "main.hpp"
#include <gtest/gtest.h>
#include <poutreIO/include/poutreIOBuffer.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>

TEST(iobuffer, ioImageFromBufferScalar)
{
    const float tab[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    { // 1D
        std::unique_ptr<poutre::IInterface> img =
            poutre::CreateDense({12}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_F32);
        poutre::details::t_ImageFromBufferWithType(*img, 12 * sizeof(float), &tab[0]);
        using ImageType_t = poutre::DenseImage<float, 1>;
        ImageType_t *img_t = dynamic_cast<ImageType_t *>(img.get());
        if (!img_t)
        {
            POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        auto it = (*img_t).begin();
        for (size_t i = 0; i < 12; ++i, ++it)
        {
            EXPECT_EQ(*it, tab[i]);
        }
    }

    { // 2D
        std::unique_ptr<poutre::IInterface> img =
            poutre::CreateDense({6, 2}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_F32);
        poutre::details::t_ImageFromBufferWithType(*img, 12 * sizeof(float), &tab[0]);
        using ImageType_t = poutre::DenseImage<float, 2>;
        ImageType_t *img_t = dynamic_cast<ImageType_t *>(img.get());
        if (!img_t)
        {
            POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        auto it = (*img_t).begin();
        for (size_t i = 0; i < 12; ++i, ++it)
        {
            EXPECT_EQ(*it, tab[i]);
        }
    }

    { // 3D
        std::unique_ptr<poutre::IInterface> img =
            poutre::CreateDense({3, 2, 2}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_F32);
        poutre::details::t_ImageFromBufferWithType(*img, 12 * sizeof(float), &tab[0]);
        using ImageType_t = poutre::DenseImage<float, 3>;
        ImageType_t *img_t = dynamic_cast<ImageType_t *>(img.get());
        if (!img_t)
        {
            POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        auto it = (*img_t).begin();
        for (size_t i = 0; i < 12; ++i, ++it)
        {
            EXPECT_EQ(*it, tab[i]);
        }
    }
}

TEST(iobuffer, ioImageToBufferScalar)
{

    { // 1D
        float tab[12];
        std::unique_ptr<poutre::IInterface> img =
            poutre::CreateDense({12}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_F32);
        using ImageType_t = poutre::DenseImage<float, 1>;
        ImageType_t *img_t = dynamic_cast<ImageType_t *>(img.get());
        if (!img_t)
        {
            POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        for (auto i = 0; i < 12; i++)
            (*img_t)[i] = static_cast<float>(i);
        poutre::details::t_ImageToBufferForceType(*img, 12 * sizeof(float), &tab[0]);
        auto it = (*img_t).begin();
        for (size_t i = 0; i < 12; ++i, ++it)
        {
            EXPECT_EQ(*it, tab[i]);
        }
    }

    { // 2D
        float tab[12];
        std::unique_ptr<poutre::IInterface> img =
            poutre::CreateDense({6, 2}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_F32);
        using ImageType_t = poutre::DenseImage<float, 2>;
        ImageType_t *img_t = dynamic_cast<ImageType_t *>(img.get());
        if (!img_t)
        {
            POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        for (auto i = 0; i < 12; i++)
            (*img_t)[i] = static_cast<float>(i);
        poutre::details::t_ImageToBufferForceType(*img, 12 * sizeof(float), &tab[0]);
        auto it = (*img_t).begin();
        for (size_t i = 0; i < 12; ++i, ++it)
        {
            EXPECT_EQ(*it, tab[i]);
        }
    }
    { // 3D
        float tab[12];
        std::unique_ptr<poutre::IInterface> img =
            poutre::CreateDense({3, 2, 2}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_F32);
        using ImageType_t = poutre::DenseImage<float, 3>;
        ImageType_t *img_t = dynamic_cast<ImageType_t *>(img.get());
        if (!img_t)
        {
            POUTRE_RUNTIME_ERROR("Dynamic cast fail");
        }
        for (auto i = 0; i < 12; i++)
            (*img_t)[i] = static_cast<float>(i);
        poutre::details::t_ImageToBufferForceType(*img, 12 * sizeof(float), &tab[0]);
        auto it = (*img_t).begin();
        for (size_t i = 0; i < 12; ++i, ++it)
        {
            EXPECT_EQ(*it, tab[i]);
        }
    }
}
