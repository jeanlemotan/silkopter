#pragma once

#include <string>
#include "Si4463.h"

namespace util
{

class RF4463F30
{
public:
    RF4463F30();
    ~RF4463F30();

    bool init(std::string const& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio);

    bool tx(void const* data, uint8_t size);
    bool rx();

    bool has_received_packet();
    bool get_packet_data(void* data, size_t& size);

private:
    bool m_is_initialized = false;
    Si4463 m_chip;
};

}

