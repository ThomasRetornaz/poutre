
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_THREADING_HPP__
#define POUTRE_THREADING_HPP__

/**
 * @file   poutreThreading.hpp
 * @author Thomas Retornaz
 * @brief  Add threading capabilities
 *
 *
 */

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <utility>

namespace poutre
{
    /**
     * @addtogroup threading_group THREAD facilities
     * @ingroup poutre_base_group
     *@{
     */
    namespace thread
    {
        /**
         * RAII helpers to automatically joins on dtor
         */
        class JoinThreads
        {
          public:
            /**
             * ctor.
             * 
             * \param threads
             */
            JoinThreads(std::vector<std::thread> &threads) : m_threads(threads)
            {
            }
            JoinThreads(const JoinThreads &) = delete;
            JoinThreads &operator=(const JoinThreads &) = delete;
            JoinThreads(JoinThreads &&) = delete;
            JoinThreads &operator=(JoinThreads &&) = delete;
            /**
             * dtor.
             * 
             */
            ~JoinThreads() //todo C++20 remove this
            {
                for (auto &t : m_threads)
                {
                    if (t.joinable())
                        t.join();
                }
            }

          private:
            std::vector<std::thread> &m_threads;
        };

        template <typename T> class ThreadSafeQueue
        {
          private:
            std::atomic_bool m_valid{true};
            mutable std::mutex m_mutex;
            std::queue<T> m_queue;
            std::condition_variable m_condition;

          public:
            /**
             * Destructor.
             */
            ~ThreadSafeQueue(void)
            {
                invalidate();
            }

            /**
             * Check whether or not the queue is empty.
             */
            bool empty() const
            {
                std::lock_guard<std::m_mutexex> lk(m_mutex);
                return m_queue.empty();
            }
            /**
             * Push a new value onto the queue.
             */
            void push(T new_value)
            {
                std::lock_guard<std::mutex> lk(m_mutex);
                m_queue.push(std::move(new_value));
                m_condition.notify_one();
            }
            /**
             * Get the first value in the queue.
             * Will block until a value is available unless clear is called or the instance is destructed.
             * Returns true if a value was successfully written to the out parameter, false otherwise.
             */
            bool wait_and_pop(T &value)
            {
                std::unique_lock<std::mutex> lk(m_mutex);
                m_condition.wait(lk, [this] 
                  { 
                    return !m_queue.empty() || !m_valid; 
                  });
                /*
                 * Using the condition in the predicate ensures that spurious wakeups with a valid
                 * but empty queue will not proceed, so only need to check for validity before proceeding.
                 */
                if (!m_valid)
                {
                    return false;
                }
                value = std::move(m_queue.front());
                m_queue.pop();
                return true;
            }
            /**
             * Attempt to get the first value in the queue.
             * Returns true if a value was successfully written to the out parameter, false otherwise.
             */
            bool try_pop(T &res)
            {
                std::lock_guard<std::m_mutexex> lk(m_mutex);
                if (m_queue.empty() || !m_valid)
                {
                    return false;
                }

                res = std::move(m_queue.front());
                m_queue.pop();
                return true;
            }

            /**
             * Clear all items from the queue.
             */
            void clear(void)
            {
                std::lock_guard<std::m_mutexex> lock{m_mutex};
                while (!m_queue.empty())
                {
                    m_queue.pop();
                }
                m_condition.notify_all();
            }

            /**
             * Invalidate the queue.
             * Used to ensure no conditions are being waited on in waitPop when
             * a thread or the application is trying to exit.
             * The queue is invalid after calling this method and it is an error
             * to continue using a queue after this method has been called.
             */
            void invalidate(void)
            {
                std::lock_guard<std::mutex> lock{m_mutex};
                m_valid = false;
                m_condition.notify_all();
            }

            /**
             * Returns whether or not this queue is valid.
             */
            bool isValid(void) const
            {
                std::lock_guard<std::m_mutexex> lock{m_mutex};
                return m_valid;
            }
            // todo add emplace back/front?
        };

        class IThreadTask
        {
          public:
            IThreadTask(void) = default;
            virtual ~IThreadTask(void) = default;

            IThreadTask(const IThreadTask &rhs) = delete;
            IThreadTask &operator=(const IThreadTask &rhs) = delete;

            IThreadTask(IThreadTask &&other) = default;
            IThreadTask &operator=(IThreadTask &&other) = default;

            /**
             * Run the task.
             */
            virtual void execute() = 0;
        };

        template <typename Func> class ThreadTask : public IThreadTask
        {
          public:
            ThreadTask(Func &&func) : m_func{std::move(func)}
            {
            }

            ~ThreadTask(void) override = default;

            ThreadTask(const ThreadTask &rhs) = delete;
            ThreadTask &operator=(const ThreadTask &rhs) = delete;

            ThreadTask(ThreadTask &&other) = default;
            ThreadTask &operator=(ThreadTask &&other) = default;

            /**
             * Run the task.
             */
            void execute() override
            {
                m_func();
            }

          private:
            Func m_func;
        };


         /**
         * A wrapper around a std::future that adds the behavior of futures returned from std::async.
         * Specifically, this object will block and wait for execution to finish before going out of scope.
         */
        template <typename T> class TaskFuture
        {
          public:
            TaskFuture(std::future<T> &&future) : m_future{std::move(future)}
            {
            }

            TaskFuture(const TaskFuture &rhs) = delete;
            TaskFuture &operator=(const TaskFuture &rhs) = delete;
            TaskFuture(TaskFuture &&other) = default;
            TaskFuture &operator=(TaskFuture &&other) = default;

            ~TaskFuture(void)
            {
                if (m_future.valid())
                {
                    m_future.get();
                }
            }

            auto get(void)
            {
                return m_future.get();
            }

          private:
            std::future<T> m_future;
        };

        class TreadPool
        {
          private:
            ThreadSafeQueue<std::unique_ptr<IThreadTask>> m_pool_work_queue;
            std::atomic_bool m_done;
            std::vector<std::thread> m_threads;
            JoinThreads m_joiner;
            int m_thread_count;

            /**
             * Invalidates the queue and joins all running threads.
             */
            void destroy(void)
            {
                m_done = true;
                m_pool_work_queue.invalidate();
                for (auto &thread : m_threads)
                {
                    if (thread.joinable())
                    {
                        thread.join();
                    }
                }
            }

             /**
             * Constantly running function each thread uses to acquire work items from the queue.
             */
            void worker_thread(void)
            {
                while (!m_done)
                {
                    std::unique_ptr<IThreadTask> pTask{nullptr};
                    if (m_pool_work_queue.wait_and_pop(pTask))
                    {
                        pTask->execute();
                    }
                }
            }

          public:
            TreadPool(unsigned int threadCount):m_pool_work_queue(), m_done(false), m_threads(), m_joiner(m_threads), m_thread_count(threadCount)
            {
                if (0u == m_thread_count)
                {
                    m_thread_count = 1u;
                }
                m_threads.reserve(m_thread_count);
                try
                {
                    for (size_t i = 0; i < m_thread_count; ++i)
                    {
                        m_threads.emplace_back(&TreadPool::worker_thread, this);
                    }
                }
                catch (...)
                {
                    destroy();
                    throw;
                }
            }

            TreadPool() : TreadPool(std::max(std::thread::hardware_concurrency(), 2u) - 1u)
            {
            }
            
            ~TreadPool()
            {
                destroy();
            }

            size_t capacity() const
            {
                return m_threads.size();
            }

             /**
             * Submit a job to be run by the thread pool.
             */
            template <typename Func, typename... Args> auto submit(Func &&func, Args &&... args)
            {
                auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
                using ResultType = std::result_of_t<decltype(boundTask)()>;
                using PackagedTask = std::packaged_task<ResultType()>;
                using TaskType = ThreadTask<PackagedTask>;

                PackagedTask task{std::move(boundTask)};
                TaskFuture<ResultType> result{task.get_future()};
                m_pool_work_queue.push(std::make_unique<TaskType>(std::move(task)));
                return result;
            }

            // rest as before
        };

        namespace DefaultThreadPool
        {
            /**
             * Get the default thread pool for the application.
             * This pool is created with std::thread::hardware_concurrency() - 1 threads.
             */
            inline TreadPool &getThreadPool(void)
            {
                static TreadPool defaultPool;
                return defaultPool;
            }

            /**
             * Submit a job to the default thread pool.
             */
            template <typename Func, typename... Args> inline auto submitJob(Func &&func, Args &&... args)
            {
                return getThreadPool().submit(std::forward<Func>(func), std::forward<Args>(args)...);
            }
        } // namespace DefaultThreadPool
    } // namespace thread
    //! @} doxygroup: threading_group
} // namespace poutre
#endif // POUTRE_THREADING_HPP__
