#pragma once

#include "IBus.h"
#include "utils/hw/IUART.h"

namespace silk
{
namespace bus
{

class IUART_Bus: public IBus
{
public:
    virtual ~IUART_Bus() = default;

    virtual util::hw::IUART& get_uart() = 0;
};


}
}
