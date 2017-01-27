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


//extern boost::asio::io_service s_async_io_service;

namespace silk
{
//template<typename F> auto async(F f) -> boost::unique_future<decltype(f())>
//{
//   typedef decltype(f()) result_type;
//   typedef boost::packaged_task<result_type> packaged_task;
//   auto task = std::make_shared<packaged_task>(std::move(f));
//   boost::unique_future<result_type> future = task->get_future();
//   s_async_io_service.post(std::bind(&packaged_task::operator(), task));
//   return future;
//}

extern void execute_async_call(std::function<void()> f);

template<typename Res> auto async(std::function<Res()> f) -> std::future<Res>
{
    typedef std::packaged_task<Res()> packaged_task;
    auto task = std::make_shared<packaged_task>(std::move(f));
    std::future<Res> future = task->get_future();
    execute_async_call(std::bind(&packaged_task::operator(), task));
//    s_async_io_service.post(std::bind(&packaged_task::operator(), task));
    return future;
}


struct At_Exit
{
    At_Exit(std::function<void()> at_exit) : m_at_exit(at_exit) { assert(m_at_exit); }
    ~At_Exit() { m_at_exit(); }

private:
    At_Exit(At_Exit const&) = delete;
    At_Exit& operator=(At_Exit const&) = delete;

    std::function<void()> m_at_exit;
};


template<class Format_String, typename... Params>
ts::Error make_error(Format_String const& fmt, Params&&... params)
{
    return ts::Error(q::util::format<std::string>(fmt, std::forward<Params>(params)...));
}


}


