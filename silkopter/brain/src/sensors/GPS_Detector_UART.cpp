#include "BrainStdAfx.h"
//#include "GPS_Detector_UART.h"

//#include <termios.h>

//#include "GPS_UBLOX_UART.h"

//namespace silk
//{

//GPS_Detector_UART::~GPS_Detector_UART()
//{
//    if (m_fd >= 0)
//    {
//        close(m_fd);
//    }
//}

//auto GPS_Detector_UART::init(std::string const& device, size_t baud) -> bool
//{
//    QLOG_TOPIC("gps_detector_uart::init");
//    QASSERT(m_fd < 0);
//    if (m_fd >= 0)
//    {
//        return true;
//    }

//    int b = -1;
//    switch (baud)
//    {
//    case 9600: b = B9600; break;
//    case 19200: b = B19200; break;
//    case 38400: b = B38400; break;
//    case 57600: b = B57600; break;
//    case 115200: b = B115200; break;
//    case 230400: b = B230400; break;
//    }

//    if (b < 0)
//    {
//        QLOGE("Invalid baud requested: {}", baud);
//        return false;
//    }

//    m_fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
//    if (m_fd < 0)
//    {
//        QLOGE("can't open {}: {}", device, strerror(errno));
//        return false;
//    }


//    struct termios options;
//    tcgetattr(m_fd, &options);
//    cfsetispeed(&options, b);						// Set baud rate
//    cfsetospeed(&options, b);

//    cfmakeraw(&options);

//    tcflush(m_fd, TCIFLUSH);
//    tcsetattr(m_fd, TCSANOW, &options);

//    QLOGI("GPS Detection started on {} : {}", device, baud);

//    m_protocols.push_back(std::unique_ptr<GPS_Protocol>(new GPS_UBLOX_UART(m_fd)));

//    return true;
//}

//auto GPS_Detector_UART::read(uint8_t* data, size_t max_size) -> size_t
//{
//    QASSERT(m_fd >= 0);
//    return m_fd >= 0 ? ::read(m_fd, data, max_size) : 0;
//}


//}
