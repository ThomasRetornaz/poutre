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
#include <poutreBase/poutreContainerView.hpp>

#include <cstdlib>
#include <vector>

namespace
{

    decltype(auto) ConstructVector(size_t size)
    {
        std::vector<unsigned int> m_vect(size);
        //m_vect.reserve(size);
        /*for (auto i = 0; i < size; ++i) {
            m_vect.push_back(rand() % size);
        }*/
        return m_vect;
    }

} // namespace

class ViewOnVect1DFixture : public ::benchmark::Fixture
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
    std::vector<unsigned int> m_vect;
};

// 15 times slower than common iterator
BENCHMARK_DEFINE_F(ViewOnVect1DFixture, BoundIterator)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        // benchmark::DoNotOptimize(m_vect[i]);
        auto view = poutre::array_view<unsigned int, 1>(m_vect);
        auto bnd = view.bound();
        auto it = bnd.begin();
        auto itend = bnd.end();
        for (; it != itend; ++it)
        {
            benchmark::DoNotOptimize(view[*it]);
            // auto val= view[*it];
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(ViewOnVect1DFixture, Iterator)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        auto it = m_vect.begin();
        auto itend = m_vect.end();
        for (; it != itend; ++it)
        {
            benchmark::DoNotOptimize(*it);
            // auto val = *it;
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_REGISTER_F(ViewOnVect1DFixture, Iterator)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256); //->Unit(benchmark::kMillisecond); //-V112
// 6 times slower ....
BENCHMARK_REGISTER_F(ViewOnVect1DFixture, BoundIterator)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Arg(256 * 256); //->Unit(benchmark::kMillisecond); //-V112
