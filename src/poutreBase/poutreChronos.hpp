
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
     * @addtogroup timer_group Timing facilities
     * @ingroup poutre_base
     *@{
     */

     /**
      * @brief Simple Timer class (wrapping std::chronos) with classical
      * start,stop interface and serialisation capabilites
      * @todo think about multi-threading
      */
    class /*BASE_API*/ Timer //see http://en.cppreference.com/w/cpp/chrono/c/clock
    {
    public:
        using  high_resolution_clock = std::chrono::high_resolution_clock;
        using  double_milliseconds = std::chrono::duration < double, std::milli >; //switch to other duration through template<rep,duration> ?
        using  timerep = double_milliseconds::rep; //todo use
                                                  //decltype(auto)
        //!ctor 
        Timer(void) :m_start(), m_accu(), m_start_cputime(0), m_accu_cputime(0), m_nbiter(0)
        {
        }

        //!dor 
        ~Timer(void) POUTRE_NOEXCEPT
        {
        }

        //!Start the timing action
        void Start() POUTRE_NOEXCEPT
        {
            m_nbiter++;
            m_start = high_resolution_clock::now();
            m_start_cputime = std::clock();
        }
        //!Stop the timing action
        void Stop() POUTRE_NOEXCEPT
        {
            m_accu += std::chrono::duration_cast<double_milliseconds>(high_resolution_clock::now() - m_start).count();
            m_accu_cputime += std::clock() - m_start_cputime;
        }

        //!Grab wall time accumulated in ms
        const timerep GetCumulativeTime() const POUTRE_NOEXCEPT
        {
            return m_accu;
        }

        //!Grab wall mean time of iteration in ms
        const timerep GetMeanTime() const POUTRE_NOEXCEPT
        {
            if (m_nbiter == 0) { return static_cast<timerep>(0); }
            return GetCumulativeTime() / static_cast<timerep>(m_nbiter);
        }

        //!Grab cpu time accumulated in ms
        const timerep GetCumulativeCPUTime() const POUTRE_NOEXCEPT
        {
            return static_cast<timerep>(m_accu_cputime/ CLOCKS_PER_SEC)*1000;
        }

        //!Grab wall mean time of iteration in ms
        const timerep GetMeanCpuTime() const POUTRE_NOEXCEPT
        {
            if (m_nbiter == 0) { return static_cast<timerep>(0); }
            return static_cast<timerep>(GetCumulativeCPUTime()) / static_cast<timerep>(m_nbiter);
        }

        //!Grab number off triggered start
        const std::size_t NbIter() const POUTRE_NOEXCEPT
        {
            return m_nbiter;
        }

        //!String serialization
        const std::string to_str() const POUTRE_NOEXCEPT
        {
            std::stringstream sstr;
            sstr << "Cumulative wall time  in ms: " << GetCumulativeTime() << "\n Nb iter " << m_nbiter << "\n Mean wall time in ms" << GetMeanTime();
            sstr << "Cumulative cpu time  in ms: " << GetCumulativeCPUTime() << "\n Nb iter " << m_nbiter << "\n Mean cpu time in ms" << GetMeanCpuTime();
            return sstr.str();
        }

        //!Reset the chrono
        void Reset()
        {
            m_accu = 0.;
            m_start_cputime = 0;
            m_accu_cputime = 0;
            m_nbiter = 0;
        }

    private:
        friend std::ostream & operator<<(std::ostream &os, Timer& timer);
        std::chrono::high_resolution_clock::time_point m_start; //!start wall time
        timerep m_accu; //! accumulate wall time over all iteration 
        std::clock_t m_start_cputime; //!start cpu time
        std::clock_t m_accu_cputime; //! accumulate cpu time
        std::size_t m_nbiter; //! nb iteration


    };

    //!Timer stream serialization
    std::ostream & operator<<(std::ostream &os, Timer& timer)
    {
        os << timer.to_str();
        return os;
    }

    /**
     * @brief Scoped (RAII) timer, start and stop of embedded timer are automatically triggered
     *
     * @todo think about multi-threading
     */
    class /*BASE_API*/ ScopedTimer
    {
    private:
        Timer& m_timer; //!Inner timer

    public:
        //!ctor
        ScopedTimer(Timer& itimer) :m_timer(itimer)
        {
            m_timer.Start();
        }
        //!dtor
        ~ScopedTimer(void)
        {
            m_timer.Stop();
        }
    };
}

/**
//! @} doxygroup: timer_group
*/
#endif //POUTRE_CHRONOS_HPP__
