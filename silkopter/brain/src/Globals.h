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
    template<typename Res, typename... ArgTypes> auto async(std::function<Res(ArgTypes...)> f) -> std::future<Res>
    {
        typedef std::packaged_task<Res(ArgTypes...)> packaged_task;
        auto task = std::make_shared<packaged_task>(std::move(f));
        std::future<Res> future = task->get_future();
        s_async_io_service.post(std::bind(&packaged_task::operator(), task));
        return future;
    }


    struct At_Exit : q::util::Noncopyable
    {
        At_Exit(std::function<void()> at_exit) : m_at_exit(at_exit) {}
        ~At_Exit() { QASSERT(m_at_exit); m_at_exit(); }
    private:
        std::function<void()> m_at_exit;
    };

}


