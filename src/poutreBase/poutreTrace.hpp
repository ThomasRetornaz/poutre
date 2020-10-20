
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

#include <fstream>
#include <ostream>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <poutreBase/poutreConfig.hpp>

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
        static GlobalLogger &get()
        {
            static GlobalLogger instance;
            // Instantiated on first use.
            return instance;
        }
        GlobalLogger(const GlobalLogger &) = delete;
        GlobalLogger &operator=(const GlobalLogger &) = delete;
        GlobalLogger(GlobalLogger &&other) = delete;
        GlobalLogger &operator=(GlobalLogger &&other) = delete;

      private:
        std::shared_ptr<spdlog::logger> m_innerlogger;
        //! Private ctor
        GlobalLogger() : m_innerlogger(spdlog::stdout_color_st("global_console_logger"))
        {
            (*m_innerlogger).set_level(spdlog::level::debug);
        }
    };
#endif // DOXYGEN_SHOULD_SKIP_THIS

    /** @name Global logger macros
     */
    /**@{*/
#define POUTRE_INFO(MSG) poutre::GlobalLogger::get()->info(poutre::format("INFO: {} at {} {}", MSG, __FILE__, __LINE__))
#define POUTRE_ERR(MSG) poutre::GlobalLogger::get()->err(poutre::format("ERROR: {} at {} {}", MSG, __FILE__, __LINE__))
#define POUTRE_CRITICAL(MSG)                                                                                           \
    poutre::GlobalLogger::get()->critical(poutre::format("CRITICAL: {} at {} {}", MSG, __FILE__, __LINE__))
#define POUTRE_TRACE(MSG)                                                                                              \
    poutre::GlobalLogger::get()->trace(poutre::format("TRACE: {} at {} {}", MSG, __FILE__, __LINE__))
#define POUTRE_WARN(MSG) poutre::GlobalLogger::get()->warn(poutre::format("WARN: {} at {} {}", MSG, __FILE__, __LINE__))
#define POUTRE_DEBUG(MSG)                                                                                              \
    poutre::GlobalLogger::get()->debug(poutre::format("DEBUG: {} at {} {}", MSG, __FILE__, __LINE__))

    /**@}*/

} // namespace poutre

// !@} doxygroup: trace_group

#endif // POUTRE_TRACE__HPP__
