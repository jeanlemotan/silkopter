#pragma once

namespace silk
{
    constexpr size_t MAX_MOTOR_COUNT = 12;
}

namespace std
{
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
}


extern boost::asio::io_service s_async_io_service;

namespace silk
{
    template<typename F> auto async(F f) -> boost::unique_future<decltype(f())>
    {
        typedef decltype(f()) result_type;
        typedef boost::packaged_task<result_type> packaged_task;
        auto task = std::make_shared<packaged_task>(std::move(f));
        boost::unique_future<result_type> future = task->get_future();
        s_async_io_service.post(std::bind(&packaged_task::operator(), task));
        return future;
    }
}


