// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreBase/poutreTrace.hpp>

#include <fstream>
#include <ostream>

namespace poutre
{
  std::ostream &operator<<(std::ostream &os, GlobalLogger::LoggerLevel level) // NOLINT(misc-unused-parameters)
  {
    switch( level )
    {
    case GlobalLogger::LoggerLevel::trace: os << "trace"; break;
    case GlobalLogger::LoggerLevel::critical: os << "critical"; break;
    case GlobalLogger::LoggerLevel::debug: os << "debug"; break;
    case GlobalLogger::LoggerLevel::err: os << "err"; break;
    case GlobalLogger::LoggerLevel::info: os << "info"; break;
    case GlobalLogger::LoggerLevel::warn: os << "warn"; break;
    default: os << "Unknown GlobalLogger::LoggerLevel"; break;
    }
    return os;
  }
} // namespace poutre