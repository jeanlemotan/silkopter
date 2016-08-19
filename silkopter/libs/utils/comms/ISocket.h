#pragma once


namespace util
{
namespace comms
{

//socket interface
class ISocket
{
public:
    virtual ~ISocket() = default;

    enum class Result
    {
        OK,
        ERROR,
        RECONNECTED,
    };

    virtual auto process() -> Result = 0;

    std::function<void(uint8_t* data, size_t size)> receive_callback;
    std::function<void(Result)> send_callback;

    virtual void async_send(void const* data, size_t size) = 0;

    virtual auto get_mtu() const -> size_t = 0;

    virtual auto lock() -> bool = 0;
    virtual void unlock() = 0;
};

}
}
