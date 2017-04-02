#pragma once

#include "IBus.h"
#include "utils/hw/ISPI.h"

namespace silk
{
namespace bus
{

class ISPI_Bus: public IBus
{
public:
    virtual ~ISPI_Bus() = default;

    virtual util::hw::ISPI& get_spi() = 0;
};


}
}
