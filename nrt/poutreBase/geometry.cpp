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
#include <poutreBase/poutreGeometry.hpp>

TEST(geometry, ctor)
{
    using namespace poutre;
    pt1D_scoord pt1;
    pt2D_scoord pt2;
    pt3D_scoord pt3;
    pt4D_scoord pt4;

    bb1D_scoord bb1;
    bb2D_scoord bb2;
    bb3D_scoord bb3;
    bb4D_scoord bb4;

    pt1D_rcoord ptr1;
    pt2D_rcoord ptr2;
    pt3D_rcoord ptr3;
    pt4D_rcoord ptr4;
}
