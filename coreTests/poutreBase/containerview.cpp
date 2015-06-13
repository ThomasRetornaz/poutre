
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"
#include <iostream>
#include <poutreBase/poutreContainerView.hpp>

BOOST_AUTO_TEST_SUITE(ContainerView)

BOOST_AUTO_TEST_CASE(init)
  {
  using view1DINt = poutre::array_view<int>;
  using view2DINt = poutre::array_view<int,2>;
  using view1DFLOAT = poutre::array_view<float>;
  view1DINt dummyview;
  BOOST_CHECK_EQUAL(dummyview.size( ), 0);
  BOOST_CHECK(dummyview.data( )==nullptr);

  //view1DINt otherdummy;
  //view1DINt f(otherdummy);
  std::vector<int> mif(10,0);
  view1DINt g(std::move(mif));
  BOOST_CHECK_EQUAL(g.size(),10);

  //poutre::bounds_1 bnd(0);
  ////we can create 1d view on 2d view
  //view1DINt 1DOver2D(view2DINt( ), bnd);
  ////but not the opposite
  //BOOST_CHECK_EXCEPTION(view2DINt,view1DINt(),std::runtime_error);


  auto vec = std::vector<int>(10);
  auto av2 = poutre::array_view<int, 2>( vec, { 2, 5 } );  // 2D view over vec
  auto av1 = poutre::array_view<int, 1>(vec, { 10 });  // 1D view over vec
  auto av1DFrom2D = poutre::array_view<int, 1>(av2, { 2 });  // 1D view from 2D view

  }
BOOST_AUTO_TEST_CASE(basic_usage_over_vector)
      {
      auto vec = std::vector<int>(10);
      auto view = poutre::array_view<int>( vec );
      view[poutre::array_view<int>::index_type{0}] = 42; //grmmlll {} to force resolution operator[index] and not slice ....
      BOOST_CHECK_EQUAL(vec[0],42);  // v == 42
      }

//BOOST_AUTO_TEST_CASE(view_on_muti_dim_c_array)
//  {
//  char a[3][1][4] {{{'H', 'i'}}};
//  auto av = poutre::array_view<char, 3>{a};
//  BOOST_CHECK((av.bounds( ) == poutre::bounds<3>{3, 1, 4}));
//  //assert((av[{0, 0, 0}] == 'H'));
//  }
//
BOOST_AUTO_TEST_SUITE_END()