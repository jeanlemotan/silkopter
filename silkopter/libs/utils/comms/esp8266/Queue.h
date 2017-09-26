#pragma once

#include <deque>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <cassert>

template<typename T> class Queue
{
public:
    Queue(size_t max_size);
    ~Queue();

    void exit();

    bool push_back(T const& t, bool block);
    bool push_back_timeout(T const& t, std::chrono::high_resolution_clock::duration timeout);

    bool pop_front(T& dst, bool block);
    bool pop_front(std::vector<T>& dst, size_t max, bool block);

    bool pop_front_timeout(T& dst, std::chrono::high_resolution_clock::duration timeout);
    bool pop_front_timeout(std::vector<T>& dst, size_t max, std::chrono::high_resolution_clock::duration timeout);

private:
    bool _push_back(T const& t, bool block, std::chrono::high_resolution_clock::duration* timeout);
    bool _pop_front(T& dst, bool block, std::chrono::high_resolution_clock::duration* timeout);
    bool _pop_front(std::vector<T>& dst, size_t max, bool block, std::chrono::high_resolution_clock::duration* timeout);

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::deque<T> m_queue;
    size_t m_max_size = 10;
    bool m_exit = false;
};


template<class T>
Queue<T>::Queue(size_t max_size)
    : m_max_size(max_size)
{
}

template<class T>
Queue<T>::~Queue()
{
    exit();
}

template<class T>
void Queue<T>::exit()
{
    {
        std::unique_lock<std::mutex> lg(m_mutex);
        m_exit = true;
    }
    m_cv.notify_all();
}

template<class T>
bool Queue<T>::push_back(T const& dst, bool block)
{
    return _push_back(dst, block, nullptr);
}

template<class T>
bool Queue<T>::push_back_timeout(T const& dst, std::chrono::high_resolution_clock::duration timeout)
{
    return _push_back(dst, true, &timeout);
}

template<typename T>
bool Queue<T>::pop_front(std::vector<T>& dst, size_t max, bool block)
{
    return _pop_front(dst, max, block, nullptr);
}

template<typename T>
bool Queue<T>::pop_front(T& dst, bool block)
{
    return _pop_front(dst, block, nullptr);
}

template<typename T>
bool Queue<T>::pop_front_timeout(std::vector<T>& dst, size_t max, std::chrono::high_resolution_clock::duration timeout)
{
    return _pop_front(dst, max, true, &timeout);
}

template<typename T>
bool Queue<T>::pop_front_timeout(T& dst, std::chrono::high_resolution_clock::duration timeout)
{
    return _pop_front(dst, true, &timeout);
}

template<class T>
bool Queue<T>::_push_back(T const& t, bool block, std::chrono::high_resolution_clock::duration* timeout)
{
    std::chrono::high_resolution_clock::time_point until = std::chrono::high_resolution_clock::now();
    if (timeout)
    {
        until += *timeout;
    }

    bool pushed_one = false;
    //send the current datagram
    {
        std::unique_lock<std::mutex> lg(m_mutex);
        if (block)
        {
            while (m_queue.size() >= m_max_size && !m_exit)
            {
                if (timeout)
                {
                    m_cv.wait_until(lg, until);
                    if (std::chrono::high_resolution_clock::now() >= until)
                    {
                        break;
                    }
                }
                else
                {
                    m_cv.wait(lg);
                }
            }
            if (m_exit)
            {
                return false;
            }
        }
        if (m_queue.size() < m_max_size)
        {
            pushed_one = true;
            m_queue.push_back(t);
        }
    }
    if (pushed_one)
    {
        m_cv.notify_all();
    }
//    else
//    {
//        printf("Skipped\n");
//    }
    return pushed_one;
}

template<typename T>
bool Queue<T>::_pop_front(std::vector<T>& dst, size_t max, bool block, std::chrono::high_resolution_clock::duration* timeout)
{
    std::chrono::high_resolution_clock::time_point until = std::chrono::high_resolution_clock::now();
    if (timeout)
    {
        until += *timeout;
    }

    bool got_some = false;
    {
        //wait for data
        std::unique_lock<std::mutex> lg(m_mutex);
        if (block)
        {
            while (m_queue.empty() && !m_exit)
            {
                if (timeout)
                {
                    m_cv.wait_until(lg, until);
                    if (std::chrono::high_resolution_clock::now() >= until)
                    {
                        break;
                    }
                }
                else
                {
                    m_cv.wait(lg);
                }
            }
            if (m_exit)
            {
                return false;
            }
        }

        while (!m_queue.empty())
        {
            if (dst.size() >= max)
            {
                break;
            }
            got_some = true;
            dst.push_back(std::move(m_queue.front()));
            m_queue.pop_front();
        }
    }
    if (got_some)
    {
        m_cv.notify_all();
    }
    return got_some;
}

template<typename T>
bool Queue<T>::_pop_front(T& dst, bool block, std::chrono::high_resolution_clock::duration* timeout)
{
    std::chrono::high_resolution_clock::time_point until = std::chrono::high_resolution_clock::now();
    if (timeout)
    {
        until += *timeout;
    }

    bool got_some = false;
    {
        //wait for data
        std::unique_lock<std::mutex> lg(m_mutex);
        if (block)
        {
            while (m_queue.empty() && !m_exit)
            {
                if (timeout)
                {
                    m_cv.wait_until(lg, until);
                    if (std::chrono::high_resolution_clock::now() >= until)
                    {
                        break;
                    }
                }
                else
                {
                    m_cv.wait(lg);
                }
            }
            if (m_exit)
            {
                return false;
            }
        }

        if (!m_queue.empty())
        {
            got_some = true;
            dst = std::move(m_queue.front());
            m_queue.pop_front();
        }
    }
    if (got_some)
    {
        m_cv.notify_all();
    }
    return got_some;
}
