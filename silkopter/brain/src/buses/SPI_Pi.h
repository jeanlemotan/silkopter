#pragma once

#include "ISPI.h"

namespace silk
{
namespace buses
{


class SPI_Pi : public ISPI
{
public:
    SPI_Pi(q::String const& name);
    ~SPI_Pi();

    auto get_name() const -> q::String const&;

    auto open(q::String const& device, size_t mode) -> bool;
    void close();

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t size) -> bool;
    auto write(uint8_t const* data, size_t size) -> bool;

    auto read_register(uint8_t reg, uint8_t* data, size_t size) -> bool;
    auto write_register(uint8_t reg, uint8_t const* data, size_t size) -> bool;

private:
    q::String m_name;
    q::String m_device;
    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;
};

DECLARE_CLASS_PTR(SPI_Pi);


}
}

