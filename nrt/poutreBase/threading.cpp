// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <chrono>
#include <future>
#include <mutex>
#include <set>
#include <stdexcept>
#include <thread>
#include <vector>

#include "main.hpp"
#include <poutreBase/include/poutreThreading.hpp>

BOOST_AUTO_TEST_SUITE(threading)

BOOST_AUTO_TEST_CASE(threadpool_ctor)
{
    poutre::thread::TreadPool pool;
    BOOST_CHECK_EQUAL(pool.capacity(), std::thread::hardware_concurrency()-1);
}

BOOST_AUTO_TEST_CASE(threadpool_ctor_defaut0)
{
    poutre::thread::TreadPool pool(0);
    BOOST_CHECK_EQUAL(pool.capacity(), 1);
}

BOOST_AUTO_TEST_CASE(threadpool_RunMoreTasksThanThreads)
{
    using namespace std::chrono_literals;
    const size_t THREAD_COUNT{2u};
    const size_t TASK_COUNT{20u};

    std::mutex mutex;
    size_t result = 0u;
    std::set<std::thread::id> thread_ids;

    poutre::thread::TreadPool pool{THREAD_COUNT};
    std::vector<poutre::thread::TaskFuture<void>> futures;
    for (size_t i = 0; i < TASK_COUNT; ++i)
    {
        futures.push_back(pool.submit([&] {
            std::this_thread::sleep_for(1ms);
            std::lock_guard<std::mutex> l{mutex};
            ++result;
            thread_ids.insert(std::this_thread::get_id());
        }));
    }
    for (auto &f : futures)
    {
        f.get();
    }
    //BOOST_CHECK_EQUAL(pool.empty(), true);
    BOOST_CHECK_EQUAL(result, TASK_COUNT);
    BOOST_CHECK_EQUAL(thread_ids.size(), THREAD_COUNT);
}

BOOST_AUTO_TEST_CASE(threadpool_VariousTypesOfTasks)
{
    using namespace poutre::thread;
    TreadPool pool{2u};
    auto fi = pool.submit([] { return 42; });
    auto fs = pool.submit([] { return std::string{"42"}; });
    BOOST_CHECK_EQUAL(fi.get(), 42);
    BOOST_CHECK_EQUAL(fs.get(), std::string{"42"});
}

BOOST_AUTO_TEST_CASE(threadpool_Lambdas)
{
    using namespace poutre::thread;
    using namespace std::chrono_literals;
    const size_t TASK_COUNT{4u};
    std::vector<poutre::thread::TaskFuture<size_t>> v;
    TreadPool pool{4u};
    for (size_t i = 0; i < TASK_COUNT; ++i)
    {
        v.push_back(pool.submit([task_num = i] {
            std::this_thread::sleep_for(1ms);
            return task_num;
        }));
    }
    for (size_t i = 0; i < TASK_COUNT; ++i)
    {
        BOOST_CHECK_EQUAL(i, v[i].get());
    }
    //BOOST_CHECK_EQUAL(pool.empty(), true);
}

BOOST_AUTO_TEST_CASE(threadpool_Exception)
{
    using namespace poutre::thread;
    TreadPool pool{1u};
    auto f = pool.submit([] { throw std::runtime_error{"Error"}; });

    BOOST_CHECK_THROW(f.get(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(threadpool_capacity)
{
    using namespace poutre::thread;
    TreadPool pool{1u};
    BOOST_CHECK_EQUAL(pool.capacity(), 1u);
}

BOOST_AUTO_TEST_CASE(threadpool_empty)
{
    using namespace std::chrono_literals;
    using namespace poutre::thread;
    TreadPool pool{4u};
    std::this_thread::sleep_for(1s);
}

static int sum(int a, int b)
{
    return a + b;
}

BOOST_AUTO_TEST_CASE(threadpool_FunctionWithArgs)
{
    using namespace std::chrono_literals;
    using namespace poutre::thread;
    TreadPool pool{4u};
    auto f = pool.submit(sum, 2, 2);
    BOOST_CHECK_EQUAL(f.get(), 4);
}

static std::thread::id test_function(size_t delay)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delay + 1));
    return std::this_thread::get_id();
}

BOOST_AUTO_TEST_CASE(threadpool_ThreadsAreReused)
{
    using namespace std::chrono_literals;
    using namespace poutre::thread;

    const size_t THREAD_COUNT{4u};
    TreadPool pool{THREAD_COUNT};
    std::vector<poutre::thread::TaskFuture<std::thread::id>> futures;
    std::set<std::thread::id> thread_ids;

    for (size_t i = 0u; i < THREAD_COUNT; ++i)
    {
        futures.push_back(pool.submit(test_function, i));
    }

    for (size_t i = 0u; i < THREAD_COUNT; ++i)
    {
        auto r = futures[i].get();
        auto iter = thread_ids.insert(r);
        // New thread is used
        BOOST_CHECK(iter.second);
    }

    futures.clear();

    for (size_t i = 0u; i < THREAD_COUNT; ++i)
    {
        futures.push_back(pool.submit(test_function, i));
    }

    for (size_t i = 0; i < THREAD_COUNT; ++i)
    {
        auto r = futures[i].get();
        auto iter = thread_ids.find(r);
        BOOST_CHECK(iter != thread_ids.end());
        thread_ids.erase(iter);
    }
}
BOOST_AUTO_TEST_SUITE_END()
