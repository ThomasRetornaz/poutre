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
#include <poutreBase/poutreCoordinate.hpp>
#include <boost/foreach.hpp>

BOOST_AUTO_TEST_SUITE(bound_index)

BOOST_AUTO_TEST_CASE(bounds)
  {
  poutre::bounds < 1 > bnd0(5);
  BOOST_CHECK_EQUAL(bnd0[0], 5);
  BOOST_CHECK_EQUAL(bnd0.at(0), 5);
  BOOST_CHECK_EQUAL(bnd0.size( ), 5);

  auto bnd1 = poutre::bounds < 3 > { 3, 1, 4 }; //-V112
  auto idx = poutre::index < 3 > { 2, -1, 0 };

  //operate with index
  poutre::bounds<3> bnd2 = bnd1 + idx;  // bnd2 is { 5, 0, 4 } and bnd1 unchanged
  BOOST_CHECK_EQUAL(bnd2[0], 5);
  BOOST_CHECK_EQUAL(bnd2[1], 0);
  BOOST_CHECK_EQUAL(bnd2[2], 4); //-V112
  BOOST_CHECK_EQUAL(bnd2.size(),0);

  bnd1 -= idx;  // bnd1 is { 1, 2, 4 } 
  BOOST_CHECK_EQUAL(bnd1[0], 1);
  BOOST_CHECK_EQUAL(bnd1[1], 2);
  BOOST_CHECK_EQUAL(bnd1[2], 4); //-V112
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
  auto index2 = poutre::index < 2 > { 2, 4 }; //-V112
  BOOST_CHECK(!bnd.contains(index2));

  }

BOOST_AUTO_TEST_CASE(index)
  {
  //Scaling
  poutre::index<2> idx{ 2, 3 };
  
  BOOST_CHECK_EQUAL(idx[0], 2);
  BOOST_CHECK_EQUAL(idx[1], 3);

  idx*=1.5f;  // res is {3, 4}
  BOOST_CHECK_EQUAL(idx[0], 3);
  BOOST_CHECK_EQUAL(idx[1], 4); //-V112
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

  BOOST_CHECK(idx != idx2);
  BOOST_CHECK(idx2 != idx3);
  }

BOOST_AUTO_TEST_CASE(bounds_iterator_1d)
  {
  using idx = poutre::index< 1 >;
  using bnd = poutre::bounds < 1 >;
  using iterator = poutre::bounds_iterator< 1 >;
  bnd bnd1D(10);

  std::vector<idx> expected = { {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9} };
  //TODO ?
  //std::vector<idx> rexpected = { {9}, {8}, {7}, {6}, {5}, {4}, {3}, {2}, {1}, {0} };

  BOOST_CHECK_EQUAL_COLLECTIONS(bnd1D.begin( ), bnd1D.end( ), expected.begin( ), expected.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(begin(bnd1D), end(bnd1D), expected.begin( ), expected.end( ));

  /*BOOST_CHECK_EQUAL_COLLECTIONS(bnd1D.rbegin( ), bnd1D.rend( ), rexpected.begin( ), rexpected.end( ));
  BOOST_CHECK_EQUAL_COLLECTIONS(rbegin(bnd1D), rend(bnd1D), rexpected.begin( ), rexpected.end( ));*/

  iterator it(bnd1D);
  ++it;
  BOOST_CHECK_EQUAL(it-bnd1D.begin(),1);
  iterator it5(bnd1D, poutre::index< 1 >(5));
  BOOST_CHECK_EQUAL(it5 - bnd1D.begin( ), 5);
  --it5;
  BOOST_CHECK_EQUAL(it5 - bnd1D.begin( ), 4); //-V112
  
  idx res;
  poutre::details::get_coord_from_offset_nostride<bnd,idx>::op(bnd1D,4,res); //-V112
  BOOST_CHECK(res==idx{4}); //-V112
    
  auto off=poutre::details::get_offset_from_coord_nostride<bnd, idx>::op(bnd1D, res);
  BOOST_CHECK_EQUAL(off, 4); //-V112

  idx outofbound = {15};
  iterator itFromOut(bnd1D, outofbound);
  BOOST_CHECK(itFromOut==bnd1D.end( ));
  }

BOOST_AUTO_TEST_CASE(bounds_iterator_2d)
  {
  using bnd = poutre::bounds<2>;
  using idx = poutre::index< 2 >;
  using iterator = poutre::bounds_iterator< 2 >;

  bnd bnd2D{ 2, 4 };
  std::vector<idx> expected = { idx{ 0, 0 }, idx{ 0, 1 }, idx{ 0, 2 }, idx{ 0, 3 }, idx{ 1, 0 }, idx{ 1, 1 }, idx{ 1, 2 }, idx{ 1, 3 } };
  BOOST_CHECK_EQUAL_COLLECTIONS(bnd2D.cbegin( ), bnd2D.cend( ), expected.begin( ), expected.end( ));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(bnd2D), cend(bnd2D), expected.begin( ), expected.end( ));
  
  iterator it(bnd2D);
  ++it;
  idx test{ 0, 1 };
  BOOST_CHECK(test==*it);
  BOOST_CHECK_EQUAL(it - bnd2D.begin( ), 1);
  test = idx{ 1, 0 };
  iterator it5(bnd2D, test);
  BOOST_CHECK_EQUAL(*it5, test);
  BOOST_CHECK_EQUAL(it5 - bnd2D.begin( ), 4); //-V112
  --it5;
  test = idx{ 0, 3 };
  BOOST_CHECK_EQUAL(*it5, test);
  BOOST_CHECK_EQUAL(it5 - bnd2D.begin( ), 3);


  idx res;
  idx expectedidx = {1,1};
  poutre::details::get_coord_from_offset_nostride<bnd, idx>::op(bnd2D, 5, res);
  BOOST_CHECK_EQUAL(res, expectedidx);

  auto off = poutre::details::get_offset_from_coord_nostride<bnd, idx>::op(bnd2D, res);
  BOOST_CHECK_EQUAL(off, 5);

  idx outofbound = { 15,15 };
  iterator itFromOut(bnd2D, outofbound);
  BOOST_CHECK(itFromOut == bnd2D.end( ));
  
  
  auto b  =  bnd{3, 2};
  auto it2d = iterator(b,idx{0,0});
  //std::vector<idx> expected = {{0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}};
  BOOST_CHECK_EQUAL(*it2d,(idx{0, 0}));
  ++it2d;
  BOOST_CHECK_EQUAL(*it2d,(idx{0, 1}));
  ++it2d;
  BOOST_CHECK_EQUAL(*it2d,(idx{1, 0}));
  ++it2d;
  BOOST_CHECK_EQUAL(*it2d,(idx{1, 1}));
  ++it2d;
  BOOST_CHECK_EQUAL(*it2d,(idx{2, 0}));
  ++it2d;
  BOOST_CHECK_EQUAL(*it2d,(idx{2, 1}));
  }

BOOST_AUTO_TEST_CASE(bounds_iterator_3d)
  {
  using idx = poutre::index< 3 >;
  using bound = poutre::bounds< 3 >;
  using iterator = poutre::bounds_iterator< 3 >;

  bound bnd{ 1, 2, 3 };
 

  std::vector<idx> expected = { idx{0, 0, 0}, idx{0, 0, 1}, idx{0, 0, 2}, idx{0, 1, 0}, idx{0, 1, 1}, idx{0, 1, 2} };
  BOOST_CHECK_EQUAL_COLLECTIONS(bnd.cbegin( ), bnd.cend( ), expected.begin( ), expected.end( ));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(bnd), cend(bnd), expected.begin( ), expected.end( ));

  iterator it(bnd);
  ++it;
  idx expectedidx{ 0, 0, 1 };
  BOOST_CHECK_EQUAL(expectedidx , *it);
  BOOST_CHECK_EQUAL(it - bnd.begin( ), 1);
  idx start{ 0, 1, 1 };
  iterator it2(bnd, start);
  BOOST_CHECK_EQUAL(*it2, start);
  BOOST_CHECK_EQUAL(it2 - bnd.begin( ), 4); //-V112
  --it2;
  expectedidx = idx{ 0, 1, 0 };
  BOOST_CHECK_EQUAL(*it2, expectedidx);
  BOOST_CHECK_EQUAL(it2 - bnd.begin( ), 3);


  idx res;
  idx expidx = idx{ 0, 1, 2 };
  poutre::details::get_coord_from_offset_nostride<bound, idx>::op(bnd, 5, res);
  BOOST_CHECK_EQUAL(res, expidx);

  auto off = poutre::details::get_offset_from_coord_nostride<bound, idx>::op(bnd, res);
  BOOST_CHECK_EQUAL(off, 5);

  idx outofbound = { 15, 15,15 };
  iterator itFromOut(bnd, outofbound);
  BOOST_CHECK(itFromOut == bnd.end( ));
  }

BOOST_AUTO_TEST_CASE(shift)
  {
      {//1D
      poutre::bounds < 1 > bnd{ 3};
      poutre::index< 1 > idx{1};
      poutre::index< 1 > outidx;

      //shift by 0 do nothing of course
      poutre::details::shift_op<poutre::bounds<1>, poutre::index<1>>::op(bnd, idx, poutre::offset(0), outidx); //TODO work on template type deduction
      BOOST_CHECK_EQUAL(outidx[0], 1);

      //shift by 1 # ++
      poutre::details::shift_op<poutre::bounds<1>, poutre::index<1>>::op(bnd, idx, poutre::offset(1), outidx);
      BOOST_CHECK_EQUAL(outidx[0], 2);

      //shift by -1 # --
      poutre::details::shift_op<poutre::bounds<1>, poutre::index<1>>::op(bnd, idx, poutre::offset(-1), outidx);
      BOOST_CHECK_EQUAL(outidx[0], 0);
      }

      {//2D
      poutre::bounds < 2 > bnd{ 3, 4 };
      poutre::index< 2 > idx{ 2, 2 };
      poutre::index< 2 > outidx;

      //shift by 0 do nothing of course
      poutre::details::shift_op<poutre::bounds<2>, poutre::index<2>>::op(bnd, idx, poutre::offset(0), outidx); //TODO work on template type deduction
      BOOST_CHECK_EQUAL(outidx[0], 2);
      BOOST_CHECK_EQUAL(outidx[1], 2);

      //shift by 1 # ++
      poutre::details::shift_op<poutre::bounds<2>, poutre::index<2>>::op(bnd, idx, poutre::offset(1), outidx);
      BOOST_CHECK_EQUAL(outidx[0], 2);
      BOOST_CHECK_EQUAL(outidx[1], 3);

      //shift by -1 # --
      poutre::details::shift_op<poutre::bounds<2>, poutre::index<2>>::op(bnd, idx, poutre::offset(-1), outidx);
      BOOST_CHECK_EQUAL(outidx[0], 2);
      BOOST_CHECK_EQUAL(outidx[1], 1);
      }

      {//3D
      poutre::bounds < 3 > bnd{ 3, 4, 2 };
      poutre::index< 3 > idx{ 2, 2, 1 };
      poutre::index< 3 > outidx;

      //shift by 0 do nothing of course
      poutre::details::shift_op<poutre::bounds<3>, poutre::index<3>>::op(bnd, idx, poutre::offset(0), outidx); //TODO work on template type deduction
      BOOST_CHECK_EQUAL(outidx[0], 2);
      BOOST_CHECK_EQUAL(outidx[1], 2);
      BOOST_CHECK_EQUAL(outidx[2], 1);

      //shift by 1 # ++
      poutre::details::shift_op<poutre::bounds<3>, poutre::index<3>>::op(bnd, idx, poutre::offset(1), outidx);
      BOOST_CHECK_EQUAL(outidx[0], 2);
      BOOST_CHECK_EQUAL(outidx[1], 3);
      BOOST_CHECK_EQUAL(outidx[2], 0);

      //shift by -1 # --
      poutre::details::shift_op<poutre::bounds<3>, poutre::index<3>>::op(bnd, idx, poutre::offset(-1), outidx);
      BOOST_CHECK_EQUAL(outidx[0], 2);
      BOOST_CHECK_EQUAL(outidx[1], 2);
      BOOST_CHECK_EQUAL(outidx[2], 0);
      }
  }

BOOST_AUTO_TEST_SUITE_END( )