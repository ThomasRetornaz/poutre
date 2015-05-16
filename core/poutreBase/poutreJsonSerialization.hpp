
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_JSON_SERIALIZATION_HPP__
#define POUTRE_JSON_SERIALIZATION_HPP__

#ifndef POUTRE_BASE_HPP__
#include <poutreBase/poutreBase.hpp>
#endif

#ifndef JSON_FORWARD_AMALGATED_H_INCLUDED
#include <json/json-forwards.h>
#endif

#ifndef BOOST_NONCOPYABLE_HPP_INCLUDED
#include <boost/noncopyable.hpp>
#endif

#include <string>

namespace poutre
  {
  //completely grabed from http://www.danielsoltyka.com/programming/2011/04/15/simple-class-serialization-with-jsoncpp/ tutorial :)

  class BASE_API InterfaceJsonSerializable
    {
    public:
      virtual ~InterfaceJsonSerializable(void) {};
      virtual void Serialize(Json::Value& root) = 0;
      virtual void Deserialize(Json::Value& root) = 0;
    };

  class BASE_API JsonSerializer : boost::noncopyable
    {
    public:
      static bool Serialize(InterfaceJsonSerializable* iptrJSObj, std::string& ostr);
      static bool Deserialize(InterfaceJsonSerializable* iptrJSObj, const std::string& istr);
    };

  }
#endif //POUTRE_JSON_SERIALIZATION_HPP__
