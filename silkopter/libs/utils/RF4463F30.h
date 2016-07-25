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

private:
    Si4463 m_chip;
};

}
