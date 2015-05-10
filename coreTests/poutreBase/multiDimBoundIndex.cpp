#include "main.hpp"
#include <iostream>
#include <thread>
#include <poutreBase/poutreMultiDimBoundIndex.hpp>

BOOST_AUTO_TEST_SUITE(geometrybase)

BOOST_AUTO_TEST_CASE(bounds)
  {
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

  //scaling
  poutre::bounds<3> bnd3 = bnd1 * 1.5f;  // bnd3 is {1,3,6} and bdnd1 unchanged
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
  BOOST_CHECK_EQUAL(res[0], 3);
  BOOST_CHECK_EQUAL(res[1], 4);
  }


BOOST_AUTO_TEST_SUITE_END( )