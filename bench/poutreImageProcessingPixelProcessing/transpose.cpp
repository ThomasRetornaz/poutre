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
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingTranspose.hpp>
#include <random>
#include <vector>

namespace {
template <typename T> void TransposeBenchmark(benchmark::State &state) {
  const auto size = state.range(0);
  while (state.KeepRunning()) {
    for (auto i = 0u; i < size; ++i) {
      auto ImageIn = poutre::Image2D<T>({(std::size_t)sqrt(size), (std::size_t)sqrt(size)});
      auto ImageOut = poutre::Image2D<T>({(std::size_t)sqrt(size), (std::size_t)sqrt(size)});
      poutre::t_transpose(ImageIn, ImageOut);
    }
  }
  state.SetItemsProcessed(state.iterations() * size);
}
} // namespace
BENCHMARK_TEMPLATE(TransposeBenchmark, poutre::pUINT8)
    ->Arg(16 * 16)
    ->Arg(32 * 32)
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Unit(benchmark::kMicrosecond); //-V112
BENCHMARK_TEMPLATE(TransposeBenchmark, poutre::pINT32)
    ->Arg(16 * 16)
    ->Arg(32 * 32)
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Unit(benchmark::kMicrosecond); //-V112
BENCHMARK_TEMPLATE(TransposeBenchmark, poutre::pFLOAT)
    ->Arg(16 * 16)
    ->Arg(32 * 32)
    ->Arg(64 * 64)
    ->Arg(128 * 128)
    ->Unit(benchmark::kMicrosecond); //-V112
