#pragma once

namespace silk
{

class i2c : public q::util::Noncopyable
{
public:
    ~i2c();

    auto open(std::string const& device) -> bool;
    void close();
    auto is_open() const -> bool;

    auto read(uint8_t address, uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto write(uint8_t address, uint8_t reg, uint8_t const* data, uint32_t size) -> bool;

    auto read_u8(uint8_t address, uint8_t reg, uint8_t& dst) -> bool;
    auto read_u16(uint8_t address, uint8_t reg, uint16_t& dst) -> bool;
    auto write_u8(uint8_t address, uint8_t reg, uint8_t const& t) -> bool;
    auto write_u16(uint8_t address, uint8_t reg, uint16_t const& t) -> bool;

private:
    std::string m_device;
    int m_fd = -1;
    std::vector<uint8_t> m_buffer;
};

}
