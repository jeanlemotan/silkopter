#pragma once

#include <deque>
#include <map>
#include <vector>
#include <atomic>
#include <mutex>
#include <memory>
#include <boost/intrusive_ptr.hpp>
#include "QBase.h"

namespace util
{
struct Pool_Item_Base
{
    std::function<void(Pool_Item_Base* p)> gc;
    std::atomic_size_t count = {0};
};

template<class T> struct Pool
{
    std::function<void(T&)> on_acquire;
    std::function<void(T&)> on_release;

    typedef boost::intrusive_ptr<T> Ptr;
    Pool();
    Ptr acquire();

private:
    std::function<void(Pool_Item_Base*)> m_garbage_collector;
    struct Items
    {
        ////
        std::mutex mutex;
        std::vector<std::unique_ptr<T>> items;
        ////
    };
    std::shared_ptr<Items> m_pool;
};


inline void intrusive_ptr_add_ref(Pool_Item_Base* p)
{
    QASSERT(p);
    if (p->count.fetch_add(1, std::memory_order_relaxed) > 999)
    {
        QASSERT(0);
    }
}
inline void intrusive_ptr_release(Pool_Item_Base* p)
{
    QASSERT(p);
    if (p->count.fetch_sub(1, std::memory_order_release) == 1u)
    {
        if (p->gc)
        {
            p->gc(p);
        }
        else
        {
            QASSERT(0);
            delete p;
        }
    }
}
template<class T> Pool<T>::Pool()
{
    m_pool = std::make_shared<Items>();

    auto items_ref = m_pool;
    m_garbage_collector = [items_ref, this](Pool_Item_Base* t)
    {
        if (on_release)
        {
            on_release(static_cast<T&>(*t));
        }

        std::lock_guard<std::mutex> lg(items_ref->mutex);
        items_ref->items.emplace_back(static_cast<T*>(t)); //will create a unique pointer from the raw one
//                QLOGI("{}// new:{} reused:{} returned:{}", m_pool.get(), x_new, x_reused, x_returned);
//                QLOGI("{}// returned: {} / {}", m_pool.get(), t, x_returned);
    };
}

template<class T> auto Pool<T>::acquire() -> Ptr
{
    //this will be called when the last shared_ptr to T dies. We can safetly return the object to pur pool

    std::lock_guard<std::mutex> lg(m_pool->mutex);
    T* item = nullptr;
    if (!m_pool->items.empty())
    {
        item = m_pool->items.back().release(); //release the raw ptr from the control of the unique ptr
        m_pool->items.pop_back();
//                QLOGI("{}// recycled: {} / {}", m_pool.get(), item, x_reused);
//                QLOGI("{}// new:{} reused:{} returned:{}", m_pool.get(), x_new, x_reused, x_returned);
    }
    else
    {
        item = new T;
        item->gc = m_garbage_collector;
        //if (x_new > 1000)
        {
//                    QLOGI("{}// allocated: {} / {}", m_pool.get(), item, x_new);
            //QLOGI("{}// new:{} reused:{} returned:{}", m_pool.get(), x_new, x_reused, x_returned);
        }
    }
    QASSERT(item);

    if (on_acquire)
    {
        on_acquire(static_cast<T&>(*item));
    }

    return Ptr(item);
}

} //util
