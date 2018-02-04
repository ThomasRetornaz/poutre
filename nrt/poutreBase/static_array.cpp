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

BOOST_AUTO_TEST_SUITE(static_container)

BOOST_AUTO_TEST_CASE(static_array_init)
  {
      {
      //ctor specialization
      poutre::static_array<int, 1> array1(1);
      poutre::static_array<int, 2> array2(1, 2);
      poutre::static_array<int, 3> array3(1, 2, 4);
      }

    {
    //initializer list
    auto array1 = { 1 };
    auto array2 = { 1, 2, 3, 4, 5, 6 };
    }

    {
    //init from carray
    int tab[3] = { 1, 2, 3 };
    auto array3(tab);
    }
  }

BOOST_AUTO_TEST_CASE(static_array_iteration_acces_capacity)
  {
  auto starray = poutre::static_array < ptrdiff_t, 6 > {1, 2, 3, 4, 5, 6};
  //stream through base
  std::cout << starray;
  BOOST_CHECK_EQUAL(starray.size( ), 6);
  BOOST_CHECK_EQUAL(starray.max_size( ), 6);
  BOOST_CHECK_EQUAL(starray.empty( ), false);

  BOOST_CHECK_EQUAL(starray.front( ), 1);
  BOOST_CHECK_EQUAL(starray.back( ), 6);
  BOOST_CHECK_EQUAL(starray[0], 1);
  BOOST_CHECK_EQUAL(starray[5], 6);
  BOOST_CHECK_EQUAL(*(starray.data()),1);

  BOOST_CHECK_EQUAL(starray.at(1), 2);

  poutre::static_array < ptrdiff_t, 6 > starray2;
  starray2.assign(5);
  BOOST_CHECK_EQUAL(starray2.at(1), 5);
  starray2.fill(4);
  BOOST_CHECK_EQUAL(starray2.at(1), 4);

  poutre::swap(starray,starray2);
  BOOST_CHECK_EQUAL(starray.at(1), 4);
  poutre::swap(starray, starray2);

  auto expected = {1, 2, 3, 4, 5, 6};
  auto rexpected = { 6,5,4,3,2,1};
  BOOST_CHECK_EQUAL_COLLECTIONS(starray.begin( ), starray.end( ),expected.begin(),expected.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(starray.rbegin( ), starray.rend( ), rexpected.begin( ), rexpected.end( ));

  auto sum_const = std::accumulate(starray.cbegin( ), starray.cend( ), ptrdiff_t(0));
  BOOST_CHECK_EQUAL(sum_const, 21);

  auto sum_constrev = std::accumulate(starray.crbegin( ), starray.crend( ), ptrdiff_t(10));
  BOOST_CHECK_EQUAL(sum_constrev, 31);

  auto sum = std::accumulate(starray.begin( ), starray.end( ), ptrdiff_t(10));
  BOOST_CHECK_EQUAL(sum, 31);

  auto sum_rev = std::accumulate(starray.crbegin( ), starray.crend( ), ptrdiff_t(5));
  BOOST_CHECK_EQUAL(sum_rev, 26);
  }

BOOST_AUTO_TEST_CASE(static_array_operator)
  {

  auto array_int_3 = poutre::static_array < ptrdiff_t, 3 > { 1, 2, 4 };

  //Arith
  auto array_cp = array_int_3;
  array_cp -= 1;
  BOOST_CHECK_EQUAL(array_cp[0], 0);
  BOOST_CHECK_EQUAL(array_cp[1], 1);
  BOOST_CHECK_EQUAL(array_cp[2], 3);

  array_cp += 2;
  BOOST_CHECK_EQUAL(array_cp[0], 2);
  BOOST_CHECK_EQUAL(array_cp[1], 3);
  BOOST_CHECK_EQUAL(array_cp[2], 5);

  array_cp /= 2;
  BOOST_CHECK_EQUAL(array_cp[0], 1);
  BOOST_CHECK_EQUAL(array_cp[1], 1);
  BOOST_CHECK_EQUAL(array_cp[2], 2);

  //scaling
  auto array_scale = array_int_3*1.5f;
  BOOST_CHECK_EQUAL(array_scale[0], 1);
  BOOST_CHECK_EQUAL(array_scale[1], 3);
  BOOST_CHECK_EQUAL(array_scale[2], 6);

  }

BOOST_AUTO_TEST_SUITE_END( )