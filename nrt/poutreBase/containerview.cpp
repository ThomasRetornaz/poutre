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
#include <poutreBase/poutreContainerView.hpp>

BOOST_AUTO_TEST_SUITE(ContainerView)

BOOST_AUTO_TEST_CASE(ctor)
{
    BOOST_TEST_MESSAGE("view");
    using view1DINt = poutre::array_view < int, 1 >;
    using view2DINt = poutre::array_view < int, 2 >;
    using view1DFLOAT = poutre::array_view < float, 1 >;

    BOOST_TEST_MESSAGE("empty view");
    view1DINt dummyview;
    BOOST_CHECK_EQUAL(dummyview.size(), 0);
    BOOST_CHECK(dummyview.data() == nullptr);
    BOOST_CHECK(dummyview.stride() == poutre::idx1d{ 1 }); //stride is 1 for empty view
    //BOOST_CHECK_THROW(dummyview[idxtype{ 0 }],std::runtime_error); //throw in debug
    //BOOST_CHECK_THROW(dummyview[5], std::runtime_error); //can't slice
    //on 1D
    BOOST_CHECK_THROW(dummyview.section(poutre::bd1d{ 3 }), std::runtime_error); //section

    BOOST_TEST_MESSAGE("sview");
    using sview1DINt = poutre::strided_array_view < int, 1 >;
    using sview2DINt = poutre::strided_array_view < int, 2 >;
    using sview1DFLOAT = poutre::strided_array_view< float, 1 >;

    BOOST_TEST_MESSAGE("empty sview");
    sview1DINt dummysview;
    BOOST_CHECK_EQUAL(dummysview.size(), 0);
    BOOST_CHECK(dummysview.stride() == poutre::idx1d{ 1 }); //stride is 1 for empty stridedview
    //BOOST_CHECK_THROW(dummysview[idxtype{ 0 }], std::runtime_error); //throw in debug
    //BOOST_CHECK_THROW(dummysview[5], std::runtime_error); //can't slice on 1D
    //BOOST_CHECK_THROW(dummysview.section(poutre::bd1d{ 3 }), std::runtime_error); //section
}


BOOST_AUTO_TEST_CASE(basic_usage_view_over_vector)
{
    using view1DINt = poutre::array_view < int, 1 >;
    using view2DINt = poutre::array_view < int, 2 >;
    using view1DFLOAT = poutre::array_view < float, 1 >;


    BOOST_TEST_MESSAGE("1D view over std::vector");
    std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    BOOST_CHECK_EQUAL(mif.size(), 10);
    auto view = view1DINt(mif);

    BOOST_CHECK_EQUAL(view.size(), 10);
    BOOST_CHECK(view.bound() == poutre::bd1d{ 10 });
    BOOST_TEST_MESSAGE("assignment through view");
    view[poutre::idx1d{ 0 }] = 42; //grmmlll poutre::idx1d{} to force resolution operator[index] and not slice ....
    BOOST_CHECK_EQUAL(mif[0], 42);  // v == 42
    BOOST_TEST_MESSAGE("copy view");
    auto viewcopy(view);
    BOOST_CHECK_EQUAL(viewcopy[poutre::idx1d{ 0 }], 42);  // v == 42
    auto viewcopy2 = view;
    BOOST_CHECK_EQUAL(viewcopy2[poutre::idx1d{ 0 }], 42);  // v == 42

    BOOST_TEST_MESSAGE("array view from ptr and bounds");
    auto viewptr = view1DINt(mif.data(), { (ptrdiff_t)mif.size() });
    BOOST_CHECK_EQUAL(viewptr[poutre::idx1d{ 0 }], 42);  // v == 42

    BOOST_TEST_MESSAGE("2D view over vec");
    auto view2d = poutre::array_view<int, 2>(mif, { 2, 5 });  // 2D view over vec
    BOOST_CHECK_EQUAL(view2d.size(), 10);
    BOOST_CHECK(view2d.bound() == (poutre::bd2d{ 2, 5 }));
    BOOST_CHECK(view2d.stride() == (poutre::idx2d{ 5, 1 }));
    BOOST_CHECK_EQUAL(view2d[(poutre::idx2d{ 0, 0 })], 42);

    BOOST_TEST_MESSAGE("1D view over vec with explicit sub bound");
    auto av1 = poutre::array_view<int, 1>(mif, { 5 });  // 1D view over vec with explicit sub bound
    BOOST_CHECK(av1.bound() == (poutre::bd1d{ 5 }));
    BOOST_CHECK(av1.stride() == (poutre::idx1d{ 1 }));
    BOOST_CHECK_EQUAL(av1[poutre::idx1d{ 0 }], 42);

    BOOST_TEST_MESSAGE("1D view from 2D view");
    auto av1DFrom2D = poutre::array_view<int, 1>(view2d, { 2 });  // 1D view from 2D view      }
    BOOST_CHECK(av1DFrom2D.bound() == (poutre::bd1d{ 2 }));
    BOOST_CHECK(av1DFrom2D.stride() == (poutre::idx1d{ 1 }));
    BOOST_CHECK_EQUAL(av1DFrom2D[poutre::idx1d{ 0 }], 42);

    BOOST_TEST_MESSAGE("2D view over vec subview");
    auto view2dsub = poutre::array_view<int, 2>(mif, { 2, 3});  // 2D view over vec
    BOOST_CHECK_EQUAL(view2dsub.size(), 6);
    BOOST_CHECK(view2dsub.bound() == (poutre::bd2d{ 2, 3 }));
    BOOST_CHECK(view2dsub.stride() == (poutre::idx2d{3, 1 }));
    BOOST_CHECK_EQUAL(view2dsub[(poutre::idx2d{ 0, 0 })], 42);
}


BOOST_AUTO_TEST_CASE(basic_usage_strided_view_over_vector)
{
    using sview1DINt = poutre::strided_array_view < int, 1 >;
    using sview2DINt = poutre::strided_array_view < int, 2 >;
    using sview1DFLOAT = poutre::strided_array_view < float, 1 >;


    BOOST_TEST_MESSAGE("1D view over std::vector");
    std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    BOOST_CHECK_EQUAL(mif.size(), 10);
    auto view = sview1DINt(&mif[0], { 10 }, { 3 });

    BOOST_CHECK_EQUAL(view.size(), 10);
    BOOST_CHECK(view.bound() == poutre::bd1d{ 10 });
    BOOST_TEST_MESSAGE("assignment through view");
    view[poutre::idx1d{ 0 }] = 42; //grmmlll poutre::idx1d{} to force resolution operator[index] and not slice ....
    BOOST_CHECK_EQUAL(mif[0], 42);  // v == 42
    BOOST_TEST_MESSAGE("copy view");
    auto viewcopy(view);
    BOOST_CHECK_EQUAL(viewcopy[poutre::idx1d{ 0 }], 42);  // v == 42
    auto viewcopy2 = view;
    BOOST_CHECK_EQUAL(viewcopy2[poutre::idx1d{ 0 }], 42);  // v == 42


}

BOOST_AUTO_TEST_CASE(section_and_slice_1d)
{
    using view1DINt = poutre::array_view < int, 1 >;
    using view2DINt = poutre::array_view < int, 2 >;
    using view1DFLOAT = poutre::array_view < float, 1 >;

    BOOST_TEST_MESSAGE("1D view over std::vector");
    std::vector<int> mif = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    BOOST_CHECK_EQUAL(mif.size(), 10);
    auto view = poutre::array_view < int, 1 >(mif);

    BOOST_TEST_MESSAGE("get section of 1d view start at idx 2 ");
    auto section = view.section(poutre::idx1d{ 2 });
    BOOST_CHECK_EQUAL(section.size(), 8);
    BOOST_CHECK(section.stride() == poutre::idx1d{ 1 });
    BOOST_CHECK(section.bound() == (poutre::bd1d{ 8 }));
    BOOST_CHECK_EQUAL(section[(poutre::idx1d{ 0 })], 2);
    BOOST_CHECK_EQUAL(section[(poutre::idx1d{ 1 })], 3);

    BOOST_TEST_MESSAGE("get section of 1d view start at idx 2 with additional bound");
    auto sectionwithbound = view.section(poutre::idx1d{ 2 }, (poutre::bd1d{ 3 }));
    BOOST_CHECK_EQUAL(sectionwithbound.size(), 3);
    BOOST_CHECK(sectionwithbound.stride() == (poutre::idx1d{ 1 }));
    BOOST_CHECK(sectionwithbound.bound() == (poutre::bd1d{ 3 }));
    BOOST_CHECK_EQUAL(sectionwithbound[poutre::idx1d{ 0 }], 2);
    BOOST_CHECK_EQUAL(sectionwithbound[poutre::idx1d{ 1 }], 3);

    BOOST_TEST_MESSAGE("throw on invalid parameter");
    BOOST_CHECK_THROW(view.section(poutre::idx1d{ 10 }), std::runtime_error);
    BOOST_CHECK_THROW(view.section(poutre::idx1d{ 0 }, (poutre::bd1d{ 11 })), std::runtime_error);

}


BOOST_AUTO_TEST_CASE(section_and_slice_2d)
{
    // view 1d view 2d  with section {{1,2},{3,2}} highlighted
    //    
    //  * * * * * * * 7 8 * * * 12 13 * * * 17 18 * * * * * *
    //    
    //  * *  *  * *   
    //  * *  7  8 *
    //  * * 12 13 *
    //  * * 17 18 *
    //  * *  *  * *

    std::vector<int> mif(25);
    for (auto i = 0; i < 25; ++i)
    {
        mif[i] = i;
    }

    {
        auto view2d = poutre::array_view<int, 2>(mif, { 5, 5 });
        BOOST_CHECK_EQUAL(view2d.size(), 25);
        BOOST_CHECK_EQUAL(view2d.bound(), (poutre::bd2d{ 5, 5 }));
        BOOST_CHECK_EQUAL(view2d.stride(), (poutre::idx2d{ 5, 1 }));

        BOOST_TEST_MESSAGE("get slice of 2d view");

        auto slicesefirstline = view2d[0];
        BOOST_CHECK_EQUAL(slicesefirstline.size(), 5);
        BOOST_CHECK_EQUAL(slicesefirstline.bound(), (poutre::bd1d{ 5 }));
        BOOST_CHECK_EQUAL(slicesefirstline.stride(), (poutre::idx1d{ 1 }));
        BOOST_CHECK_EQUAL(slicesefirstline[poutre::idx1d{ 0 }], 0);
        BOOST_CHECK_EQUAL(slicesefirstline[poutre::idx1d{ 1 }], 1);
        BOOST_CHECK_EQUAL(slicesefirstline[poutre::idx1d{ 2 }], 2);
        BOOST_CHECK_EQUAL(slicesefirstline[poutre::idx1d{ 3 }], 3);
        BOOST_CHECK_EQUAL(slicesefirstline[poutre::idx1d{ 4 }], 4);

        auto slicesecondline = view2d[1];
        BOOST_CHECK_EQUAL(slicesecondline.size(), 5);
        BOOST_CHECK_EQUAL(slicesecondline.bound(), (poutre::bd1d{ 5 }));
        BOOST_CHECK_EQUAL(slicesecondline.stride(), (poutre::idx1d{ 1 }));
        BOOST_CHECK_EQUAL(slicesecondline[poutre::idx1d{ 0 }], 5);
        BOOST_CHECK_EQUAL(slicesecondline[poutre::idx1d{ 1 }], 6);
        BOOST_CHECK_EQUAL(slicesecondline[poutre::idx1d{ 2 }], 7);
        BOOST_CHECK_EQUAL(slicesecondline[poutre::idx1d{ 3 }], 8);
        BOOST_CHECK_EQUAL(slicesecondline[poutre::idx1d{ 4 }], 9);

        auto slicethrirdline = view2d[2];
        BOOST_CHECK_EQUAL(slicethrirdline.size(), 5);
        BOOST_CHECK_EQUAL(slicethrirdline.bound(), (poutre::bd1d{ 5 }));
        BOOST_CHECK_EQUAL(slicethrirdline.stride(), (poutre::idx1d{ 1 }));
        BOOST_CHECK_EQUAL(slicethrirdline[poutre::idx1d{ 0 }], 10);
        BOOST_CHECK_EQUAL(slicethrirdline[poutre::idx1d{ 1 }], 11);
        BOOST_CHECK_EQUAL(slicethrirdline[poutre::idx1d{ 2 }], 12);
        BOOST_CHECK_EQUAL(slicethrirdline[poutre::idx1d{ 3 }], 13);
        BOOST_CHECK_EQUAL(slicethrirdline[poutre::idx1d{ 4 }], 14);


    }

    {
        BOOST_TEST_MESSAGE("get section of 2d view no specific bound");
        auto view2d = poutre::array_view<int, 2>(mif, { 5, 5 });
        auto section2d = view2d.section(poutre::idx2d{ 1,1 });
        BOOST_CHECK_EQUAL(section2d.size(), 16);
        BOOST_CHECK_EQUAL(section2d.bound(), (poutre::bd2d({ 4,4 })));
        BOOST_CHECK_EQUAL(section2d.stride(), (poutre::idx2d({ 5,1 })));
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,0 })], 6);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,1 })], 7);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,2 })], 8);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,3 })], 9);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,0 })], 11);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,1 })], 12);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,2 })], 13);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,3 })], 14);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,0 })], 16);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,1 })], 17);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,2 })], 18);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,3 })], 19);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,0 })], 21);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,1 })], 22);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,2 })], 23);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,3 })], 24);
    }
    {
        BOOST_TEST_MESSAGE("get section of 2d view no specific bound");
        std::vector<int> tmp(30);
        for (auto i = 0; i < 30; ++i)
        {
            tmp[i] = i;
        }
        auto view2d = poutre::array_view<int, 2>(tmp, { 5, 6 });
        auto section2d = view2d.section(poutre::idx2d{ 1,1 });
        BOOST_CHECK_EQUAL(section2d.size(), 20);
        BOOST_CHECK_EQUAL(section2d.bound(), (poutre::bd2d({ 4,5 })));
        BOOST_CHECK_EQUAL(section2d.stride(), (poutre::idx2d({ 6,1 })));
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,0 })], 7);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,1 })], 8);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,2 })], 9);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,3 })], 10);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,4 })], 11);
        
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,0 })], 25);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,1 })], 26);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,2 })], 27);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,3 })], 28);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,4 })], 29);
    }
    {
        BOOST_TEST_MESSAGE("get section of 2d view");
        auto view2d = poutre::array_view<int, 2>(mif, { 5, 5 });
        auto section2d = view2d.section(poutre::idx2d{ 1,2 });
        BOOST_CHECK_EQUAL(section2d.size(), 12);
        BOOST_CHECK_EQUAL(section2d.bound(), (poutre::bd2d({ 4,3 })));
        BOOST_CHECK_EQUAL(section2d.stride(), (poutre::idx2d({ 5,1 })));
        //auto bnd = section2d.bound();
        //std::cout << std::endl;
        //for (auto idx : bnd)
        //{
        //    std::cout << idx << std::endl;
        //}

        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,0 })], 7);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,1 })], 8);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,2 })], 9);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,0 })], 12);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,1 })], 13);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,2 })], 14);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,0 })], 17);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,1 })], 18);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,2 })], 19);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,0 })], 22);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,1 })], 23);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 3,2 })], 24);

        BOOST_TEST_MESSAGE("get slice on strided view");
        auto slice = section2d[0];
        BOOST_CHECK_EQUAL(slice.size(), 3);
        BOOST_CHECK_EQUAL(slice.bound(), (poutre::bd1d({ 3 })));
        BOOST_CHECK_EQUAL(slice.stride(), (poutre::idx1d({ 1 })));

        BOOST_CHECK_EQUAL(slice[poutre::idx1d({ 0 })], 7);
        BOOST_CHECK_EQUAL(slice[poutre::idx1d({ 1 })], 8);
        BOOST_CHECK_EQUAL(slice[poutre::idx1d({ 2 })], 9);


        auto slice2 = section2d[1];
        BOOST_CHECK_EQUAL(slice2.size(), 3);
        BOOST_CHECK_EQUAL(slice2.bound(), (poutre::bd1d({ 3 })));
        BOOST_CHECK_EQUAL(slice2.stride(), (poutre::idx1d({ 1 })));

        BOOST_CHECK_EQUAL(slice2[poutre::idx1d({ 0 })], 12);
        BOOST_CHECK_EQUAL(slice2[poutre::idx1d({ 1 })], 13);
        BOOST_CHECK_EQUAL(slice2[poutre::idx1d({ 2 })], 14);
    }


    {

        BOOST_TEST_MESSAGE("get section of 2d view with explicit bound");
        auto view2d = poutre::array_view<int, 2>(mif, { 5, 5 });
        auto section2d = view2d.section((poutre::idx2d{ 1,2 }), (poutre::bd2d{ 3,2 }));
        BOOST_CHECK_EQUAL(section2d.size(), 6);
        BOOST_CHECK_EQUAL(section2d.bound(), (poutre::bd2d({ 3,2 })));
        BOOST_CHECK_EQUAL(section2d.stride(), (poutre::idx2d({ 5,1 })));

        //  for(auto idx : section2d.bound())
        //  {
        //      std::cout<<"\nidx"<<idx;
        //      std::cout<<"\nvalue"<<section2d[idx];
        //  }
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,0 })], 7);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 0,1 })], 8);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,0 })], 12);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 1,1 })], 13);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,0 })], 17);
        BOOST_CHECK_EQUAL(section2d[poutre::idx2d({ 2,1 })], 18);


        //  * 1  2  3 *   
        //  * 6  7  8 *
        //  * *  *  * *
        //  * *  *  * *
        //  * *  *  * *
        BOOST_TEST_MESSAGE("get section of 2d view with explicit bound 2");
        auto sec2d = view2d.section((poutre::idx2d{ 0,1 }), (poutre::bd2d{ 2,3 }));
        BOOST_CHECK_EQUAL(sec2d.size(), 6);
        BOOST_CHECK_EQUAL(sec2d.bound(), (poutre::bd2d({ 2,3 })));
        BOOST_CHECK_EQUAL(sec2d.stride(), (poutre::idx2d({ 5,1 })));

        //  for(auto idx : sec2d.bound())
        //  {
        //      std::cout<<"\nidx"<<idx;
        //      std::cout<<"\nvalue"<<sec2d[idx];
        //  }
        BOOST_CHECK_EQUAL(sec2d[poutre::idx2d({ 0,0 })], 1);
        BOOST_CHECK_EQUAL(sec2d[poutre::idx2d({ 0,1 })], 2);
        BOOST_CHECK_EQUAL(sec2d[poutre::idx2d({ 0,2 })], 3);
        BOOST_CHECK_EQUAL(sec2d[poutre::idx2d({ 1,0 })], 6);
        BOOST_CHECK_EQUAL(sec2d[poutre::idx2d({ 1,1 })], 7);
        BOOST_CHECK_EQUAL(sec2d[poutre::idx2d({ 1,2 })], 8);

        //  1 2 3    
        //  6 7 8
        BOOST_TEST_MESSAGE("get section of 2d section with explicit bound");
        auto subsec2d = sec2d.section((poutre::idx2d{ 1,1 }));//,(poutre::bd2d{1,2}));
        for (auto idx : subsec2d.bound())
        {
            std::cout << "\nidx" << idx;
            std::cout << "\nvalue" << subsec2d[idx];
        }

        BOOST_CHECK_EQUAL(subsec2d.size(), 2);
        BOOST_CHECK_EQUAL(subsec2d.bound(), (poutre::bd2d({ 1,2 })));
        BOOST_CHECK_EQUAL(subsec2d.stride(), (poutre::idx2d({ 5,1 })));
        BOOST_CHECK_EQUAL(subsec2d[poutre::idx2d({ 0,0 })], 7);
        BOOST_CHECK_EQUAL(subsec2d[poutre::idx2d({ 0,1 })], 8);

    }
}

BOOST_AUTO_TEST_CASE(view_on_c_array)
{
    int a[10] = { 0,1,2,3,4,5,6,7,8,9 };
    auto av = poutre::array_view<int, 1>{ a };
    BOOST_CHECK_EQUAL(av.size(), 10);
    BOOST_CHECK(av.bound() == poutre::bd1d{ 10 });
    //assignment through view
    av[poutre::idx1d{ 0 }] = 42; //grmmlll poutre::idx1d{} to force resolution operator[index] and not slice ....
    BOOST_CHECK_EQUAL(a[0], 42);  // v == 42
    //copy view
    auto viewcopy(av);
    BOOST_CHECK_EQUAL(viewcopy[poutre::idx1d{ 0 }], 42);  // v == 42
    auto viewcopy2 = av;
    BOOST_CHECK_EQUAL(viewcopy2[poutre::idx1d{ 0 }], 42);  // v == 42
}

BOOST_AUTO_TEST_CASE(strided_view)
{
    // view 1d view 2d  with section {{1,2},{3,2}} highlighted
    //    
    //  * * * * * * * 7 8 * * * 12 13 * * * 17 18 * * * * * *
    //    
    //   0  1  2  3  4    
    //   5  6  7  8  9
    //  10 11 12 13 14
    //   *  * 17 18  *
    //   *  *  *  *  *

    std::vector<int> mif(25);
    for (auto i = 0; i < 25; ++i)
    {
        mif[i] = i;
    }
    {
        //auto view2d = poutre::array_view<int, 2>(mif, { 5, 5 });
        auto sview2d = poutre::strided_array_view<int, 2>(&mif[0], poutre::bd2d{ 5, 2 }, poutre::idx2d{ 5,3 }); //stride in least significant dimension

        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,0 })], 0);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,1 })], 3);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,0 })], 5);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,1 })], 8);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,0 })], 10);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,1 })], 13);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 3,0 })], 15);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 3,1 })], 18);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 4,0 })], 20);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 4,1 })], 23);
    }
    {
        auto sview2d = poutre::strided_array_view<int, 2>(&mif[0], poutre::bd2d{ 3, 5 }, poutre::idx2d{ 10,1 });
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,0 })], 0);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,1 })], 1);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,2 })], 2);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,3 })], 3);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,4 })], 4);

        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,0 })], 10);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,1 })], 11);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,2 })], 12);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,3 })], 13);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,4 })], 14);

        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,0 })], 20);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,1 })], 21);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,2 })], 22);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,3 })], 23);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,4 })], 24);

    }

    {
        auto sview2d = poutre::strided_array_view<int, 2>(&mif[0], poutre::bd2d{ 3, 2 }, poutre::idx2d{ 10,3 }); //two different stride
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,0 })], 0);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 0,1 })], 3);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,0 })], 10);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 1,1 })], 13);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,0 })], 20);
        BOOST_CHECK_EQUAL(sview2d[poutre::idx2d({ 2,1 })], 23);
    }
   
}


BOOST_AUTO_TEST_CASE(static_dispatch)
{

    static_assert(false == poutre::is_strided<poutre::array_view< int, 1 > >::value, "oups");
    static_assert(true == poutre::is_strided<poutre::strided_array_view< int, 1 > >::value, "oups");
    static_assert(false == poutre::is_strided<poutre::array_view< const int, 1 > >::value, "oups");
    static_assert(true == poutre::is_strided<poutre::strided_array_view< const int, 1 > >::value, "oups");

}
BOOST_AUTO_TEST_SUITE_END()
