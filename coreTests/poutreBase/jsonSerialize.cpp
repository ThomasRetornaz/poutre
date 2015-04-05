#include "main.hpp"

#include <iostream>
#include <poutreBase/poutreJsonSerialization.hpp>
#include <json/json.h>

class TestClassA : public poutre::InterfaceJsonSerializable
  {
  public:
    TestClassA(void);
    virtual ~TestClassA(void);
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
    int           m_nTestInt;
    double        m_fTestFloat;
    std::string   m_TestString;
    bool          m_bTestBool;
    std::vector<float> m_array;
  };

TestClassA::TestClassA(void) : m_nTestInt(42), m_fTestFloat(3.14159), m_TestString("foo"), m_bTestBool(true), m_array()
  {
  m_array = { 1.1f, 2.1f, 3.1f };
  }
TestClassA::~TestClassA(void)
  {

  }

void TestClassA::Deserialize(Json::Value& root)
  {
   // deserialize primitives
    m_nTestInt = root.get("m_nTestInt", 0).asInt();
    m_fTestFloat = root.get("m_fTestFloat", 0.0).asDouble();
    m_TestString = root.get("m_TestString", "").asString();
    m_bTestBool = root.get("m_bTestBool", false).asBool();
    const Json::Value& jsonarray = root.get("m_array",Json::Value(Json::arrayValue));
    m_array.clear();
    m_array.reserve(jsonarray.size());
    for (const auto& val: jsonarray)
      {
      m_array.push_back(val.asFloat());
      }
  }

void TestClassA::Serialize(Json::Value& root)
  {
  // serialize primitives
  root["m_nTestInt"] = m_nTestInt;
  root["m_fTestFloat"] = m_fTestFloat;
  root["m_TestString"] = m_TestString;
  root["m_bTestBool"] = m_bTestBool;
  //const Json::Value& array = root["m_array"];
  for (const auto& var: m_array)
    {
    root["m_array"].append(var).asFloat();
    }
  }


BOOST_AUTO_TEST_SUITE(json)

BOOST_AUTO_TEST_CASE(serializer)
  {
  TestClassA testClass;
  std::string output;
  poutre::JsonSerializer::Serialize(&testClass, output);
  std::string expected= "\
{\n\
   \"m_TestString\" : \"foo\",\n\
   \"m_array\" : [ 1.100000023841858, 2.099999904632568, 3.099999904632568 ],\n\
   \"m_bTestBool\" : true,\n\
   \"m_fTestFloat\" : 3.14159,\n\
   \"m_nTestInt\" : 42\n\
}\n";
  BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), output.begin(), output.end());
  std::cout << "testClass Serialized Output\n" << output << "\n\n\n";
  }

BOOST_AUTO_TEST_SUITE_END()



