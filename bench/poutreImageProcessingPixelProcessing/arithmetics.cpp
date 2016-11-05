#include "benchmark/benchmark.h"

#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingArith.hpp>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <random>

namespace {

    decltype(auto) ConstructVector(size_t size)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0,255);

        std::vector<poutre::pUINT8> m_vect;
        m_vect.reserve(size);
        for (auto i = 0; i < size; ++i) {
            m_vect.push_back(dis(gen));
        }
        return m_vect;
    }
}

class ArithmeticsFixture : public ::benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& st) {
        m_vect = ConstructVector(st.range(0));
    }
    void TearDown(const ::benchmark::State&) {
        m_vect.clear();
    }
    std::vector<poutre::pUINT8> m_vect;
};

BENCHMARK_DEFINE_F(ArithmeticsFixture, supremum_view)(benchmark::State& state) {
    const int size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
          std::vector<poutre::pUINT8> res(m_vect.size());
          auto sizeextent=sqrt(size);
          auto view2din=poutre::carray_view<poutre::pUINT8,2>(m_vect,{int(sizeextent),int(sizeextent)});
          auto view2dout=poutre::array_view<poutre::pUINT8,2>(res,{int(sizeextent),int(sizeextent)});
          poutre::t_ArithSup(view2din,view2din,view2dout);
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(ArithmeticsFixture, supremum_strided_view)(benchmark::State& state) {
    const int size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            std::vector< poutre::pUINT8> res(m_vect.size());
            auto sizeextent = sqrt(size);
            auto view2din = poutre::carray_view< poutre::pUINT8, 2>(m_vect, { int(sizeextent),int(sizeextent) });
            auto view2dout = poutre::array_view< poutre::pUINT8, 2>(res, { int(sizeextent),int(sizeextent) });
            auto stridedview2dout = view2dout.section({ 0,0 });
            poutre::t_ArithSup(view2din,view2din, stridedview2dout);
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(ArithmeticsFixture, add_const_view)(benchmark::State& state) {
    const int size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            std::vector< poutre::pUINT8> res(m_vect.size());
            auto sizeextent = sqrt(size);
            auto view2din = poutre::carray_view< poutre::pUINT8, 2>(m_vect, { int(sizeextent),int(sizeextent) });
            auto view2dout = poutre::array_view< poutre::pUINT8, 2>(res, { int(sizeextent),int(sizeextent) });
            poutre::t_ArithSaturatedAddConstant(view2din, (poutre::pUINT8)10, view2dout);
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(ArithmeticsFixture, add_const_strided_view)(benchmark::State& state) {
    const int size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {
            std::vector< poutre::pUINT8> res(m_vect.size());
            auto sizeextent = sqrt(size);
            auto view2din = poutre::carray_view< poutre::pUINT8, 2>(m_vect, { int(sizeextent),int(sizeextent) });
            auto view2dout = poutre::array_view< poutre::pUINT8, 2>(res, { int(sizeextent),int(sizeextent) });
            auto stridedview2dout = view2dout.section({ 0,0 });//poutre::strided_array_view<unsigned int, 2>(view2dout);
            poutre::t_ArithSaturatedAddConstant(view2din, (poutre::pUINT8)10, stridedview2dout);
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

//BENCHMARK_REGISTER_F(ArithmeticsFixture, supremum_view)->Arg(128 * 128)->Arg(256*256)->Unit(benchmark::kMillisecond);
//BENCHMARK_REGISTER_F(ArithmeticsFixture, supremum_strided_view)->Arg(128 * 128)->Arg(256 * 256)->Unit(benchmark::kMillisecond);
//
//BENCHMARK_REGISTER_F(ArithmeticsFixture, add_const_view)->Arg(128 * 128)->Arg(256 * 256)->Unit(benchmark::kMillisecond);
//BENCHMARK_REGISTER_F(ArithmeticsFixture, add_const_strided_view)->Arg(128 * 128)->Arg(256 * 256)->Unit(benchmark::kMillisecond);

BENCHMARK_REGISTER_F(ArithmeticsFixture, supremum_view)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(ArithmeticsFixture, supremum_strided_view)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond);

BENCHMARK_REGISTER_F(ArithmeticsFixture, add_const_view)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(ArithmeticsFixture, add_const_strided_view)->Arg(16 * 16)->Arg(32 * 32)->Arg(64 * 64)->Unit(benchmark::kMicrosecond);