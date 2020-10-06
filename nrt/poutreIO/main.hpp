
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef _PIOTESTS_MAIN_HPP__
#define _PIOTESTS_MAIN_HPP__

#include <iostream>
#include <gtest/gtest.h>

template <typename LeftIter, typename RightIter>
::testing::AssertionResult CheckEqualCollections(LeftIter left_begin, LeftIter left_end, RightIter right_begin)
{
    bool equal(true);
    std::string message;
    std::size_t index(0);
    while (left_begin != left_end)
    {
        if (*left_begin++ != *right_begin++)
        {
            equal = false;
            message += "\n\tMismatch at index " + std::to_string(index);
        }
        ++index;
    }
    if (message.size())
        message += "\n\t";
    return equal ? ::testing::AssertionSuccess() : ::testing::AssertionFailure() << message;
}
#endif //_PIOTESTS_MAIN_HPP__

