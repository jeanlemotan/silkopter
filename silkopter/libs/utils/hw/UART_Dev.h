#pragma once

#include "IUART.h"
#include <string>
#include <vector>
#include <atomic>

namespace util
{
namespace hw
{

class UART_Dev : public IUART
{
public:
    UART_Dev();
    ~UART_Dev();

    enum class Baud
    {
        _9600,
        _19200,
        _38400,
        _57600,
        _115200,
        _230400,
    };

    ts::Result<void> init(std::string const& device, Baud baud);

    size_t read(uint8_t* data, size_t max_size) override;
    bool write(uint8_t const* data, size_t size) override;

    void send_break() override;

private:
    void close();

    std::string m_device;
    int m_fd = -1;

    std::vector<uint8_t> m_buffer;

    mutable std::atomic_bool m_is_used = { false };
};

}
}
