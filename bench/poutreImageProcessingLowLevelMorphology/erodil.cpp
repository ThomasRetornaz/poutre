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

#include "benchmark/benchmark.h"

#include <cmath>
#include <cstdlib>
#include <poutreImageProcessingLowLevelMorphology/include/poutreImageProcessingEroDil.hpp>
#include <random>
#include <vector>

namespace
{
    template <typename T> decltype(auto) ConstructVector(size_t size)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

        std::vector<T> m_vect;
        m_vect.reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            m_vect.push_back(dis(gen));
        }
        return m_vect;
    }
} // namespace

template <typename T> class DilateFixtureView : public ::benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &st)
    {
        m_vect = ConstructVector<T>(st.range(0));
    }
    void TearDown(const ::benchmark::State &)
    {
        m_vect.clear();
    }
    std::vector<T> m_vect;
};

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Square_UINT8, poutre::pUINT8)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c8_2d, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Square_UINT8)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Square_INT32, poutre::pINT32)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pINT32> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pINT32, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pINT32, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c8_2d, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Square_INT32)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Cross_UINT8, poutre::pUINT8)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c4_2d, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Cross_UINT8)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Cross_INT32, poutre::pINT32)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pINT32> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pINT32, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pINT32, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c4_2d, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Cross_INT32)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Seg0_UINT8, poutre::pUINT8)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c2_2dH, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Seg0_UINT8)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Seg0_INT32, poutre::pINT32)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pINT32> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pINT32, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pINT32, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c2_2dH, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Seg0_INT32)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Seg90_UINT8, poutre::pUINT8)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c2_2dV, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Seg90_UINT8)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Seg90_INT32, poutre::pINT32)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pINT32> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pINT32, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pINT32, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c2_2dV, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Seg90_INT32)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Seg45_UINT8, poutre::pUINT8)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c2_2d_Diag45, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Seg45_UINT8)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Seg45_INT32, poutre::pINT32)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pINT32> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pINT32, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pINT32, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c2_2d_Diag45, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Seg45_INT32)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Seg135_UINT8, poutre::pUINT8)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c2_2d_Diag135, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Seg135_UINT8)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixtureView, DilateFixtureView_Seg135_INT32, poutre::pINT32)
(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pINT32> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pINT32, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pINT32, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_Dilate(view2din, poutre::se::NLS::NLS_c2_2d_Diag135, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixtureView, DilateFixtureView_Seg135_INT32)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256)
    ->Arg(512 * 512)
    ->Arg(1024 * 1024)
    ->Unit(benchmark::kMicrosecond); //-V112

//template <typename T> class ShiftOpFixture : public ::benchmark::Fixture
//{
//  public:
//    void SetUp(const ::benchmark::State &st)
//    {
//        m_vect = ConstructVector<T>(st.range(0));
//    }
//    void TearDown(const ::benchmark::State &)
//    {
//        m_vect.clear();
//    }
//    std::vector<T> m_vect;
//};
//
//BENCHMARK_TEMPLATE_DEFINE_F(ShiftOpFixture, ShiftOpFixture_UINT8, poutre::pUINT8)
//(benchmark::State &state)
//{
//    const auto size = state.range(0);
//    for (auto _ : state)
//    {
//        std::vector<poutre::pUINT8> res(m_vect.size());
//        auto view1din = poutre::array_view<poutre::pUINT8, 1>(m_vect, {size});
//        auto view1dout = poutre::array_view<poutre::pUINT8, 1>(res, {size});
//        poutre::t_LineBufferShiftRight(view1din.data(), size, 3, std::numeric_limits<poutre::pUINT8>::min(),
//                                       view1dout.data());
//    }
//    state.SetItemsProcessed(state.iterations() * size);
//}
//BENCHMARK_REGISTER_F(ShiftOpFixture, ShiftOpFixture_UINT8)
//    ->Arg(16 * 16)
//    ->Arg(32 * 32)
//    ->Arg(64 * 64)
//    ->Arg(128 * 128)
//    ->Arg(256 * 256)
//    ->Arg(512 * 512)
//    ->Arg(1024 * 1024)
//    ->Unit(benchmark::kMicrosecond); //-V112
//
//BENCHMARK_TEMPLATE_DEFINE_F(ShiftOpFixture, ShiftOpFixture_INT32, poutre::pINT32)
//(benchmark::State &state)
//{
//    const auto size = state.range(0);
//    for (auto _ : state)
//    {
//        std::vector<poutre::pINT32> res(m_vect.size());
//        auto view1din = poutre::array_view<poutre::pINT32, 1>(m_vect, {size});
//        auto view1dout = poutre::array_view<poutre::pINT32, 1>(res, {size});
//        poutre::t_LineBufferShiftRight(view1din.data(), size, 3, std::numeric_limits<poutre::pINT32>::min(),
//                                       view1dout.data());
//    }
//    state.SetItemsProcessed(state.iterations() * size);
//}
//BENCHMARK_REGISTER_F(ShiftOpFixture, ShiftOpFixture_INT32)
//    ->Arg(16 * 16)
//    ->Arg(32 * 32)
//    ->Arg(64 * 64)
//    ->Arg(128 * 128)
//    ->Arg(256 * 256)
//    ->Arg(512 * 512)
//    ->Arg(1024 * 1024)
//    ->Unit(benchmark::kMicrosecond); //-V112

// LINE SE

template <typename T> class DilateXFixtureView : public ::benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &st)
    {
        m_vect = ConstructVector<T>(2048 * 2048);
    }
    void TearDown(const ::benchmark::State &)
    {
        m_vect.clear();
    }
    std::vector<T> m_vect;
};

BENCHMARK_TEMPLATE_DEFINE_F(DilateXFixtureView, DilateXFixtureView_UINT8, poutre::pUINT8)
(benchmark::State &state)
{
    const auto sizeSE = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt((double)m_vect.size());
        auto view2din = poutre::array_view<poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_DilateX(view2din, sizeSE, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * m_vect.size());
}

BENCHMARK_REGISTER_F(DilateXFixtureView, DilateXFixtureView_UINT8)
    ->Arg(16)
    ->Arg(32) //-V112
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Arg(512)
    ->Arg(1024)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateXFixtureView, DilateXFixtureView_INT32, poutre::pINT32)
(benchmark::State &state)
{
    const auto sizeSE = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pINT32> res(m_vect.size());
        auto sizeextent = sqrt((double)m_vect.size());
        auto view2din = poutre::array_view<poutre::pINT32, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pINT32, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_DilateX(view2din, sizeSE, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * m_vect.size());
}

BENCHMARK_REGISTER_F(DilateXFixtureView, DilateXFixtureView_UINT8)
    ->Arg(16)
    ->Arg(32) //-V112
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Arg(512)
    ->Arg(1024)
    ->Unit(benchmark::kMicrosecond); //-V112