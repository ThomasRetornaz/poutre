#include "main.hpp"
#include <boost/simd/sdk/simd/pack.hpp>
#include <boost/simd/sdk/simd/io.hpp>
#include <boost/simd/include/functions/splat.hpp>
#include <boost/simd/include/functions/plus.hpp>
#include <boost/simd/include/functions/multiplies.hpp>
#include <iostream>



BOOST_AUTO_TEST_SUITE(boostSimdImport)

BOOST_AUTO_TEST_CASE(SIMDFlagSupported)
  {
  std::cout << "******************SIMD CONFIG***********************"<< std::endl;
  std::cout << "BOOST_SIMD_STRING " <<BOOST_SIMD_STRING<<std::endl;
  std::cout << "BOOST_SIMD_STRING_LIST " << BOOST_SIMD_STRING_LIST << std::endl;
  std::cout << "BOOST_SIMD_BYTES " << BOOST_SIMD_BYTES << std::endl;
  std::cout << "BOOST_SIMD_BITS " << BOOST_SIMD_BITS << std::endl;
  std::cout << "****************END SIMD CONFIG*********************" << std::endl;
  }

//http://nt2.metascale.fr/doc/html/tutorials/simd_hello_world.html
BOOST_AUTO_TEST_CASE(tutorial)
  {
  typedef boost::simd::pack<float> p_t;

  p_t res;
  p_t u(10);
  p_t r = boost::simd::splat<p_t>(11);

  res = (u + r) * 2.f;
  
  //std::vector<float> expected = { 42, 42, 42, 42 };
  //BOOST_CHECK_EQUAL_COLLECTIONS(res.begin(), res.end(), expected.begin(), expected.end());
  std::cout << res << std::endl;
  }
BOOST_AUTO_TEST_SUITE_END()