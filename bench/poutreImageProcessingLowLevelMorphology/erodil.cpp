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

#include <poutreImageProcessingLowLevelMorphology/include/poutreImageProcessingEroDil.hpp>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <random>

namespace {
    template<typename T>
    decltype(auto) ConstructVector(size_t size)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(std::numeric_limits<T>::min(),std::numeric_limits<T>::max());

        std::vector<T> m_vect;
        m_vect.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            m_vect.push_back(dis(gen));
        }
        return m_vect;
    }
}

template<typename T>
class DilateFixture : public ::benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& st) {
        m_vect = ConstructVector<T>(st.range(0));
    }
    void TearDown(const ::benchmark::State&) {
        m_vect.clear();
    }
    std::vector<T> m_vect;
};

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixture, DilateFixture_UINT8,poutre::pUINT8)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (auto i = 0u; i < size; ++i) {
          std::vector<poutre::pUINT8> res(m_vect.size());
          auto sizeextent=sqrt(size);
          auto view2din=poutre::array_view<poutre::pUINT8,2>(m_vect,{int(sizeextent),int(sizeextent)});
          auto view2dout=poutre::array_view<poutre::pUINT8,2>(res,{int(sizeextent),int(sizeextent)});          
          poutre::t_Dilate(view2din, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DCT8,view2dout);
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixture, DilateFixture_UINT8)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Arg(128 * 128)->Unit(benchmark::kMillisecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(DilateFixture, DilateFixture_UINT32, poutre::pUINT32)(benchmark::State& state) {
   const auto size = state.range(0);
   while (state.KeepRunning()) {
      for (auto i = 0u; i < size; ++i) {
         std::vector<poutre::pUINT32> res(m_vect.size());
         auto sizeextent = sqrt(size);
         auto view2din = poutre::array_view<poutre::pUINT32, 2>(m_vect, { int(sizeextent),int(sizeextent) });
         auto view2dout = poutre::array_view<poutre::pUINT32, 2>(res, { int(sizeextent),int(sizeextent) });
         poutre::t_Dilate(view2din, poutre::se::NeighborListStaticSE::NeighborListStaticSE2DCT8, view2dout);
      }
   }
   state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(DilateFixture, DilateFixture_UINT32)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Arg(128 * 128)->Unit(benchmark::kMillisecond); //-V112

template<typename T>
class ShiftOpFixture : public ::benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& st) {
        m_vect = ConstructVector<T>(st.range(0));
    }
    void TearDown(const ::benchmark::State&) {
        m_vect.clear();
    }
    std::vector<T> m_vect;
};
BENCHMARK_TEMPLATE_DEFINE_F(ShiftOpFixture, ShiftOpFixture_UINT8, poutre::pUINT8)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (auto i = 0u; i < size; ++i) {
            std::vector<poutre::pUINT8> res(m_vect.size());
            auto view1din = poutre::array_view<poutre::pUINT8, 1>(m_vect, { size });
            auto view1dout = poutre::array_view<poutre::pUINT8, 1>(res, { size });
            poutre::t_LineBufferShiftRight(view1din,3,std::numeric_limits<poutre::pUINT8>::min(),view1dout);
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(ShiftOpFixture, ShiftOpFixture_UINT8)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Arg(128 * 128)->Unit(benchmark::kMillisecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(ShiftOpFixture, ShiftOpFixture_UINT32, poutre::pUINT32)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (auto i = 0u; i < size; ++i) {
            std::vector<poutre::pUINT32> res(m_vect.size());
            auto view1din = poutre::array_view<poutre::pUINT32, 1>(m_vect, { size });
            auto view1dout = poutre::array_view<poutre::pUINT32, 1>(res, { size });
            poutre::t_LineBufferShiftRight(view1din, 3, std::numeric_limits<poutre::pUINT32>::min(), view1dout);

        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(ShiftOpFixture, ShiftOpFixture_UINT32)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Arg(128 * 128)->Unit(benchmark::kMillisecond); //-V112