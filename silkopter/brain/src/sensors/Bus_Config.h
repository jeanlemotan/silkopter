#pragma once

namespace silk
{

struct Bus_Config
{
    std::string bus;
    boost::optional<size_t> spi_speed;
    boost::optional<size_t> spi_mode;
    boost::optional<size_t> uart_baud;
};


}
