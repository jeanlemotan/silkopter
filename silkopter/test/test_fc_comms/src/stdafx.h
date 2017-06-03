#pragma once

#if defined __cplusplus

#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <algorithm>
#include <numeric>
#include <deque>
#include <unordered_map>
#include <future>

//#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
//#include <boost/thread.hpp>

#include "_qmath.h"
#include "QBase.h"

#include "def_lang/Result.h"


namespace std
{
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
}

//extern boost::asio::io_service s_async_io_service;

namespace silk
{

template<typename Res, typename... ArgTypes> auto async(std::function<Res(ArgTypes...)> f) -> std::future<Res>
{
    typedef std::packaged_task<Res(ArgTypes...)> packaged_task;
    auto task = std::make_shared<packaged_task>(std::move(f));
    std::future<Res> future = task->get_future();
//    s_async_io_service.post(std::bind(&packaged_task::operator(), task));
    return future;
}

}


#endif
