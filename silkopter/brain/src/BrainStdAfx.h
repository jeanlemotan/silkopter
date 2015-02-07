//#include <thread>
//#include <functional>
//#include <unordered_map>
//#include <future>

#include <type_traits>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>

#include "QBase.h"
#include "QData.h"
#include "qmath.h"

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
        s_async_io_service.post(boost::bind(&packaged_task::operator(), task));
        return future;
    }
}

