
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
#include <poutreBase/poutreCoordinate.hpp>
#include <boost/foreach.hpp>

BOOST_AUTO_TEST_SUITE(bound_index)

BOOST_AUTO_TEST_CASE(bounds)
  {
  poutre::bounds < 1 > bnd0(5);
  BOOST_CHECK_EQUAL(bnd0[0], 5);
  BOOST_CHECK_EQUAL(bnd0.at(0), 5);
  BOOST_CHECK_EQUAL(bnd0.size( ), 5);

  auto bnd1 = poutre::bounds < 3 > { 3, 1, 4 };
  auto idx = poutre::index < 3 > { 2, -1, 0 };

  //operate with index
  poutre::bounds<3> bnd2 = bnd1 + idx;  // bnd2 is { 5, 0, 4 } and bnd1 unchanged
  BOOST_CHECK_EQUAL(bnd2[0], 5);
  BOOST_CHECK_EQUAL(bnd2[1], 0);
  BOOST_CHECK_EQUAL(bnd2[2], 4);
  BOOST_CHECK_EQUAL(bnd2.size(),0);

  bnd1 -= idx;  // bnd1 is { 1, 2, 4 } 
  BOOST_CHECK_EQUAL(bnd1[0], 1);
  BOOST_CHECK_EQUAL(bnd1[1], 2);
  BOOST_CHECK_EQUAL(bnd1[2], 4);
  BOOST_CHECK_EQUAL(bnd1.size( ), 8);

  //Arith
  poutre::bounds<3> bnd4 = bnd1;  // bnd4 is {0,1,3} and bdnd1 unchanged
  bnd4 -= 1;
  BOOST_CHECK_EQUAL(bnd4[0], 0);
  BOOST_CHECK_EQUAL(bnd4[1], 1);
  BOOST_CHECK_EQUAL(bnd4[2], 3);

  bnd4 += 2;
  BOOST_CHECK_EQUAL(bnd4[0], 2);
  BOOST_CHECK_EQUAL(bnd4[1], 3);
  BOOST_CHECK_EQUAL(bnd4[2], 5);

  //scaling
  poutre::bounds<3> bnd3 = bnd1*1.5f;  // bnd3 is {1,3,6} and bdnd1 unchanged
  BOOST_CHECK_EQUAL(bnd3[0], 1);
  BOOST_CHECK_EQUAL(bnd3[1], 3);
  BOOST_CHECK_EQUAL(bnd3[2], 6);

  bnd1 /= 2;
  BOOST_CHECK_EQUAL(bnd1[0], 0);
  BOOST_CHECK_EQUAL(bnd1[1], 1);
  BOOST_CHECK_EQUAL(bnd1[2], 2);

  //contains
  auto bnd = poutre::bounds < 2 > { 3, 3 };
  auto index = poutre::index < 2 > { 2, 2 };
  BOOST_CHECK(bnd.contains(index));
  auto index2 = poutre::index < 2 > { 2, 4 };
  BOOST_CHECK(!bnd.contains(index2));

  }

BOOST_AUTO_TEST_CASE(index)
  {
  //Scaling
  poutre::index<2> idx{ 2, 3 };
  poutre::index<2> res = idx * 1.5f;  // res is {3, 4}
  BOOST_CHECK_EQUAL(idx[0], 2);
  BOOST_CHECK_EQUAL(idx[1], 3);

  BOOST_CHECK_EQUAL(res[0], 3);
  BOOST_CHECK_EQUAL(res[1], 4);
  }

BOOST_AUTO_TEST_CASE(index_ordering)
  {
      
  poutre::index<2> idx{ 2, 3 };
  poutre::index<2> idx2{ 1, 3 };
  poutre::index<2> idx3{ 3, 3 };

  BOOST_CHECK(idx2<idx);
  BOOST_CHECK(idx2<=idx);
  BOOST_CHECK(idx3>idx);
  BOOST_CHECK(idx3>=idx);

  BOOST_CHECK(idx<=idx);
  BOOST_CHECK(idx>=idx);
  BOOST_CHECK(idx == idx);
  BOOST_CHECK(idx != idx2);
  BOOST_CHECK(idx2 != idx3);
  }

BOOST_AUTO_TEST_CASE(bounds_iterator_1d)
  {
  using idx = poutre::index< 1 >;
  poutre::bounds < 1 > bnd1D(10);

  std::vector<idx> expected = { {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9} };
  std::vector<idx> rexpected = { {9}, {8}, {7}, {6}, {5}, {4}, {3}, {2}, {1}, {0} };

  BOOST_CHECK_EQUAL_COLLECTIONS(bnd1D.begin( ), bnd1D.end( ), expected.begin( ), expected.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(begin(bnd1D), end(bnd1D), expected.begin( ), expected.end( ));

  }

BOOST_AUTO_TEST_CASE(bounds_iterator_2d)
  {
  poutre::bounds < 2 > bnd2D{2,4};
  using idx = poutre::index< 2 >;
  
  std::vector<idx> expected = { idx{ 0, 0 }, idx{ 0, 1 }, idx{ 0, 2 }, idx{ 0, 3 }, idx{ 1, 0 }, idx{ 1, 1 }, idx{ 1, 2 }, idx{ 1, 3 } };
  BOOST_CHECK_EQUAL_COLLECTIONS(bnd2D.cbegin( ), bnd2D.cend( ), expected.begin( ), expected.end( ));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(bnd2D), cend(bnd2D), expected.begin( ), expected.end( ));
   
  }

BOOST_AUTO_TEST_CASE(bounds_iterator_3d)
  {
  poutre::bounds < 3 > bnd{ 1,2,3 };
  using idx = poutre::index< 3 >;

  std::vector<idx> expected = { idx{0, 0, 0}, idx{0, 0, 1}, idx{0, 0, 2}, idx{0, 1, 0}, idx{0, 1, 1}, idx{0, 1, 2} };
  BOOST_CHECK_EQUAL_COLLECTIONS(bnd.cbegin( ), bnd.cend( ), expected.begin( ), expected.end( ));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(bnd), cend(bnd), expected.begin( ), expected.end( ));
  }
BOOST_AUTO_TEST_SUITE_END( )