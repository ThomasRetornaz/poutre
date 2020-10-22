#include <poutreBase/include/poutreThreading.hpp>

#include <cstdlib> // getenv

namespace poutre
{
  namespace thread
  {
    ScopedForceNbThreads::ScopedForceNbThreads(unsigned int nbThread) : nbThreadToRestore(POUTRE_NUM_THREADS)
    {
      if( nbThread == 0 )
        throw std::runtime_error("Number of thread must be a positive non zero value");
      std::lock_guard<std::mutex> lock(m_mutex);
      POUTRE_NUM_THREADS = nbThread;
      if( std::getenv("OMP_NUM_THREADS") == NULL )
      {
        omp_set_num_threads(POUTRE_NUM_THREADS);
      }
    }
    ScopedForceNbThreads::~ScopedForceNbThreads()
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      POUTRE_NUM_THREADS = nbThreadToRestore;
      if( std::getenv("OMP_NUM_THREADS") == NULL )
      {
        omp_set_num_threads(POUTRE_NUM_THREADS);
      }
    }
  } // namespace thread
} // namespace poutre