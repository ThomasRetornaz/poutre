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
#include <queue>
#include <boost/heap/priority_queue.hpp>


namespace {

    decltype(auto) ConstructVector(size_t size)
    {
        std::vector<unsigned int> m_vect;
        m_vect.reserve(size);
        for (size_t i = 0u; i < size; ++i) {
            m_vect.push_back(rand() % size);
        }
        return m_vect;
    }


    template<typename T>
    class PriorityQueueReserve :public  std::priority_queue<T>
    {
    public:
        PriorityQueueReserve(size_t size) :std::priority_queue<T>()
        {
            (this->c).reserve(size);
        }
    };

    template<typename T, bool stable=false>
    class BoostPriorityQueueReserve :public boost::heap::priority_queue<T,boost::heap::stable<stable>>
    {
    public:
        BoostPriorityQueueReserve(size_t size) :boost::heap::priority_queue<T,boost::heap::stable<stable>>()
        {
            this->reserve(size);
        }
    };

}  // namespace

class PriorityQueueFixture : public ::benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& st) {
        m_vect = ConstructVector(st.range(0));
    }
    void TearDown(const ::benchmark::State&) {
        m_vect.clear();
    }
    std::vector<unsigned int> m_vect;
};

BENCHMARK_DEFINE_F(PriorityQueueFixture, stlpriorityqueue)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            std::priority_queue<unsigned int> pqueue;

            for (const auto& val : m_vect)
            {
                pqueue.push(val);
            }
            while (!pqueue.empty())
            {
                auto top = pqueue.top();
                pqueue.pop();
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, stlpriorityqueue_reserve)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            PriorityQueueReserve<unsigned int> pqueue(m_vect.size());
            for (const auto& val : m_vect)
            {
                pqueue.push(val);
            }
            while (!pqueue.empty())
            {
                auto top = pqueue.top();
                pqueue.pop();
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, boostpriorityqueue)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            boost::heap::priority_queue<unsigned int> pqueue;

            for (const auto& val : m_vect)
            {
                pqueue.push(val);
            }
            while (!pqueue.empty())
            {
                auto top = pqueue.top();
                pqueue.pop();
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, boostpriorityqueue_reserve)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            BoostPriorityQueueReserve<unsigned int> pqueue(m_vect.size());

            for (const auto& val : m_vect)
            {
                pqueue.push(val);
            }
            while (!pqueue.empty())
            {
                auto top = pqueue.top();
                pqueue.pop();
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, boostpriorityqueue_respectfifoorder)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            boost::heap::priority_queue<unsigned int, boost::heap::stable<true>> pqueue;

            for (const auto& val : m_vect)
            {
                pqueue.push(val);
            }
            while (!pqueue.empty())
            {
                auto top = pqueue.top();
                pqueue.pop();
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, boostpriorityqueue_reserve_respectfifoorder)(benchmark::State& state) {
    const auto size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            BoostPriorityQueueReserve<unsigned int,true> pqueue(m_vect.size());

            for (const auto& val : m_vect)
            {
                pqueue.push(val);
            }
            while (!pqueue.empty())
            {
                auto top = pqueue.top();
                pqueue.pop();
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

/*STL priority queue*/
BENCHMARK_REGISTER_F(PriorityQueueFixture, stlpriorityqueue)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond); //-V112
//5% gain with visual 2015 SP3
BENCHMARK_REGISTER_F(PriorityQueueFixture, stlpriorityqueue_reserve)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond); //-V112

/*boost priority queue*/
BENCHMARK_REGISTER_F(PriorityQueueFixture, boostpriorityqueue)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond); //-V112
//AT least 5% gain with visual 2015 SP3
BENCHMARK_REGISTER_F(PriorityQueueFixture, boostpriorityqueue_reserve)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond); //-V112
//2.5 times slower
BENCHMARK_REGISTER_F(PriorityQueueFixture, boostpriorityqueue_respectfifoorder)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond); //-V112
//AT least 5% gain with visual 2015 SP3
BENCHMARK_REGISTER_F(PriorityQueueFixture, boostpriorityqueue_reserve_respectfifoorder)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond); //-V112

