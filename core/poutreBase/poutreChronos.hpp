#ifndef POUTRECHRONOS_HPP__
#define POUTRECHRONOS_HPP__

#include <chrono>
#include <iostream>
#include <sstream>

#ifndef POUTREBASE_HPP__
#include <poutreBase/poutreBase.hpp>
#endif

namespace poutre
  {
  
  class /*BASE_API*/ Timer
    {
    public:
      using  high_resolution_clock = std::chrono::high_resolution_clock;
      using  float_milliseconds = std::chrono::duration<float, std::milli>; //switch to other duration through template<rep,duration> ?
      using  timerep = float_milliseconds::rep; //todo use decltype(auto)
      Timer(void) :m_start(), m_accu(0), m_nbiter(0)
        {
        }
      ~Timer(void)
        {
        }
      void Start()
        {
        m_nbiter++;
        m_start = high_resolution_clock::now();
        }
      void Stop()
        {
        m_accu += std::chrono::duration_cast<float_milliseconds>(high_resolution_clock::now() - m_start).count();
        }

      const timerep GetCumulativeTime() const
        {
        return m_accu;
        }

      const std::size_t NbIter() const
        {
        return m_nbiter;
        }

      const timerep GetMeanTime() const
        {
        if (m_nbiter == 0) { return static_cast<timerep>(0); }
        return m_accu / static_cast<timerep>(m_nbiter);
        }

      const std::string to_str() const
        {
        std::stringstream sstr;
        sstr << "Cumulative time  in ms: " << GetCumulativeTime() << "\n Nb iter " << m_nbiter << "\n Mean time in ms" << GetMeanTime();
        return sstr.str();
        }
    private:
      friend std::ostream & operator<<(std::ostream &os, Timer& timer);
      std::chrono::high_resolution_clock::time_point m_start;
      timerep m_accu;
      std::size_t m_nbiter;
    };


  std::ostream & operator<<(std::ostream &os, Timer& timer)
    {      
     os << timer.to_str();
     return os;
    }

  //@todo think about conccurrency issues
  class /*BASE_API*/ ScopedTimer
    {
    private:
      Timer& m_timer;

    public:
      ScopedTimer(Timer& itimer) :m_timer(itimer)
        {
        m_timer.Start();
        }
      ~ScopedTimer(void)
        {
        m_timer.Stop();
        }
    };
  }
#endif //POUTRECHRONOS