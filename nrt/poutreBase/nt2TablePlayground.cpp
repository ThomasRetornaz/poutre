#include "main.hpp"

#include <iostream>

#include <nt2/table.hpp>
#include <nt2/include/functions/ones.hpp>


BOOST_AUTO_TEST_SUITE(playground)


BOOST_AUTO_TEST_CASE(matrixdefault)
  {
  using namespace nt2;
  table<float> x;
  table<float> y = ones(4, 4);

  x = 40.0 * y + 2.0;

  NT2_DISPLAY(x);
  }

//BOOST_AUTO_TEST_CASE(matrix)
//  {
//
//  nt2::table<float, nt2::_2D> x;
//    //, nt2::aligned_, nt2::global_padding_>
//  nt2::table<float, nt2::_2D> y = nt2::ones(4, 4);
//
//  x = 40.0 * y + 2.0;
//
//  NT2_DISPLAY(x);
//  }

BOOST_AUTO_TEST_SUITE_END()
