#ifndef POUTREJSONSERIALIZATION_HPP__
#define POUTREJSONSERIALIZATION_HPP__

#ifndef POUTREBASE_HPP__
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
#endif //POUTREJSONSERIALIZATION_HPP__