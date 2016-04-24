#pragma once

#include "common/bus/ISPI.h"

namespace silk
{
namespace bus
{

class SPI_BCM : public ISPI
{
public:
    SPI_BCM(ts::IDeclaration_Scope const& scope);
    ~SPI_BCM();

    bool init(std::shared_ptr<ts::IValue> descriptor) override;
    std::shared_ptr<const ts::IValue> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    virtual auto transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed = 0) -> bool override;
    virtual auto transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed = 0) -> bool override;

private:
    bool open(size_t dev, size_t speed, size_t mode);

    auto get_divider(uint32_t speed) const -> uint32_t;

    auto do_transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed) -> bool;

    std::shared_ptr<ts::IValue> m_descriptor;
    size_t m_dev = 0;
    size_t m_speed = 0;

    uint32_t m_clock_divider = 0;

    std::vector<uint8_t> m_tx_buffer;
    std::vector<uint8_t> m_rx_buffer;
    //std::vector<uint8_t> m_dummy_buffer;

    static std::mutex s_mutex;
};

}
}
