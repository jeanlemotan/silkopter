#include "RCP_RFMON_Socket.h"

namespace util
{

RCP_RFMON_Socket::RCP_RFMON_Socket()
{
}

RCP_RFMON_Socket::~RCP_RFMON_Socket()
{
}

void RCP_RFMON_Socket::open(std::string const& interface)
{
    m_interface = interface;
}

void RCP_RFMON_Socket::async_send(uint8_t const* data, size_t size)
{
}

auto RCP_RFMON_Socket::process() -> Result
{
    return Result::OK;
}


}
