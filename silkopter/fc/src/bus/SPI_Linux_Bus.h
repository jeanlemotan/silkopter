#pragma once

#include "common/bus/ISPI.h"
#include "utils/hw/SPI_Dev.h"

namespace silk
{
namespace hal
{
struct SPI_Linux_Descriptor;
}
}

namespace silk
{
namespace bus
{

class SPI_Linux_Bus : public ISPI_Bus
{
public:
    SPI_Linux_Bus();
    ~SPI_Linux_Bus();

    ts::Result<void> init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    util::hw::ISPI& get_spi() override;

private:
    std::shared_ptr<hal::SPI_Linux_Descriptor> m_descriptor;
    util::hw::SPI_Dev m_spi;
};

}
}
