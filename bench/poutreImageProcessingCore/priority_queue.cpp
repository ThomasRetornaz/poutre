// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreImageProcessingCore/include/poutreImageProcessingPriorityQueue.hpp>

#include "benchmark/benchmark.h"
#include <cstdlib>
#include <queue>
#include <vector>

namespace
{
  decltype(auto) ConstructVector(poutre::offset size)
  {
    std::vector<std::pair<poutre::offset, poutre::offset>> m_vect;
    m_vect.reserve(size);
    for( poutre::offset i = 0u; i < size; ++i )
    {
      m_vect.push_back({rand() % 255, i});
    }
    return m_vect;
  }

  decltype(auto) ConstructVectorUint8(poutre::offset size)
  {
    std::vector<std::pair<poutre::pUINT8, poutre::offset>> m_vect;
    m_vect.reserve(size);
    for( poutre::offset i = 0u; i < size; ++i )
    {
      m_vect.push_back({rand() % 255, i});
    }
    return m_vect;
  }
} // namespace

class PriorityQueueFixture : public ::benchmark::Fixture
{
  public:
  void SetUp(const ::benchmark::State &st) { m_vect = ConstructVector(st.range(0)); }
  void TearDown(const ::benchmark::State &) { m_vect.clear(); }
  std::vector<std::pair<poutre::offset, poutre::offset>> m_vect;
};

class PriorityQueueFixtureUint8 : public ::benchmark::Fixture
{
  public:
  void SetUp(const ::benchmark::State &st) { m_vect = ConstructVectorUint8(st.range(0)); }
  void TearDown(const ::benchmark::State &) { m_vect.clear(); }
  std::vector<std::pair<poutre::pUINT8, poutre::offset>> m_vect;
};

BENCHMARK_DEFINE_F(PriorityQueueFixture, priorityqueue)(benchmark::State &state)
{
  const auto size = state.range(0);
  for( auto _ : state )
  {
    poutre::poutre_pq<poutre::offset, poutre::offset> pqueue;

    for( const auto &val : m_vect )
    {
      pqueue.push(val);
    }
    while( !pqueue.empty() )
    {
      auto top = pqueue.top(); //-V808
      pqueue.pop();
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, priorityqueue_reserve)(benchmark::State &state)
{
  const auto size = state.range(0);
  for( auto _ : state )
  {
    poutre::poutre_pq<poutre::offset, poutre::offset> pqueue(m_vect.size());
    for( const auto &val : m_vect )
    {
      pqueue.push(val);
    }
    while( !pqueue.empty() )
    {
      auto top = pqueue.top();
      pqueue.pop();
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixture, priorityqueue_respectfifoorder)(benchmark::State &state)
{
  const auto size = state.range(0);
  for( auto _ : state )
  {
    poutre::poutre_pq_stable<poutre::offset, poutre::offset> pqueue(m_vect.size());

    for( const auto &val : m_vect )
    {
      pqueue.push(val);
    }
    while( !pqueue.empty() )
    {
      auto top = pqueue.top(); //-V808
      pqueue.pop();
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixtureUint8, priorityqueue_uint8)(benchmark::State &state)
{
  const auto size = state.range(0);
  for( auto _ : state )
  {
    poutre::poutre_pq<poutre::pUINT8, poutre::offset> pqueue;

    for( const auto &val : m_vect )
    {
      pqueue.push(val);
    }
    while( !pqueue.empty() )
    {
      auto top = pqueue.top(); //-V808
      pqueue.pop();
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(PriorityQueueFixtureUint8, priorityqueue_respectfifoorder_uint8)(benchmark::State &state)
{
  const auto size = state.range(0);
  for( auto _ : state )
  {
    poutre::poutre_pq_stable<poutre::pUINT8, poutre::offset> pqueue;

    for( const auto &val : m_vect )
    {
      pqueue.push(val);
    }
    while( !pqueue.empty() )
    {
      auto top = pqueue.top(); //-V808
      pqueue.pop();
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}


BENCHMARK_REGISTER_F(PriorityQueueFixture, priorityqueue)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
BENCHMARK_REGISTER_F(PriorityQueueFixture, priorityqueue_reserve)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
BENCHMARK_REGISTER_F(PriorityQueueFixtureUint8, priorityqueue_uint8)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
BENCHMARK_REGISTER_F(PriorityQueueFixture, priorityqueue_respectfifoorder)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112
BENCHMARK_REGISTER_F(PriorityQueueFixtureUint8, priorityqueue_respectfifoorder_uint8)
    ->Arg(16 * 16)
    ->Arg(32 * 32) //-V112
    ->Arg(64 * 64)
    ->Unit(benchmark::kMicrosecond); //-V112