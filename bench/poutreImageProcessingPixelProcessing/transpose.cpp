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

#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingTranspose.hpp>
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
class TransposeFixture : public ::benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& st) {
        m_vect = ConstructVector<T>(st.range(0));
    }
    void TearDown(const ::benchmark::State&) {
        m_vect.clear();
    }
    std::vector<T> m_vect;
};

BENCHMARK_TEMPLATE_DEFINE_F(TransposeFixture, TransposeFixture_UINT8,poutre::pUINT8)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (auto i = 0u; i < size; ++i) {
          std::vector<poutre::pUINT8> res(m_vect.size());
          auto sizeextent=sqrt(size);
          auto view2din=poutre::array_view<poutre::pUINT8,2>(m_vect,{int(sizeextent),int(sizeextent)});
          auto view2dout=poutre::array_view<poutre::pUINT8,2>(res,{int(sizeextent),int(sizeextent)});          
          poutre::t_transpose2DAlongX(view2din,view2dout);
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(TransposeFixture, TransposeFixture_UINT8)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Arg(128 * 128)->Unit(benchmark::kMicrosecond); //-V112

BENCHMARK_TEMPLATE_DEFINE_F(TransposeFixture, TransposeFixture_FLOAT, poutre::pFLOAT)(benchmark::State& state) {
   const auto size = state.range(0);
   while (state.KeepRunning()) {
      for (auto i = 0u; i < size; ++i) {
         std::vector<poutre::pFLOAT> res(m_vect.size());
         auto sizeextent = sqrt(size);
         auto view2din = poutre::array_view<poutre::pFLOAT, 2>(m_vect, { int(sizeextent),int(sizeextent) });
         auto view2dout = poutre::array_view<poutre::pFLOAT, 2>(res, { int(sizeextent),int(sizeextent) });
         poutre::t_transpose2DAlongX(view2din, view2dout);
      }
   }
   state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(TransposeFixture, TransposeFixture_FLOAT)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Arg(128 * 128)->Unit(benchmark::kMicrosecond); //-V112
