#include "main.hpp"
#include <boost/simd/sdk/simd/pack.hpp>
#include <boost/simd/sdk/simd/io.hpp>
#include <boost/simd/include/functions/splat.hpp>
#include <boost/simd/include/functions/plus.hpp>
#include <boost/simd/include/functions/multiplies.hpp>
#include <iostream>



BOOST_AUTO_TEST_SUITE(boostSimdImport)

//http://nt2.metascale.fr/doc/html/tutorials/simd_hello_world.html
BOOST_AUTO_TEST_CASE(tutorial)
  {
  typedef boost::simd::pack<float> p_t;

  p_t res;
  p_t u(10);
  p_t r = boost::simd::splat<p_t>(11);

  res = (u + r) * 2.f;
  
  std::vector<float> expected = { 42, 42, 42, 42 };
  BOOST_CHECK_EQUAL_COLLECTIONS(res.begin(), res.end(), expected.begin(), expected.end());
  }
BOOST_AUTO_TEST_SUITE_END()