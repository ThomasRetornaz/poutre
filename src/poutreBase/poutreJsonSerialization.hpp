
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_JSON_SERIALIZATION_HPP__
#define POUTRE_JSON_SERIALIZATION_HPP__

/**
 * @file   poutreJsonSerialization.hpp
 * @author Thomas Retornaz
 * @brief  Define json serialization interface
 *
 *
 */

#ifndef POUTRE_BASE_HPP__
#include <poutreBase/poutreBase.hpp>
#endif

#ifndef JSON_FORWARDS_H_INCLUDED
#include <json/forwards.h>
#endif

#ifndef BOOST_NONCOPYABLE_HPP_INCLUDED
#include <boost/noncopyable.hpp>
#endif

#include <string>

namespace poutre
{
    /*!
     * @defgroup json_group Define serialisations process in json
     * @ingroup poutre_base_group
     * @{
     */

    // completely grabed from
    // http://www.danielsoltyka.com/programming/2011/04/15/simple-class-serialization-with-jsoncpp/ tutorial :)

    /**
     * @brief Interface class.
     */
    class BASE_API InterfaceJsonSerializable
    {
      public:
        virtual ~InterfaceJsonSerializable(void) = default;
        virtual void Serialize(Json::Value &root) = 0;
        virtual void Deserialize(Json::Value &root) = 0;
    };

    /**
     * @brief Default json serailizer object
     */
    class BASE_API JsonSerializer
    {
      public:
        JsonSerializer() = default;
        JsonSerializer(const JsonSerializer &) = delete; // cannot be copy constructed
        JsonSerializer &operator=(const JsonSerializer &orig) = delete;

        static bool Serialize(InterfaceJsonSerializable *iptrJSObj, std::string &ostr);
        static bool Deserialize(InterfaceJsonSerializable *iptrJSObj, const std::string &istr);
    };
    /**
    //! @} doxygroup: json_group
    */

} // namespace poutre
#endif // POUTRE_JSON_SERIALIZATION_HPP__
