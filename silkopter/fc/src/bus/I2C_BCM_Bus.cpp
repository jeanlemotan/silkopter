#include "FCStdAfx.h"
#include "bus/I2C_BCM_Bus.h"

#include "hal.def.h"

namespace silk
{
namespace bus
{

I2C_BCM_Bus::I2C_BCM_Bus()
    : m_descriptor(new hal::I2C_BCM_Descriptor())
{
}

I2C_BCM_Bus::~I2C_BCM_Bus()
{
}

ts::Result<void> I2C_BCM_Bus::init(hal::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<hal::I2C_BCM_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return m_i2c.init(specialized->get_dev(), specialized->get_baud());
}

std::shared_ptr<const hal::IBus_Descriptor> I2C_BCM_Bus::get_descriptor() const
{
    return m_descriptor;
}

util::hw::II2C& I2C_BCM_Bus::get_i2c()
{
    return m_i2c;
}

}
}
