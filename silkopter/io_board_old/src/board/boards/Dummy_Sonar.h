#pragma once

#include "board/Sonar.h"

namespace board
{
    //simple implementation that has its data filled
    // by someone else (the imu and barometer for example)
    class Dummy_Sonar : public Sonar
    {
    public:
        Data_Config get_data_config() const { return m_data_config; }

        bool get_data(Data& data) const
        {
            if (m_data.id != data.id)
            {
                data = m_data;
                return true;
            }
            return false;
        }

        void start()
        {
            //nothing. The dummy sonar is started from the owner not by calling this method.
        }
        bool is_started() const { return m_is_started; }

        Data_Config m_data_config;
        Data m_data;
        bool m_is_started = false;
    };

}
