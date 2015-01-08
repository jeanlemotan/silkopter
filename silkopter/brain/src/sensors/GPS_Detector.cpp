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
    QLOG_TOPIC("gps_detector::init");
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
        QLOGE("Invalid baud requested: {}", baud);
        return false;
    }

    m_fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", device, strerror(errno));
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

    QLOGI("GPS Detection started on {}", device);

    return true;
}

void GPS_Detector::process()
{
    QLOG_TOPIC("gps_detector::process");
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
                        if (!p->init(m_fd))
                        {
                            QLOGE("Detected GPS protocol {} failed to initialize", idx);
                            continue;
                        }
                        QLOGI("Detected GPS protocol {}", idx);
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
