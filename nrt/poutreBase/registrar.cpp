// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"

#include <iostream>
#include <poutreBase/poutreRegistrar.hpp>

class Base
  {
  public:
    Base(void){};
    virtual ~Base(void){};
    //static std::unique_ptr<Base> Create() { return std::make_unique<Base>(); }
    static Base* Create() { return new Base(); }
    virtual std::string WhoIam(void)  { return "Base"; }
  };

class Derived : Base
  {
  public:
    Derived(void):Base(){};
    virtual ~Derived(void){};
    //static std::unique_ptr<Base> Create() { return std::make_unique<Derived>(); }
    static Base* Create() { return new Derived(); }
    virtual std::string WhoIam(void) override { return "Derived"; }
  };

BOOST_AUTO_TEST_SUITE(registrar)

BOOST_AUTO_TEST_CASE(dummy)
  {
  poutre::Registrar<Base>::getInstance().Register("Base", Base::Create);
  poutre::Registrar<Base>::getInstance().Register("Derived", Derived::Create);

  }

BOOST_AUTO_TEST_SUITE_END()



