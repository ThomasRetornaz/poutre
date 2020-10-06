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
#include "gtest/gtest.h"
#include <iostream>
#include <poutreBase/poutreChronos.hpp>
#include <thread>

TEST(chronos, init)
{
    poutre::Timer timer;
    EXPECT_EQ(timer.GetMeanTime(), 0);
    EXPECT_EQ(timer.GetCumulativeTime(), 0);
    EXPECT_EQ(timer.NbIter(), 0);
    // std::cout <<"\n"<< timer<<"\n";
}

TEST(chronos, scoped)
{
    poutre::Timer timer;
    std::chrono::milliseconds dura(100);
    for (size_t i = 0; i < 10; i++)
    {
        poutre::ScopedTimer stimer(timer);
        std::this_thread::sleep_for(dura);
    }
    /*EXPECT_NEAR(timer.GetMeanTime(), 100.f, 2);
    EXPECT_NEAR(timer.GetCumulativeTime(), 1000.f, 2);*/
    EXPECT_EQ(timer.NbIter(), 10);
    // std::cout << "\n" << timer << "\n";
}
