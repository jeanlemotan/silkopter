#pragma once

#include "board/Compass.h"

namespace board
{
    //simple implementation that has its data filled
    // by someone else (the imu and barometer for example)
    class Dummy_Compass : public Compass
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

        Data_Config m_data_config;
        Data m_data;
    };

}
