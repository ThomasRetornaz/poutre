#ifndef POUTRE_REGISTRATOR__HPP__
#define POUTRE_REGISTRATOR__HPP__


#ifndef POUTREBASE_HPP__
#include <poutreBase/poutreBase.hpp>
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
      typedef std::unordered_map< std::string, std::function<Interface*(void)>> KeyFactoryMap;
      //typedef std::unordered_map< std::string, std::function<std::unique_ptr<Interface>(void)>> KeyFactoryMap;
      static Registrar& getInstance()
        {
        static Registrar instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
        }
    private:
      Registrar(){};
      Registrar(const Registrar&);
      Registrar& operator=(const Registrar&);
      KeyFactoryMap m_keyfactory_map;
    public:
      //void Register(const std::string& key, std::function<std::unique_ptr<Interface>(void)> ImplFactoryFunction)
      void Register(const std::string& key, std::function<Interface*(void)> ImplFactoryFunction)
        {
        auto it = m_keyfactory_map.find(key);
        if (it != m_keyfactory_map.end()) throw(std::runtime_error("Registrar::Add key already used"));
        m_keyfactory_map[key] = ImplFactoryFunction;
        }
    };
  }
#endif


