
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"
#include <vector>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include <poutreBase/poutreContainerView.hpp>
#include <poutreIO/poutreIOString.hpp>


#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingCompare.hpp>

namespace bs = boost::simd;

BOOST_AUTO_TEST_SUITE(poutreImageProcessingCompareOp)

BOOST_AUTO_TEST_CASE(CompareOpsss)
{
    poutre::DenseImage<poutre::pINT32> img1({ 5, 6 });
    img1.fill(1);
    poutre::DenseImage<poutre::pINT32> imgout({ 5, 6 });
    imgout.fill(4);
    auto v_img1 = poutre::view(img1);
    auto v_imgout = poutre::view(imgout);

    ViewCompare_sss_dispatch(v_img1,poutre::CompOpType::CompOpEqual,1,1,0,v_imgout);

    std::string expected = "Dense Scalar GINT32 2 5 6 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1 \
1 1 1 1 1 1\
";
    auto imgstr = poutre::ImageToString(imgout);
    BOOST_CHECK_EQUAL(imgstr, expected);

}


BOOST_AUTO_TEST_SUITE_END()



