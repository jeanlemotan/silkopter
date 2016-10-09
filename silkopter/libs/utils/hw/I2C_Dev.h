#pragma once

namespace util
{
namespace hw
{

class I2C_Dev
{
public:
    I2C_Dev();
    ~I2C_Dev();

    ts::Result<void> init(std::string const& device);

    bool read(uint8_t address, uint8_t* data, size_t size);
    bool write(uint8_t address, uint8_t const* data, size_t size);

    bool read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size);
    bool write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size);

    bool read_register_u16(uint8_t address, uint8_t reg, uint16_t& dst);
    bool read_register_u8(uint8_t address, uint8_t reg, uint8_t& dst);
    bool write_register_u8(uint8_t address, uint8_t reg, uint8_t t);
    bool write_register_u16(uint8_t address, uint8_t reg, uint16_t t);

private:
    void close();

    std::string m_device;
    int m_fd = -1;
    std::vector<uint8_t> m_buffer;
};

}
}
