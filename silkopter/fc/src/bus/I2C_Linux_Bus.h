#pragma once

#include "common/bus/II2C.h"
#include "utils/hw/I2C_Dev.h"

namespace silk
{
namespace hal
{
struct I2C_Linux_Descriptor;
}
}

namespace silk
{
namespace bus
{

class I2C_Linux_Bus : public II2C_Bus
{
public:
    I2C_Linux_Bus();
    ~I2C_Linux_Bus();

    ts::Result<void> init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    util::hw::II2C& get_i2c() override;

private:
    std::shared_ptr<hal::I2C_Linux_Descriptor> m_descriptor;
    util::hw::I2C_Dev m_i2c;
};

}
}
