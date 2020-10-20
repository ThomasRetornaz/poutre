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
#include <gtest/gtest.h>
#include <poutreBase/include/poutreThreading.hpp>

TEST(threading, threadpool_ctor)
{
    poutre::thread::TreadPool pool;
    EXPECT_EQ(pool.capacity(), std::thread::hardware_concurrency() - 2);
}

TEST(threading, threadpool_ctor_defaut0)
{
    poutre::thread::TreadPool pool(0);
    EXPECT_EQ(pool.capacity(), 1);
}

TEST(threading, threadpool_RunMoreTasksThanThreads)
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
    //EXPECT_EQ(pool.empty(), true);
    EXPECT_EQ(result, TASK_COUNT);
    EXPECT_EQ(thread_ids.size(), THREAD_COUNT);
}

TEST(threading, threadpool_VariousTypesOfTasks)
{
    using namespace poutre::thread;
    TreadPool pool{2u};
    auto fi = pool.submit([] { return 42; });
    auto fs = pool.submit([] { return std::string{"42"}; });
    EXPECT_EQ(fi.get(), 42);
    EXPECT_EQ(fs.get(), std::string{"42"});
}

TEST(threading, threadpool_Lambdas)
{
    using namespace poutre::thread;
    using namespace std::chrono_literals;
    const size_t TASK_COUNT{4u}; //-V112
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
        EXPECT_EQ(i, v[i].get());
    }
    // EXPECT_EQ(pool.empty(), true);
}

TEST(threading, threadpool_Exception)
{
    using namespace poutre::thread;
    TreadPool pool{1u};
    auto f = pool.submit([] { throw std::runtime_error{"Error"}; });

    EXPECT_THROW(f.get(), std::runtime_error);
}

TEST(threading, threadpool_capacity)
{
    using namespace poutre::thread;
    TreadPool pool{1u};
    EXPECT_EQ(pool.capacity(), 1u);
}

TEST(threading, threadpool_empty)
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

TEST(threading, threadpool_FunctionWithArgs)
{
    using namespace std::chrono_literals;
    using namespace poutre::thread;
    TreadPool pool{4u};
    auto f = pool.submit(sum, 2, 2);
    EXPECT_EQ(f.get(), 4); //-V112
}

static std::thread::id test_function(size_t delay)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delay + 1));
    return std::this_thread::get_id();
}

TEST(threading, threadpool_ThreadsAreReused)
{
    using namespace std::chrono_literals;
    using namespace poutre::thread;

    const size_t THREAD_COUNT{2u};
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
        EXPECT_TRUE(iter.second);
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
        EXPECT_TRUE(iter != thread_ids.end());
        thread_ids.erase(iter);
    }
}
