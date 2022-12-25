/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Author: Chen Rudy
- Date: 12/9/22
- Api for Thread Pool
- compile with makefile
- make sure you have my test.hpp in your include directory
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef __RD12123_THREAD_POOL_HPP__
#define __RD12123_THREAD_POOL_HPP__

#include <functional> //pair
#include <list>       //list
#include <thread>     //hardware_concurrency


#include "shared_ptr.hpp"
#include "waitable_queue.hpp"
#include "PQ_wrap.hpp"

namespace ilrd
{
    enum PRIORITY
    {
        STOP_RUNNING, // find a way to hide from user???
        HIGH,
        MEDIUM,
        LOW
    };

    enum STATE
    {
        RUN,
        PAUSE,
        TERMINATE_POOL,
        TERMINATE_THREAD
    };

    class Task;

    class ThreadPool
    {
    private:
        class ThreadUnit; // forward declaration

        using packege = std::pair<std::shared_ptr<Task>, PRIORITY>;

    public:
        explicit ThreadPool(size_t n_threads =
                                std::thread::hardware_concurrency(),
                            size_t niceness = 0);
        ~ThreadPool() noexcept;
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;
        void Add(std::shared_ptr<Task> function, PRIORITY pr);
        void Run() noexcept;
        void Pause() noexcept; // stops pulling tasks from queue
        void Stop() noexcept;  // Pause + flush for task queue
        void SetNumberOfThreads(size_t n_threads);
        // void Remove(std::shared_ptr<Task> function); <<----- (ADVANCED)

    private:
        class CmpProprity
        {
        public:
            bool operator()(const packege &lhs, const packege &rhs);
        };

        Waitable_Queue<packege, PQ_wrap<packege, std::vector<packege>, CmpProprity>> m_task_queue;
        std::list<std::shared_ptr<ThreadUnit>> m_threads;
        std::atomic<STATE> m_state;
        const size_t m_niceness;
        std::condition_variable m_cond_stop;
        std::condition_variable m_cond_pause;
        std::condition_variable m_cond_remove;
        std::atomic<size_t> m_stop;
        std::atomic<size_t> m_remove;

        void AddThreads(size_t threads_to_add, const char *error_handling);
        void ClearTasks();
        void WakeThreadsFromPop();
        void WaitForThreads() noexcept;
        void RemoveThreads(size_t n_threads) noexcept;
        void RestorePrevState(STATE prev_state) noexcept;

        class ThreadUnit // nested class
        {
        public:
            explicit ThreadUnit(ThreadPool *thread_pool);
            ~ThreadUnit();
            ThreadUnit(const ThreadUnit &) = delete;
            ThreadUnit operator=(const ThreadUnit &) = delete;
            int GetThreadState() const noexcept;
            void SetThreadState(int new_state) noexcept;

        private:
            void ThreadFunction();
            void WrapUpThread();
            ThreadPool *m_thread_pool;
            std::thread m_thread;
            int m_thread_state;
        };
    };

    class Task
    {
    public:
        void virtual operator()() = 0;
        virtual ~Task();

    private:
    };

    class Default_Task : public Task
    {
    public:
        Default_Task(std::function<void()> m_func);
        void virtual operator()();
        virtual ~Default_Task();

    private:
        std::function<void()> m_func;
    };

} // namespace ilrd

#endif //__RD12123_THREAD_POOL_HPP__