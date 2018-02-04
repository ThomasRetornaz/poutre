// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutreBase/poutreJsonSerialization.hpp>
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
