#pragma once

#include "IBus.h"
#include "utils/hw/II2C.h"

namespace silk
{
namespace bus
{

class II2C_Bus: public IBus
{
public:
    virtual ~II2C_Bus() = default;

    virtual util::hw::II2C& get_i2c() = 0;
};

}
}
