#include "BrainStdAfx.h"
#include "GPS_Detector.h"

#include <termios.h>

#include "GPS_UBLOX.h"


namespace silk
{


GPS_Detector::~GPS_Detector()
{
    if (m_fd >= 0)
    {
        close(m_fd);
    }
}

auto GPS_Detector::init(std::string const& device, size_t baud) -> bool
{
    QASSERT(m_fd < 0);
    if (m_fd >= 0)
    {
        return true;
    }

    int b = -1;
    switch (baud)
    {
    case 9600: b = B9600; break;
    case 19200: b = B19200; break;
    case 38400: b = B38400; break;
    case 57600: b = B57600; break;
    case 115200: b = B115200; break;
    case 230400: b = B230400; break;
    }

    if (b < 0)
    {
        SILK_ERR("Invalid baud requested: {}", baud);
        return false;
    }

    m_fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_fd < 0)
    {
        SILK_ERR("can't open {}: {}", device, strerror(errno));
        return false;
    }


    struct termios options;
    tcgetattr(m_fd, &options);
    cfsetispeed(&options, b);						// Set baud rate
    cfsetospeed(&options, b);

    cfmakeraw(&options);

    tcflush(m_fd, TCIFLUSH);
    tcsetattr(m_fd, TCSANOW, &options);

    m_protocols.push_back(std::unique_ptr<GPS_Protocol>(new GPS_UBLOX()));

    SILK_INFO("GPS Detection started on {}", device);

    return true;
}

void GPS_Detector::process()
{
    if (m_gps)
    {
        m_gps->process();
    }
    else
    {
        do
        {
            auto res = read(m_fd, m_buffer.data(), m_buffer.size());
            if (res > 0)
            {
                for (auto& p: m_protocols)
                {
                    if (p->detect(m_buffer.data(), res))
                    {
                        auto idx = &p - &m_protocols.front();
                        SILK_INFO("Detected GPS protocol {}", idx);
                        if (!p->init(m_fd))
                        {
                            SILK_ERR("Detected GPS protocol {} failed to initialize", idx);
                            continue;
                        }
                        m_gps = std::move(p);
                        m_protocols.clear();
                        break;
                    }
                }
            }
            else
            {
                break;
            }
        } while (true);
    }
}

auto GPS_Detector::get() -> boost::optional<GPS_Protocol&>
{
    return m_gps ? boost::optional<GPS_Protocol&>(*m_gps) : boost::none;
}


}
