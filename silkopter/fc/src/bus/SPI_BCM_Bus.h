#pragma once

#include "common/bus/ISPI.h"
#include "utils/hw/SPI_BCM.h"

namespace silk
{
namespace hal
{
struct SPI_BCM_Descriptor;
}
}

namespace silk
{
namespace bus
{

class SPI_BCM_Bus : public ISPI_Bus
{
public:
    SPI_BCM_Bus();
    ~SPI_BCM_Bus();

    ts::Result<void> init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    util::hw::ISPI& get_spi() override;

private:
    std::shared_ptr<hal::SPI_BCM_Descriptor> m_descriptor;
    util::hw::SPI_BCM m_spi;
};

}
}
