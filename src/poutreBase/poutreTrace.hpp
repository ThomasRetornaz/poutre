
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_TRACE__HPP__
#define POUTRE_TRACE__HPP__

/**
 * @file   poutreTrace.hpp
 * @author Thomas Retornaz
 * @brief  Trace capabilities
 *
 *
 */

#include <poutreBase/poutreBase.hpp>
#include <poutreBase/poutreConfig.hpp>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <ostream>

/**
 * @addtogroup trace_group Trace facilities
 * @ingroup poutre_base_group
 *@{
 */
namespace poutre
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  class GlobalLogger
  {
    public:
    enum class LoggerLevel
    {
      trace,
      debug,
      info,
      warn,
      err,
      critical
      /* off ,
       n_levels*/
    };
    static GlobalLogger &get()
    {
      static GlobalLogger instance;
      // Instantiated on first use.
      return instance;
    }
    GlobalLogger(const GlobalLogger &) = delete;
    GlobalLogger &operator=(const GlobalLogger &) = delete;
    GlobalLogger(GlobalLogger &&other)            = delete;
    GlobalLogger &operator=(GlobalLogger &&other) = delete;

    void setLoggerLevel(LoggerLevel l)
    {
      switch( l )
      {
      case LoggerLevel::debug: (*m_innerlogger).set_level(spdlog::level::debug); break;
      case LoggerLevel::info: (*m_innerlogger).set_level(spdlog::level::info); break;
      case LoggerLevel::critical: (*m_innerlogger).set_level(spdlog::level::critical); break;
      case LoggerLevel::err: (*m_innerlogger).set_level(spdlog::level::err); break;
      case LoggerLevel::trace: (*m_innerlogger).set_level(spdlog::level::trace); break;
      case LoggerLevel::warn: (*m_innerlogger).set_level(spdlog::level::warn); break;
      default: POUTRE_RUNTIME_ERROR(poutre::format("Logger unsupported enum case {}", boost::lexical_cast<std::string>(l)));
      }
    }
    void call(const char *MSG, LoggerLevel l = LoggerLevel::debug)
    {
      switch( l )
      {
      case LoggerLevel::debug: m_innerlogger->debug(MSG); break;
      case LoggerLevel::info: m_innerlogger->info(MSG); break;
      case LoggerLevel::critical: m_innerlogger->critical(MSG); break;
      case LoggerLevel::err: m_innerlogger->error(MSG); break;
      case LoggerLevel::trace: m_innerlogger->trace(MSG); break;
      case LoggerLevel::warn: m_innerlogger->warn(MSG); break;
      default: POUTRE_RUNTIME_ERROR(poutre::format("Logger unsupported enum case {}", boost::lexical_cast<std::string>(l)));
      }
    }

    private:
    std::shared_ptr<spdlog::logger> m_innerlogger;
    //! Private ctor
    GlobalLogger() : m_innerlogger(spdlog::stdout_color_st("global_console_logger"))
    {
      (*m_innerlogger).set_level(spdlog::level::debug);
    }
  };

  //! operator<< for GlobalLogger::LoggerLevel
  BASE_API std::ostream &operator<<(std::ostream &, GlobalLogger::LoggerLevel level);
#endif // DOXYGEN_SHOULD_SKIP_THIS

  /** @name Global logger macros
   */
  /**@{*/
#define POUTRE_DEBUG(MSG)                                                                                                   \
  poutre::GlobalLogger::get().call(poutre::format("DEBUG: {} at {} {}", MSG, __FILE__, __LINE__).c_str(),                   \
                                   GlobalLogger::LoggerLevel::debug)
#define POUTRE_INFO(MSG)                                                                                                    \
  poutre::GlobalLogger::get().call(poutre::format("INFO: {} at {} {}", MSG, __FILE__, __LINE__).c_str(),                    \
                                   GlobalLogger::LoggerLevel::info)
#define POUTRE_CRITICAL(MSG)                                                                                                \
  poutre::GlobalLogger::get().call(poutre::format("CRITICAL: {} at {} {}", MSG, __FILE__, __LINE__).c_str(),                \
                                   GlobalLogger::LoggerLevel::critical)
#define POUTRE_ERROR(MSG)                                                                                                   \
  poutre::GlobalLogger::get().call(poutre::format("ERROR: {} at {} {}", MSG, __FILE__, __LINE__).c_str(),                   \
                                   GlobalLogger::LoggerLevel::err)
#define POUTRE_TRACE(MSG)                                                                                                   \
  poutre::GlobalLogger::get().call(poutre::format("TRACE: {} at {} {}", MSG, __FILE__, __LINE__).c_str(),                   \
                                   GlobalLogger::LoggerLevel::trace)
#define POUTRE_WARN(MSG)                                                                                                    \
  poutre::GlobalLogger::get().call(poutre::format("WARNING: {} at {} {}", MSG, __FILE__, __LINE__).c_str(),                 \
                                   GlobalLogger::LoggerLevel::warn)
  /**@}*/

} // namespace poutre

// !@} doxygroup: trace_group

#endif // POUTRE_TRACE__HPP__
