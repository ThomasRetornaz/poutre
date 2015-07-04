
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
  //view
  using view1DINt = poutre::array_view < int > ;
  using idxtype = poutre::array_view<int>::index_type;
  using bndtype = poutre::array_view<int>::bounds_type;
  using view2DINt = poutre::array_view < int, 2 > ;
  using view1DFLOAT = poutre::array_view < float > ;

  //empty view
  view1DINt dummyview;
  BOOST_CHECK_EQUAL(dummyview.size( ), 0);
  BOOST_CHECK(dummyview.data( ) == nullptr);
  BOOST_CHECK(dummyview.stride( ) == idxtype{1}); //stride is 1 for empty view
  //BOOST_CHECK_THROW(dummyview[idxtype{ 0 }],std::runtime_error); //throw in debug
  //BOOST_CHECK_THROW(dummyview[5], std::runtime_error); //can't slice on 1D
  BOOST_CHECK_THROW(dummyview.section(bndtype{ 3 }), std::runtime_error); //section

  //sview
  using sview1DINt = poutre::strided_array_view < int >;
  using sview2DINt = poutre::strided_array_view < int, 2 >;
  using sview1DFLOAT = poutre::strided_array_view< float >;

  //empty sview
  sview1DINt dummysview;
  BOOST_CHECK_EQUAL(dummysview.size( ), 0);
  BOOST_CHECK(dummysview.stride( ) == idxtype{ 1 }); //stride is 1 for empty stridedview
  //BOOST_CHECK_THROW(dummysview[idxtype{ 0 }], std::runtime_error); //throw in debug
  //BOOST_CHECK_THROW(dummysview[5], std::runtime_error); //can't slice on 1D
  BOOST_CHECK_THROW(dummysview.section(bndtype{ 3 }), std::runtime_error); //section
  }

BOOST_AUTO_TEST_CASE(basic_usage_view_over_vector)
  {
  using view1DINt = poutre::array_view < int > ;
  using idxtype = poutre::array_view<int>::index_type;
  using view2DINt = poutre::array_view < int, 2 > ;
  using view1DFLOAT = poutre::array_view < float > ;

  //1D view over std::vector
  std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  BOOST_CHECK_EQUAL(mif.size( ), 10);
  auto view = poutre::array_view < int >(mif);
  BOOST_CHECK_EQUAL(view.size( ), 10);
  BOOST_CHECK(view.bounds( ) == poutre::bounds_1{ 10 });
  //assignment through view
  view[idxtype{ 0 }] = 42; //grmmlll idxtype{} to force resolution operator[index] and not slice ....
  BOOST_CHECK_EQUAL(mif[0], 42);  // v == 42
  //copy view
  auto viewcopy(view);
  BOOST_CHECK_EQUAL(viewcopy[idxtype{ 0 }], 42);  // v == 42
  auto viewcopy2=view;
  BOOST_CHECK_EQUAL(viewcopy2[idxtype{ 0 }], 42);  // v == 42
  //array view from ptr and bounds
  auto viewptr = view1DINt(mif.data( ), { (ptrdiff_t)mif.size( )});
  BOOST_CHECK_EQUAL(viewptr[idxtype{ 0 }], 42);  // v == 42

  auto view2d = poutre::array_view<int, 2>(mif, { 2, 5 });  // 2D view over vec
  BOOST_CHECK_EQUAL(view2d.size( ),10);
  BOOST_CHECK(view2d.bounds( ) == (poutre::bounds_2{ 2, 5 }));
  BOOST_CHECK(view2d.stride( ) == (poutre::index_2{ 5, 1 }));
  BOOST_CHECK_EQUAL(view2d[(poutre::index_2{ 0, 0 })], 42);

  auto av1 = poutre::array_view<int, 1>(mif, { 5 });  // 1D view over vec with explicit sub bound
  BOOST_CHECK(av1.bounds( ) == (poutre::bounds_1{ 5 }));
  BOOST_CHECK(av1.stride( ) == (poutre::index_1{ 1 }));
  BOOST_CHECK_EQUAL(av1[idxtype{ 0 }], 42);

  auto av1DFrom2D = poutre::array_view<int, 1>(view2d, { 2 });  // 1D view from 2D view      }
  BOOST_CHECK(av1DFrom2D.bounds( ) == (poutre::bounds_1{ 2 }));
  BOOST_CHECK(av1DFrom2D.stride( ) == (poutre::index_1{ 1 }));
  BOOST_CHECK_EQUAL(av1DFrom2D[idxtype{ 0 }], 42);
  }


BOOST_AUTO_TEST_CASE(section_and_slice_1d)
  {
  using view1DINt = poutre::array_view < int >;
  using idxtype = poutre::array_view<int>::index_type;
  using view2DINt = poutre::array_view < int, 2 >;
  using view1DFLOAT = poutre::array_view < float >;

  //1D view over std::vector
  std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  BOOST_CHECK_EQUAL(mif.size( ), 10);
  auto view = poutre::array_view < int >(mif);

  //get section of 1d view start at idx 2 
  auto section = view.section(idxtype{ 2 });
  BOOST_CHECK_EQUAL(section.size( ), 8);
  BOOST_CHECK(section.stride( ) == idxtype{ 1 });
  BOOST_CHECK(section.bounds( ) == (poutre::bounds_1{ 8 }));
  BOOST_CHECK_EQUAL(section[idxtype{ 0 }], 2);
  BOOST_CHECK_EQUAL(section[idxtype{ 1 }], 3);
  //get section of 1d view start at idx 2 with additional bound
  auto sectionwithbound = view.section(idxtype{ 2 }, (poutre::bounds_1{ 3 }));
  BOOST_CHECK_EQUAL(sectionwithbound.size( ), 3);
  BOOST_CHECK(sectionwithbound.stride( ) == idxtype{ 1 });
  BOOST_CHECK(sectionwithbound.bounds( ) == (poutre::bounds_1{ 3 }));
  BOOST_CHECK_EQUAL(sectionwithbound[idxtype{ 0 }], 2);
  BOOST_CHECK_EQUAL(sectionwithbound[idxtype{ 1 }], 3);
  //throw on invalid parameter
  BOOST_CHECK_THROW(view.section(idxtype{ 9 }), std::runtime_error);
  BOOST_CHECK_THROW(view.section(idxtype{ 0 }, (poutre::bounds_1{ 11 })),std::runtime_error);

  }


BOOST_AUTO_TEST_CASE(section_and_slice_2d)
  {
  using idxtype = poutre::array_view<int,1>::index_type;
  using idxtype2d = poutre::array_view<int,2>::index_type;
  
  std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  // 2D view over vec
  auto view2d = poutre::array_view<int, 2>(mif, { 2, 5 });
  //get slice of 2d view
  auto slice = view2d[1];
  BOOST_CHECK_EQUAL(slice.size( ), 5);
  BOOST_CHECK_EQUAL(slice[idxtype{ 0 }], 5);
  BOOST_CHECK_EQUAL(slice[idxtype{ 1 }], 6);
  BOOST_CHECK_EQUAL(slice[idxtype{ 2 }], 7);
  BOOST_CHECK_EQUAL(slice[idxtype{ 3 }], 8);
  BOOST_CHECK_EQUAL(slice[idxtype{ 4 }], 9);

  //get section of 2d view
  auto section = view2d.section(idxtype2d{1,3});
  BOOST_CHECK_EQUAL(section.size( ), 3);
  BOOST_CHECK(section.stride( ) == (idxtype2d{1,1}));
  BOOST_CHECK(section.bounds( ) == (poutre::bounds_2{1,3}));
  //BOOST_CHECK_EQUAL(section[idxtype{ 0 }], 2);
  //BOOST_CHECK_EQUAL(section[idxtype{ 1 }], 3);
  }

  //BOOST_AUTO_TEST_CASE(view_on_muti_dim_c_array)
  //  {
  //  char a[3][1][4] {{{'H', 'i'}}};
  //  auto av = poutre::array_view<char, 3>{a};
  //  BOOST_CHECK((av.bounds( ) == poutre::bounds<3>{3, 1, 4}));
  //  //assert((av[{0, 0, 0}] == 'H'));
  //  }
  //
  BOOST_AUTO_TEST_SUITE_END( )