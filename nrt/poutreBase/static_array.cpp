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
#include <numeric>
#include <poutreBase/poutreStaticContainer.hpp>
#include <gtest/gtest.h>

TEST(static_container,static_array_init)
  {
      {
      //ctor specialization
      poutre::static_array<int, 1> array1(1);
      poutre::static_array<int, 2> array2(1, 2);
      poutre::static_array<int, 3> array3(1, 2, 4); //-V112
      }

    {
    //initializer list
    auto array1 = { 1 };
    auto array2 = { 1, 2, 3, 4, 5, 6 }; //-V112 //-V808
    }

    {
    //init from carray
    int tab[3] = { 1, 2, 3 };
    auto array3(tab);
    }
  }

TEST(static_container,static_array_iteration_acces_capacity)
  {
  auto starray = poutre::static_array < ptrdiff_t, 6 > {1, 2, 3, 4, 5, 6}; //-V112
  //stream through base
  //std::cout << starray;
  EXPECT_EQ(starray.size( ), 6);
  EXPECT_EQ(starray.max_size( ), 6);
  EXPECT_EQ(starray.empty( ), false);

  EXPECT_EQ(starray.front( ), 1);
  EXPECT_EQ(starray.back( ), 6);
  EXPECT_EQ(starray[0], 1);
  EXPECT_EQ(starray[5], 6);
  EXPECT_EQ(*(starray.data()),1);

  EXPECT_EQ(starray.at(1), 2);

  poutre::static_array < ptrdiff_t, 6 > starray2;
  starray2.assign(5);
  EXPECT_EQ(starray2.at(1), 5);
  starray2.fill(4); //-V112
  EXPECT_EQ(starray2.at(1), 4); //-V112

  poutre::swap(starray,starray2);
  EXPECT_EQ(starray.at(1), 4); //-V112
  poutre::swap(starray, starray2);

  auto expected = {1, 2, 3, 4, 5, 6}; //-V112
  auto rexpected = { 6,5,4,3,2,1}; //-V112
  EXPECT_TRUE(CheckEqualCollections(starray.begin( ), starray.end( ),expected.begin()));
  EXPECT_TRUE(CheckEqualCollections(starray.rbegin(), starray.rend(), rexpected.begin()));

  auto sum_const = std::accumulate(starray.cbegin( ), starray.cend( ), ptrdiff_t(0));
  EXPECT_EQ(sum_const, 21);

  auto sum_constrev = std::accumulate(starray.crbegin( ), starray.crend( ), ptrdiff_t(10));
  EXPECT_EQ(sum_constrev, 31);

  auto sum = std::accumulate(starray.begin( ), starray.end( ), ptrdiff_t(10));
  EXPECT_EQ(sum, 31);

  auto sum_rev = std::accumulate(starray.crbegin( ), starray.crend( ), ptrdiff_t(5));
  EXPECT_EQ(sum_rev, 26);
  }

TEST(static_container,static_array_operator)
  {

  auto array_int_3 = poutre::static_array < ptrdiff_t, 3 > { 1, 2, 4 }; //-V112

  //Arith
  auto array_cp = array_int_3;
  array_cp -= 1;
  EXPECT_EQ(array_cp[0], 0);
  EXPECT_EQ(array_cp[1], 1);
  EXPECT_EQ(array_cp[2], 3);

  array_cp += 2;
  EXPECT_EQ(array_cp[0], 2);
  EXPECT_EQ(array_cp[1], 3);
  EXPECT_EQ(array_cp[2], 5);

  array_cp /= 2;
  EXPECT_EQ(array_cp[0], 1);
  EXPECT_EQ(array_cp[1], 1);
  EXPECT_EQ(array_cp[2], 2);

  //scaling
  auto array_scale = array_int_3*1.5f;
  EXPECT_EQ(array_scale[0], 1);
  EXPECT_EQ(array_scale[1], 3);
  EXPECT_EQ(array_scale[2], 6);

  }
