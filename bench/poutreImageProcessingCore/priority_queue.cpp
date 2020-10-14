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
#include <poutreImageProcessingCore/include/poutreImageProcessingPriorityQueue.hpp>

#include <boost/heap/priority_queue.hpp>
#include <cstdlib>
#include <queue>
#include <vector>

namespace
{

    decltype(auto) ConstructVector(poutre::offset size)
    {
        std::vector<std::pair<unsigned char, poutre::offset>> m_vect;
        m_vect.reserve(size);
        for (poutre::offset i = 0u; i < size; ++i)
        {
            m_vect.push_back({rand() % 255,i});
        }
        return m_vect;
    }

    template <typename T> class STDPriorityQueueReserve : public std::priority_queue<T>
    {
      public:
        STDPriorityQueueReserve(poutre::offset size) : std::priority_queue<T>()
        {
            (this->c).reserve(size);
        }
    };

    template <typename T, bool stable = false>
    class BoostPriorityQueueReserve : public boost::heap::priority_queue<T, boost::heap::stable<stable>>
    {
      public:
        BoostPriorityQueueReserve(poutre::offset size) : boost::heap::priority_queue<T, boost::heap::stable<stable>>()
        {
            this->reserve(size);
        }
    };

} // namespace

class PriorityQueueFixture : public ::benchmark::Fixture
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
    std::vector<std::pair<unsigned char, poutre::offset>> m_vect;
};

BENCHMARK_DEFINE_F(PriorityQueueFixture, stlpriorityqueue)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        std::priority_queue<std::pair<unsigned char, poutre::offset>> pqueue;

        for (const auto &val : m_vect)
        {
            pqueue.push(val);
        }
        while (!pqueue.empty())
        {
            auto top = pqueue.top(); //-V808
            pqueue.pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, stlpriorityqueue_reserve)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        STDPriorityQueueReserve<std::pair<unsigned char, poutre::offset>> pqueue(m_vect.size());
        for (const auto &val : m_vect)
        {
            pqueue.push(val);
        }
        while (!pqueue.empty())
        {
            auto top = pqueue.top();
            pqueue.pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, boostpriorityqueue)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        boost::heap::priority_queue<std::pair<unsigned char, poutre::offset>> pqueue;

        for (const auto &val : m_vect)
        {
            pqueue.push(val);
        }
        while (!pqueue.empty())
        {
            auto top = pqueue.top(); //-V808
            pqueue.pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, boostpriorityqueue_reserve)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        BoostPriorityQueueReserve<std::pair<unsigned char, poutre::offset>> pqueue(m_vect.size());

        for (const auto &val : m_vect)
        {
            pqueue.push(val);
        }
        while (!pqueue.empty())
        {
            auto top = pqueue.top(); //-V808
            pqueue.pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, boostpriorityqueue_respectfifoorder)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        boost::heap::priority_queue<std::pair<unsigned char, poutre::offset>, boost::heap::stable<true>> pqueue;

        for (const auto &val : m_vect)
        {
            pqueue.push(val);
        }
        while (!pqueue.empty())
        {
            auto top = pqueue.top(); //-V808
            pqueue.pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, boostpriorityqueue_reserve_respectfifoorder)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        BoostPriorityQueueReserve<std::pair<unsigned char, poutre::offset>, true> pqueue(m_vect.size());

        for (const auto &val : m_vect)
        {
            pqueue.push(val);
        }
        while (!pqueue.empty())
        {
            auto top = pqueue.top(); //-V808
            pqueue.pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, hand_made_respect_fifo_oder_optim_quant)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        poutre::PriorityQueue<poutre::pUINT8,poutre::offset> pqueue;

        for (const auto &val : m_vect)
        {
            pqueue.push(val);
        }
        while (!pqueue.empty())
        {
            auto top = pqueue.top();
            pqueue.pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, hand_made_respect_fifo_oder)(benchmark::State &state)
{
    const auto size = state.range(0);
    for (auto _ : state)
    {
        poutre::PriorityQueue < poutre::pUINT32, poutre::offset,
            poutre::lesserKey<poutre::pUINT32, poutre::offset>,true> pqueue;

        for (const auto &val : m_vect)
        {
            pqueue.push(val);
        }
        while (!pqueue.empty())
        {
            auto top = pqueue.top();
            pqueue.pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}
/*STL priority queue*/
BENCHMARK_REGISTER_F(PriorityQueueFixture, stlpriorityqueue)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
// 5% gain with visual 2015 SP3
BENCHMARK_REGISTER_F(PriorityQueueFixture, stlpriorityqueue_reserve)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112

/*boost priority queue*/
BENCHMARK_REGISTER_F(PriorityQueueFixture, boostpriorityqueue)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
// AT least 5% gain with visual 2015 SP3
BENCHMARK_REGISTER_F(PriorityQueueFixture, boostpriorityqueue_reserve)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
// 2.5 times slower
BENCHMARK_REGISTER_F(PriorityQueueFixture, boostpriorityqueue_respectfifoorder)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
// AT least 5% gain with visual 2015 SP3
BENCHMARK_REGISTER_F(PriorityQueueFixture, boostpriorityqueue_reserve_respectfifoorder)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112

//2* speedup under visual 2019
BENCHMARK_REGISTER_F(PriorityQueueFixture, hand_made_respect_fifo_oder_optim_quant)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
//fall back to boost
BENCHMARK_REGISTER_F(PriorityQueueFixture, hand_made_respect_fifo_oder)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112