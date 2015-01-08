//#include <thread>
//#include <functional>
//#include <unordered_map>
//#include <future>

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
