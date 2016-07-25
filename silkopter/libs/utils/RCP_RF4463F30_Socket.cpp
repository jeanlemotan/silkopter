#include "RCP_RF4463F30_Socket.h"
#include "RF4463F30.h"

namespace util
{

struct RCP_RF4463F30_Socket::Impl
{
    RF4463F30 rf_chip;
};


///////////////////////////////////////////////////////////////////////////////////////

RCP_RF4463F30_Socket::RCP_RF4463F30_Socket(std::string const& device, uint32_t speed)
    : m_impl(new Impl)
    , m_device(device)
    , m_speed(speed)
{

}

RCP_RF4463F30_Socket::~RCP_RF4463F30_Socket()
{

}

RCP_RF4463F30_Socket::Result RCP_RF4463F30_Socket::process()
{
    return Result::OK;
}

bool RCP_RF4463F30_Socket::start()
{
    if (m_is_initialized)
    {
        return true;
    }
    return initialize();
}

void RCP_RF4463F30_Socket::async_send(void const* data, size_t size)
{

}

size_t RCP_RF4463F30_Socket::get_mtu() const
{
    return 64;
}

bool RCP_RF4463F30_Socket::lock()
{
    if (m_send_in_progress.exchange(true))
    {
        //already locked
        return false;
    }
    return true;
}

void RCP_RF4463F30_Socket::unlock()
{
    m_send_in_progress = false;
}

bool RCP_RF4463F30_Socket::initialize()
{
    return m_impl->rf_chip.init(m_device, m_speed, 6, 5);
}


}
