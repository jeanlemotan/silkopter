#pragma once

#include <tuple>
#include <vector>
#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace util
{


template<typename T>
class Queue
{
public:
    Queue(size_t max_length);

    void finish();

    typedef T Buffer;

    std::unique_ptr<Buffer> begin_consuming(bool block);
    void end_consuming(std::unique_ptr<Buffer> buffer);

    std::unique_ptr<Buffer> begin_producing();
    void end_producing(std::unique_ptr<Buffer> buffer, bool block);
    void cancel_producing(std::unique_ptr<Buffer> buffer);

    void clear();

private:

    size_t m_max_length = 0;
    bool m_is_finished = false;

    std::mutex m_pool_mutex;
    std::vector<std::unique_ptr<Buffer>> m_pool;

    std::mutex m_queue_mutex;
    std::condition_variable m_queue_cv;
    std::vector<std::unique_ptr<Buffer>> m_queue;
    size_t m_consumer_index = 0;
    size_t m_producer_index = 0;
};


template<typename T>
Queue<T>::Queue(size_t max_length)
    : m_max_length(max_length)
{
    m_queue.resize(m_max_length);
}

template<typename T>
void Queue<T>::finish()
{
    m_is_finished = true;
    m_queue_cv.notify_all();
}

template<typename T>
void Queue<T>::clear()
{
    std::unique_lock<std::mutex> lg(m_queue_mutex);
    for (std::unique_ptr<T>& buffer: m_queue)
    {
        std::lock_guard<std::mutex> lg(m_pool_mutex);
        m_pool.push_back(std::move(buffer));
    }
}

template<typename T>
std::unique_ptr<typename Queue<T>::Buffer> Queue<T>::begin_consuming(bool block)
{
    if (m_is_finished)
    {
        return nullptr;
    }

    std::unique_ptr<Buffer> buffer;

    {
        std::unique_lock<std::mutex> lg(m_queue_mutex);
        size_t new_index = (m_consumer_index + 1) % m_queue.size();
        if (m_queue[new_index] == nullptr)
        {
            if (!block)
            {
                return nullptr;
            }

            m_queue_cv.wait(lg, [this, new_index]{ return m_queue[new_index] != nullptr || m_is_finished == true; });
            if (m_is_finished)
            {
                return nullptr;
            }
        }
        m_consumer_index = new_index;
        buffer = std::move(m_queue[m_consumer_index]);
    }
    m_queue_cv.notify_all();

    return std::move(buffer);
}

template<typename T>
void Queue<T>::end_consuming(std::unique_ptr<Buffer> buffer)
{
    if (buffer)
    {
        std::lock_guard<std::mutex> lg(m_pool_mutex);
        m_pool.push_back(std::move(buffer));
    }
}

template<typename T>
std::unique_ptr<typename Queue<T>::Buffer> Queue<T>::begin_producing()
{
    if (m_is_finished)
    {
        return nullptr;
    }

    std::lock_guard<std::mutex> lg(m_pool_mutex);

    std::unique_ptr<Buffer> buffer;
    if (m_pool.empty())
    {
        buffer.reset(new Buffer());
    }
    else
    {
        buffer = std::move(m_pool.back());
        m_pool.pop_back();
    }
    return std::move(buffer);
}

template<typename T>
void Queue<T>::end_producing(std::unique_ptr<Buffer> buffer, bool block)
{
    if (m_is_finished)
    {
        return;
    }
    if (!buffer)
    {
        return;
    }

    {
        std::unique_lock<std::mutex> lg(m_queue_mutex);
        size_t new_index = (m_producer_index + 1) % m_queue.size();
        if (m_queue[new_index] != nullptr)
        {
            if (!block)
            {
                return;
            }
            m_queue_cv.wait(lg, [this, new_index]{ return m_queue[new_index] == nullptr || m_is_finished == true; });
            if (m_is_finished)
            {
                return;
            }
        }
        m_producer_index = new_index;
        m_queue[m_producer_index] = std::move(buffer);
    }
    m_queue_cv.notify_all();
}

template<typename T>
void Queue<T>::cancel_producing(std::unique_ptr<Buffer> buffer)
{
    if (!buffer)
    {
        return;
    }

    buffer->clear();

    std::lock_guard<std::mutex> lg(m_pool_mutex);
    m_pool.push_back(std::move(buffer));
}


}
