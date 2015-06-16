#pragma once

#include "RCP.h"

namespace util
{

class RCP_RFMON_Socket : public RCP_Socket
{
public:
    RCP_RFMON_Socket();
    ~RCP_RFMON_Socket();

    auto process() -> Result;

    void open(std::string const& interface);

private:
    void async_send(uint8_t const* data, size_t size);

    std::string m_interface;
};

}
