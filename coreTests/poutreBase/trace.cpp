#include "main.hpp"

#include <iostream>
#include <poutreBase/poutreTrace.hpp>

BOOST_AUTO_TEST_SUITE(trace)

BOOST_AUTO_TEST_CASE(dummy)
  {
  
    BOOST_LOG_SEV(poutre::global_logger::get(), boost::log::trivial::trace) << "Hello, world!";
    POUTRE_TRACE << "trace";
    POUTRE_DEBUG << "debug";
    POUTRE_INFO << "info";
    POUTRE_WARNING << "warning";
    POUTRE_ERROR << "error";
    POUTRE_FATAL << "fatal";
    
}

BOOST_AUTO_TEST_SUITE_END()



