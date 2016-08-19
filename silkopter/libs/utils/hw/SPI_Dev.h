#pragma once

namespace util
{
namespace hw
{

class SPI_Dev
{
public:
    SPI_Dev();
    ~SPI_Dev();

    ts::Result<void> init(std::string const& device, uint32_t speed);

    bool transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0);
    bool transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0);

private:
    bool do_transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed);

    std::string m_device;
    uint32_t m_speed = 0;

    int m_fd = -1;
    std::vector<uint8_t> m_tx_buffer;
    std::vector<uint8_t> m_rx_buffer;
};

}
}
