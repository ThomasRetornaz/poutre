
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"
#include <poutreImageProcessingCore/include/poutreImageProcessingPriorityQueue.hpp>



BOOST_AUTO_TEST_SUITE(poutreImageProcessingPriorityQueue)
BOOST_AUTO_TEST_CASE(ctor)
{
    poutre::PriorityQueue<unsigned char, poutre::offset> pq;
}

BOOST_AUTO_TEST_CASE(increase_nostable)
{
    poutre::PriorityQueue<poutre::offset, unsigned char> pq;
    pq.emplace(0, 1);
    pq.emplace(50, 1);
    pq.emplace(0, 2);
    pq.emplace(50, 2);
    pq.emplace(80, 1);
    pq.emplace(80, 2);
    pq.emplace(80, 3);
    pq.emplace(0, 3);

    std::vector < std::pair<poutre::offset, unsigned char>> expected = { {80,1},{80,2},{80,3},{50,1},{50,2},{0,2},{0,1},{0,3} };
    std::vector < std::pair<poutre::offset, unsigned char>> results;
    while (!pq.empty())
    {
        results.push_back(pq.top());
        pq.pop();
    }
    //BOOST_CHECK_EQUAL_COLLECTIONS(results.begin(), results.end(), expected.begin(), expected.end());
    BOOST_REQUIRE(expected.size() == results.size());
    for (auto i = 0; i < expected.size(); i++)
    {
        BOOST_CHECK_EQUAL(expected[i].first, results[i].first);
        BOOST_CHECK_EQUAL(expected[i].second, results[i].second);
    }
}


BOOST_AUTO_TEST_CASE(increase_stable)
{
    poutre::PriorityQueue<poutre::offset, unsigned char, poutre::lesserKey<poutre::offset, unsigned char>, true> pq;
    pq.emplace(0, 1);
    pq.emplace(50, 1);
    pq.emplace(0, 2);
    pq.emplace(50, 2);
    pq.emplace(80, 1);
    pq.emplace(80, 2);
    pq.emplace(80, 3);
    pq.emplace(0, 3);

    std::vector < std::pair<poutre::offset, unsigned char>> expected = { { 80,1 },{ 80,2 },{ 80,3 },{ 50,1 },{ 50,2 },{ 0,1 },{ 0,2 },{ 0,3 } };
    std::vector < std::pair<poutre::offset, unsigned char>> results;
    while (!pq.empty())
    {
        results.push_back(pq.top());
        pq.pop();
    }
    //BOOST_CHECK_EQUAL_COLLECTIONS(results.begin(), results.end(), expected.begin(), expected.end());
    BOOST_REQUIRE(expected.size() == results.size());
    for (auto i = 0; i < expected.size(); i++)
    {
        BOOST_CHECK_EQUAL(expected[i].first, results[i].first);
        BOOST_CHECK_EQUAL(expected[i].second, results[i].second);
    }
}


BOOST_AUTO_TEST_CASE(decrease_nostable)
{
    poutre::PriorityQueue<poutre::offset, unsigned char, poutre::greaterKey<poutre::offset, unsigned char>> pq;
    pq.emplace(0, 1);
    pq.emplace(50, 1);
    pq.emplace(0, 2);
    pq.emplace(50, 2);
    pq.emplace(80, 1);
    pq.emplace(80, 2);
    pq.emplace(80, 3);
    pq.emplace(0, 3);
    std::vector < std::pair<poutre::offset, unsigned char>> expected = { {0,1}, {0,2},{0,3}, {50,2},{50,1},{80,2},{80,3},{80,1} };
    std::vector < std::pair<poutre::offset, unsigned char>> results;
    while (!pq.empty())
    {
        results.push_back(pq.top());
        pq.pop();
    }
    //BOOST_CHECK_EQUAL_COLLECTIONS(results.begin(), results.end(), expected.begin(), expected.end());
    BOOST_REQUIRE(expected.size() == results.size());
    for (auto i = 0; i < expected.size(); i++)
    {
        BOOST_CHECK_EQUAL(expected[i].first, results[i].first);
        BOOST_CHECK_EQUAL(expected[i].second, results[i].second);
    }
}


BOOST_AUTO_TEST_CASE(decrease_stable)
{
    poutre::PriorityQueue<poutre::offset, unsigned char, poutre::greaterKey<poutre::offset, unsigned char>, true> pq;
    pq.emplace(0, 1);
    pq.emplace(50, 1);
    pq.emplace(0, 2);
    pq.emplace(50, 2);
    pq.emplace(80, 1);
    pq.emplace(80, 2);
    pq.emplace(80, 3);
    pq.emplace(0, 3);
    std::vector < std::pair<poutre::offset, unsigned char>> expected = { { 0,1 },{ 0,2 },{ 0,3 },{ 50,1 },{ 50,2 },{ 80,1 }, { 80,2 },{ 80,3 }};
    std::vector < std::pair<poutre::offset, unsigned char>> results;
    while (!pq.empty())
    {
        results.push_back(pq.top());
        pq.pop();
    }
    //BOOST_CHECK_EQUAL_COLLECTIONS(results.begin(), results.end(), expected.begin(), expected.end());
    BOOST_REQUIRE(expected.size() == results.size());
    for (auto i = 0; i < expected.size(); i++)
    {
        BOOST_CHECK_EQUAL(expected[i].first, results[i].first);
        BOOST_CHECK_EQUAL(expected[i].second, results[i].second);
    }
}
BOOST_AUTO_TEST_SUITE_END()



