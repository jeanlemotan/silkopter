#include "BrainStdAfx.h"
#include "GPS_Detector.h"


namespace silk
{


GPS_Detector::~GPS_Detector()
{
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
            auto res = read(m_buffer.data(), m_buffer.size());
            if (res > 0)
            {
                for (auto it = m_protocols.begin(); it != m_protocols.end(); /* no ++ here! */)
                {
                    auto& p = *it;
                    if (p->detect(m_buffer.data(), res))
                    {
                        auto idx = std::distance(m_protocols.begin(), it);
                        QLOGI("Detected GPS protocol {}, initializing...", idx);
                        if (!p->init())
                        {
                            QLOGE("Detected GPS protocol {} failed to initialize. Removing from detection list!", idx);
                            m_protocols.erase(it);
                            break;
                        }
                        QLOGI("GPS protocol {} initialized.", idx);
                        m_gps = std::move(p);
                        m_protocols.clear();
                        break;
                    }
                    ++it;
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
