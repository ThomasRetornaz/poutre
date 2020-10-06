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
#include <gtest/gtest.h>

TEST(coordinate,bounds)
  {
  poutre::bounds < 1 > bnd0(5);
  EXPECT_EQ(bnd0[0], 5);
  EXPECT_EQ(bnd0.at(0), 5);
  EXPECT_EQ(bnd0.size( ), 5);

  auto bnd1 = poutre::bounds < 3 > { 3, 1, 4 }; //-V112
  auto idx = poutre::index < 3 > { 2, -1, 0 };

  //operate with index
  poutre::bounds<3> bnd2 = bnd1 + idx;  // bnd2 is { 5, 0, 4 } and bnd1 unchanged
  EXPECT_EQ(bnd2[0], 5);
  EXPECT_EQ(bnd2[1], 0);
  EXPECT_EQ(bnd2[2], 4); //-V112
  EXPECT_EQ(bnd2.size(),0);

  bnd1 -= idx;  // bnd1 is { 1, 2, 4 } 
  EXPECT_EQ(bnd1[0], 1);
  EXPECT_EQ(bnd1[1], 2);
  EXPECT_EQ(bnd1[2], 4); //-V112
  EXPECT_EQ(bnd1.size( ), 8);

  //Arith
  poutre::bounds<3> bnd4 = bnd1;  // bnd4 is {0,1,3} and bdnd1 unchanged
  bnd4 -= 1;
  EXPECT_EQ(bnd4[0], 0);
  EXPECT_EQ(bnd4[1], 1);
  EXPECT_EQ(bnd4[2], 3);

  bnd4 += 2;
  EXPECT_EQ(bnd4[0], 2);
  EXPECT_EQ(bnd4[1], 3);
  EXPECT_EQ(bnd4[2], 5);

  //scaling
  poutre::bounds<3> bnd3 = bnd1*1.5f;  // bnd3 is {1,3,6} and bdnd1 unchanged
  EXPECT_EQ(bnd3[0], 1);
  EXPECT_EQ(bnd3[1], 3);
  EXPECT_EQ(bnd3[2], 6);

  bnd1 /= 2;
  EXPECT_EQ(bnd1[0], 0);
  EXPECT_EQ(bnd1[1], 1);
  EXPECT_EQ(bnd1[2], 2);

  //contains
  auto bnd = poutre::bounds < 2 > { 3, 3 };
  auto index = poutre::index < 2 > { 2, 2 };
  EXPECT_TRUE(bnd.contains(index));
  auto index2 = poutre::index < 2 > { 2, 4 }; //-V112
  EXPECT_TRUE(!bnd.contains(index2));

  }

TEST(coordinate,index)
  {
  //Scaling
  poutre::index<2> idx{ 2, 3 };
  
  EXPECT_EQ(idx[0], 2);
  EXPECT_EQ(idx[1], 3);

  idx*=1.5f;  // res is {3, 4}
  EXPECT_EQ(idx[0], 3);
  EXPECT_EQ(idx[1], 4); //-V112
  }

TEST(coordinate,index_ordering)
  {
      
  poutre::index<2> idx{ 2, 3 };
  poutre::index<2> idx2{ 1, 3 };
  poutre::index<2> idx3{ 3, 3 };
  
  EXPECT_TRUE(idx2<idx);
  EXPECT_TRUE(idx2<=idx);
  EXPECT_TRUE(idx3>idx);
  EXPECT_TRUE(idx3>=idx);

  EXPECT_TRUE(idx != idx2);
  EXPECT_TRUE(idx2 != idx3);
  }

TEST(coordinate,bounds_iterator_1d)
  {
  using idx = poutre::index< 1 >;
  using bnd = poutre::bounds < 1 >;
  using iterator = poutre::bounds_iterator< 1 >;
  bnd bnd1D(10);

  std::vector<idx> expected = { {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9} };
  //TODO ?
  //std::vector<idx> rexpected = { {9}, {8}, {7}, {6}, {5}, {4}, {3}, {2}, {1}, {0} };

  EXPECT_TRUE(CheckEqualCollections(bnd1D.begin(), bnd1D.end(), expected.begin()));
  EXPECT_TRUE(CheckEqualCollections(begin(bnd1D), end(bnd1D), expected.begin()));

  /*EXPECT_EQ_COLLECTIONS(bnd1D.rbegin( ), bnd1D.rend( ), rexpected.begin( ), rexpected.end( ));
  EXPECT_EQ_COLLECTIONS(rbegin(bnd1D), rend(bnd1D), rexpected.begin( ), rexpected.end( ));*/

  iterator it(bnd1D);
  ++it;
  EXPECT_EQ(it-bnd1D.begin(),1);
  iterator it5(bnd1D, poutre::index< 1 >(5));
  EXPECT_EQ(it5 - bnd1D.begin( ), 5);
  --it5;
  EXPECT_EQ(it5 - bnd1D.begin( ), 4); //-V112
  
  idx res;
  poutre::details::get_coord_from_offset_nostride<bnd,idx>::op(bnd1D,4,res); //-V112
  EXPECT_TRUE(res==idx{4}); //-V112
    
  auto off=poutre::details::get_offset_from_coord_nostride<bnd, idx>::op(bnd1D, res);
  EXPECT_EQ(off, 4); //-V112

  idx outofbound = {15};
  iterator itFromOut(bnd1D, outofbound);
  EXPECT_TRUE(itFromOut==bnd1D.end( ));
  }

TEST(coordinate,bounds_iterator_2d)
  {
  using bnd = poutre::bounds<2>;
  using idx = poutre::index< 2 >;
  using iterator = poutre::bounds_iterator< 2 >;

  bnd bnd2D{ 2, 4 };
  std::vector<idx> expected = { idx{ 0, 0 }, idx{ 0, 1 }, idx{ 0, 2 }, idx{ 0, 3 }, idx{ 1, 0 }, idx{ 1, 1 }, idx{ 1, 2 }, idx{ 1, 3 } };
  EXPECT_TRUE(CheckEqualCollections(bnd2D.cbegin( ), bnd2D.cend( ), expected.begin( )));
  EXPECT_TRUE(CheckEqualCollections(cbegin(bnd2D), cend(bnd2D), expected.begin( )));
  
  iterator it(bnd2D);
  ++it;
  idx test{ 0, 1 };
  EXPECT_TRUE(test==*it);
  EXPECT_EQ(it - bnd2D.begin( ), 1);
  test = idx{ 1, 0 };
  iterator it5(bnd2D, test);
  EXPECT_EQ(*it5, test);
  EXPECT_EQ(it5 - bnd2D.begin( ), 4); //-V112
  --it5;
  test = idx{ 0, 3 };
  EXPECT_EQ(*it5, test);
  EXPECT_EQ(it5 - bnd2D.begin( ), 3);


  idx res;
  idx expectedidx = {1,1};
  poutre::details::get_coord_from_offset_nostride<bnd, idx>::op(bnd2D, 5, res);
  EXPECT_EQ(res, expectedidx);

  auto off = poutre::details::get_offset_from_coord_nostride<bnd, idx>::op(bnd2D, res);
  EXPECT_EQ(off, 5);

  idx outofbound = { 15,15 };
  iterator itFromOut(bnd2D, outofbound);
  EXPECT_TRUE(itFromOut == bnd2D.end( ));
  
  
  auto b  =  bnd{3, 2};
  auto it2d = iterator(b,idx{0,0});
  //std::vector<idx> expected = {{0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}};
  EXPECT_EQ(*it2d,(idx{0, 0}));
  ++it2d;
  EXPECT_EQ(*it2d,(idx{0, 1}));
  ++it2d;
  EXPECT_EQ(*it2d,(idx{1, 0}));
  ++it2d;
  EXPECT_EQ(*it2d,(idx{1, 1}));
  ++it2d;
  EXPECT_EQ(*it2d,(idx{2, 0}));
  ++it2d;
  EXPECT_EQ(*it2d,(idx{2, 1}));
  }

TEST(coordinate,bounds_iterator_3d)
  {
  using idx = poutre::index< 3 >;
  using bound = poutre::bounds< 3 >;
  using iterator = poutre::bounds_iterator< 3 >;

  bound bnd{ 1, 2, 3 };
 

  std::vector<idx> expected = { idx{0, 0, 0}, idx{0, 0, 1}, idx{0, 0, 2}, idx{0, 1, 0}, idx{0, 1, 1}, idx{0, 1, 2} };
  EXPECT_TRUE(CheckEqualCollections(bnd.cbegin( ), bnd.cend( ), expected.begin( )));
  EXPECT_TRUE(CheckEqualCollections(cbegin(bnd), cend(bnd), expected.begin( )));

  iterator it(bnd);
  ++it;
  idx expectedidx{ 0, 0, 1 };
  EXPECT_EQ(expectedidx , *it);
  EXPECT_EQ(it - bnd.begin( ), 1);
  idx start{ 0, 1, 1 };
  iterator it2(bnd, start);
  EXPECT_EQ(*it2, start);
  EXPECT_EQ(it2 - bnd.begin( ), 4); //-V112
  --it2;
  expectedidx = idx{ 0, 1, 0 };
  EXPECT_EQ(*it2, expectedidx);
  EXPECT_EQ(it2 - bnd.begin( ), 3);


  idx res;
  idx expidx = idx{ 0, 1, 2 };
  poutre::details::get_coord_from_offset_nostride<bound, idx>::op(bnd, 5, res);
  EXPECT_EQ(res, expidx);

  auto off = poutre::details::get_offset_from_coord_nostride<bound, idx>::op(bnd, res);
  EXPECT_EQ(off, 5);

  idx outofbound = { 15, 15,15 };
  iterator itFromOut(bnd, outofbound);
  EXPECT_TRUE(itFromOut == bnd.end( ));
  }

TEST(coordinate,shift)
  {
      {//1D
      poutre::bounds < 1 > bnd{ 3};
      poutre::index< 1 > idx{1};
      poutre::index< 1 > outidx;

      //shift by 0 do nothing of course
      poutre::details::shift_op<poutre::bounds<1>, poutre::index<1>>::op(bnd, idx, poutre::offset(0), outidx); //TODO work on template type deduction
      EXPECT_EQ(outidx[0], 1);

      //shift by 1 # ++
      poutre::details::shift_op<poutre::bounds<1>, poutre::index<1>>::op(bnd, idx, poutre::offset(1), outidx);
      EXPECT_EQ(outidx[0], 2);

      //shift by -1 # --
      poutre::details::shift_op<poutre::bounds<1>, poutre::index<1>>::op(bnd, idx, poutre::offset(-1), outidx);
      EXPECT_EQ(outidx[0], 0);
      }

      {//2D
      poutre::bounds < 2 > bnd{ 3, 4 };
      poutre::index< 2 > idx{ 2, 2 };
      poutre::index< 2 > outidx;

      //shift by 0 do nothing of course
      poutre::details::shift_op<poutre::bounds<2>, poutre::index<2>>::op(bnd, idx, poutre::offset(0), outidx); //TODO work on template type deduction
      EXPECT_EQ(outidx[0], 2);
      EXPECT_EQ(outidx[1], 2);

      //shift by 1 # ++
      poutre::details::shift_op<poutre::bounds<2>, poutre::index<2>>::op(bnd, idx, poutre::offset(1), outidx);
      EXPECT_EQ(outidx[0], 2);
      EXPECT_EQ(outidx[1], 3);

      //shift by -1 # --
      poutre::details::shift_op<poutre::bounds<2>, poutre::index<2>>::op(bnd, idx, poutre::offset(-1), outidx);
      EXPECT_EQ(outidx[0], 2);
      EXPECT_EQ(outidx[1], 1);
      }

      {//3D
      poutre::bounds < 3 > bnd{ 3, 4, 2 };
      poutre::index< 3 > idx{ 2, 2, 1 };
      poutre::index< 3 > outidx;

      //shift by 0 do nothing of course
      poutre::details::shift_op<poutre::bounds<3>, poutre::index<3>>::op(bnd, idx, poutre::offset(0), outidx); //TODO work on template type deduction
      EXPECT_EQ(outidx[0], 2);
      EXPECT_EQ(outidx[1], 2);
      EXPECT_EQ(outidx[2], 1);

      //shift by 1 # ++
      poutre::details::shift_op<poutre::bounds<3>, poutre::index<3>>::op(bnd, idx, poutre::offset(1), outidx);
      EXPECT_EQ(outidx[0], 2);
      EXPECT_EQ(outidx[1], 3);
      EXPECT_EQ(outidx[2], 0);

      //shift by -1 # --
      poutre::details::shift_op<poutre::bounds<3>, poutre::index<3>>::op(bnd, idx, poutre::offset(-1), outidx);
      EXPECT_EQ(outidx[0], 2);
      EXPECT_EQ(outidx[1], 2);
      EXPECT_EQ(outidx[2], 0);
      }
  }
