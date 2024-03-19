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
#include <gtest/gtest.h>
#include <iostream>
#include <poutreBase/poutreContainerView.hpp>

TEST(view, ctor)
{
    using view1DINt = poutre::array_view<int, 1>;

    view1DINt dummyview;
    EXPECT_EQ(dummyview.size(), 0);
    EXPECT_TRUE(dummyview.data() == nullptr);
    EXPECT_TRUE(dummyview.stride() == poutre::idx1d{1}); // stride is 1 for empty view

    // on 1D
    using sview1DINt = poutre::strided_array_view<int, 1>;

    sview1DINt dummysview;
    EXPECT_EQ(dummysview.size(), 0);
    // std::cout << "dummysview.stride()" << dummysview.stride() << std::endl;
    EXPECT_TRUE(dummysview.stride() == poutre::idx1d{0}); // stride is 0 for empty stridedview
}

TEST(view, basic_usage_view_over_vector)
{
    using view1DINt = poutre::array_view<int, 1>;

    std::vector<int> mif = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(mif.size(), 10);
    auto view = view1DINt(mif);

    EXPECT_EQ(view.size(), 10);
    EXPECT_TRUE(view.bound() == poutre::bd1d{10});
    view[poutre::idx1d{0}] = 42; // grmmlll poutre::idx1d{} to force resolution operator[index] and not slice ....
    EXPECT_EQ(mif[0], 42);       // v == 42

    auto viewcopy(view);
    EXPECT_EQ(viewcopy[poutre::idx1d{0}], 42); // v == 42
    auto viewcopy2 = view;
    EXPECT_EQ(viewcopy2[poutre::idx1d{0}], 42); // v == 42

    auto viewptr = view1DINt(mif.data(), {(ptrdiff_t)mif.size()});
    EXPECT_EQ(viewptr[poutre::idx1d{0}], 42); // v == 42

    auto view2d = poutre::array_view<int, 2>(mif, {2, 5}); // 2D view over vec
    EXPECT_EQ(view2d.size(), 10);
    EXPECT_TRUE(view2d.bound() == (poutre::bd2d{2, 5}));
    EXPECT_TRUE(view2d.stride() == (poutre::idx2d{5, 1}));
    EXPECT_EQ(view2d[(poutre::idx2d{0, 0})], 42);

    auto av1 = poutre::array_view<int, 1>(mif, {5}); // 1D view over vec with explicit sub bound
    EXPECT_TRUE(av1.bound() == (poutre::bd1d{5}));
    EXPECT_TRUE(av1.stride() == (poutre::idx1d{1}));
    EXPECT_EQ(av1[poutre::idx1d{0}], 42);

    auto av1DFrom2D = poutre::array_view<int, 1>(view2d, {2}); // 1D view from 2D view      }
    EXPECT_TRUE(av1DFrom2D.bound() == (poutre::bd1d{2}));
    EXPECT_TRUE(av1DFrom2D.stride() == (poutre::idx1d{1}));
    EXPECT_EQ(av1DFrom2D[poutre::idx1d{0}], 42);

    auto view2dsub = poutre::array_view<int, 2>(mif, {2, 3}); // 2D view over vec
    EXPECT_EQ(view2dsub.size(), 6);
    EXPECT_TRUE(view2dsub.bound() == (poutre::bd2d{2, 3}));
    EXPECT_TRUE(view2dsub.stride() == (poutre::idx2d{3, 1}));
    EXPECT_EQ(view2dsub[(poutre::idx2d{0, 0})], 42);
}

TEST(view, basic_usage_strided_view_over_vector)
{
    using sview1DINt = poutre::strided_array_view<int, 1>;

    std::vector<int> mif = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(mif.size(), 10);
    auto view = sview1DINt(&mif[0], {10}, {3});

    EXPECT_EQ(view.size(), 10);
    EXPECT_TRUE(view.bound() == poutre::bd1d{10});

    view[poutre::idx1d{0}] = 42; // grmmlll poutre::idx1d{} to force resolution operator[index] and not slice ....
    EXPECT_EQ(mif[0], 42);       // v == 42

    auto viewcopy(view);
    EXPECT_EQ(viewcopy[poutre::idx1d{0}], 42); // v == 42
    auto viewcopy2 = view;
    EXPECT_EQ(viewcopy2[poutre::idx1d{0}], 42); // v == 42
}

TEST(view, section_and_slice_1d)
{
    std::vector<int> mif = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(mif.size(), 10);
    auto view = poutre::array_view<int, 1>(mif);

    auto section = view.section(poutre::idx1d{2});
    EXPECT_EQ(section.size(), 8);
    EXPECT_TRUE(section.stride() == poutre::idx1d{1});
    EXPECT_TRUE(section.bound() == (poutre::bd1d{8}));
    EXPECT_EQ(section[(poutre::idx1d{0})], 2);
    EXPECT_EQ(section[(poutre::idx1d{1})], 3);

    auto sectionwithbound = view.section(poutre::idx1d{2}, (poutre::bd1d{3}));
    EXPECT_EQ(sectionwithbound.size(), 3);
    EXPECT_TRUE(sectionwithbound.stride() == (poutre::idx1d{1}));
    EXPECT_TRUE(sectionwithbound.bound() == (poutre::bd1d{3}));
    EXPECT_EQ(sectionwithbound[poutre::idx1d{0}], 2);
    EXPECT_EQ(sectionwithbound[poutre::idx1d{1}], 3);
}

TEST(view, section_and_slice_2d)
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
        auto view2d = poutre::array_view<int, 2>(mif, {5, 5});
        EXPECT_EQ(view2d.size(), 25);
        EXPECT_EQ(view2d.bound(), (poutre::bd2d{5, 5}));
        EXPECT_EQ(view2d.stride(), (poutre::idx2d{5, 1}));

        auto slicesefirstline = view2d[0];
        EXPECT_EQ(slicesefirstline.size(), 5);
        EXPECT_EQ(slicesefirstline.bound(), (poutre::bd1d{5}));
        EXPECT_EQ(slicesefirstline.stride(), (poutre::idx1d{1}));
        EXPECT_EQ(slicesefirstline[poutre::idx1d{0}], 0);
        EXPECT_EQ(slicesefirstline[poutre::idx1d{1}], 1);
        EXPECT_EQ(slicesefirstline[poutre::idx1d{2}], 2);
        EXPECT_EQ(slicesefirstline[poutre::idx1d{3}], 3);
        EXPECT_EQ(slicesefirstline[poutre::idx1d{4}], 4); //-V112

        auto slicesecondline = view2d[1];
        EXPECT_EQ(slicesecondline.size(), 5);
        EXPECT_EQ(slicesecondline.bound(), (poutre::bd1d{5}));
        EXPECT_EQ(slicesecondline.stride(), (poutre::idx1d{1}));
        EXPECT_EQ(slicesecondline[poutre::idx1d{0}], 5);
        EXPECT_EQ(slicesecondline[poutre::idx1d{1}], 6);
        EXPECT_EQ(slicesecondline[poutre::idx1d{2}], 7);
        EXPECT_EQ(slicesecondline[poutre::idx1d{3}], 8);
        EXPECT_EQ(slicesecondline[poutre::idx1d{4}], 9); //-V112

        auto slicethrirdline = view2d[2];
        EXPECT_EQ(slicethrirdline.size(), 5);
        EXPECT_EQ(slicethrirdline.bound(), (poutre::bd1d{5}));
        EXPECT_EQ(slicethrirdline.stride(), (poutre::idx1d{1}));
        EXPECT_EQ(slicethrirdline[poutre::idx1d{0}], 10);
        EXPECT_EQ(slicethrirdline[poutre::idx1d{1}], 11);
        EXPECT_EQ(slicethrirdline[poutre::idx1d{2}], 12);
        EXPECT_EQ(slicethrirdline[poutre::idx1d{3}], 13);
        EXPECT_EQ(slicethrirdline[poutre::idx1d{4}], 14); //-V112
    }

    {
        auto view2d = poutre::array_view<int, 2>(mif, {5, 5});
        auto section2d = view2d.section(poutre::idx2d{1, 1});
        EXPECT_EQ(section2d.size(), 16);
        EXPECT_EQ(section2d.bound(), (poutre::bd2d({4, 4}))); //-V112
        EXPECT_EQ(section2d.stride(), (poutre::idx2d({5, 1})));
        EXPECT_EQ(section2d[poutre::idx2d({0, 0})], 6);
        EXPECT_EQ(section2d[poutre::idx2d({0, 1})], 7);
        EXPECT_EQ(section2d[poutre::idx2d({0, 2})], 8);
        EXPECT_EQ(section2d[poutre::idx2d({0, 3})], 9);
        EXPECT_EQ(section2d[poutre::idx2d({1, 0})], 11);
        EXPECT_EQ(section2d[poutre::idx2d({1, 1})], 12);
        EXPECT_EQ(section2d[poutre::idx2d({1, 2})], 13);
        EXPECT_EQ(section2d[poutre::idx2d({1, 3})], 14);
        EXPECT_EQ(section2d[poutre::idx2d({2, 0})], 16);
        EXPECT_EQ(section2d[poutre::idx2d({2, 1})], 17);
        EXPECT_EQ(section2d[poutre::idx2d({2, 2})], 18);
        EXPECT_EQ(section2d[poutre::idx2d({2, 3})], 19);
        EXPECT_EQ(section2d[poutre::idx2d({3, 0})], 21);
        EXPECT_EQ(section2d[poutre::idx2d({3, 1})], 22);
        EXPECT_EQ(section2d[poutre::idx2d({3, 2})], 23);
        EXPECT_EQ(section2d[poutre::idx2d({3, 3})], 24);
    }
    {
        std::vector<int> tmp(30);
        for (auto i = 0; i < 30; ++i)
        {
            tmp[i] = i;
        }
        auto view2d = poutre::array_view<int, 2>(tmp, {5, 6});
        auto section2d = view2d.section(poutre::idx2d{1, 1});
        EXPECT_EQ(section2d.size(), 20);
        EXPECT_EQ(section2d.bound(), (poutre::bd2d({4, 5}))); //-V112
        EXPECT_EQ(section2d.stride(), (poutre::idx2d({6, 1})));
        EXPECT_EQ(section2d[poutre::idx2d({0, 0})], 7);
        EXPECT_EQ(section2d[poutre::idx2d({0, 1})], 8);
        EXPECT_EQ(section2d[poutre::idx2d({0, 2})], 9);
        EXPECT_EQ(section2d[poutre::idx2d({0, 3})], 10);
        EXPECT_EQ(section2d[poutre::idx2d({0, 4})], 11); //-V112

        EXPECT_EQ(section2d[poutre::idx2d({3, 0})], 25);
        EXPECT_EQ(section2d[poutre::idx2d({3, 1})], 26);
        EXPECT_EQ(section2d[poutre::idx2d({3, 2})], 27);
        EXPECT_EQ(section2d[poutre::idx2d({3, 3})], 28);
        EXPECT_EQ(section2d[poutre::idx2d({3, 4})], 29); //-V112
    }
    {
        auto view2d = poutre::array_view<int, 2>(mif, {5, 5});
        auto section2d = view2d.section(poutre::idx2d{1, 2});
        EXPECT_EQ(section2d.size(), 12);
        EXPECT_EQ(section2d.bound(), (poutre::bd2d({4, 3}))); //-V112
        EXPECT_EQ(section2d.stride(), (poutre::idx2d({5, 1})));
        // auto bnd = section2d.bound();
        // std::cout << std::endl;
        // for (auto idx : bnd)
        //{
        //    std::cout << idx << std::endl;
        //}

        EXPECT_EQ(section2d[poutre::idx2d({0, 0})], 7);
        EXPECT_EQ(section2d[poutre::idx2d({0, 1})], 8);
        EXPECT_EQ(section2d[poutre::idx2d({0, 2})], 9);
        EXPECT_EQ(section2d[poutre::idx2d({1, 0})], 12);
        EXPECT_EQ(section2d[poutre::idx2d({1, 1})], 13);
        EXPECT_EQ(section2d[poutre::idx2d({1, 2})], 14);
        EXPECT_EQ(section2d[poutre::idx2d({2, 0})], 17);
        EXPECT_EQ(section2d[poutre::idx2d({2, 1})], 18);
        EXPECT_EQ(section2d[poutre::idx2d({2, 2})], 19);
        EXPECT_EQ(section2d[poutre::idx2d({3, 0})], 22);
        EXPECT_EQ(section2d[poutre::idx2d({3, 1})], 23);
        EXPECT_EQ(section2d[poutre::idx2d({3, 2})], 24);

        auto slice = section2d[0];
        EXPECT_EQ(slice.size(), 3);
        EXPECT_EQ(slice.bound(), (poutre::bd1d({3})));
        EXPECT_EQ(slice.stride(), (poutre::idx1d({1})));

        EXPECT_EQ(slice[poutre::idx1d({0})], 7);
        EXPECT_EQ(slice[poutre::idx1d({1})], 8);
        EXPECT_EQ(slice[poutre::idx1d({2})], 9);

        auto slice2 = section2d[1];
        EXPECT_EQ(slice2.size(), 3);
        EXPECT_EQ(slice2.bound(), (poutre::bd1d({3})));
        EXPECT_EQ(slice2.stride(), (poutre::idx1d({1})));

        EXPECT_EQ(slice2[poutre::idx1d({0})], 12);
        EXPECT_EQ(slice2[poutre::idx1d({1})], 13);
        EXPECT_EQ(slice2[poutre::idx1d({2})], 14);
    }

    {
        auto view2d = poutre::array_view<int, 2>(mif, {5, 5});
        auto section2d = view2d.section((poutre::idx2d{1, 2}), (poutre::bd2d{3, 2}));
        EXPECT_EQ(section2d.size(), 6);
        EXPECT_EQ(section2d.bound(), (poutre::bd2d({3, 2})));
        EXPECT_EQ(section2d.stride(), (poutre::idx2d({5, 1})));

        //  for(auto idx : section2d.bound())
        //  {
        //      std::cout<<"\nidx"<<idx;
        //      std::cout<<"\nvalue"<<section2d[idx];
        //  }
        EXPECT_EQ(section2d[poutre::idx2d({0, 0})], 7);
        EXPECT_EQ(section2d[poutre::idx2d({0, 1})], 8);
        EXPECT_EQ(section2d[poutre::idx2d({1, 0})], 12);
        EXPECT_EQ(section2d[poutre::idx2d({1, 1})], 13);
        EXPECT_EQ(section2d[poutre::idx2d({2, 0})], 17);
        EXPECT_EQ(section2d[poutre::idx2d({2, 1})], 18);

        //  * 1  2  3 *
        //  * 6  7  8 *
        //  * *  *  * *
        //  * *  *  * *
        //  * *  *  * *
        auto sec2d = view2d.section((poutre::idx2d{0, 1}), (poutre::bd2d{2, 3}));
        EXPECT_EQ(sec2d.size(), 6);
        EXPECT_EQ(sec2d.bound(), (poutre::bd2d({2, 3})));
        EXPECT_EQ(sec2d.stride(), (poutre::idx2d({5, 1})));

        //  for(auto idx : sec2d.bound())
        //  {
        //      std::cout<<"\nidx"<<idx;
        //      std::cout<<"\nvalue"<<sec2d[idx];
        //  }
        EXPECT_EQ(sec2d[poutre::idx2d({0, 0})], 1);
        EXPECT_EQ(sec2d[poutre::idx2d({0, 1})], 2);
        EXPECT_EQ(sec2d[poutre::idx2d({0, 2})], 3);
        EXPECT_EQ(sec2d[poutre::idx2d({1, 0})], 6);
        EXPECT_EQ(sec2d[poutre::idx2d({1, 1})], 7);
        EXPECT_EQ(sec2d[poutre::idx2d({1, 2})], 8);

        //  1 2 3
        //  6 7 8
        auto subsec2d = sec2d.section((poutre::idx2d{1, 1})); //,(poutre::bd2d{1,2}));
       /* for (auto idx : subsec2d.bound())
        {
            std::cout << "\nidx" << idx;
            std::cout << "\nvalue" << subsec2d[idx];
        }*/

        EXPECT_EQ(subsec2d.size(), 2);
        EXPECT_EQ(subsec2d.bound(), (poutre::bd2d({1, 2})));
        EXPECT_EQ(subsec2d.stride(), (poutre::idx2d({5, 1})));
        EXPECT_EQ(subsec2d[poutre::idx2d({0, 0})], 7);
        EXPECT_EQ(subsec2d[poutre::idx2d({0, 1})], 8);
    }
}

TEST(view, view_on_c_array)
{
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto av = poutre::array_view<int, 1>{a};
    EXPECT_EQ(av.size(), 10);
    EXPECT_TRUE(av.bound() == poutre::bd1d{10});
    // assignment through view
    av[poutre::idx1d{0}] = 42; // grmmlll poutre::idx1d{} to force resolution operator[index] and not slice ....
    EXPECT_EQ(a[0], 42);       // v == 42
    // copy view
    auto viewcopy(av);
    EXPECT_EQ(viewcopy[poutre::idx1d{0}], 42); // v == 42
    auto viewcopy2 = av;
    EXPECT_EQ(viewcopy2[poutre::idx1d{0}], 42); // v == 42
}

TEST(view, strided_view)
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
        // auto view2d = poutre::array_view<int, 2>(mif, { 5, 5 });
        auto sview2d = poutre::strided_array_view<int, 2>(&mif[0], poutre::bd2d{5, 2},
                                                          poutre::idx2d{5, 3}); // stride in least significant dimension

        EXPECT_EQ(sview2d[poutre::idx2d({0, 0})], 0);
        EXPECT_EQ(sview2d[poutre::idx2d({0, 1})], 3);
        EXPECT_EQ(sview2d[poutre::idx2d({1, 0})], 5);
        EXPECT_EQ(sview2d[poutre::idx2d({1, 1})], 8);
        EXPECT_EQ(sview2d[poutre::idx2d({2, 0})], 10);
        EXPECT_EQ(sview2d[poutre::idx2d({2, 1})], 13);
        EXPECT_EQ(sview2d[poutre::idx2d({3, 0})], 15);
        EXPECT_EQ(sview2d[poutre::idx2d({3, 1})], 18);
        EXPECT_EQ(sview2d[poutre::idx2d({4, 0})], 20); //-V112
        EXPECT_EQ(sview2d[poutre::idx2d({4, 1})], 23); //-V112
    }
    {
        auto sview2d = poutre::strided_array_view<int, 2>(&mif[0], poutre::bd2d{3, 5}, poutre::idx2d{10, 1});
        EXPECT_EQ(sview2d[poutre::idx2d({0, 0})], 0);
        EXPECT_EQ(sview2d[poutre::idx2d({0, 1})], 1);
        EXPECT_EQ(sview2d[poutre::idx2d({0, 2})], 2);
        EXPECT_EQ(sview2d[poutre::idx2d({0, 3})], 3);
        EXPECT_EQ(sview2d[poutre::idx2d({0, 4})], 4); //-V112

        EXPECT_EQ(sview2d[poutre::idx2d({1, 0})], 10);
        EXPECT_EQ(sview2d[poutre::idx2d({1, 1})], 11);
        EXPECT_EQ(sview2d[poutre::idx2d({1, 2})], 12);
        EXPECT_EQ(sview2d[poutre::idx2d({1, 3})], 13);
        EXPECT_EQ(sview2d[poutre::idx2d({1, 4})], 14); //-V112

        EXPECT_EQ(sview2d[poutre::idx2d({2, 0})], 20);
        EXPECT_EQ(sview2d[poutre::idx2d({2, 1})], 21);
        EXPECT_EQ(sview2d[poutre::idx2d({2, 2})], 22);
        EXPECT_EQ(sview2d[poutre::idx2d({2, 3})], 23);
        EXPECT_EQ(sview2d[poutre::idx2d({2, 4})], 24); //-V112
    }

    {
        auto sview2d = poutre::strided_array_view<int, 2>(&mif[0], poutre::bd2d{3, 2},
                                                          poutre::idx2d{10, 3}); // two different stride
        EXPECT_EQ(sview2d[poutre::idx2d({0, 0})], 0);
        EXPECT_EQ(sview2d[poutre::idx2d({0, 1})], 3);
        EXPECT_EQ(sview2d[poutre::idx2d({1, 0})], 10);
        EXPECT_EQ(sview2d[poutre::idx2d({1, 1})], 13);
        EXPECT_EQ(sview2d[poutre::idx2d({2, 0})], 20);
        EXPECT_EQ(sview2d[poutre::idx2d({2, 1})], 23);
    }
}

TEST(view, static_dispatch)
{
    static_assert(false == poutre::is_strided<poutre::array_view<int, 1>>::value, "oups");
    static_assert(true == poutre::is_strided<poutre::strided_array_view<int, 1>>::value, "oups");
    static_assert(false == poutre::is_strided<poutre::array_view<const int, 1>>::value, "oups");
    static_assert(true == poutre::is_strided<poutre::strided_array_view<const int, 1>>::value, "oups");
}
