#include <PoutreBase/poutreChronos.hpp>

namespace poutre
{
	//!ctor 
	Timer::Timer(void) :m_start(), m_accu(), m_start_cputime(0), m_accu_cputime(0), m_nbiter(0)
	{
	}

	//!dor 
	Timer::~Timer(void) POUTRE_NOEXCEPT
	{
	}

	//!Start the timing action
	void Timer::Start() POUTRE_NOEXCEPT
	{
		m_nbiter++;
		m_start = high_resolution_clock::now();
		m_start_cputime = std::clock();
	}
	//!Stop the timing action
	void Timer::Stop() POUTRE_NOEXCEPT
	{
		m_accu += std::chrono::duration_cast<double_milliseconds>(high_resolution_clock::now() - m_start).count();
		m_accu_cputime += std::clock() - m_start_cputime;
	}

	//!Grab wall time accumulated in ms
	const Timer::timerep Timer::GetCumulativeTime() const POUTRE_NOEXCEPT
	{
		return m_accu;
	}

	//!Grab wall mean time of iteration in ms
	const Timer::timerep Timer::GetMeanTime() const POUTRE_NOEXCEPT
	{
		if (m_nbiter == 0) { return static_cast<timerep>(0); }
		return GetCumulativeTime() / static_cast<timerep>(m_nbiter);
	}

	//!Grab cpu time accumulated in ms
	const Timer::timerep Timer::GetCumulativeCPUTime() const POUTRE_NOEXCEPT
	{
		return static_cast<timerep>(m_accu_cputime / CLOCKS_PER_SEC) * 1000;
	}

	//!Grab wall mean time of iteration in ms
	const Timer::timerep Timer::GetMeanCpuTime() const POUTRE_NOEXCEPT
	{
		if (m_nbiter == 0) { return static_cast<timerep>(0); }
		return static_cast<timerep>(GetCumulativeCPUTime()) / static_cast<timerep>(m_nbiter);
	}

	//!Grab number off triggered start
	const std::size_t Timer::NbIter() const POUTRE_NOEXCEPT
	{
		return m_nbiter;
	}

	//!String serialization
	const std::string Timer::to_str() const POUTRE_NOEXCEPT
	{
		std::stringstream sstr;
		sstr << "Cumulative wall time in ms: " << GetCumulativeTime() << "\n Nb iter " << m_nbiter << "\n Mean wall time in ms: " << GetMeanTime();
		sstr << "Cumulative cpu time in ms: " << GetCumulativeCPUTime() << "\n Nb iter " << m_nbiter << "\n Mean cpu time in ms: " << GetMeanCpuTime();
		return sstr.str();
	}

	//!Reset the chrono
	void Timer::Reset() POUTRE_NOEXCEPT
	{
		m_accu = 0.;
		m_start_cputime = 0;
		m_accu_cputime = 0;
		m_nbiter = 0;
	}

	std::ostream & poutre::operator<<(std::ostream &os, Timer& timer)
	{
		os << timer.to_str();
		return os;
	}

	//!ctor
	ScopedTimer::ScopedTimer(Timer& itimer) :m_timer(itimer)
	{
		m_timer.Start();
	}
	//!dtor
	ScopedTimer::~ScopedTimer(void)
	{
		m_timer.Stop();
	}

}