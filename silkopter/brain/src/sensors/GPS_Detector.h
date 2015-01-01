#pragma once

#include "sensors/GPS_Protocol.h"

namespace silk
{

class GPS_Detector : q::util::Noncopyable
{
public:
    ~GPS_Detector();

    auto init(std::string const& device, size_t baud) -> bool;
    void process();
    auto get() -> boost::optional<GPS_Protocol&>;

private:
    int m_fd = -1;
    std::vector<std::unique_ptr<GPS_Protocol>> m_protocols;
    std::unique_ptr<GPS_Protocol> m_gps;

    std::array<uint8_t, 1024> m_buffer;
};





}
