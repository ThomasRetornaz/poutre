#ifndef POUTRE_TRACE__HPP__
#define POUTRE_TRACE__HPP__



//#define BOOST_LOG_DYN_LINK 1 // necessary when linking the boost_log library dynamically
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <ostream>
#include <boost/format.hpp>

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

/*!@file
 * Common
 */

namespace poutre
  {
  namespace logging = boost::log;
  namespace src = boost::log::sources;
  namespace expr = boost::log::expressions;
  namespace sinks = boost::log::sinks;
  namespace attrs = boost::log::attributes;


#define POUTRE_TRACE   BOOST_LOG_SEV(global_logger::get(),boost::log::trivial::trace)
#define POUTRE_DEBUG   BOOST_LOG_SEV(global_logger::get(),boost::log::trivial::debug)
#define POUTRE_INFO    BOOST_LOG_SEV(global_logger::get(),boost::log::trivial::info)
#define POUTRE_WARNING BOOST_LOG_SEV(global_logger::get(),boost::log::trivial::warning)
#define POUTRE_ERROR   BOOST_LOG_SEV(global_logger::get(),boost::log::trivial::error)
#define POUTRE_FATAL   BOOST_LOG_SEV(global_logger::get(),boost::log::trivial::fatal)

  // just log messages with severity >= SEVERITY_THRESHOLD are written
#define SEVERITY_THRESHOLD logging::trivial::warning

  // Initializing global boost::log logger
  typedef boost::log::sources::severity_channel_logger_mt<SEVERITY_THRESHOLD, std::string> global_logger_type;

  /*BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(global_logger, global_logger_type)
    {
    return global_logger_type(boost::log::keywords::channel = "global_logger");
    }*/

    BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
    BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
    BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::trivial::severity_level)



  // register a global logger
  //BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)
  BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(global_logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)
    {
    src::severity_logger_mt<boost::log::trivial::severity_level> logger;

    // add attributes
    logger.add_attribute("LineID", attrs::counter<unsigned int>(1));     // lines are sequentially numbered
    logger.add_attribute("TimeStamp", attrs::local_clock());             // each log line gets a timestamp

    // add a text sink
    typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

    // add a logfile stream to our sink
    //sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(LOGFILE));

    // add "console" output stream to our sink
    sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
    // flush with caution
    sink->locked_backend()->auto_flush(false);

    // specify the format of the log message
    logging::formatter formatter = expr::stream
      << std::setw(7) << std::setfill('0') << line_id << std::setfill(' ') << " | "
      << expr::format_date_time(timestamp, "%Y-%m-%d, %H:%M:%S.%f") << " "
      << "[" << logging::trivial::severity << "]"
      << " - " << expr::smessage;
    sink->set_formatter(formatter);

    // only messages with severity >= SEVERITY_THRESHOLD are written
    sink->set_filter(severity >= SEVERITY_THRESHOLD);

    // "register" our sink
    logging::core::get()->add_sink(sink);

    return logger;
    }

    // ===== log macros =====



#define POUTRE_RUNTIME_ERROR( MSG ) throw std::runtime_error(boost::format("Throw %s at %s(%lu)") % MSG % __FILE__ % __LINE__);
#define POUTRE_CHECK( CONDITION, MSG )           \
       {                                             \
      if ( !( CONDITION ) )                      \
              {                                          \
         POUTRE_RUNTIME_ERROR( MSG );            \
              }                                          \
       }

#ifdef _DEBUG
#if defined(_MSC_VER)
#define POUTRE_ASSERTCHECK( CONDITION, MSG ){    \
    if ( !( CONDITION ) )                        \
            {                                          \
         __debugbreakpoint();                    \
            }                                          \
       }
#else
#define POUTRE_ASSERTCHECK( CONDITION, MSG ){    \
    if ( !( CONDITION ) )                        \
            {                                          \
         __builtin_trap();                       \
            }                                          \
       }
#endif
#else
#define POUTRE_ASSERTCHECK( CONDITION, MSG )
#endif





  } //namespace poutre

#endif //POUTRE_TRACE__HPP__


