#include <poutreJsonSerialization.hpp>
#include <json/json.h>

namespace poutre
  {

  bool JsonSerializer::Serialize(InterfaceJsonSerializable* iptrJSObj, std::string& ostr)
    {
    if (iptrJSObj == nullptr)
      return false;

    Json::Value serializeRoot;
    iptrJSObj->Serialize(serializeRoot);

    Json::StyledWriter writer;
    ostr = writer.write(serializeRoot);

    return true;
    }

  bool JsonSerializer::Deserialize(InterfaceJsonSerializable* iptrJSObj, const std::string& istr)
    {
    if (iptrJSObj == nullptr)
      return false;

    Json::Value deserializeRoot;
    Json::Reader reader;

    if (!reader.parse(istr, deserializeRoot))
      return false;

    iptrJSObj->Deserialize(deserializeRoot);

    return true;
    }

  }
