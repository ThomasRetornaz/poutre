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
#include <thread>
#include <poutreBase/poutreChronos.hpp>

BOOST_AUTO_TEST_SUITE(chronos)

BOOST_AUTO_TEST_CASE(init)
  {
  poutre::Timer timer;
  BOOST_CHECK_EQUAL(timer.GetMeanTime(), 0);
  BOOST_CHECK_EQUAL(timer.GetCumulativeTime(), 0);
  BOOST_CHECK_EQUAL(timer.NbIter(), 0);
  //std::cout <<"\n"<< timer<<"\n";
  }

BOOST_AUTO_TEST_CASE(scoped)
  {
  poutre::Timer timer;
  std::chrono::milliseconds dura(1);
  for (size_t i=0; i < 10;i++)
    {
    poutre::ScopedTimer stimer(timer);
    std::this_thread::sleep_for(dura);
    }
  BOOST_CHECK(timer.GetMeanTime() >= 1.f);
  BOOST_CHECK(timer.GetCumulativeTime() >= 10.f);
  BOOST_CHECK_EQUAL(timer.NbIter(), 10);
  //std::cout << "\n" << timer << "\n";
  }

BOOST_AUTO_TEST_SUITE_END()