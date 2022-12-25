/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Author: Chen Rudy
- Date: 12/9/22
- Implementation for Thread Pool
- compile with makefile
- make sure you have my test.hpp in your include directory
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <thread>             //thread
#include <list>               //list
#include <mutex>              //mutex
#include <condition_variable> //condition_variable
#include <chrono>             //chrono::seconds /milli
#include <stdexcept>          //except
#include <unistd.h>           //nice
#include <iostream>           //cerr

#include "thread_pool.hpp"
#include "shared_ptr.hpp"

namespace ilrd
{
    /******************* ThreadPool  **********************/

    ThreadPool::ThreadPool(size_t n_threads, size_t niceness)
        : m_state(PAUSE),
          m_niceness(niceness),
          m_stop(0),
          m_remove(0)

    {
        errno = 0;
        if (-1 == nice(niceness) && 0 != errno)
        {
            std::cerr << "set nice value failed" << std::endl;
            throw std::system_error(errno, std::generic_category(),
                                    "set nice value failed");
        }

        AddThreads(n_threads, "Thread Pool Creation Failed");
    }

    ThreadPool::~ThreadPool() noexcept
    {
        Pause();
        m_state = TERMINATE_POOL;

        m_cond_pause.notify_all(); // incase threads in pause cond.wait

        WaitForThreads();

        m_threads.clear();
        ClearTasks();
    }

    void ThreadPool::Add(std::shared_ptr<Task> function, PRIORITY pr)
    {
        try
        {
            m_task_queue.Push(std::pair<std::shared_ptr<Task>, PRIORITY>(function, pr));
        }
        catch (const std::exception &e)
        {
            std::cerr << "failed to add tasks" << e.what() << '\n';
            this->~ThreadPool();
            throw e;
        }
    }

    void ThreadPool::Run() noexcept
    {
        m_state = RUN;

        m_cond_pause.notify_all();
    }

    void ThreadPool::Pause() noexcept
    {
        m_state = PAUSE;
        WakeThreadsFromPop(); // incase threads in run->pop cond.wait
    }

    void ThreadPool::Stop() noexcept
    {
        Pause();
        ClearTasks();
    }

    void ThreadPool::SetNumberOfThreads(size_t n_threads)
    {
        STATE prev_state = m_state;
        Pause();

        if (n_threads > m_threads.size())
        {
            AddThreads(n_threads - m_threads.size(), "Failed Adding Threads");
        }
        else if (n_threads < m_threads.size())
        {
            RemoveThreads(m_threads.size() - n_threads);
        }

        RestorePrevState(prev_state);
    }

    /******************* ThreadPool help member funcs **********************/

    void ThreadPool::RestorePrevState(STATE prev_state) noexcept
    {
        if (RUN == prev_state)
        {
            Run();
        }
        m_state = prev_state;
    }

    void ThreadPool::RemoveThreads(size_t n_threads) noexcept
    {
        for (size_t i = 0; i < n_threads; ++i)
        {
            std::list<std::shared_ptr<ThreadUnit>>::iterator thread = m_threads.begin();
            thread->get()->SetThreadState(TERMINATE_THREAD);
            m_cond_pause.notify_all();
            m_threads.erase(thread);
        }
    }

    void ThreadPool::AddThreads(size_t threads_to_add, const char *error_handling)
    {
        try
        {
            for (size_t i = 0; i < threads_to_add; ++i)
            {
                m_threads.push_back(std::shared_ptr<ThreadUnit>(new ThreadUnit(this)));
            }
        }
        catch (const std::exception &e)
        {

            std::cerr << error_handling << e.what() << '\n';
            this->~ThreadPool();
            throw e;
        }
    }

    void ThreadPool::ClearTasks()
    {
        const std::chrono::seconds t(5);
        packege temp;

        while (!m_task_queue.IsEmpty())
        {
            m_task_queue.Pop(temp, t);
        }
    }

    void ThreadPool::WakeThreadsFromPop()
    {
        try
        {
            for (size_t i = 0; i < m_threads.size(); ++i)
            {
                this->Add(std::shared_ptr<Task>(new Default_Task([]() {})),
                          STOP_RUNNING);
            }
        }
        catch (const std::exception &e) // swallow exception
        {
            std::cerr << "failed in adding tasks in stop" << e.what() << '\n';
        }
    }

    void ThreadPool::WaitForThreads() noexcept
    {
        std::mutex local_lock;
        std::unique_lock<std::mutex> lock(local_lock);

        while (m_stop != m_threads.size())
        {
            m_cond_stop.wait(lock);
        }
    }

    /********************* CmpFunction for PQ help funcs *********************/

    bool ThreadPool::CmpProprity::operator()(const packege &lhs, const packege &rhs)
    {
        return lhs.second > rhs.second;
    }

    /********************* ThreadUnit member funcs **************************/

    ThreadPool::ThreadUnit::ThreadUnit(ThreadPool *thread_pool)
        : m_thread_pool(thread_pool),
          m_thread(std::thread(&ThreadPool::ThreadUnit::ThreadFunction, this)),
          m_thread_state(PAUSE)

    {

        // empty
    }

    ThreadPool::ThreadUnit::~ThreadUnit()
    {
        m_thread.join();
    }

    void ThreadPool::ThreadUnit::ThreadFunction()
    {
        std::mutex local_lock;
        packege temp;

        while (TERMINATE_POOL != m_thread_pool->m_state)
        {
            if (TERMINATE_THREAD == m_thread_state)
            {
                return;
            }

            while (PAUSE == m_thread_pool->m_state)
            {
                std::unique_lock<std::mutex> lock(local_lock);
                m_thread_pool->m_cond_pause.wait(lock);
            }

            while (RUN == m_thread_pool->m_state)
            {
                m_thread_pool->m_task_queue.Pop(temp);
                (*(temp.first))();
            }
        }

        WrapUpThread();
    }

    int ThreadPool::ThreadUnit::GetThreadState() const noexcept
    {
        return m_thread_state;
    }

    void ThreadPool::ThreadUnit::SetThreadState(int new_state) noexcept
    {
        m_thread_state = new_state;
    }

    void ThreadPool::ThreadUnit::WrapUpThread()
    {
        ++m_thread_pool->m_stop;

        if (m_thread_pool->m_stop == m_thread_pool->m_threads.size())
        {
            m_thread_pool->m_cond_stop.notify_one();
        }
    }

    /********************* Task Base Virtual Funcs**************************/
    Task::~Task()
    {
        // empty;
    }

    /********************* Default_Task **************************/

    Default_Task::Default_Task(std::function<void()> func) : m_func(func)
    {
        // empty;
    }

    void Default_Task::operator()()
    {
        m_func();
    }

    Default_Task::~Default_Task()
    {
        // empty;
    }

    /********************* Default_Task **************************/
}