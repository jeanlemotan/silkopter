#pragma once

namespace silk
{

class i2c : public q::util::Noncopyable
{
public:
    ~i2c();

    enum class Result
    {
        OK,
        FAILED
    };

    Result open(std::string const& device);
    void close();

    void read(uint8_t address, uint8_t reg, uint8_t* data, uint32_t size);
    uint16_t read_u16(uint8_t address, uint8_t reg)
    {
        uint8_t val[2];
        read(address, reg, val, 2);
        uint16_t t = 0;
        reinterpret_cast<uint8_t*>(&t)[1] = val[0];
        reinterpret_cast<uint8_t*>(&t)[0] = val[1];
        return t;
    }
    uint8_t read_u8(uint8_t address, uint8_t reg)
    {
        uint8_t t = 0;
        read(address, reg, &t, 1);
        return t;
    }


    void write(uint8_t address, uint8_t reg, uint8_t const* data, uint32_t size);
    void write(uint8_t address, uint8_t reg)
    {
        write(address, reg, nullptr, 0);
    }
    void write_u8(uint8_t address, uint8_t reg, uint8_t const& t)
    {
        write(address, reg, &t, 1);
    }
    void write_u16(uint8_t address, uint8_t reg, uint16_t const& t)
    {
        uint8_t val[2];
        val[0] = reinterpret_cast<uint8_t const*>(&t)[1];
        val[1] = reinterpret_cast<uint8_t const*>(&t)[0];
        write(address, reg, val, 2);
    }
private:
    int m_fd = -1;
    std::vector<uint8_t> m_buffer;
};

}
