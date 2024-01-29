// This is an open source non-commercial project. Dear PVS-Studio, please check it.

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
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingArith.hpp>
#include <random>
#include <vector>

namespace
{

    decltype(auto) ConstructVector(size_t size)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        std::vector<poutre::pUINT8> m_vect;
        m_vect.reserve(size);
        for (size_t i = 0u; i < size; ++i)
        {
          m_vect.push_back(static_cast<poutre::pUINT8>(dis(gen)));
        }
        return m_vect;
    }
} // namespace

class ArithmeticsFixture : public ::benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &st)
    {
        m_vect = ConstructVector(st.range(0));
    }
    void TearDown(const ::benchmark::State &)
    {
        m_vect.clear();
    }
    std::vector<poutre::pUINT8> m_vect;
};

BENCHMARK_DEFINE_F(ArithmeticsFixture, supremum_view)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view</*const*/ poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_ArithSup(view2din, view2din, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(ArithmeticsFixture, supremum_strided_view)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view</*const*/ poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        auto stridedview2dout = view2dout.section({0, 0});
        poutre::t_ArithSup(view2din, view2din, stridedview2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(ArithmeticsFixture, add_const_view)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view<poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        poutre::t_ArithSaturatedAddConstant(view2din, (poutre::pUINT8)10, view2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(ArithmeticsFixture, supremum_view)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Unit(benchmark::kMicrosecond); //-V112
BENCHMARK_REGISTER_F(ArithmeticsFixture, supremum_strided_view)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_DEFINE_F(ArithmeticsFixture, add_const_strided_view)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::vector<poutre::pUINT8> res(m_vect.size());
        auto sizeextent = sqrt(size);
        auto view2din = poutre::array_view</*const*/ poutre::pUINT8, 2>(m_vect, {int(sizeextent), int(sizeextent)});
        auto view2dout = poutre::array_view<poutre::pUINT8, 2>(res, {int(sizeextent), int(sizeextent)});
        auto stridedview2dout = view2dout.section({0, 0}); // poutre::strided_array_view<unsigned int,
                                                           // 2>(view2dout);
        poutre::t_ArithSaturatedAddConstant(view2din, (poutre::pUINT8)10, stridedview2dout);
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_REGISTER_F(ArithmeticsFixture, add_const_view)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Unit(benchmark::kMicrosecond); //-V112
BENCHMARK_REGISTER_F(ArithmeticsFixture, add_const_strided_view)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
