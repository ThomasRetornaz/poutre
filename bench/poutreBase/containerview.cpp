
#include "benchmark/benchmark.h"
#include <poutreBase/poutreContainerView.hpp>

#include <cstdlib>
#include <vector>

namespace {

    decltype(auto) ConstructVector(size_t size) 
    {
        std::vector<unsigned int> m_vect;
        m_vect.reserve(size);
        /*for (auto i = 0; i < size; ++i) {
            m_vect.push_back(rand() % size);
        }*/
        return m_vect;
    }

}  // namespace

class ViewOnVect1DFixture : public ::benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& st) {
        m_vect = ConstructVector(st.range(0));
    }
    void TearDown(const ::benchmark::State&) {
        m_vect.clear();
    }
    std::vector<unsigned int> m_vect;
};

//ten times slower than common iterator
BENCHMARK_DEFINE_F(ViewOnVect1DFixture, BoundIterator)(benchmark::State& state) {
    const int size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {            
            //benchmark::DoNotOptimize(m_vect[i]);
            auto view = poutre::array_view<unsigned int,1>(m_vect);
            auto bnd = view.bound();
            auto it = bnd.begin();
            auto itend = bnd.end();
            for (; it != itend; ++it)
            {
                benchmark::DoNotOptimize(view[*it]);
                //auto val= view[*it];
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK_DEFINE_F(ViewOnVect1DFixture, Iterator)(benchmark::State& state) {
    const int size = state.range(0);
    while (state.KeepRunning()) {
        for (int i = 0; i < size; ++i) {           
            auto it = m_vect.begin();
            auto itend = m_vect.end();
            for (; it != itend; ++it)
            {
                benchmark::DoNotOptimize(*it);
                //auto val = *it;
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK_REGISTER_F(ViewOnVect1DFixture, Iterator)->Range(1 << 3, 1 << 12);
BENCHMARK_REGISTER_F(ViewOnVect1DFixture, BoundIterator)->Range(1 << 3, 1 << 12);
