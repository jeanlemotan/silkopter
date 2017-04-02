#include "FCStdAfx.h"
#include "bus/SPI_Linux_Bus.h"

#include "hal.def.h"

namespace silk
{
namespace bus
{

SPI_Linux_Bus::SPI_Linux_Bus()
    : m_descriptor(new hal::SPI_Linux_Descriptor())
{
}

SPI_Linux_Bus::~SPI_Linux_Bus()
{
}

ts::Result<void> SPI_Linux_Bus::init(hal::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<hal::SPI_Linux_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return m_spi.init(m_descriptor->get_dev(), m_descriptor->get_speed());
}

std::shared_ptr<const hal::IBus_Descriptor> SPI_Linux_Bus::get_descriptor() const
{
    return m_descriptor;
}

util::hw::ISPI& SPI_Linux_Bus::get_spi()
{
    return m_spi;
}

}
}
