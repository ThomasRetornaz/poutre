
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_REGISTRATOR__HPP__
#define POUTRE_REGISTRATOR__HPP__


#ifndef POUTRE_BASE_HPP__
#include <poutreBase/poutreBase.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif

#include <unordered_map>
#include <string>
#include <functional>

/*!@file
 * Define static objetc which store map to factories
 */

namespace poutre
  {


  template<class Interface>
  class Registrar
    {
    public:
      using  KeyFactoryMap = std::unordered_map< std::string, std::function<Interface*(void)>>;
      //typedef std::unordered_map< std::string, std::function<std::unique_ptr<Interface>(void)>> KeyFactoryMap;
      static Registrar& getInstance()
        {
        static Registrar instance; // Guaranteed to be destroyed. AND http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
        // Instantiated on first use.
        return instance;
        }

      Registrar(const Registrar&) = delete;
      Registrar& operator=(const Registrar&) = delete;
      Registrar(Registrar&& other) = delete;
      Registrar& operator= (Registrar&& other) = delete;

    private:
      Registrar(){};


      KeyFactoryMap m_keyfactory_map;
    public:
      //void Register(const std::string& key, std::function<std::unique_ptr<Interface>(void)> ImplFactoryFunction)
      void Register(const std::string& key, std::function<Interface*(void)> ImplFactoryFunction)
        {
        auto it = m_keyfactory_map.find(key);
        if (it != m_keyfactory_map.end()) POUTRE_RUNTIME_ERROR("Registrar::Add key already used");
        m_keyfactory_map[key] = ImplFactoryFunction;
        }
    };
  }
#endif //POUTRE_REGISTRATOR__HPP__


