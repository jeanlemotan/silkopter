#include "FCStdAfx.h"
#include "bus/SPI_BCM_Bus.h"

#include "hal.def.h"

///////////////////////////////////////////////////////////////////

namespace silk
{
namespace bus
{

SPI_BCM_Bus::SPI_BCM_Bus()
    : m_descriptor(new hal::SPI_BCM_Descriptor())
{
}

SPI_BCM_Bus::~SPI_BCM_Bus()
{
}

ts::Result<void> SPI_BCM_Bus::init(hal::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<hal::SPI_BCM_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return m_spi.init(specialized->get_dev(), specialized->get_speed(), specialized->get_mode());
}

std::shared_ptr<const hal::IBus_Descriptor> SPI_BCM_Bus::get_descriptor() const
{
    return m_descriptor;
}

util::hw::ISPI& SPI_BCM_Bus::get_spi()
{
    return m_spi;
}



}
}


