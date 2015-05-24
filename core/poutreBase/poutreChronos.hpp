
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_CHRONOS_HPP__
#define POUTRE_CHRONOS_HPP__

/**
 * @file   poutreChronos.hpp
 * @author Thomas Retornaz
 * @brief  Define helper objects around std::chrono facilities
 * 
 * 
 */

#include <chrono>
#include <iostream>
#include <sstream>

#ifndef POUTRE_BASE_HPP__
#include <poutreBase/poutreBase.hpp>
#endif

namespace poutre
  {
/**
 * @addtogroup timer_group Helper objects around std::chrono facilities
 *
 *@{
 */     

/**
 * @brief Simple Timer class (wrapping std::chronos) with classical
 * start,stop interface and serialisation capabilites 
 */
class /*BASE_API*/ Timer
    {
    public:
      using  high_resolution_clock = std::chrono::high_resolution_clock;
      using  float_milliseconds = std::chrono::duration < float, std::milli > ; //switch to other duration through template<rep,duration> ?
      using  timerep = float_milliseconds::rep; //todo use
                                                //decltype(auto)
       
      Timer(void) :m_start(), m_accu(0), m_nbiter(0)
        {
        }
      ~Timer(void) POUTRE_NOEXCEPT
        {
        }
        void Start( ) POUTRE_NOEXCEPT
        {
        m_nbiter++;
        m_start = high_resolution_clock::now();
        }
          void Stop( ) POUTRE_NOEXCEPT
      {
      m_accu += std::chrono::duration_cast<float_milliseconds>(high_resolution_clock::now() - m_start).count();
      }

        const timerep GetCumulativeTime( ) const POUTRE_NOEXCEPT
      {
      return m_accu;
      }

        const std::size_t NbIter( ) const POUTRE_NOEXCEPT
      {
      return m_nbiter;
      }

        const timerep GetMeanTime( ) const POUTRE_NOEXCEPT
      {
      if (m_nbiter == 0) { return static_cast<timerep>(0); }
      return m_accu / static_cast<timerep>(m_nbiter);
      }

        const std::string to_str( ) const POUTRE_NOEXCEPT
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

/**
 * @brief Scoped (RAII) timer, start and stop of embeded timer are automatically triggered 
 *
 * @todo think about conccurrency issues
 */
class /*BASE_API*/ ScopedTimer//! @} doxygroup: timer_group
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

/**
*@}
*/ 
#endif //POUTRE_CHRONOS_HPP__
