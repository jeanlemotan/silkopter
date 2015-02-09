#pragma once

//#include "sensors/GPS_Protocol.h"

//namespace silk
//{

//class GPS_Detector : q::util::Noncopyable
//{
//public:
//    virtual ~GPS_Detector();

//    auto init() -> bool;
//    void process();
//    auto get() -> boost::optional<GPS_Protocol&>;

//protected:
//    std::vector<std::unique_ptr<GPS_Protocol>> m_protocols;
//    virtual auto read(uint8_t* data, size_t max_size) -> size_t = 0;

//private:
//    std::unique_ptr<GPS_Protocol> m_gps;

//    std::array<uint8_t, 1024> m_buffer;
//};





//}
