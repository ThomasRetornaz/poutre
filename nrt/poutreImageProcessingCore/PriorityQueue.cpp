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
#include <poutreImageProcessingCore/include/poutreImageProcessingPriorityQueue.hpp>

TEST(poutreImageProcessingPriorityQueue,ctor)
{ poutre::poutre_pq<unsigned char, poutre::offset> pq; }

TEST(poutreImageProcessingPriorityQueue,increase_nostable)
{
  poutre::poutre_pq<unsigned char, poutre::offset> pq;
    pq.emplace(0, 1); //-V525
    pq.emplace(50, 1);
    pq.emplace(0, 2);
    pq.emplace(50, 2);
    pq.emplace(80, 1);
    pq.emplace(80, 2);
    pq.emplace(80, 3);
    pq.emplace(0, 3);

    std::vector < std::pair<unsigned char, poutre::offset>> expected = { {80,1},{80,2},{80,3},{50,1},{50,2},{0,2},{0,1},{0,3} };
    std::vector < std::pair<unsigned char, poutre::offset>> results;
    while (!pq.empty())
    {
        results.push_back(pq.top());
        pq.pop();
    }
    EXPECT_EQ(results.size(), expected.size());
    EXPECT_TRUE(CheckEqualCollections(results.begin(), results.end(), expected.begin()));
}

TEST(poutreImageProcessingPriorityQueue,increase_stable)
{
  poutre::poutre_pq_stable<unsigned char, poutre::offset> pq;
    pq.emplace(0, 1); //-V525
    pq.emplace(50, 1);
    pq.emplace(0, 2);
    pq.emplace(50, 2);
    pq.emplace(80, 1);
    pq.emplace(80, 2);
    pq.emplace(80, 3);
    pq.emplace(0, 3);

    std::vector < std::pair<unsigned char, poutre::offset>> expected = { { 80,1 },{ 80,2 },{ 80,3 },{ 50,1 },{ 50,2 },{ 0,1 },{ 0,2 },{ 0,3 } };
    std::vector < std::pair<unsigned char, poutre::offset>> results;
    while (!pq.empty())
    {
        results.push_back(pq.top());
        pq.pop();
    }
    EXPECT_EQ(results.size(), expected.size());
    EXPECT_TRUE(CheckEqualCollections(results.begin(), results.end(), expected.begin()));
}
TEST(poutreImageProcessingPriorityQueue,decrease_nostable)
{
  poutre::poutre_rpq<unsigned char,
                    poutre::offset> pq;
    pq.emplace(0, 1); //-V525
    pq.emplace(50, 1);
    pq.emplace(0, 2);
    pq.emplace(50, 2);
    pq.emplace(80, 1);
    pq.emplace(80, 2);
    pq.emplace(80, 3);
    pq.emplace(0, 3);
    std::vector < std::pair<unsigned char, poutre::offset>> expected = { {0,1}, {0,2},{0,3}, {50,2},{50,1},{80,2},{80,3},{80,1} };
    std::vector < std::pair<unsigned char, poutre::offset>> results;
    while (!pq.empty())
    {
        results.push_back(pq.top());
        pq.pop();
    }
    EXPECT_EQ(results.size(), expected.size());
    EXPECT_TRUE(CheckEqualCollections(results.begin(), results.end(), expected.begin()));
}
TEST(poutreImageProcessingPriorityQueue,decrease_stable)
{
  poutre::poutre_rpq_stable<unsigned char, poutre::offset> pq;
    pq.emplace(0, 1); //-V525
    pq.emplace(50, 1);
    pq.emplace(0, 2);
    pq.emplace(50, 2);
    pq.emplace(80, 1);
    pq.emplace(80, 2);
    pq.emplace(80, 3);
    pq.emplace(0, 3);
    std::vector < std::pair<unsigned char, poutre::offset>> expected = { { 0,1 },{ 0,2 },{ 0,3 },{ 50,1 },{ 50,2 },{ 80,1 }, { 80,2 },{ 80,3 }};
    std::vector < std::pair<unsigned char, poutre::offset>> results;
    while (!pq.empty())
    {
        results.push_back(pq.top());
        pq.pop();
    }
    EXPECT_EQ(results.size(), expected.size());
    EXPECT_TRUE(CheckEqualCollections(results.begin(), results.end(), expected.begin()));
}